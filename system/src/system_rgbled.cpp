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

#include "ui_hal.h"

void system_rgb_off(void)
{
    HAL_UI_RGB_Color(0, 0, 0);
}

void system_rgb_color(uint8_t red, uint8_t green, uint8_t blue)
{
    HAL_UI_RGB_Color(red, green, blue);
}

void system_rgb_blink(uint8_t red, uint8_t green, uint8_t blue, uint16_t period)
{
    HAL_UI_RGB_Blink(red, green, blue, period);
}

void system_rgb_breath(uint8_t red, uint8_t green, uint8_t blue, uint16_t period)
{
    HAL_UI_RGB_Breath(red, green, blue, period);
}

