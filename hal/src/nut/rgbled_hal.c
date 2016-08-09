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

#include "rgbled_hal.h"
//RGB Basic Colors
//in SK6812 GRB
#define RGB_COLOR_RED		0x00FF00
#define RGB_COLOR_GREEN		0xFF0000
#define RGB_COLOR_BLUE		0x0000FF
#define RGB_COLOR_YELLOW	0xFFFF00
#define RGB_COLOR_CYAN		0xFF00FF
#define RGB_COLOR_MAGENTA	0x00FFFF
#define RGB_COLOR_WHITE		0xFFFFFF
#define RGB_COLOR_ORANGE    0x60FF00
#define RGB_COLOR_GREY      0x1F1F1F
//Extended LED Types
#define LED_RGB				LED3_LED4_LED2
#define LED_USER			LED1


void HAL_LED_Initial(void)
{

}

void HAL_LED_RGB_Color(uint8_t red, uint8_t green, uint8_t blue)
{
}

void HAL_LED_RGB_Blink(uint8_t red, uint8_t green, uint8_t blue, uint16_t period)
{
}

void HAL_LED_RGB_Breath(uint8_t red, uint8_t green, uint8_t blue, uint16_t period)
{
}

void HAL_LED_User(uint8_t state)
{

}

void HAL_LED_Toggle_User(void)
{

}

