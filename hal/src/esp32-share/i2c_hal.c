/**
 ******************************************************************************
 * @file    i2c_hal.c
 * @author  Satish Nair, Brett Walach
 * @version V1.0.0
 * @date    12-Sept-2014
 * @brief
 ******************************************************************************
  Copyright (c) 2013-2015 IntoRobot Industries, Inc.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "i2c_hal.h"
#include "gpio_hal.h"
#include "pinmap_impl.h"

#include "esp32-hal-i2c.h"
#include "esp32-hal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "rom/ets_sys.h"
#include "soc/i2c_reg.h"
#include "soc/i2c_struct.h"
#include "soc/dport_reg.h"

#define TOTAL_WIRE_NUM FIRMWARE_CONFIG_WIRING_WIRE

#define BUFFER_LENGTH   (I2C_BUFFER_LENGTH)

// default pin
// I2C SDA D0 GPIO21
// I2C SCL D1 GPIO22
// The other is define by self which can be changed
// We define below
typedef enum I2C_Num_Def {
    I2C_0 = 0,
    I2C_1 = 1
} I2C_Num_Def;

typedef struct ESP32_I2C_Info {
    uint8_t I2C_SCL_Pin;
    uint8_t I2C_SDA_Pin;

    bool I2C_Enabled;
    i2c_t* i2c;
    uint8_t rxBuffer[BUFFER_LENGTH];
    uint16_t rxIndex;
    uint16_t rxLength;
    uint16_t rxQueued; //@stickBreaker

    uint8_t txBuffer[BUFFER_LENGTH];
    uint16_t txIndex;
    uint16_t txLength;
    uint16_t txAddress;
    uint16_t txQueued; //@stickBreaker

    uint8_t transmitting;
    void (*callback_onRequest)(void);
    void (*callback_onReceive)(int);
} ESP32_I2C_Info;

/*
 * I2C mapping
 */
ESP32_I2C_Info I2C_MAP[TOTAL_WIRE_NUM] =
{
    { SCL, SDA},    // I2C0  D0, D1
    { SCL1, SDA1}   // I2C1  A3, A4
};

ESP32_I2C_Info *i2cMap[TOTAL_WIRE_NUM]; // pointer to I2C_MAP[] containing I2C peripheral register locations (etc)

void HAL_I2C_Initial(HAL_I2C_Interface i2c, void* reserved)
{
    i2cMap[i2c] = &I2C_MAP[i2c];

    i2cMap[i2c]->I2C_Enabled = false;
    i2cMap[i2c]->transmitting = 0;
}

void HAL_I2C_Set_Speed(HAL_I2C_Interface i2c, uint32_t speed, void* reserved)
{
    if(i2cMap[i2c]->i2c == NULL){
        return;
    }

    i2cSetFrequency(i2cMap[i2c]->i2c, speed);
}

void HAL_I2C_Stretch_Clock(HAL_I2C_Interface i2c, bool stretch, void* reserved)
{

}

void HAL_I2C_Begin(HAL_I2C_Interface i2c, I2C_Mode mode, uint8_t address, void* reserved)
{
    EESP32_Pin_Info* PIN_MAP = HAL_Pin_Map();

    i2cMap[i2c]->i2c = i2cInit(i2c,
            PIN_MAP[i2cMap[i2c]->I2C_SDA_Pin].gpio_pin, PIN_MAP[i2cMap[i2c]->I2C_SCL_Pin].gpio_pin, 100000);
    HAL_I2C_Flush_Data(i2c, NULL);
}

void HAL_I2C_End(HAL_I2C_Interface i2c, void* reserved)
{
    // xxx: To be done
}

uint32_t HAL_I2C_Request_Data(HAL_I2C_Interface i2c, uint8_t address, uint8_t quantity, uint8_t stop, void* reserved)
{
    //use internal Wire rxBuffer, multiple requestFrom()'s may be pending, try to share rxBuffer
    uint32_t cnt = i2cMap[i2c]->rxQueued; // currently queued reads, next available position in rxBuffer
    if(cnt < (BUFFER_LENGTH-1) && (quantity + cnt) <= BUFFER_LENGTH) { // any room left in rxBuffer
        i2cMap[i2c]->rxQueued += quantity;
    } else { // no room to receive more!
        cnt = 0;
        HAL_I2C_Flush_Data(i2c, NULL);
        return cnt;
    }

    if(I2C_ERROR_OK == i2cRead(i2cMap[i2c]->i2c, address, &i2cMap[i2c]->rxBuffer[cnt], quantity, stop, 50, &cnt)) {
        i2cMap[i2c]->rxIndex = 0;
        i2cMap[i2c]->rxLength = i2cMap[i2c]->rxQueued;
        i2cMap[i2c]->rxQueued = 0;
        i2cMap[i2c]->txQueued = 0; // the SendStop=true will restart all Queueing
    } else {
        cnt = 0;
    }
    return cnt;
}

void HAL_I2C_Begin_Transmission(HAL_I2C_Interface i2c, uint8_t address,void* reserved)
{
    i2cMap[i2c]->transmitting = 1;
    i2cMap[i2c]->txAddress = address;
    i2cMap[i2c]->txIndex = i2cMap[i2c]->txQueued;
    i2cMap[i2c]->txLength = i2cMap[i2c]->txQueued;
}

uint8_t HAL_I2C_End_Transmission(HAL_I2C_Interface i2c, uint8_t stop, void* reserved)
{
    if (i2cMap[i2c]->transmitting) {
        if(I2C_ERROR_CONTINUE != i2cWrite(i2cMap[i2c]->i2c, i2cMap[i2c]->txAddress, \
                    i2cMap[i2c]->txBuffer[i2cMap[i2c]->txQueued], i2cMap[i2c]->txLength - i2cMap[i2c]->txQueued, stop, 50)) {
            i2cMap[i2c]->rxIndex = 0;
            i2cMap[i2c]->rxLength = i2cMap[i2c]->rxQueued;
            i2cMap[i2c]->rxQueued = 0;
            i2cMap[i2c]->txQueued = 0;
        } else {
            // txlength is howmany bytes in txbuffer have been use
            i2cMap[i2c]->txQueued = i2cMap[i2c]->txLength;
        }
    } else {
        HAL_I2C_Flush_Data(i2c, NULL);
    }
    i2cMap[i2c]->txIndex = 0;
    i2cMap[i2c]->txLength = 0;
    i2cMap[i2c]->transmitting = 0;
    return 0;
}

uint32_t HAL_I2C_Write_Data(HAL_I2C_Interface i2c, uint8_t data, void* reserved)
{
    if (i2cMap[i2c]->transmitting) {
        if (i2cMap[i2c]->txLength >= BUFFER_LENGTH) {
            return 0;
        }
        i2cMap[i2c]->txBuffer[i2cMap[i2c]->txIndex++] = data;
        i2cMap[i2c]->txLength = i2cMap[i2c]->txIndex;
        return 1;
    }
    return 0;
}

int32_t HAL_I2C_Available_Data(HAL_I2C_Interface i2c, void* reserved)
{
    return (i2cMap[i2c]->rxLength - i2cMap[i2c]->rxIndex);
}

int32_t HAL_I2C_Read_Data(HAL_I2C_Interface i2c, void* reserved)
{
    int value = -1;
    if(i2cMap[i2c]->rxIndex < i2cMap[i2c]->rxLength){
        value = i2cMap[i2c]->rxBuffer[i2cMap[i2c]->rxIndex++];
    }
    return value;
}

int32_t HAL_I2C_Peek_Data(HAL_I2C_Interface i2c, void* reserved)
{
    int value = -1;
    if(i2cMap[i2c]->rxIndex < i2cMap[i2c]->rxLength){
        value = i2cMap[i2c]->rxBuffer[i2cMap[i2c]->rxIndex];
    }
    return value;
}

void HAL_I2C_Flush_Data(HAL_I2C_Interface i2c, void* reserved)
{
    i2cMap[i2c]->rxIndex = 0;
    i2cMap[i2c]->rxLength = 0;
    i2cMap[i2c]->txIndex = 0;
    i2cMap[i2c]->txLength = 0;
    i2cMap[i2c]->rxQueued = 0;
    i2cMap[i2c]->txQueued = 0;
    i2cFlush(i2cMap[i2c]->i2c); // cleanup
}

bool HAL_I2C_Is_Enabled(HAL_I2C_Interface i2c, void* reserved)
{
    return i2cMap[i2c]->I2C_Enabled;
}

void HAL_I2C_Set_Callback_On_Receive(HAL_I2C_Interface i2c, void (*function)(int), void* reserved)
{
    i2cMap[i2c]->callback_onReceive = function;
}

void HAL_I2C_Set_Callback_On_Request(HAL_I2C_Interface i2c, void (*function)(void), void* reserved)
{
    i2cMap[i2c]->callback_onRequest = function;
}

void HAL_I2C_Enable_DMA_Mode(HAL_I2C_Interface i2c, bool enable,void* reserved)
{
}

