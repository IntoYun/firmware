/**
 ******************************************************************************
 * @file     : lib_console.cpp
 * @author   : robin
 * @version  : V1.0.0
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

#include "lib_console.h"

// Default constructor uses global Bridge instance
/*********************************************************************************
  *Function          :   ConsoleClass::ConsoleClass()     
  *Description      :   constructor function
                              Process is the base class for all Bridge based calls for communicating with the openwrt's shell.
                              It is not called directly, but invoked whenever you use a function that relies on it.
  *Input              :    none
  *Output            :    none
  *Return            :     none   
  *author            :     robot   
  *date               :     2015-04-03      
  *Others            :    
**********************************************************************************/
ConsoleClass::ConsoleClass() :
    bridge(Bridge), inBuffered(0), inReadPos(0), inBuffer(NULL),autoFlush(true)
{
    // Empty
}

/*********************************************************************************
  *Function          :     ConsoleClass::ConsoleClass(BridgeClass &_b)  
  *Description      :     constructor function
  *Input              :
  *Output            :         
  *Return            :        
  *author            :        
  *date               :           
  *Others            :    
**********************************************************************************/
ConsoleClass::ConsoleClass(BridgeClass &_b) :
    bridge(_b), inBuffered(0), inReadPos(0), inBuffer(NULL),autoFlush(true)
{
    // Empty
}
    
/*********************************************************************************
  *Function          :     ConsoleClass::ConsoleClass(BridgeClass &_b)  
  *Description      :     destructor function
  *Input              :
  *Output            :         
  *Return            :        
  *author            :        
  *date               :           
  *Others            :    
**********************************************************************************/
ConsoleClass::~ConsoleClass() 
{
    end();
}

/*********************************************************************************
  *Function          :   size_t ConsoleClass::write(uint8_t c)     
  *Description      :   Writes data to a Linux process. This data is sent as a byte or series of bytes. 
                              write() inherits from the Stream utility class.
  *Input              :    
  *Output            :         
  *Return            :        
  *author            :        
  *date               :           
  *Others            :        
**********************************************************************************/
size_t ConsoleClass::write(uint8_t c) 
{
    if (autoFlush) 
    {
        uint8_t tmp[] = { 'P', c };
        bridge.transfer(tmp, 2);
        return 1;
    } 
    else 
    {
        outBuffer[outBuffered++] = c;
        if (outBuffered == outBufferSize)
        flush();
    }
    return 1;
}

/*********************************************************************************
  *Function          :       
  *Description      :
  *Input              :
  *Output            :         
  *Return            :        
  *author            :        
  *date               :           
  *Others            :       
**********************************************************************************/
size_t ConsoleClass::write(const uint8_t *buff, size_t size) 
{
    if (autoFlush) 
    {
        //TODO: do it in a more efficient way
        uint8_t *tmp = new uint8_t[size + 1];
        tmp[0] = 'P';
        memcpy(tmp + 1, buff, size);
        bridge.transfer(tmp, size + 1);
        delete[] tmp;
        return size;
    } 
    else 
    {
        while (size > 0) 
        {
            outBuffer[outBuffered++] = *buff++;
            size--;
            if (outBuffered == outBufferSize)
            flush();
        }
        return size;
    }
}

/*********************************************************************************
  *Function          :       
  *Description      :
  *Input              :
  *Output            :         
  *Return            :        
  *author            :        
  *date               :           
  *Others            :         
**********************************************************************************/
void ConsoleClass::flush(void) 
{
    if (autoFlush)
        return;

    bridge.transfer(outBuffer, outBuffered);
    outBuffered = 1;
}

/*********************************************************************************
  *Function          :       
  *Description      :
  *Input              :
  *Output            :         
  *Return            :        
  *author            :        
  *date               :           
  *Others            :       
**********************************************************************************/
void ConsoleClass::noBuffer(void) 
{
    if (autoFlush)
        return;
    delete[] outBuffer;
    autoFlush = true;
}

/*********************************************************************************
  *Function          :       
  *Description      :
  *Input              :
  *Output            :         
  *Return            :        
  *author            :        
  *date               :           
  *Others            :      
**********************************************************************************/
void ConsoleClass::buffer(uint8_t size) 
{
    noBuffer();
    if (size == 0)
        return;
    outBuffer = new uint8_t[size + 1];
    outBuffer[0] = 'P'; // WRITE tag
    outBufferSize = size + 1;
    outBuffered = 1;
    autoFlush = false;
}

/*********************************************************************************
  *Function          :       
  *Description      :
  *Input              :
  *Output            :         
  *Return            :        
  *author            :        
  *date               :           
  *Others            :         
**********************************************************************************/
bool ConsoleClass::connected(void) 
{
    uint8_t tmp = 'a';
    bridge.transfer(&tmp, 1, &tmp, 1);
    return tmp == 1;
}

/*********************************************************************************
  *Function          :       
  *Description      :
  *Input              :
  *Output            :         
  *Return            :        
  *author            :        
  *date               :           
  *Others            :       
**********************************************************************************/
int ConsoleClass::available(void) 
{
    // Look if there is new data available
    doBuffer();
    return inBuffered;
}

/*********************************************************************************
  *Function          :       
  *Description      :
  *Input              :
  *Output            :         
  *Return            :        
  *author            :        
  *date               :           
  *Others            :     
**********************************************************************************/
int ConsoleClass::read(void) 
{
    doBuffer();
    if (inBuffered == 0)
    return -1; // no chars available
    else 
    {
        inBuffered--;
        return inBuffer[inReadPos++];
    }
}

/*********************************************************************************
  *Function          :       
  *Description      :
  *Input              :
  *Output            :         
  *Return            :        
  *author            :        
  *date               :           
  *Others            :     
**********************************************************************************/
int ConsoleClass::peek(void) 
{
    doBuffer();
    if (inBuffered == 0)
        return -1; // no chars available
    else
        return inBuffer[inReadPos];
}

/*********************************************************************************
  *Function          :       
  *Description      :
  *Input              :
  *Output            :         
  *Return            :        
  *author            :        
  *date               :           
  *Others            :       
**********************************************************************************/
void ConsoleClass::doBuffer(void) 
{
    // If there are already char in buffer exit
    if (inBuffered > 0)
    return;

    // Try to buffer up to 32 characters
    inReadPos = 0;
    uint8_t tmp[] = { 'p', BUFFER_SIZE };
    inBuffered = bridge.transfer(tmp, 2, inBuffer, BUFFER_SIZE);
}

/*********************************************************************************
  *Function          :       
  *Description      :
  *Input              :
  *Output            :         
  *Return            :        
  *author            :        
  *date               :           
  *Others            :       
**********************************************************************************/
void ConsoleClass::begin(void) 
{
    bridge.begin();
    end();
    inBuffer = new uint8_t[BUFFER_SIZE];
}

/*********************************************************************************
  *Function          :       
  *Description      :
  *Input              :
  *Output            :         
  *Return            :        
  *author            :        
  *date               :           
  *Others            :       
**********************************************************************************/
void ConsoleClass::end(void) 
{
    noBuffer();
    if (inBuffer) 
    {
        delete[] inBuffer;
        inBuffer = NULL;
    }
}

ConsoleClass Console;
