/**
 ******************************************************************************
 * @file        :  lib_rgb.cpp
 * @author   :  robin
 * @version  :  V1.0.0
 * @date      :  2015-02-01
 * @brief      :
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


#include "lib_rgb_hal.h"
#include "lib_rgb.h"
//#include "lib_system_all.h"



/*********************************************************************************
  *Function		:     RGBClass()
  *Description	:     constructor function
  *Input              :     none
  *Output		:     none
  *Return		:     none
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
RGBClass::RGBClass()
{
    _control=true;
    //mo_RGBClass_hal();
}

/*********************************************************************************
  *Function		:   ~RGBClass()
  *Description	:   destructor function
  *Input              :   none
  *Output		:   none
  *Return		:   none
  *author		:   robot
  *date			:   2015-02-01
  *Others		:
**********************************************************************************/
RGBClass::~RGBClass()
{

}

/*********************************************************************************
  *Function		:   bool controlled(void)
  *Description	:   whether rgb led control by user
  *Input              :   none
  *Output		:   none
  *Return		:   true: control   false: not control
  *author		:   robot
  *date			:   2015-02-01
  *Others		:
**********************************************************************************/
bool RGBClass::controlled(void)
{
    return _control;
}

/*********************************************************************************
  *Function		:   void control(bool flag)
  *Description	:   user get the rgb led control right
  *Input              :   true: user get the rgb control right       false: not control
  *Output		:   none
  *Return		:   none
  *author		:   robot
  *date			:   2015-02-01
  *Others		:
**********************************************************************************/
void RGBClass::control(bool flag)
{
    _control=flag;
}


/*********************************************************************************
  *Function		:     bool off(void)
  *Description	:     turn off the rgb led
  *Input              :     none
  *Output		:     none
  *Return		:     true:  success       false: fail
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
bool RGBClass::off(void)
{
    if(!_control)
    {return false;}

    mo_RGBClass_off_hal();

    return true;
}

/*********************************************************************************
  *Function		:   bool color(uint32_t rgb)
  *Description	:   control the rgb led color
  *Input              :   rgb(uint32_t):  the rgb led
  *Output		:   none
  *Return		:   true: success    false: fail
  *author		:   robot
  *date			:   2015-02-01
  *Others		:
**********************************************************************************/
bool RGBClass::color(uint32_t rgb)
{
    return color((rgb>>16)&0xFF, (rgb>>8)&0xFF, (rgb)&0xFF);
}

/*********************************************************************************
  *Function		:    bool color(uint8_t red, uint8_t green, uint8_t blue)
  *Description	:    control the rgb led color
  *Input              :    red(uint8_t): the red led brightness     255: the max value
                              green(uint8_t): the green led brightness
                              blue(uint8_t): the blue led brightness
  *Output		:    none
  *Return		:    true: success   false: fail
  *author		:    robot
  *date			:    2015-02-01
  *Others		:
**********************************************************************************/
bool RGBClass::color(uint8_t red, uint8_t green, uint8_t blue)
{


    //	MO_ERROR(("test:%d",__LINE__));
    if(!_control)
    {return false;}
    //	MO_ERROR(("test:%d",__LINE__));
    off();
    //	MO_ERROR(("test:%d",__LINE__));
    mo_RGBClass_color_hal(red,  green,  blue);

    return true;
}

/*********************************************************************************
  *Function		:   bool blink(uint32_t rgb, uint16_t period)
  *Description	:   control the rgb led blink at a particular frequency
  *Input              :   rgb(uint32_t): rgb color
                             period(uint16_t): blink period
  *Output		:   none
  *Return		:   true: success   false: false
  *author		:   robot
  *date			:   2015-02-01
  *Others		:
**********************************************************************************/
bool RGBClass::blink(uint32_t rgb, uint16_t period)
{
    return blink((rgb>>16)&0xFF, (rgb>>8)&0xFF, (rgb)&0xFF, period);
}

/*********************************************************************************
  *Function		:    bool blink(uint8_t red, uint8_t green, uint8_t blue, uint16_t period)
  *Description	:    control the rgb led blink at a particular frequency
  *Input              :    red(uint8_t): the red led brightness     255: the max value
                              green(uint8_t): the green led brightness
                              blue(uint8_t): the blue led brightness
                              period(uint16_t): blink period
  *Output		:    none
  *Return		:    true: success   false: false
  *author		:    robot
  *date			:    2015-02-01
  *Others		:
**********************************************************************************/
bool RGBClass::blink(uint8_t red, uint8_t green, uint8_t blue, uint16_t period)
{
    if(!_control)
    {return false;}

    off();


    mo_RGBClass_blink_hal( red,  green,  blue,  period);
    return true;
}

/*********************************************************************************
  *Function		:    bool breath(uint32_t rgb, uint16_t period)
  *Description	:    control the rgb led breath at a particular frequency
  *Input              :    rgb(uint32_t): rgb color
                              period(uint16_t): breath period
  *Output		:    none
  *Return		:    true: success   false: false
  *author		:    robot
  *date			:    2015-02-01
  *Others		:
**********************************************************************************/
bool RGBClass::breath(uint32_t rgb, uint16_t period)
{
    return breath((rgb>>16)&0xFF, (rgb>>8)&0xFF, (rgb)&0xFF, period);
}

/*********************************************************************************
  *Function		:    bool breath(uint8_t red, uint8_t green, uint8_t blue, uint16_t period)
  *Description	:    control the rgb led breath at a particular frequency
  *Input              :    red(uint8_t): the red led brightness    255: the max value
                              green(uint8_t): the green led brightness
                              blue(uint8_t): the blue led brightness
                              period(uint16_t): breath period
  *Output		:    none
  *Return		:    true: success   false: false
  *author		:    robot
  *date			:    2015-02-01
  *Others		:
**********************************************************************************/
bool RGBClass::breath(uint8_t red, uint8_t green, uint8_t blue, uint16_t period)
{
    if(!_control)
    {return false;}

    off();


    mo_RGBClass_breath_hal( red,  green,  blue,  period);

    return true;
}

RGBClass RGB;
