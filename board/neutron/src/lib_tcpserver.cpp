/**
 ******************************************************************************
 * @file     : lib_tcpserver.cpp
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

#include "lib_tcpserver.h"
#include "lib_tcpclient.h"

#include "lib_tcpserver_hal.h"


/*********************************************************************************
  *Function           :  TcpServer::TcpServer(uint16_t _p, BridgeClass &_b)
  *Description       :  constructor function
  *Input               :  
  *Output             :
  *Return             :
  *author             :
  *date                :
  *Others             :   
**********************************************************************************/
TcpServer::TcpServer(uint16_t _p) :
  port(_p), listening(false), useLocalhost(false) 
{
}

/*********************************************************************************
  *Function           : void TcpServer::begin(void) 
  *Description       :  Tells the server to begin listening for incoming connections. 
                             The Bridge server communicates on port 5555 at localhost.
  *Input               :  none
  *Output             :  none
  *Return             :  none 
  *author             :  robot
  *date                :  2015-02-01
  *Others             :   
**********************************************************************************/
void TcpServer::begin(void) 
{
    
    listening=mo_TcpServer_begin_hal(port,useLocalhost);

/*

    uint8_t tmp[] = 
    {
        'N',
        (uint8_t)((port >> 8) & 0xFF),
        (uint8_t)(port & 0xFF)
    };
    uint8_t res[1];
    String address = F("127.0.0.1");
    if (!useLocalhost)
    address = F("0.0.0.0");
    bridge.transfer(tmp, 3, (const uint8_t *)address.c_str(), address.length(), res, 1);
    listening = (res[0] == 1);*/
}

/*********************************************************************************
  *Function           : TcpClient TcpServer::accept(void) 
  *Description       : check if have incoming connections
  *Input               : none
  *Output             : none 
  *Return             : the pointer of incoming connections
  *author             : robot
  *date                : 2015-02-01
  *Others             :      
**********************************************************************************/
TcpClient TcpServer::accept(void) 
{
    
    int8_t ret; 

    ret=mo_TcpServer_accept_hal();
    if(ret==-1)
    {
        return TcpClient();
    }
    else
    {
        return TcpClient(ret);
    }

/*
    uint8_t cmd[] = {'k'};
    uint8_t res[1];
    unsigned int l = bridge.transfer(cmd, 1, res, 1);
    if (l == 0)
    {return TcpClient();}
    return TcpClient(res[0]);*/
}

/*********************************************************************************
  *Function           :  size_t TcpServer::write(uint8_t c) 
  *Description       :  Write data to all the clients connected to a server.
  *Input               :  the value to write (byte or char)
  *Output             :  none
  *Return             :  write() returns the number of bytes written. It is not necessary to read this.
  *author             :  robot
  *date                :  2015-02-01
  *Others             :
**********************************************************************************/
size_t TcpServer::write(uint8_t c) 
{

    mo_TcpServer_write_hal(c);
    return 1;

/*
    uint8_t cmd[] = { 'b', c };
    bridge.transfer(cmd, 2);
    return 1;*/
}

