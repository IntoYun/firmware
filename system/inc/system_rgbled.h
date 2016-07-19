/**
 ******************************************************************************
  Copyright (c) 2015 Particle Industries, Inc.  All rights reserved.

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

#ifndef SYSTEM_RGBLED_H_
#define SYSTEM_RGBLED_H_

#include "system_task.h"
#include "rgbled_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

bool system_rgb_off(void);
bool system_rgb_color(uint8_t red, uint8_t green, uint8_t blue);
bool system_rgb_blink(uint8_t red, uint8_t green, uint8_t blue, uint16_t period);
bool system_rgb_breath(uint8_t red, uint8_t green, uint8_t blue, uint16_t period);

#ifdef __cplusplus
}
#endif

#endif /*SYSTEM_RGBLED_H_*/
