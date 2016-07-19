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

#ifdef __cplusplus
extern "C" {
#endif

void System1MsTick(void);
system_tick_t GetSystem1MsTick();

system_tick_t millis(void);
unsigned long micros(void);
void delay(unsigned long ms);
void DWT_Init(void);
void delayMicroseconds(unsigned int us);
u32 timerGetId(void);
bool timerIsEnd(u32 timerID, u32 time);

#ifdef __cplusplus
}
#endif

#endif /* WIRING_H_ */
