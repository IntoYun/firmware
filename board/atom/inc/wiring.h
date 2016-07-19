/**
 ******************************************************************************
 * @file     : wring.h
 * @author   : robin
 * @version	 : V1.0.0
 * @date     : 6-December-2014
 * @brief    :  
 *******************************************************************************
  Copyright (c) 2013-2014 IntoRobot Team.  All right reserved.

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
**/

#ifndef WIRING_H_
#define WIRING_H_

#include "variant.h"  
#include "wiring_character.h"
#include "wiring_constants.h"
#include "wiring_random.h"


#ifdef __cplusplus
extern "C" {
#endif


void System1MsTick(void);
system_tick_t GetSystem1MsTick();

/*
* ADC
*/
void setADCSampleTime(uint8_t ADC_SampleTime);

/*
* GPIO
*/
void pinMode(uint16_t pin, PinMode mode);
int32_t digitalRead(uint16_t pin);
void digitalWrite(uint16_t pin, uint8_t value);
int32_t analogRead(uint16_t pin);
void analogWrite(uint16_t pin, uint8_t value);
void analogWriteAndSetF(uint16_t pin, uint8_t value,uint32_t fre);

/*
* Timing
*/
system_tick_t millis(void);
unsigned long micros(void);
void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);
u32 timerGetId(void);
bool timerIsEnd(u32 timerID, u32 time);

/*
*shift
*/
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);
uint32_t pulseIn(uint8_t pin, uint8_t state, uint32_t ulTimeout = 1000000L);

#ifdef __cplusplus
}
#endif

#endif /* WIRING_H_ */
