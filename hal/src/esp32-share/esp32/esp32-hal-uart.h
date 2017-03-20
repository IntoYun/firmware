// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef MAIN_ESP32_HAL_UART_H_
#define MAIN_ESP32_HAL_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

struct uart_struct_t;
typedef struct uart_struct_t uart_t;

uart_t* uartBegin(uint8_t uart_nr, uint32_t baudrate, uint32_t config, int8_t rxPin, int8_t txPin, uint16_t queueLen, bool inverted);
void uartEnd(uart_t* uart);

uint32_t uartAvailable(uart_t* uart);
uint8_t uartRead(uart_t* uart);
uint8_t uartPeek(uart_t* uart);

void uartWrite(uart_t* uart, uint8_t c);
void uartWriteBuf(uart_t* uart, const uint8_t * data, size_t len);

void uartFlush(uart_t* uart);

void uartSetBaudRate(uart_t* uart, uint32_t baud_rate);
uint32_t uartGetBaudRate(uart_t* uart);

void uartSetDebug(uart_t* uart);
int uartGetDebug();

#ifdef __cplusplus
}
#endif

#endif /* MAIN_ESP32_HAL_UART_H_ */
