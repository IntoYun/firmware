/**
 ******************************************************************************
 * @file       :  lib_rgb.h
 * @author  :  robot
 * @version :  V1.0.0
 * @date     :  2015-02-01
 * @brief     :    
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

#ifndef   LIB_RGB_H_
#define   LIB_RGB_H_

#include <stdint.h>
#include "stdbool.h"
//#include "lib_process.h"


//RGB Basic Colors
#define RGB_COLOR_RED		0xFF0000
#define RGB_COLOR_GREEN		0x00FF00
#define RGB_COLOR_BLUE		0x0000FF
#define RGB_COLOR_YELLOW	0xFFFF00
#define RGB_COLOR_CYAN		0x00FFFF
#define RGB_COLOR_MAGENTA	0xFF00FF
#define RGB_COLOR_WHITE		0xFFFFFF
#define RGB_COLOR_ORANGE    0xFF6000

class RGBClass
{
    public:
        RGBClass();
        ~RGBClass();

        bool controlled(void);
        void control(bool flag);
        bool off(void);
        bool color(uint32_t rgb);
        bool color(uint8_t red, uint8_t green, uint8_t blue);
        bool blink(uint32_t rgb,uint16_t period);
        bool blink(uint8_t red, uint8_t green, uint8_t blue,uint16_t period);
        bool breath(uint32_t rgb,uint16_t period);
        bool breath(uint8_t red, uint8_t green, uint8_t blue,uint16_t period);

    private:
        bool _control;
};

extern RGBClass RGB;

#endif /*LIB_RGB_H_*/

