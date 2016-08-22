/**
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
*/

#include "system_rgbled.h"
#include "wiring_rgb.h"

bool system_rgb_off(void)
{
    if(!RGB.controlled())
    {
        HAL_UI_RGB_Color(RGB_COLOR_BLACK);
        return true;
    }
    return false;
}

bool system_rgb_color(uint32_t rgb)
{
    if(!RGB.controlled())
    {
        HAL_UI_RGB_Color(rgb);
        return true;
    }
    return false;
}

bool system_rgb_blink(uint32_t rgb, uint16_t period)
{
    if(!RGB.controlled())
    {
        HAL_UI_RGB_Blink(rgb, period);
        return true;
    }
    return false;
}

bool system_rgb_breath(uint32_t rgb, uint16_t period)
{
    if(!RGB.controlled())
    {
        HAL_UI_RGB_Breath(rgb, period);
        return true;
    }
    return false;
}

