/**
 ******************************************************************************
 * @file     : wiring_usbserial.cpp
 * @author   : robin
 * @version	 : V1.0.0
 * @date     : 6-December-2014
 * @brief    :  
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

#include "wiring_usbserial.h"
extern "C" {
#include "usb_conf.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "usb_prop.h"
}

/*********************************************************************************
  *Function		: USBSerial()  
  *Description	: Constructor
  *Input		      : none         
  *Output		: none        
  *Return		: none       
  *author		: lz       
  *date			: 2015-2-1          
  *Others		: none        
**********************************************************************************/
USBSerial::USBSerial()
{
}

/*********************************************************************************
  *Function		: void USBSerial::begin(void)       
  *Description	: usb virtual com begin  
  *Input		      : none         
  *Output		: none        
  *Return		: none       
  *author		: lz       
  *date			: 2015-2-1          
  *Others		: none        
**********************************************************************************/
void USBSerial::begin(void)
{
    begin(115200);
}

/*********************************************************************************
  *Function		: void USBSerial::begin(long speed)       
  *Description	: usb virtual com begin 
  *Input		      : speed set Baud rate         
  *Output		: none        
  *Return		: none       
  *author		: lz       
  *date			: 2015-2-1          
  *Others		: none        
**********************************************************************************/
void USBSerial::begin(long speed)
{
    if (started)
    return;
    started = true;
    USB_USART_Init(speed);
}

/*********************************************************************************
  *Function		: void USBSerial::end(void)       
  *Description	: none  
  *Input		      : none         
  *Output		: none        
  *Return		: none       
  *author		: lz       
  *date			: 2015-2-1          
  *Others		: none        
**********************************************************************************/
void USBSerial::end(void)
{
    if (started)
        started = false;
    //To Do
}

/*********************************************************************************
  *Function		: int USBSerial::read(void)       
  *Description	: Read data from buffer  
  *Input		      : none         
  *Output		: none        
  *Return		: data       
  *author		: lz       
  *date			: 2015-2-1          
  *Others		: none        
**********************************************************************************/
int USBSerial::read(void)
{ 
    if (!started)
        return -2;
    return USB_USART_Receive_Data();
}

/*********************************************************************************
  *Function		: int USBSerial::available(void)       
  *Description	: Return the length of available data received from USB.  
  *Input		      : none         
  *Output		: none        
  *Return		: 1 or 0       
  *author		: lz       
  *date			: 2015-2-1          
  *Others		: none        
**********************************************************************************/
int USBSerial::available(void)
{
    return USB_USART_Available_Data();
}

/*********************************************************************************
  *Function		: size_t USBSerial::write(uint8_t byte)      
  *Description	: usb send data  
  *Input		      : data         
  *Output		: none        
  *Return		: 1       
  *author		: lz      
  *date			: 2015-2-1           
  *Others		: none        
**********************************************************************************/
size_t USBSerial::write(uint8_t byte)
{
    if (!started)
        return -2;
    USB_USART_Send_Data(byte);
    return 1;
}

/*********************************************************************************
  *Function		: void USBSerial::flush(void)   
  *Description	: Clears the buffer of any bytes.
  *Input		      : none         
  *Output		: none        
  *Return		: 1       
  *author		: lz      
  *date			: 2015-2-1           
  *Others		: none        
**********************************************************************************/
void USBSerial::flush(void)
{
    //To Do
}

/*********************************************************************************
  *Function		: int USBSerial::peek(void)
  *Description	: Returns the next byte (character) of incoming data without removing it from the internal buffer. 
  *Input		      : none         
  *Output		: none        
  *Return		: int : the first byte of incoming data available (or -1 if no data is available)  
  *author		: lz      
  *date			: 2015-2-1           
  *Others		: none        
**********************************************************************************/
int USBSerial::peek(void)
{
    return -1;
}

/*********************************************************************************
  *Function		: USBSerial::operator bool(void)        
  *Description	: check the usb serial port is ready
  *Input		      : none         
  *Output		: none        
  *Return		: true: ready    false: not ready       
  *author		: lz       
  *date			: 2015-2-1          
  *Others		: none        
**********************************************************************************/
USBSerial::operator bool(void) 
{
    return true;
}

USBSerial SerialUSB;

