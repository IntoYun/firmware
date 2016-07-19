/**
 ******************************************************************************
 * @file     : lib_tcpclient.h
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

#ifndef   LIB_TCPCLIENT_H_
#define   LIB_TCPCLIENT_H_

#include <stdint.h>

#include "wiring_client.h"
//#include "lib_bridge.h"


class TcpClient :public Client
{
    public:
        // Constructor with a user provided BridgeClass instance
 //       TcpClient();
        TcpClient(int _h);
        TcpClient();
        ~TcpClient();

        // Stream methods
        // (read message)
    //    uint8_t status(void);
        virtual int available(void);
        virtual int read(void);
        virtual int read(uint8_t *buf, size_t size);
        virtual int peek(void);
        // (write response)
        virtual size_t write(uint8_t);
        virtual size_t write(const uint8_t *buf, size_t size);
        virtual void flush(void);
        //TODO: add optimized function for block write

        virtual operator bool (void) {
        return opened;
        }

        TcpClient& operator=(const TcpClient &_x);

        virtual void stop(void);
        virtual uint8_t connected(void);

        virtual int connect(IPAddress ip, uint16_t port);
        int connect(const char *host, uint16_t port);


        unsigned char handle;
        
        private:
   //     BridgeClass &bridge;
        
        char opened;
        uint32_t time_count; //connected 执行计时器
        uint8_t cache_conect;   //1连接  0断开

    private:
        void doBuffer(void);
        uint8_t buffered;
        uint8_t readPos;
   //     const int BUFFER_SIZE = 64;
        uint8_t buffer[64];



};


//TcpClient test_TcpClient;


#endif /*LIB_TCPCLIENT_H_*/

