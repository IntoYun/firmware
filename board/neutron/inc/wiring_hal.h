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

#ifndef WIRING_HAL_H_
#define WIRING_HAL_H_

#include "stdint.h"


unsigned int mo_millis_hal(void);


unsigned long mo_micros_hal(void);

void mo_delay_hal(unsigned long ms);

uint8_t mo_timerIsEnd_hal(uint32_t timerID, uint32_t time);


/*
返回脉冲宽度单位微秒
*/
uint32_t mo_pulseIn_hal(uint8_t pin, uint8_t state, uint32_t timeout);



#endif
