/**
 ******************************************************************************
 * @file    esp8266-hal-uart.h
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

#ifndef ESP8266_HAL_UART_H_
#define ESP8266_HAL_UART_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define UART0                     0
#define UART1                     1
#define UART_NO                  -1

struct uart_struct_t;
typedef struct uart_struct_t uart_t;

uart_t* uartBegin(uint8_t uart_nr, uint32_t baudrate, uint32_t config, int8_t rxPin, int8_t txPin, size_t rxSize);
void uartEnd(uart_t* uart);

uint32_t uartAvailable(uart_t* uart);
uint8_t uartRead(uart_t* uart);
int32_t uartPeek(uart_t* uart);

void uartWrite(uart_t* uart, uint8_t c);
void uartWriteBuf(uart_t* uart, const uint8_t * data, size_t len);

void uartFlush(uart_t* uart);

void uartSetBaudRate(uart_t* uart, uint32_t baud_rate);
uint32_t uartGetBaudRate(uart_t* uart);

void uartSetDebug(int uart_nr);
int uartGetDebug();

#ifdef __cplusplus
}
#endif

#endif // ESP8266_HAL_UART_H_
