/**
 ******************************************************************************
 * @file    usart_hal.c
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
#include "hw_config.h"
#include "usart_hal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp32-hal-uart.h"
#include "rom/uart.h"
#include "soc/uart_reg.h"
#include "soc/uart_struct.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_sig_map.h"
#include "pinmap_impl.h"

#define UART_MUTEX_LOCK()    do {} while (xSemaphoreTake(usartMap[serial]->uart->lock, portMAX_DELAY) != pdPASS)
#define UART_MUTEX_UNLOCK()  xSemaphoreGive(usartMap[serial]->uart->lock)

struct uart_struct_t {
    uart_dev_t * dev;
    xSemaphoreHandle lock;
    uint8_t num;
    xQueueHandle queue;
};

static uart_t _uart_bus_array[3] = {
    {(volatile uart_dev_t *)(DR_REG_UART_BASE), NULL, 0, NULL},
    {(volatile uart_dev_t *)(DR_REG_UART1_BASE), NULL, 1, NULL},
    {(volatile uart_dev_t *)(DR_REG_UART2_BASE), NULL, 2, NULL}
};

// default pin
// rxpin = RX, GPIO_NUM_3
// txpin = TX, GPIO_NUM_1
// The other two are define by self which can be changed
// We define below
typedef enum USART_Num_Def {
    USART_0 = 0,
    USART_1 = 1,
    USART_2 = 2
} USART_Num_Def;

typedef struct ESP32_USART_Info {
    uint8_t usart_tx_pin;
    uint8_t usart_rx_pin;

    bool usart_enabled;
    int peek_char;
    uart_t* uart;
} ESP32_USART_Info;

/*
 * USART mapping
 */
ESP32_USART_Info USART_MAP[TOTAL_USARTS] =
{
    /*
     * TX pin
     * RX pin
     * <usart enabled> used internally and does not appear below
     * <usart transmitting> used internally and does not appear below
     */
     { TX, RX},           // USART 0 TX, RX
     { D3, D4},           // USART 1 D3, D4
     { A3, A4},           // USART 2 A3, A4
};

static ESP32_USART_Info *usartMap[TOTAL_USARTS]; // pointer to USART_MAP[] containing USART peripheral register locations (etc)


void HAL_USART_Initial(HAL_USART_Serial serial)
{
    /* if(serial > 2) */
    /* {return;} */
    usartMap[serial] = &USART_MAP[serial];

    usartMap[serial]->usart_enabled = false;
    usartMap[serial]->peek_char = -1;
}

void HAL_USART_Begin(HAL_USART_Serial serial, uint32_t baudrate)
{
    HAL_USART_BeginConfig(serial, baudrate, 0, 0);
}

void HAL_USART_BeginConfig(HAL_USART_Serial serial, uint32_t baudrate, uint32_t config_ex, void *ptr)
{
    /* if (serial > 2) */
    /* {return;} */
    uint16_t queueLen = 256;
    bool inverted = false;
    uint32_t config = SERIAL_8N1;

    usartMap[serial]->uart = &_uart_bus_array[serial];

    if(usartMap[serial]->uart->lock == NULL) {
        usartMap[serial]->uart->lock = xSemaphoreCreateMutex();
        if(usartMap[serial]->uart->lock == NULL) {
            return NULL;
        }
    }

    if(queueLen && usartMap[serial]->uart->queue == NULL) {
        usartMap[serial]->uart->queue = xQueueCreate(queueLen, sizeof(uint8_t)); //initialize the queue
        if(usartMap[serial]->uart->queue == NULL) {
            return NULL;
        }
    }

    uartFlush(usartMap[serial]->uart);
    uartSetBaudRate(usartMap[serial]->uart, baudrate);
    UART_MUTEX_LOCK();
    usartMap[serial]->uart->dev->conf0.val = config;
    UART_MUTEX_UNLOCK();

    EESP32_Pin_Info* PIN_MAP = HAL_Pin_Map();
    pin_t tx_pin = PIN_MAP[usartMap[serial]->usart_tx_pin].gpio_pin;
    pin_t rx_pin = PIN_MAP[usartMap[serial]->usart_rx_pin].gpio_pin;

    uartAttachRx(usartMap[serial]->uart, rx_pin, inverted);
    uartAttachTx(usartMap[serial]->uart, tx_pin, inverted);

    usartMap[serial]->usart_enabled = true;
}

void HAL_USART_End(HAL_USART_Serial serial)
{
    if(usartMap[serial]->uart == NULL) {
        return;
    }

    UART_MUTEX_LOCK();
    if(usartMap[serial]->uart->queue != NULL) {
        vQueueDelete(usartMap[serial]->uart->queue);
    }

    uartDetachRx(usartMap[serial]->uart);
    uartDetachTx(usartMap[serial]->uart);

    usartMap[serial]->uart->dev->conf0.val = 0;
    UART_MUTEX_UNLOCK();
}

uint32_t HAL_USART_Write_Data(HAL_USART_Serial serial, uint8_t data)
{
    if(usartMap[serial]->uart == NULL) {
        return;
    }
    UART_MUTEX_LOCK();
    while(usartMap[serial]->uart->dev->status.rxfifo_cnt == 0x7F);
    usartMap[serial]->uart->dev->fifo.rw_byte = data;
    UART_MUTEX_UNLOCK();
    return 0;
}

uint32_t HAL_USART_Write_NineBitData(HAL_USART_Serial serial, uint16_t data)
{
    return 1;
}

int32_t HAL_USART_Available_Data(HAL_USART_Serial serial)
{
    if(usartMap[serial]->uart == NULL || usartMap[serial]->uart->queue == NULL) {
        return 0;
    }
    return uxQueueMessagesWaiting(usartMap[serial]->uart->queue);
}

int32_t HAL_USART_Available_Data_For_Write(HAL_USART_Serial serial)
{
    return 1;
}

int32_t HAL_USART_Read_Data(HAL_USART_Serial serial)
{
    if(usartMap[serial]->uart == NULL || usartMap[serial]->uart->queue == NULL) {
        return 0;
    }
    uint8_t c;
    if(xQueueReceive(usartMap[serial]->uart->queue, &c, 0)) {
        return c;
    }
    return 0;
}

int32_t HAL_USART_Peek_Data(HAL_USART_Serial serial)
{
    if(usartMap[serial]->uart == NULL || usartMap[serial]->uart->queue == NULL) {
        return 0;
    }
    uint8_t c;
    if(xQueuePeek(usartMap[serial]->uart->queue, &c, 0)) {
        return c;
    }
    return 0;
}

void HAL_USART_Flush_Data(HAL_USART_Serial serial)
{
    if(usartMap[serial]->uart == NULL) {
        return;
    }

    UART_MUTEX_LOCK();
    while(usartMap[serial]->uart->dev->status.txfifo_cnt);

    usartMap[serial]->uart->dev->conf0.txfifo_rst = 1;
    usartMap[serial]->uart->dev->conf0.txfifo_rst = 0;

    usartMap[serial]->uart->dev->conf0.rxfifo_rst = 1;
    usartMap[serial]->uart->dev->conf0.rxfifo_rst = 0;
    UART_MUTEX_UNLOCK();
}

bool HAL_USART_Is_Enabled(HAL_USART_Serial serial)
{
    return usartMap[serial]->usart_enabled;
}

void HAL_USART_Half_Duplex(HAL_USART_Serial serial, bool Enable)
{
}
