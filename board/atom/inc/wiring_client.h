/**
 ******************************************************************************
 * @file     : wiring_client.h
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

#ifndef   WIRING_CLIENT_H_
#define   WIRING_CLIENT_H_

#include "wiring_print.h"
#include "wiring_stream.h"
#include "wiring_ipaddress.h"

class Client : public Stream 
{
    public:
        virtual int connect(IPAddress ip, uint16_t port) =0;
        virtual int connect(const char *host, uint16_t port) =0;
        virtual size_t write(uint8_t) =0;
        virtual size_t write(const uint8_t *buf, size_t size) =0;
        virtual int available() = 0;
        virtual int read() = 0;
        virtual int read(uint8_t *buf, size_t size) = 0;
        virtual int peek() = 0;
        virtual void flush() = 0;
        virtual void stop() = 0;
        virtual uint8_t connected() = 0;
        virtual operator bool() = 0;
        protected:
        uint8_t* rawIPAddress(IPAddress& addr) { return addr.raw_address(); };
};

#endif /*WIRING_CLIENT_H_*/

