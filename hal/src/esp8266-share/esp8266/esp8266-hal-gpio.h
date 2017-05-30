/**
 ******************************************************************************
 * @file    esp8266-hal-gpio.h
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

#ifndef ESP8266_HAL_GPIO_H_
#define ESP8266_HAL_GPIO_H_


#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "c_types.h"


#ifdef __cplusplus
extern "C" {
#endif

#define LOW                             0x0
#define HIGH                            0x1

//GPIO FUNCTIONS
#define ESP8266_INPUT                   0x00
#define ESP8266_INPUT_PULLUP            0x02
#define ESP8266_INPUT_PULLDOWN_16       0x04 // PULLDOWN only possible for pin16
#define ESP8266_OUTPUT                  0x01
#define ESP8266_OUTPUT_OPEN_DRAIN       0x03
#define ESP8266_WAKEUP_PULLUP           0x05
#define ESP8266_WAKEUP_PULLDOWN         0x07
#define ESP8266_SPECIAL                 0xF8 //defaults to the usable BUSes uart0rx/tx uart1tx and hspi
#define ESP8266_FUNCTION_0              0x08
#define ESP8266_FUNCTION_1              0x18
#define ESP8266_FUNCTION_2              0x28
#define ESP8266_FUNCTION_3              0x38
#define ESP8266_FUNCTION_4              0x48
#define ESP8266_ANALOG                  0xC0

//Interrupt Modes
#define ESP8266_DISABLED                0x00
#define ESP8266_RISING                  0x01
#define ESP8266_FALLING                 0x02
#define ESP8266_CHANGE                  0x03
#define ESP8266_ONLOW                   0x04
#define ESP8266_ONHIGH                  0x05
#define ESP8266_ONLOW_WE                0x0C
#define ESP8266_ONHIGH_WE               0x0D


typedef void (*voidFuncPtr)(void);

extern void __pinMode(uint8_t pin, uint8_t mode);
extern void ICACHE_RAM_ATTR __digitalWrite(uint8_t pin, uint8_t val);
extern int ICACHE_RAM_ATTR __digitalRead(uint8_t pin);
extern void ICACHE_RAM_ATTR __attachInterrupt(uint8_t pin, voidFuncPtr userFunc, int mode);
extern void ICACHE_RAM_ATTR __detachInterrupt(uint8_t pin);


#ifdef __cplusplus
}
#endif

#endif /* ESP8266_HAL_GPIO_H_ */
