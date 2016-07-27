/**
 ******************************************************************************
 * @file    i2c_hal.c
 * @author  Satish Nair, Brett Walach
 * @version V1.0.0
 * @date    12-Sept-2014
 * @brief
 ******************************************************************************
  Copyright (c) 2013-2015 Particle Industries, Inc.  All rights reserved.

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
#include "stm32f4xx.h"
#include "service_debug.h"

/* Private define ------------------------------------------------------------*/
// XXX: Change
#define TOTAL_I2C   2
#define BUFFER_LENGTH   (I2C_BUFFER_LENGTH)
#define EVENT_TIMEOUT   100000 // 100ms

#define TRANSMITTER     0x00
#define RECEIVER        0x01

// XXX: Change
/* Private typedef -----------------------------------------------------------*/
typedef enum I2C_Num_Def {
    I2C1_SENSOR_SDA_SCL = 0,
    I2C3_D2_D1 = 1
} I2C_Num_Def;

typedef enum I2C_Transaction_Ending_Condition {
    I2C_ENDING_UNKNOWN,
    I2C_ENDING_STOP,
    I2C_ENDING_START
} I2C_Transaction_Ending_Condition;

typedef struct STM32_I2C_Info {
    I2C_TypeDef* I2C_Peripheral;

    GPIO_TypeDef* I2C_SDA_Port;
    GPIO_TypeDef* I2C_SCL_Port;
    uint16_t I2C_SDA_Pin;
    uint16_t I2C_SCL_Pin;
    uint8_t I2C_AF_Mapping;

    IRQn_Type I2C_ER_IRQn;
    IRQn_Type I2C_EV_IRQn;

    uint32_t I2C_ClockSpeed;
    uint8_t  I2C_Ownaddress1;

    I2C_HandleTypeDef I2CHandle;

    bool I2C_Enabled;

    uint8_t rxBuffer[BUFFER_LENGTH];
    uint8_t rxBufferIndex;
    uint8_t rxBufferLength;
    uint8_t txAddress;
    uint8_t txBuffer[BUFFER_LENGTH];
    uint8_t txBufferIndex;
    uint8_t txBufferLength;

    uint8_t transmitting;

    void (*callback_onRequest)(void);
    void (*callback_onReceive)(int);

    I2C_Mode mode;
    volatile uint8_t prevEnding;
    uint8_t clkStretchingEnabled;
} STM32_I2C_Info;

/* Private variables ---------------------------------------------------------*/
// XXX: Change
STM32_I2C_Info I2C_MAP[TOTAL_I2C] =
{
    /* I2C1, PB9, PB8  for sensors*/
    /* I2C3, PB4, PA8  for usrs*/
    { I2C1, GPIOB, GPIOB, GPIO_PIN_9, GPIO_PIN_8, GPIO_AF4_I2C1, I2C1_ER_IRQn, I2C1_EV_IRQn},
    { I2C3, GPIOB, GPIOA, GPIO_PIN_4, GPIO_PIN_8, GPIO_AF9_I2C3, I2C3_ER_IRQn, I2C3_EV_IRQn}
};

static STM32_I2C_Info *i2cMap[TOTAL_I2C]; // pointer to I2C_MAP[] containing I2C peripheral info

void HAL_I2C_GPIO_DeInit(HAL_I2C_Interface i2c)
{
    // XXX: Change
    /*##-1- Reset peripherals ##################################################*/
    if (i2cMap[i2c]->I2C_Peripheral == I2C1)
    {
        __HAL_RCC_I2C1_FORCE_RESET();
        __HAL_RCC_I2C1_RELEASE_RESET();
    }
    else if (i2cMap[i2c]->I2C_Peripheral == I2C3)
    {
        __HAL_RCC_I2C3_FORCE_RESET();
        __HAL_RCC_I2C3_RELEASE_RESET();
    }
    /*##-2- Disable peripherals and GPIO Clocks ################################*/
    HAL_GPIO_DeInit(i2cMap[i2c]->I2C_SDA_Port, i2cMap[i2c]->I2C_SDA_Pin);
    HAL_GPIO_DeInit(i2cMap[i2c]->I2C_SCL_Port, i2cMap[i2c]->I2C_SCL_Pin);

    /*##-3- Disable the NVIC for I2C ###########################################*/
    HAL_NVIC_DisableIRQ(i2cMap[i2c]->I2C_ER_IRQn);
    HAL_NVIC_DisableIRQ(i2cMap[i2c]->I2C_EV_IRQn);
}

void HAL_I2C_GPIO_Init(HAL_I2C_Interface i2c)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    // XXX: Change
    /* I2C1, PB9, PB8  for sensors*/
    /* I2C3, PB4, PA8  for usrs*/

    /*##-1- Enable GPIO Clocks #################################################*/
    /* Enable GPIO TX/RX clock */
    if (i2cMap[i2c]->I2C_Peripheral == I2C1)
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }
    else if (i2cMap[i2c]->I2C_Peripheral == I2C3)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }

    /*##-2- Configure peripheral GPIO ##########################################*/
    /* I2C TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = i2cMap[i2c]->I2C_SCL_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = i2cMap[i2c]->I2C_AF_Mapping;
    HAL_GPIO_Init(i2cMap[i2c]->I2C_SDA_Port, &GPIO_InitStruct);

    /* I2C RX GPIO pin configuration  */
    GPIO_InitStruct.Pin = i2cMap[i2c]->I2C_SCL_Pin;
    GPIO_InitStruct.Alternate = i2cMap[i2c]->I2C_AF_Mapping;
    HAL_GPIO_Init(i2cMap[i2c]->I2C_SDA_Port, &GPIO_InitStruct);

    /*##-3- Enable I2C peripherals Clock #######################################*/
    /* Enable I2C clock */
     if (i2cMap[i2c]->I2C_Peripheral == I2C1)
    {
        __HAL_RCC_I2C1_CLK_ENABLE();
    }
    else if (i2cMap[i2c]->I2C_Peripheral == I2C3)
    {
        __HAL_RCC_I2C1_CLK_ENABLE();
    }

    /*##-4- Configure the NVIC for I2C #########################################*/
    /* NVIC for I2C */
    HAL_NVIC_SetPriority(i2cMap[i2c]->I2C_ER_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(i2cMap[i2c]->I2C_ER_IRQn);
    HAL_NVIC_SetPriority(i2cMap[i2c]->I2C_EV_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(i2cMap[i2c]->I2C_EV_IRQn);

    /*##-5- Configure the I2C   #################################################*/
    I2C_HandleTypeDef I2CHandle;
    I2CHandle.Instance = i2cMap[i2c]->I2C_Peripheral;
    I2CHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    I2CHandle.Init.ClockSpeed      = i2cMap[i2c]->I2C_ClockSpeed; //400000;
    I2CHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    I2CHandle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
    I2CHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    I2CHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
    I2CHandle.Init.OwnAddress1     = i2cMap[i2c]->I2C_Ownaddress1;//0x00;
    I2CHandle.Init.OwnAddress2     = 0x00;

    i2cMap[i2c]->I2CHandle = I2CHandle;
    HAL_I2C_Init(&(i2cMap[i2c]->I2CHandle));
}
static void HAL_I2C_SoftwareReset(HAL_I2C_Interface i2c)
{
    /* Deinit I2C */
    HAL_I2C_GPIO_DeInit(i2c);
    /* Init I2C */
    HAL_I2C_GPIO_Init(i2c);
    /* Set prevEnding */
    i2cMap[i2c]->prevEnding = I2C_ENDING_UNKNOWN;
}

void HAL_I2C_Initial(HAL_I2C_Interface i2c, void* reserved)
{
    DEBUG("HAL_I2C_Initial Start!\r\n");
    if(i2c == HAL_I2C_INTERFACE2) // for sensors
    {
        i2cMap[i2c] = &I2C_MAP[I2C1_SENSOR_SDA_SCL];
    }
    else if(i2c == HAL_I2C_INTERFACE1) // for users
    {
        i2cMap[i2c] = &I2C_MAP[I2C3_D2_D1];
    }

    i2cMap[i2c]->I2C_ClockSpeed = CLOCK_SPEED_400KHZ;
    i2cMap[i2c]->I2C_Enabled = false;

    i2cMap[i2c]->rxBufferIndex = 0;
    i2cMap[i2c]->rxBufferLength = 0;

    i2cMap[i2c]->txAddress = 0;
    i2cMap[i2c]->txBufferIndex = 0;
    i2cMap[i2c]->txBufferLength = 0;

    i2cMap[i2c]->transmitting = 0;

    i2cMap[i2c]->prevEnding = I2C_ENDING_UNKNOWN;

    i2cMap[i2c]->clkStretchingEnabled = 1;
}

void HAL_I2C_Set_Speed(HAL_I2C_Interface i2c, uint32_t speed, void* reserved)
{
    i2cMap[i2c]->I2C_ClockSpeed = speed;
}

void HAL_I2C_Stretch_Clock(HAL_I2C_Interface i2c, bool stretch, void* reserved)
{
    // TODO: to be check
//    if(stretch == true)
//    {
//        I2C_StretchClockCmd(i2cMap[i2c]->I2C_Peripheral, ENABLE);
//    }
//    else
//    {
//        I2C_StretchClockCmd(i2cMap[i2c]->I2C_Peripheral, DISABLE);
//    }
//    i2cMap[i2c]->clkStretchingEnabled = stretch;
}

void HAL_I2C_Begin(HAL_I2C_Interface i2c, I2C_Mode mode, uint8_t address, void* reserved)
{
    DEBUG("HAL_I2C_Begin Start!\r\n");
    i2cMap[i2c]->rxBufferIndex = 0;
    i2cMap[i2c]->rxBufferLength = 0;
    i2cMap[i2c]->txBufferIndex = 0;
    i2cMap[i2c]->txBufferLength = 0;

    i2cMap[i2c]->mode = mode;
    i2cMap[i2c]->prevEnding = I2C_ENDING_UNKNOWN;
    i2cMap[i2c]->I2C_Ownaddress1 = address<<1;

    HAL_I2C_GPIO_Init(i2c);

    i2cMap[i2c]->I2C_Enabled = true;
}

void HAL_I2C_End(HAL_I2C_Interface i2c,void* reserved)
{
    if(i2cMap[i2c]->I2C_Enabled != false)
    {
        HAL_I2C_GPIO_DeInit(i2c);
        i2cMap[i2c]->I2C_Enabled = false;
    }
}

uint32_t HAL_I2C_Request_Data(HAL_I2C_Interface i2c, uint8_t address, uint8_t quantity, uint8_t stop,void* reserved)
{
    uint32_t _micros;
    uint8_t bytesRead = 0;
    int state;
    // clamp to buffer length
    if(quantity > BUFFER_LENGTH)
    {
        quantity = BUFFER_LENGTH;
    }

    address = address << 1;
    while(HAL_I2C_Master_Receive(&(i2cMap[i2c]->I2CHandle), address, i2cMap[i2c]->rxBuffer, quantity, 100) != HAL_OK)
    {
        if(EVENT_TIMEOUT < (HAL_Timer_Get_Micro_Seconds() - _micros))
        {
            /* SW Reset the I2C Peripheral */
            HAL_I2C_SoftwareReset(i2c);
            i2cMap[i2c]->prevEnding = I2C_ENDING_STOP;
            // debug
            return 0;
        }
    }
    bytesRead = quantity;
    // set rx buffer iterator vars
    i2cMap[i2c]->rxBufferIndex = 0;
    i2cMap[i2c]->rxBufferLength = bytesRead;
    return bytesRead;
}

void HAL_I2C_Begin_Transmission(HAL_I2C_Interface i2c, uint8_t address,void* reserved)
{
    // indicate that we are transmitting
    i2cMap[i2c]->transmitting = 1;
    // set address of targeted slave
    i2cMap[i2c]->txAddress = address << 1;
    // reset tx buffer iterator vars
    i2cMap[i2c]->txBufferIndex = 0;
    i2cMap[i2c]->txBufferLength = 0;
}

uint8_t HAL_I2C_End_Transmission(HAL_I2C_Interface i2c, uint8_t stop,void* reserved)
{
    uint32_t startTime;
    startTime = HAL_Timer_Get_Micro_Seconds();
    while(HAL_I2C_Master_Transmit( &(i2cMap[i2c]->I2CHandle), (uint16_t)i2cMap[i2c]->txAddress, &i2cMap[i2c]->txBuffer[0], i2cMap[i2c]->txBufferLength, 100) != HAL_OK)
    {
        if(EVENT_TIMEOUT < (HAL_Timer_Get_Micro_Seconds() - startTime))
        {
            /* SW Reset the I2C Peripheral */
            HAL_I2C_SoftwareReset(i2c);
            // i2cMap[i2c]->prevEnding = I2C_ENDING_START;
            return 1;
        }
    }
    // reset tx buffer iterator vars
    i2cMap[i2c]->txBufferIndex = 0;
    i2cMap[i2c]->txBufferLength = 0;

    // indicate that we are done transmitting
    i2cMap[i2c]->transmitting = 0;
    return 0;
}

uint32_t HAL_I2C_Write_Data(HAL_I2C_Interface i2c, uint8_t data,void* reserved)
{
    if(i2cMap[i2c]->transmitting)
    {
        // in master/slave transmitter mode
        // don't bother if buffer is full
        if(i2cMap[i2c]->txBufferLength >= BUFFER_LENGTH)
        {
            return 0;
        }
        // put byte in tx buffer
        i2cMap[i2c]->txBuffer[i2cMap[i2c]->txBufferIndex++] = data;
        // update amount in buffer
        i2cMap[i2c]->txBufferLength = i2cMap[i2c]->txBufferIndex;
    }
    return 1;
}

int32_t HAL_I2C_Available_Data(HAL_I2C_Interface i2c,void* reserved)
{
    return i2cMap[i2c]->rxBufferLength - i2cMap[i2c]->rxBufferIndex;
}

int32_t HAL_I2C_Read_Data(HAL_I2C_Interface i2c,void* reserved)
{
    int value = -1;
    // get each successive byte on each call
    if(i2cMap[i2c]->rxBufferIndex < i2cMap[i2c]->rxBufferLength)
    {
        value = i2cMap[i2c]->rxBuffer[i2cMap[i2c]->rxBufferIndex++];
    }
    return value;
}

int32_t HAL_I2C_Peek_Data(HAL_I2C_Interface i2c,void* reserved)
{
    int value = -1;
    if(i2cMap[i2c]->rxBufferIndex < i2cMap[i2c]->rxBufferLength)
    {
        value = i2cMap[i2c]->rxBuffer[i2cMap[i2c]->rxBufferIndex];
    }
    return value;
}

void HAL_I2C_Flush_Data(HAL_I2C_Interface i2c,void* reserved)
{
    // TODO: to be implemented.
}

bool HAL_I2C_Is_Enabled(HAL_I2C_Interface i2c,void* reserved)
{
    return i2cMap[i2c]->I2C_Enabled;
}

void HAL_I2C_Set_Callback_On_Receive(HAL_I2C_Interface i2c, void (*function)(int),void* reserved)
{
    i2cMap[i2c]->callback_onReceive = function;
}

void HAL_I2C_Set_Callback_On_Request(HAL_I2C_Interface i2c, void (*function)(void),void* reserved)
{
    i2cMap[i2c]->callback_onRequest = function;
}

/*******************************************************************************
 * Function Name  : HAL_I2C1_EV_Handler (Declared as weak in stm32_it.cpp)
 * Description    : This function handles I2C1 Event interrupt request(Only for Slave mode).
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void HAL_I2C1_EV_Handler(void)
{
    // TODO
}

/*******************************************************************************
 * Function Name  : HAL_I2C1_ER_Handler (Declared as weak in stm32_it.cpp)
 * Description    : This function handles I2C1 Error interrupt request.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void HAL_I2C1_ER_Handler(void)
{
    // TODO
}

void HAL_I2C_Enable_DMA_Mode(HAL_I2C_Interface i2c, bool enable,void* reserved)
{
    //TODO
    /* Presently I2C Master mode uses polling and I2C Slave mode uses Interrupt */
}
