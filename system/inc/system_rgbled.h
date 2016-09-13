/**
 ******************************************************************************
  Copyright (c) 2015 IntoRobot Industries, Inc.  All rights reserved.

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

#include <stdint.h>
#include "ui_hal.h"

// This is temporary until we can get scoped LED management in place.
class RGBLEDState
{
    rgb_info_t rgb_info;

public:
    void save()
    {
        HAL_UI_RGB_Get_Info(&rgb_info);
    }

    void restore()
    {
        HAL_UI_RGB_Set_Info(rgb_info);
    }

};

#ifdef __cplusplus
extern "C" {
#endif

bool system_rgb_off(void);
bool system_rgb_color(uint32_t rgb);
bool system_rgb_blink(uint32_t rgb, uint16_t period);
bool system_rgb_breath(uint32_t rgb, uint16_t period);

#ifdef __cplusplus
}
#endif

#endif /*SYSTEM_RGBLED_H_*/
