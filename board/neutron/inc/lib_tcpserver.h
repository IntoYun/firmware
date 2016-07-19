/**
 ******************************************************************************
 * @file     : lib_tcpserver.h
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

#ifndef   WIRING_TCPSERVER_H_
#define   WIRING_TCPSERVER_H_

#include "wiring_server.h"
//#include "lib_bridge.h"


class TcpClient;

class TcpServer : public Server 
{
    public:
        // Constructor with a user provided BridgeClass instance
        TcpServer(uint16_t port = 5555);

        uint8_t status(void);
        void begin(void);
        TcpClient accept(void);

        virtual size_t write(uint8_t c);
        //virtual size_t write(const uint8_t *buf, size_t size);
        using Print::write;
        /*Tells the server to begin listening for incoming connections. 
            The Bridge server communicates on port 5555 at localhost.
            */
        void listenOnLocalhost(void)   {
            useLocalhost = true;
        }
        /*Tells the server to begin listening for incoming connections. 
            The Bridge server communicates on port 5555 at a specified address.*/
        void noListenOnLocalhost(void) {
            useLocalhost = false;
        }

    private:

//        BridgeClass &bridge;
        uint16_t port;
        bool listening;
        bool useLocalhost;
};

#endif /* WIRING_TCPSERVER_H_*/