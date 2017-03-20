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

#define UART0    0
#define UART1    1
#define UART2    2
#define UART_NO -1

// Options for `config` argument of uart_init
#define UART_NB_BIT_5         0B00000000
#define UART_NB_BIT_6         0B00000100
#define UART_NB_BIT_7         0B00001000
#define UART_NB_BIT_8         0B00001100

#define UART_PARITY_NONE      0B00000000
#define UART_PARITY_EVEN      0B00000010
#define UART_PARITY_ODD       0B00000011

#define UART_NB_STOP_BIT_0    0B00000000
#define UART_NB_STOP_BIT_1    0B00010000
#define UART_NB_STOP_BIT_15   0B00100000
#define UART_NB_STOP_BIT_2    0B00110000

#define UART_TX_FIFO_SIZE     0x80

#define UART_8N1 ( UART_NB_BIT_8 | UART_PARITY_NONE | UART_NB_STOP_BIT_1 )

/* Private typedef -----------------------------------------------------------*/
typedef enum USART_Num_Def {
    USART_1_3 = 0,
    USART_16_17 = 1,
} USART_Num_Def;

/* Private macro -------------------------------------------------------------*/
// IS_USART_CONFIG_VALID(config) - returns true for 8 data bit, any flow control, any parity, any stop byte configurations
#define IS_USART_CONFIG_VALID(CONFIG) (((CONFIG & SERIAL_VALID_CONFIG) >> 2) != 0b11)

#define UART_REG_BASE(u)    ((u==0)?DR_REG_UART_BASE:(      (u==1)?DR_REG_UART1_BASE:(    (u==2)?DR_REG_UART2_BASE:0)))
#define UART_RXD_IDX(u)     ((u==0)?U0RXD_IN_IDX:(          (u==1)?U1RXD_IN_IDX:(         (u==2)?U2RXD_IN_IDX:0)))
#define UART_TXD_IDX(u)     ((u==0)?U0TXD_OUT_IDX:(         (u==1)?U1TXD_OUT_IDX:(        (u==2)?U2TXD_OUT_IDX:0)))
#define UART_INTR_SOURCE(u) ((u==0)?ETS_UART0_INTR_SOURCE:( (u==1)?ETS_UART1_INTR_SOURCE:((u==2)?ETS_UART2_INTR_SOURCE:0)))

#define UART_MUTEX_LOCK()    do {} while (xSemaphoreTake(usartMap[serial]->uart->lock, portMAX_DELAY) != pdPASS)
#define UART_MUTEX_UNLOCK()  xSemaphoreGive(usartMap[serial]->uart->lock)

/* Private variables ---------------------------------------------------------*/
typedef struct {
    int usart_nr;
    uint8_t usart_tx_pin;
    uint8_t usart_rx_pin;

    uart_dev_t * dev;
    xSemaphoreHandle lock;
    uint8_t num;

    bool usart_enabled;
    bool usart_transmitting;
    int peek_char;
} ESP32_USART_Info;

/*
 * USART mapping
 */
ESP32_USART_Info USART_MAP[TOTAL_USARTS] =
{
    /*
     * uart number
     * TX pin
     * RX pin
     * <usart enabled> used internally and does not appear below
     * <usart transmitting> used internally and does not appear below
     */
     {UART0, TX, RX, (volatile uart_dev_t *)(DR_REG_UART_BASE), NULL, 0},             // USART0 TX, RX
     {UART2, TX1, RX1, (volatile uart_dev_t *)(DR_REG_UART2_BASE), NULL, 2},           // USART1 D3, D4
};

static ESP32_USART_Info *usartMap[TOTAL_USARTS]; // pointer to USART_MAP[] containing USART peripheral register locations (etc)

struct uart_struct_t {
    uart_dev_t * dev;
    xSemaphoreHandle lock;
    uint8_t num;
    xQueueHandle queue;
};

static uart_struct_t _uart_bus_array[2] = {
    {(volatile uart_dev_t *)(DR_REG_UART_BASE), NULL, 0, NULL},
    {(volatile uart_dev_t *)(DR_REG_UART2_BASE), NULL, 2, NULL}
};

static ESP32_USART_Info *usartMap[TOTAL_USARTS]; // pointer to USART_MAP[] containing USART peripheral register locations (etc)

void HAL_USART_Initial(HAL_USART_Serial serial)
{
    if(serial == HAL_USART_SERIAL1) {
        usartMap[serial] = &USART_MAP[USART_1_3];
    }
    else {
        usartMap[serial] = &USART_MAP[USART_16_17];
    }

    usartMap[serial]->usart_enabled = false;
    usartMap[serial]->usart_transmitting = false;
    usartMap[serial]->peek_char = -1;
}

void HAL_USART_Begin(HAL_USART_Serial serial, uint32_t baud)
{
    HAL_USART_BeginConfig(serial, baudrate, 0, 0);
}

static void IRAM_ATTR _uart_isr(void *arg)
{
    uint8_t i, c;
    BaseType_t xHigherPriorityTaskWoken;
    uart_t* uart;

    for(i=0;i<3;i++){
        uart = &_uart_bus_array[i];
        uart->dev->int_clr.rxfifo_full = 1;
        uart->dev->int_clr.frm_err = 1;
        uart->dev->int_clr.rxfifo_tout = 1;
        while(uart->dev->status.rxfifo_cnt) {
            c = uart->dev->fifo.rw_byte;
            if(uart->queue != NULL && !xQueueIsQueueFullFromISR(uart->queue)) {
                xQueueSendFromISR(uart->queue, &c, &xHigherPriorityTaskWoken);
            }
        }
    }

    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

static void uartEnableGlobalInterrupt()
{
    xt_set_interrupt_handler(ETS_UART_INUM, _uart_isr, NULL);
    ESP_INTR_ENABLE(ETS_UART_INUM);
}

static void uartDisableGlobalInterrupt()
{
    ESP_INTR_DISABLE(ETS_UART_INUM);
    xt_set_interrupt_handler(ETS_UART_INUM, NULL, NULL);
}

static void uartEnableInterrupt(ESP32_USART_Info *info)
{
    UART_MUTEX_LOCK();
    info->dev->conf1.rxfifo_full_thrhd = 112;
    info->dev->conf1.rx_tout_thrhd = 2;
    info->dev->conf1.rx_tout_en = 1;
    info->dev->int_ena.rxfifo_full = 1;
    info->dev->int_ena.frm_err = 1;
    info->dev->int_ena.rxfifo_tout = 1;
    info->dev->int_clr.val = 0xffffffff;

    intr_matrix_set(xPortGetCoreID(), UART_INTR_SOURCE(info->num), ETS_UART_INUM);
    UART_MUTEX_UNLOCK();
}

static void uartDisableInterrupt(ESP32_USART_Info *info)
{
    UART_MUTEX_LOCK();
    info->dev->conf1.val = 0;
    info->dev->int_ena.val = 0;
    info->dev->int_clr.val = 0xffffffff;
    UART_MUTEX_UNLOCK();
}

static void uartDetachRx(ESP32_USART_Info *info)
{
    if(info == NULL) {
        return;
    }
    pinMatrixInDetach(UART_RXD_IDX(info->num), false, false);
    uartDisableInterrupt(info);
}

static void uartDetachTx(ESP32_USART_Info *info)
{
    if(info == NULL) {
        return;
    }
    pinMatrixOutDetach(UART_TXD_IDX(info->num), false, false);
}

static void uartAttachRx(ESP32_USART_Info *info, bool inverted)
{
    if(info == NULL || info->usart_rx_pin > 39) {
        return;
    }
    pinMode(info->usart_rx_pin, INPUT);
    pinMatrixInAttach(info->usart_rx_pin, UART_RXD_IDX(info->num), inverted);
    uartEnableInterrupt(info);
    uartEnableGlobalInterrupt();
}

static void uartAttachTx(ESP32_USART_Info *info, bool inverted)
{
    if(info == NULL || info->usart_tx_pin > 39) {
        return;
    }
    pinMode(info->usart_tx_pin, OUTPUT);
    pinMatrixOutAttach(info->usart_tx_pin, UART_TXD_IDX(info->num), inverted, false);
}

void HAL_USART_BeginConfig(HAL_USART_Serial serial, uint32_t baudrate, uint32_t config_ex, void *ptr)
{
    if( true == usartMap[serial]->usart_enabled ) {
        return;
    }

    // Verify UART configuration, exit if it's invalid.
    if (!IS_USART_CONFIG_VALID(config)) {
        usartMap[serial]->usart_enabled = false;
        return;
    }

    if(usartMap[serial]->lock == NULL) {
        usartMap[serial]->lock = xSemaphoreCreateMutex();
        if(usartMap[serial]->lock == NULL) {
            return;
        }
    }

    // set tx pin 1 mode
    uartAttachTx(usartMap[serial], false);
    //set rx pin 3 mode
    uartAttachRx(usartMap[serial], false);

    uint32 esp32_uart_config = 0;
    // Stop bit configuration.
    switch (config & SERIAL_STOP_BITS) {
        case 0b00: // 1 stop bit
            esp32_uart_config |= UART_NB_STOP_BIT_1;
            break;
        case 0b01: // 2 stop bits
            esp32_uart_config |= UART_NB_STOP_BIT_2;
            break;
    }

    // Eight / Nine data bit configuration
    if (config & SERIAL_NINE_BITS) {
        // Nine data bits, no parity.
        esp32_uart_config |= UART_PARITY_NONE;
        esp32_uart_config |= UART_NB_BIT_8;
    } else {
        // eight data bits, parity configuration (impacts word length)
        switch ((config & SERIAL_PARITY_BITS) >> 2) {
            case 0b00: // none
                esp32_uart_config |= UART_PARITY_NONE;
                esp32_uart_config |= UART_NB_BIT_8;
                break;
            case 0b01: // even
                esp32_uart_config |= UART_PARITY_EVEN;
                esp32_uart_config |= UART_NB_BIT_8;
                break;
            case 0b10: // odd
                esp32_uart_config |= UART_PARITY_ODD;
                esp32_uart_config |= UART_NB_BIT_8;
                break;
        }
    }

    UART_MUTEX_LOCK();
    // set baudrate
    uint32_t clk_div = ((UART_CLK_FREQ<<4)/baud);
    usartMap[serial]->dev->clk_div.div_int = clk_div>>4 ;
    usartMap[serial]->dev->clk_div.div_frag = clk_div & 0xf;
    // uart config
    usartMap[serial]->dev->conf0.val = esp32_uart_config;
    // uart flush
    while(usartMap[serial]->dev->status.txfifo_cnt);
    usartMap[serial]->dev->conf0.txfifo_rst = 1;
    usartMap[serial]->dev->conf0.txfifo_rst = 0;
    usartMap[serial]->dev->conf0.rxfifo_rst = 1;
    usartMap[serial]->dev->conf0.rxfifo_rst = 0;
    UART_MUTEX_UNLOCK();



    USC1(usartMap[serial]->usart_nr) = 0;

    usartMap[serial]->usart_enabled = true;
    usartMap[serial]->usart_transmitting = false;
    usartMap[serial]->peek_char = -1;







    /* if (serial > 2) */
    /* {return;} */
    uint16_t queueLen = 256;
    bool inverted = false;
    uint32_t config = SERIAL_8N1;
    EESP32_Pin_Info* PIN_MAP = HAL_Pin_Map();
    pin_t tx_pin = PIN_MAP[usartMap[serial]->usart_tx_pin].gpio_pin;
    pin_t rx_pin = PIN_MAP[usartMap[serial]->usart_rx_pin].gpio_pin;


    usartMap[serial]->uart = uartBegin(serial, baudrate, config, rx_pin, tx_pin, queueLen, false);

    usartMap[serial]->uart->num = serial;

#if 0
    usartMap[serial]->uart = &_uart_bus_array[serial];
    usartMap[serial]->uart->num = serial;
    printf("serial num : %d \n",usartMap[serial]->uart->num);

#if !CONFIG_DISABLE_HAL_LOCKS

    if(usartMap[serial]->uart->lock == NULL) {
        usartMap[serial]->uart->lock = xSemaphoreCreateMutex();
        if(usartMap[serial]->uart->lock == NULL) {
            return NULL;
        }
    }
#endif

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
#endif
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
    /* while(usartMap[serial]->uart->dev->status.rxfifo_cnt == 0x7F); */
    while(usartMap[serial]->uart->dev->status.txfifo_cnt == 0x7F);
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
        return -1;
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
        return -1;
    }
    uint8_t c;
    if(xQueueReceive(usartMap[serial]->uart->queue, &c, 0)) {
        return c;
    }
    return -1;
}

int32_t HAL_USART_Peek_Data(HAL_USART_Serial serial)
{
    if(usartMap[serial]->uart == NULL || usartMap[serial]->uart->queue == NULL) {
        return -1;
    }
    uint8_t c;
    if(xQueuePeek(usartMap[serial]->uart->queue, &c, 0)) {
        return c;
    }
    return -1;
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
