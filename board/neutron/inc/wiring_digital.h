/**
 ******************************************************************************
 * @file     : wring_digital.h
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

#ifndef WIRING_DIGITAL_H_
#define WIRING_DIGITAL_H_

#include "variant.h"

#ifdef __cplusplus
extern "C" {
#endif

void pinMode(uint16_t pin, PinMode mode);
int32_t digitalRead(uint16_t pin);
void digitalWrite(uint16_t pin, uint8_t value);

#ifdef __cplusplus
}
#endif

#endif /* WIRING_DIGITAL_H_ */