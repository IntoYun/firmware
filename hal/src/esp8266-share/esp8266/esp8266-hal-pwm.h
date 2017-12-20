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

#ifndef ESP8266_HAL_PWM_H_
#define ESP8266_HAL_PWM_H_


#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif

void ICACHE_RAM_ATTR pwm_stop_pin(uint8_t pin);
extern void __analogWrite(uint8_t pin, int value);
extern void __analogWriteFreq(uint32_t freq);
extern void __analogWriteRange(uint32_t range);
extern uint32_t __analogWriteGetFreq(void);
extern uint16_t __analogWriteGetValue(uint8_t pin);
extern uint32_t __analogWriteGetRange(void);

#ifdef __cplusplus
}
#endif

#endif /* ESP8266_HAL_PWM_H_ */
