/**
 ******************************************************************************
 * @file    esp8266-hal-uart.c
 * @authors robin
 * @version V1.0.0
 * @date    2016-05-04
 * @brief
 ******************************************************************************
 Copyright (c) 2013-2017 IntoRobot Industries, Inc.  All rights reserved.

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

/**
 * UART GPIOs
 *
 * UART0 TX: GPIO1
 * UART0 RX: GPIO3
 *
 * UART1 TX: GPIO2
 * UART1 RX: NC
 *
 * NC = Not Connected to Module Pads --> No Access
 *
 */
#include <stdlib.h>
#include "esp8266_peri.h"
#include "user_interface.h"
#include "esp8266-hal-uart.h"
#include "esp8266-hal-gpio.h"

static int s_uart_debug_nr = UART0;

struct uart_rx_buffer_ {
    size_t size;
    size_t rpos;
    size_t wpos;
    uint8_t *buffer;
};

struct uart_struct_t {
    int uart_nr;
    int baud_rate;
    bool rx_enabled;
    struct uart_rx_buffer_ rx_buffer;
};

static uart_t _uart_bus_array[2] = {
    {UART0, 0, true, {0, 0, 0, NULL}},
    {UART1, 0, false, {0, 0, 0, NULL}},
};

static void ICACHE_RAM_ATTR _uart_isr(void * arg)
{
    uart_t* uart = (uart_t*)arg;
    if(uart == NULL || !uart->rx_enabled) {
        USIC(uart->uart_nr) = USIS(uart->uart_nr);
        ETS_UART_INTR_DISABLE();
        return;
    }
    if(USIS(uart->uart_nr) & ((1 << UIFF) | (1 << UITO))){
        while((USS(uart->uart_nr) >> USRXC) & 0x7F){
            uint8_t data = USF(uart->uart_nr);
            size_t nextPos = (uart->rx_buffer.wpos + 1) % uart->rx_buffer.size;
            if(nextPos != uart->rx_buffer.rpos) {
                uart->rx_buffer.buffer[uart->rx_buffer.wpos] = data;
                uart->rx_buffer.wpos = nextPos;
            }
        }
    }
    USIC(uart->uart_nr) = USIS(uart->uart_nr);
}

void uartEnableInterrupt(uart_t* uart)
{
    if(uart == NULL || !uart->rx_enabled) {
        return;
    }
    USC1(uart->uart_nr) = (127 << UCFFT) | (0x02 << UCTOT) | (1 <<UCTOE );
    USIC(uart->uart_nr) = 0xffff;
    USIE(uart->uart_nr) = (1 << UIFF) | (1 << UIFR) | (1 << UITO);
    ETS_UART_INTR_ATTACH(_uart_isr,  (void *)uart);
    ETS_UART_INTR_ENABLE();
}

void uartDisableInterrupt(uart_t* uart)
{
    if(uart == NULL || !uart->rx_enabled) {
        return;
    }
    ETS_UART_INTR_DISABLE();
    USC1(uart->uart_nr) = 0;
    USIC(uart->uart_nr) = 0xffff;
    USIE(uart->uart_nr) = 0;
    ETS_UART_INTR_ATTACH(NULL, NULL);
}

uint32_t uartAvailable(uart_t* uart)
{
    if(uart == NULL || !uart->rx_enabled) {
        return 0;
    }
    if(uart->rx_buffer.wpos < uart->rx_buffer.rpos) {
      return (uart->rx_buffer.wpos + uart->rx_buffer.size) - uart->rx_buffer.rpos;
    }
    return uart->rx_buffer.wpos - uart->rx_buffer.rpos;
}

uint8_t uartRead(uart_t* uart)
{
    int data = uartPeek(uart);
    if(data != -1) {
        uart->rx_buffer.rpos = (uart->rx_buffer.rpos + 1) % uart->rx_buffer.size;
    }
    return data;
}

int32_t uartPeek(uart_t* uart)
{
    if(uart == NULL || !uart->rx_enabled) {
        return -1;
    }
    if (!uartAvailable(uart)) {
        return -1;
    }
    return uart->rx_buffer.buffer[uart->rx_buffer.rpos];
}

void uartWrite(uart_t* uart, uint8_t c)
{
    if(uart == NULL) {
        return;
    }
    while((USS(uart->uart_nr) >> USTXC) >= 0x7f);
    USF(uart->uart_nr) = c;
}

void uartWriteBuf(uart_t* uart, const uint8_t * data, size_t len)
{
    if(uart == NULL) {
        return;
    }
    while(len--) {
        uartWrite(uart, *data++);
    }
}

void uartFlush(uart_t* uart)
{
    if(uart == NULL) {
        return;
    }

    uint32_t tmp = 0x00000000;
    if(uart->rx_enabled) {
        tmp |= (1 << UCRXRST);
        ETS_UART_INTR_DISABLE();
        uart->rx_buffer.rpos = 0;
        uart->rx_buffer.wpos = 0;
        ETS_UART_INTR_ENABLE();
    }
    tmp |= (1 << UCTXRST);

    USC0(uart->uart_nr) |= (tmp);
    USC0(uart->uart_nr) &= ~(tmp);
}

void uartSetBaudRate(uart_t* uart, uint32_t baud_rate)
{
    if(uart == NULL) {
        return;
    }

    uart->baud_rate = baud_rate;
    USD(uart->uart_nr) = (ESP8266_CLOCK / uart->baud_rate);
}

uint32_t uartGetBaudRate(uart_t* uart)
{
    if(uart == NULL) {
        return 0;
    }
    return uart->baud_rate;
}

uart_t* uartBegin(uint8_t uart_nr, uint32_t baudrate, uint32_t config, int8_t rxPin, int8_t txPin, size_t rxSize)
{
    if(uart_nr > 1) {
        return NULL;
    }

    if(rxPin == -1 && txPin == -1) {
        return NULL;
    }

    uart_t* uart = &_uart_bus_array[uart_nr];
    switch(uart->uart_nr) {
        case UART0:
            ETS_UART_INTR_DISABLE();
            ETS_UART_INTR_ATTACH(NULL, NULL);
            uart->rx_buffer.size = rxSize;
            uart->rx_buffer.rpos = 0;
            uart->rx_buffer.wpos = 0;

            if(NULL == uart->rx_buffer.buffer) {
                uart->rx_buffer.buffer = (uint8_t *)malloc(uart->rx_buffer.size);
                if(uart->rx_buffer.buffer == NULL) {
                    return NULL;
                }
            }
            __pinMode(rxPin, ESP8266_SPECIAL);
            __pinMode(txPin, ESP8266_FUNCTION_0);
            IOSWAP &= ~(1 << IOSWAPU0);
            break;
        case UART1:
            __pinMode(txPin, ESP8266_SPECIAL);
            break;
        case UART_NO:
        default:
            return NULL;
    }

    uartSetBaudRate(uart, baudrate);
    USC0(uart->uart_nr) = config;
    uartFlush(uart);
    USC1(uart->uart_nr) = 0;
    USIC(uart->uart_nr) = 0xffff;
    USIE(uart->uart_nr) = 0;
    if(uart->rx_enabled) {
        uartEnableInterrupt(uart);
    }

    return uart;
}

void uartEnd(uart_t* uart)
{
    if(uart == NULL) {
        return;
    }

    if(uart->rx_enabled){
        if(NULL != uart->rx_buffer.buffer) {
            free(uart->rx_buffer.buffer);
            uart->rx_buffer.buffer = NULL;
        }
        uartDisableInterrupt(uart);
    }
}

static void uart_ignore_char(char c)
{
    (void) c;
}

static void uart0_write_char(char c)
{
    while(((USS(0) >> USTXC) & 0xff) >= 0x7F) {
        delay(0);
    }
    USF(0) = c;
}

static void uart1_write_char(char c)
{
    while(((USS(1) >> USTXC) & 0xff) >= 0x7F) {
        delay(0);
    }
    USF(1) = c;
}

void uartSetDebug(int uart_nr)
{
    s_uart_debug_nr = uart_nr;
    switch(s_uart_debug_nr) {
        case UART0:
            system_set_os_print(1);
            ets_install_putc1((void *) &uart0_write_char);
            break;
        case UART1:
            system_set_os_print(1);
            ets_install_putc1((void *) &uart1_write_char);
            break;
        case UART_NO:
        default:
            system_set_os_print(0);
            ets_install_putc1((void *) &uart_ignore_char);
            break;
    }
}

int uartGetDebug()
{
    return s_uart_debug_nr;
}
