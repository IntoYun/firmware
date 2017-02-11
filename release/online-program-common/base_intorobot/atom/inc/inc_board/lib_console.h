/**
 ******************************************************************************
 * @file     : lib_console.h
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
#ifndef   LIB_CONSOLE_H_
#define   LIB_CONSOLE_H_

#include "lib_bridge.h"

#define SP_CONSOLE      "CO"
#define CO_WRITE        "W"
#define CO_READ         "R"
#define CO_CONNECT      "C"

class ConsoleClass : public Stream 
{
    public:
    // Default constructor uses global Bridge instance
    ConsoleClass();
    // Constructor with a user provided BridgeClass instance
    ConsoleClass(BridgeClass &_b);
    ~ConsoleClass();

    void begin(void);
    void end(void);

    void buffer(uint8_t size);
    void noBuffer(void);

    bool connected(void);

    // Stream methods
    // (read from console socket)
    int available(void);
    int read(void);
    int peek(void);
    // (write to console socket)
    size_t write(uint8_t);
    size_t write(const uint8_t *buffer, size_t size);
    void flush(void);

    operator bool (void) 
    {
        return connected();
    }

    private:
    BridgeClass &bridge;

    void doBuffer();
    uint8_t inBuffered;
    uint8_t inReadPos;
    static const int BUFFER_SIZE = 32;
    uint8_t *inBuffer;

    bool autoFlush;
    uint8_t outBuffered;
    uint8_t outBufferSize;
    uint8_t *outBuffer;
};

extern ConsoleClass Console;

#endif /*LIB_CONSOLE_H_*/

