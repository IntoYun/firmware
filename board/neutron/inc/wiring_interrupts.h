/**
 ******************************************************************************
 * @file     : wiring_interrupts.h
 * @author   : robin
 * @version	 : V1.0.0
 * @date     : 6-December-2014
 * @brief    : 
 ******************************************************************************
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
#ifndef	  WIRING_INTERRUPTS_H_
#define   WIRING_INTERRUPTS_H_

#include "wiring.h"

/*
*Interrupts
*/

typedef enum InterruptMode 
{
    CHANGE, // 电平变化
    RISING, // 上升沿
    FALLING // 下降沿
} InterruptMode;

typedef void (*voidFuncPtr)(void);

void attachInterrupt(uint16_t pin, voidFuncPtr handler, InterruptMode mode);
void detachInterrupt(uint16_t pin);
void interrupts(void);
void noInterrupts(void);

#endif /* WIRING_INTERRUPTS_H_ */
