/**
 ******************************************************************************
 * @file     : lib_udpsocket.h
 * @author   : chy
 * @version  : V1.0.0
 * @date     : 6-December-2015
 * @brief    :    
 ******************************************************************************
  Copyright (c) 2015-2017 IntoRobot Team@ MOLMC Ltd. co.  All right reserved.

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

#ifndef   LIB_UDPSERVER_H_
#define   LIB_UDPSERVER_H_

#include "wiring_server.h"
#include "lib_bridge.h"

struct socket_info
{
	uint8_t id;
	int port;	
};

class UdpServer
{
    public:
        // Constructor with a user provided BridgeClass instance
        UdpServer(uint16_t port = 5556, BridgeClass &_b = Bridge);

        uint8_t status(void);
        bool    begin(int receive_socket_port, int send_socket_port);
	void    write_data(const uint8_t * data, size_t size); 
        void    write_begin(String address, uint8_t broadcast);
        void    write_end(void);
	void    write(const uint8_t * buffer, size_t size); 

        int     available(void);
        String  receive(void);
        int     recvBegin(void);	
	void    doBuffer(void);

	void    stop(void); 
	bool    get_remote_ip(void); 

    private:
        int buffered;
        BridgeClass &bridge;
        uint16_t port;
        bool useLocalhost;
        char remote_ip[4];
        struct socket_info receive_socket;
	struct socket_info send_socket;
};

#endif /* LIB_UDPSERVER_H_*/
