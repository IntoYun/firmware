/**
 ******************************************************************************
 * @file    usart_hal.c
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
#include "hw_config.h"
#include "usart_hal.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"
#include "delay_hal.h"
#include "service_debug.h"

#define UART0    0
#define UART1    1
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

static int s_uart_debug_nr = UART0;

/* Private typedef -----------------------------------------------------------*/
typedef enum USART_Num_Def {
    USART_0 = 0,
    USART_1 = 1
} USART_Num_Def;

/* Private macro -------------------------------------------------------------*/
// IS_USART_CONFIG_VALID(config) - returns true for 8 data bit, any flow control, any parity, any stop byte configurations
#define IS_USART_CONFIG_VALID(CONFIG) (((CONFIG & SERIAL_VALID_CONFIG) >> 2) != 0b11)

/* Private variables ---------------------------------------------------------*/
typedef struct STM32_USART_Info {
    int usart_nr;
    uint8_t usart_tx_pin;
    uint8_t usart_rx_pin;

    bool usart_enabled;
    bool usart_transmitting;
    int peek_char;
} ESP8266_USART_Info;

/*
 * USART mapping
 */
ESP8266_USART_Info USART_MAP[TOTAL_USARTS] =
{
    /*
     * USART_number (UART0, UART1)
     * TX pin
     * RX pin
     * <usart enabled> used internally and does not appear below
     * <usart transmitting> used internally and does not appear below
     */
    { UART0, 1, 3 },           // USART 0
    { UART1, 2, 0xFF }         // USART 1
};

static ESP8266_USART_Info *usartMap[TOTAL_USARTS]; // pointer to USART_MAP[] containing USART peripheral register locations (etc)

static void uart1_write_char(char c)
{
    while(((USS(1) >> USTXC) & 0xff) >= 0x7F) {
        delay(0);
    }
    USF(1) = c;
}

static void uart_ignore_char(char c)
{
}

void HAL_USART_Initial(HAL_USART_Serial serial)
{
    if(serial == HAL_USART_SERIAL1) {
        usartMap[serial] = &USART_MAP[USART_0];
    }
    else {
        usartMap[serial] = &USART_MAP[USART_1];
    }

    usartMap[serial]->usart_enabled = false;
    usartMap[serial]->usart_transmitting = false;
    usartMap[serial]->peek_char = -1;
}

void HAL_USART_Begin(HAL_USART_Serial serial, uint32_t baud)
{
    HAL_USART_BeginConfig(serial, baud, 0, 0); // Default serial configuration is 8N1
}

void HAL_USART_BeginConfig(HAL_USART_Serial serial, uint32_t baud, uint32_t config, void *ptr)
{
    if( true == usartMap[serial]->usart_enabled )
    {
        return;
    }

    // Verify UART configuration, exit if it's invalid.
    if (!IS_USART_CONFIG_VALID(config)) {
        usartMap[serial]->usart_enabled = false;
        return;
    }

    // set debug UART_NO
    system_set_os_print(0);
    ets_install_putc1((void *) &uart_ignore_char);

    if (HAL_USART_SERIAL1 == serial) {
        // set tx pin 1 mode
        GPC(usartMap[serial]->usart_tx_pin) = (GPC(usartMap[serial]->usart_tx_pin) & (0xF << GPCI));
        GPEC = (1 << usartMap[serial]->usart_tx_pin);
        GPF(usartMap[serial]->usart_tx_pin) = GPFFS((0x08 >> 4) & 0x07);
        //set rx pin 3 mode
        GPC(usartMap[serial]->usart_rx_pin) = (GPC(usartMap[serial]->usart_rx_pin) & (0xF << GPCI));
        GPEC = (1 << usartMap[serial]->usart_rx_pin);
        GPF(usartMap[serial]->usart_rx_pin) = GPFFS(GPFFS_BUS(usartMap[serial]->usart_rx_pin));
        GPF(usartMap[serial]->usart_rx_pin) |= (1 << GPFPU); // rx pin = 3

        IOSWAP &= ~(1 << IOSWAPU0);
    }
    else {
        //set tx pin 2 mode
        GPC(usartMap[serial]->usart_tx_pin) = (GPC(usartMap[serial]->usart_tx_pin) & (0xF << GPCI));
        GPEC = (1 << usartMap[serial]->usart_tx_pin);
        GPF(usartMap[serial]->usart_tx_pin) = GPFFS(GPFFS_BUS(usartMap[serial]->usart_tx_pin));
    }

    uint32 esp8266_uart_config = 0;
    // Stop bit configuration.
    switch (config & SERIAL_STOP_BITS) {
        case 0b00: // 1 stop bit
            esp8266_uart_config |= UART_NB_STOP_BIT_1;
            break;
        case 0b01: // 2 stop bits
            esp8266_uart_config |= UART_NB_STOP_BIT_2;
            break;
    }

    // Eight / Nine data bit configuration
    if (config & SERIAL_NINE_BITS) {
        // Nine data bits, no parity.
        esp8266_uart_config |= UART_PARITY_NONE;
        esp8266_uart_config |= UART_NB_BIT_8;
    } else {
        // eight data bits, parity configuration (impacts word length)
        switch ((config & SERIAL_PARITY_BITS) >> 2) {
            case 0b00: // none
                esp8266_uart_config |= UART_PARITY_NONE;
                esp8266_uart_config |= UART_NB_BIT_8;
                break;
            case 0b01: // even
                esp8266_uart_config |= UART_PARITY_EVEN;
                esp8266_uart_config |= UART_NB_BIT_8;
                break;
            case 0b10: // odd
                esp8266_uart_config |= UART_PARITY_ODD;
                esp8266_uart_config |= UART_NB_BIT_8;
                break;
        }
    }

    // set baudrate
    USD(usartMap[serial]->usart_nr) = (ESP8266_CLOCK / baud);
    // uart config
    USC0(usartMap[serial]->usart_nr) = esp8266_uart_config;
    // uart flush
    uint32_t tmp = 0x00000000;

    if (HAL_USART_SERIAL1 == serial){
        tmp |= (1 << UCRXRST); // if rx_enabled
    }
    tmp |= (1 << UCTXRST); // if tx_enabled
    USC0(usartMap[serial]->usart_nr) |= (tmp);
    USC0(usartMap[serial]->usart_nr) &= ~(tmp);

    USC1(usartMap[serial]->usart_nr) = 0;

    usartMap[serial]->usart_enabled = true;
    usartMap[serial]->usart_transmitting = false;
    usartMap[serial]->peek_char = -1;
}

void HAL_USART_End(HAL_USART_Serial serial)
{
    system_set_os_print(0);
    ets_install_putc1((void *) &uart_ignore_char);

    GPF(usartMap[serial]->usart_tx_pin) = GPFFS(GPFFS_GPIO(usartMap[serial]->usart_tx_pin));
    GPEC = (1 << usartMap[serial]->usart_tx_pin);
    GPC(usartMap[serial]->usart_tx_pin) = (GPC(usartMap[serial]->usart_tx_pin) & (0xF << GPCI)) | (1 << GPCD);

    if( HAL_USART_SERIAL1 == serial){
        GPF(usartMap[serial]->usart_rx_pin) = GPFFS(GPFFS_GPIO(usartMap[serial]->usart_rx_pin));
        GPEC = (1 << usartMap[serial]->usart_rx_pin);
        GPC(usartMap[serial]->usart_rx_pin) = (GPC(usartMap[serial]->usart_rx_pin) & (0xF << GPCI)) | (1 << GPCD);
    }

    usartMap[serial]->usart_enabled = false;
    usartMap[serial]->usart_transmitting = false;
    usartMap[serial]->peek_char = -1;
}

uint32_t HAL_USART_Write_Data(HAL_USART_Serial serial, uint8_t data)
{
    if (!usartMap[serial] || !(usartMap[serial]->usart_enabled)) {
        return 0;
    }
    while((USS(usartMap[serial]->usart_nr) >> USTXC) >= 0x7f);
    USF(usartMap[serial]->usart_nr) = data;
    return 1;
}

uint32_t HAL_USART_Write_NineBitData(HAL_USART_Serial serial, uint16_t data)
{
    return 1;
}

int32_t HAL_USART_Available_Data(HAL_USART_Serial serial)
{
    // if serial refer to uart1, not right, uart1 don't have rx pin
    if ( ( HAL_USART_SERIAL2 == serial) || !usartMap[serial] || !usartMap[serial]->usart_enabled) {
        return 0;
    }

    int result = static_cast<int>((USS(usartMap[serial]->usart_nr) >> USRXC) & 0xff);
    if (usartMap[serial]->peek_char != -1) {
        result += 1;
    }
    if (!result) {
        optimistic_yield(10000);
    }
    return result;
}

int32_t HAL_USART_Available_Data_For_Write(HAL_USART_Serial serial)
{
    if (!usartMap[serial] || !usartMap[serial]->usart_enabled) {
        return 0;
    }
    return static_cast<int>(UART_TX_FIFO_SIZE - ((USS(usartMap[serial]->usart_nr) >> USTXC) & 0xff));
}

int32_t HAL_USART_Read_Data(HAL_USART_Serial serial)
{
    if ((HAL_USART_SERIAL1 != serial) || !usartMap[serial] || !usartMap[serial]->usart_enabled){
        return -1;
    }

    if (usartMap[serial]->peek_char != -1) {
        auto tmp = usartMap[serial]->peek_char;
        usartMap[serial]->peek_char = -1;
        return tmp;
    }

    if (!HAL_USART_Available_Data(serial)) {
        return -1;
    }

    return USF(usartMap[serial]->usart_nr) & 0xff;
}

int32_t HAL_USART_Peek_Data(HAL_USART_Serial serial)
{
    if ((HAL_USART_SERIAL1 != serial) || !usartMap[serial] || !usartMap[serial]->usart_enabled){
        return -1;
    }

    if (usartMap[serial]->peek_char != -1) {
        return usartMap[serial]->peek_char;
    }
    usartMap[serial]->peek_char = USF(usartMap[serial]->usart_nr) & 0xff;
    return usartMap[serial]->peek_char;
}

void HAL_USART_Flush_Data(HAL_USART_Serial serial)
{
    if (!usartMap[serial] || !usartMap[serial]->usart_enabled) {
        return;
    }

    while(((USS(usartMap[serial]->usart_nr) >> USTXC) & 0xff) >> 0) {
        HAL_Delay_Milliseconds(0);
    }
}

bool HAL_USART_Is_Enabled(HAL_USART_Serial serial)
{
    return usartMap[serial]->usart_enabled;
}

void HAL_USART_Half_Duplex(HAL_USART_Serial serial, bool Enable)
{
}
