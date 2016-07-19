/**
 ******************************************************************************
 * @file        :  openwrt_pin.cpp
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
#include "openwrt_pin.h"


/*********************************************************************************
  *Function		:     OpenwrtPinClass::OpenwrtPinClass()
  *Description	:     constructor function
  *Input		      :     none     
  *Output		:     none    
  *Return		:     none  
  *author		:     robot  
  *date			:     2015-02-01     
  *Others		:               
**********************************************************************************/
OpenwrtPinClass::OpenwrtPinClass()
{
}

/*********************************************************************************
  *Function		:   OpenwrtPinClass::OpenwrtPinClass()
  *Description	:   destructor function
  *Input		      :   none     
  *Output		:   none    
  *Return		:   none  
  *author		:   robot  
  *date			:   2015-02-01     
  *Others		:          
**********************************************************************************/
OpenwrtPinClass::~OpenwrtPinClass()
{
}

/*********************************************************************************
  *Function		:    void OpenwrtPinClass::openwrt_pininit(void) 
  *Description	:    pin init output low
  *Input		      :    none
  *Output		:    none    
  *Return		:    none 
  *author		:    robot    
  *date			:    2015-02-01        
  *Others		:                
**********************************************************************************/
void OpenwrtPinClass::openwrt_pininit(void)
{
    begin("openwrt_digital");
    addParameter("init");
    run();
}

/*********************************************************************************
  *Function		:    void OpenwrtPinClass::Openwrt_digital_write(uint16_t pin, uint8_t value) 
  *Description	:    write openwrt GPIO pin
  *Input		      :    pin(uint16_t): write the GPIO pin
                              value(uint8_t): the GPIO pin value 
  *Output		:    none    
  *Return		:    none 
  *author		:    robot    
  *date			:    2015-02-01        
  *Others		:                
**********************************************************************************/
void OpenwrtPinClass::openwrt_pinmode(uint16_t pin, PinMode setMode)
{
    begin("openwrt_digital");
    addParameter("mode");
    switch(pin)
    {
        case E0:
            addParameter("E0");break;
        case E1:
            addParameter("E1");break;
        case E2:
            addParameter("E2");break;
        case E3:
            addParameter("E3");break;
        case E4:
            addParameter("E4");break;
        default:
            return;break;
    }
    switch(setMode)
    {
        case INPUT:
            addParameter("INPUT");break;           
        default:
            addParameter("OUTPUT");break;
    }
    run();
}

/*********************************************************************************
  *Function		:    void OpenwrtPinClass::Openwrt_digital_write(uint16_t pin, uint8_t value) 
  *Description	:    write openwrt GPIO pin
  *Input		      :    pin(uint16_t): write the GPIO pin
                              value(uint8_t): the GPIO pin value 
  *Output		:    none    
  *Return		:    none 
  *author		:    robot    
  *date			:    2015-02-01        
  *Others		:                
**********************************************************************************/
void OpenwrtPinClass::openwrt_digital_write(uint16_t pin, uint8_t value)
{
    begin("openwrt_digital");
    addParameter("write");
    switch(pin)
    {
        case E0:
            addParameter("E0");break;
        case E1:
            addParameter("E1");break;
        case E2:
            addParameter("E2");break;
        case E3:
            addParameter("E3");break;
        case E4:
            addParameter("E4");break;
        default:
            return;break;
    }
    addParameter(String(value));
    run();
}

/*********************************************************************************
  *Function		:    int32_t OpenwrtPinClass::Openwrt_digital_read(uint16_t pin)
  *Description	:    read the openwrt GPIO pin
  *Input		      :    pin(uint16_t): write the GPIO pin
  *Output		:    none    
  *Return		:    the value of the GPIO pin  status
  *author		:    robot    
  *date			:    2015-02-01        
  *Others		:                 
**********************************************************************************/
int32_t OpenwrtPinClass::openwrt_digital_read(uint16_t pin)
{
    String tmp;
    int32_t pinValue = 0;

    begin("openwrt_digital");
    addParameter("read");
    switch(pin)
    {
        case E0:
            addParameter("E0");break;
        case E1:
            addParameter("E1");break;
        case E2:
            addParameter("E2");break;
        case E3:
            addParameter("E3");break;
        case E4:
            addParameter("E4");break;
        default:
            return 0;break;
    }

    int res = run();
    if(res == 0) 
    {   
        while (available()) 
        {
            tmp+=(char)read();
        }
        pinValue=tmp.toInt();
    }
    return pinValue;
}

OpenwrtPinClass OpenwrtPin;

