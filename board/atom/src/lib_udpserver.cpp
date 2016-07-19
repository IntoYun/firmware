/**
 ******************************************************************************
 * @file     : lib_udpsocket.cpp
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

#include "lib_udpserver.h"

/*********************************************************************************
  *Function           :  UdpServer::UdpServer(uint16_t _p, BridgeClass &_b)
  *Description       :  constructor function
  *Input               :
  *Output             :
  *Return             :
  *author             :
  *date                :
  *Others             :
**********************************************************************************/
UdpServer::UdpServer(uint16_t _p, BridgeClass &_b) :
  bridge(_b), port(_p), buffered(0), useLocalhost(false)
{
}


/*********************************************************************************
  *Function           : bool UdpServer::begin(int receive_socket_port, send_socket_port)
  *Description        : set up UDP receive and send sockets at their port.
  *Input              :  @receive_socket_port socket port for receiving data
                         @send_socket_port socket port for sending data
  *Output             :  false or true
  *Return             :  none
  *author             :  chy
  *date               :  2015-12-01
  *Others             :
**********************************************************************************/
bool UdpServer::begin(int receive_socket_port, int send_socket_port)
{
    uint8_t res[3];
    String address = F("127.0.0.1");
    if (!useLocalhost)
    	address = F("");

    //setup receive socket
    uint8_t cmd[] =
    {
        'e',
        (uint8_t)((receive_socket_port >> 8) & 0xFF),
        (uint8_t)(receive_socket_port & 0xFF)
    };
    bridge.transfer(cmd, 3, (const uint8_t *)address.c_str(), address.length(), res, 3);
    if(res[0] == 1)
    {
	return 0;  //fail to create a socket
    }
    else
    {
        receive_socket.id = res[1];   //res[1] store the socket_id, so id < 255
        receive_socket.port = receive_socket_port;
    }
    if(receive_socket_port == send_socket_port)  //receive and send use the same socket
    {
        send_socket.id = res[1];   //res[1] store the socket_id, so id < 255
        send_socket.port = send_socket_port;
	return 1;
    }

    //setup send socket
    cmd[1] = (uint8_t)((send_socket_port >> 8) & 0xFF);
    cmd[2] = (uint8_t)(send_socket_port & 0xFF);
    bridge.transfer(cmd, 3, (const uint8_t *)address.c_str(), address.length(), res, 3);
    if(res[0] == 1)
    {
	return 0;  //fail to create a socket
    }
    else
    {
        send_socket.id = res[1];   //res[1] store the socket_id, so id < 255
        send_socket.port = send_socket_port;
    }
    return 1;
}

/*********************************************************************************
  *Function           :  size_t UdpServer::write_begin(String address, uint8_t broadcast)
  *Description        :  Preparation for writing data to the socket at address:port.
  *Input              :  @address remote ip address where data to be sent
                         @broadcast whether to use broadcast mode
  *Output             :  none
  *Return             :  void
  *author             :  chy
  *date               :  2015-12-01
  *Others             :
**********************************************************************************/
void UdpServer::write_begin(String address, uint8_t broadcast)
{
    uint8_t cmd[] =
    {
       'v',
        send_socket.id,
        (uint8_t)((send_socket.port >> 8) & 0xFF),
        (uint8_t)(send_socket.port & 0xFF)
    };

    DEBUG("IP: %s\n", address.c_str());

    if(!broadcast)
    {
	cmd[0] = 'E';
    	bridge.transfer(cmd, 4, (const uint8_t *)address.c_str(), address.length(), NULL, 0);
    }
    else
    {
	address = "<broadcast>";
    	bridge.transfer(cmd, 4, (const uint8_t *)address.c_str(), address.length(), NULL, 0);
    }
}

/*********************************************************************************
  *Function           :  size_t UdpServer::write_data(const uint8_t * data, size_t size)
  *Description        :  Write data to the cur buffer.
  *Input              :  @data the value to write (byte or char),
                         @size data length
  *Output             :  none
  *Return             :  void.
  *author             :  chy
  *date               :  2015-12-01
  *Others             :
**********************************************************************************/
void UdpServer::write_data(const uint8_t * data, size_t size)
{
    uint8_t cmd[] =
    {
       'h',
        send_socket.id,
    };

    bridge.transfer(cmd, 2, (const uint8_t *)data, size, NULL, 0);
}

/*********************************************************************************
  *Function           :  size_t UdpServer::write_end(uint8_t c, )
  *Description        :  move the data to buffer to be sent.
                         A general step for sending data is write_begin -> write_data -> write_end
  *Input              :
  *Output             :  none
  *Return             :  none.
  *author             :  chy
  *date               :  2015-12-01
  *Others             :
**********************************************************************************/
void UdpServer::write_end()
{
    uint8_t cmd[] =
    {
       'H',
        send_socket.id,
    };

    bridge.transfer(cmd, 2);
}

/*********************************************************************************
  *Function           :  size_t UdpServer::remote_ip()
  *Description        :  get the remote ip for the read UDP packet.
  *Input              :
  *Output             :  none
  *Return             :  true or false
  *author             :  chy
  *date               :  2015-12-01
  *Others             :
**********************************************************************************/
bool UdpServer::get_remote_ip()
{
    uint8_t cmd[] =
    {
       'T',
        receive_socket.id,
    };
    uint8_t res[8];
    bridge.transfer(cmd, 2, res, 8);

    if(res[0] == 0)
    {
	DEBUG("No remote_ip");
	remote_ip[0] = 0xFF;
	remote_ip[1] = 0xFF;
	remote_ip[2] = 0xFF;
	remote_ip[3] = 0xFF;
        return 0;
    }
    else if(res[0] == 1)
    {
        remote_ip[0] = res[1];
	remote_ip[1] = res[2];
	remote_ip[2] = res[3];
	remote_ip[3] = res[4];
	//socket_port = (res[6] << 8) + res[7];
	return 1;
    }
}

/*********************************************************************************
  *Function           :  void UdpServer::write(const uint8_t * buf, size_t size)
  *Description        :  Write data to the send_socket.
  *Input              :  the value to write (byte or char)
  *Output             :  none
  *Return             :  none.
  *author             :  chy
  *date               :  2015-12-01
  *Others             :
**********************************************************************************/
void UdpServer::write(const uint8_t * buf, size_t size)
{
    if(remote_ip[0] != 0xFF)
    {
       char ip[16];
       sprintf(ip, "%d.%d.%d.%d", remote_ip[0], remote_ip[1], remote_ip[2], remote_ip[3]);
       String ip_str(ip);
       write_begin(ip_str, 0);
    }
    else
       write_begin("<broadcast>", 1);

    write_data(buf, size);
    write_end();
}

/*********************************************************************************
  *Function           :  void UdpServer::available(void)
  *Description        :  check whether new data is available
  *Input              :
  *Output             :  available data length
  *Return             :  none.
  *author             :  chy
  *date               :  2015-12-01
  *Others             :
**********************************************************************************/
int UdpServer::available(void)
{
    recvBegin();
    doBuffer();
    return buffered;
}

/*********************************************************************************
  *Function           :  void UdpServer::recvBegin(void)
  *Description        :  Preparation for data reading
  *Input              :
  *Output             :  none
  *Return             :  available data length.
  *author             :  chy
  *date               :  2015-12-01
  *Others             :
**********************************************************************************/
int UdpServer::recvBegin()
{
    int rec_len;
    uint8_t cmd[] =
    {
       'Q',
        0,
    };
    uint8_t res[3];
    bridge.transfer(cmd, 2, res, 3);

    if(res[0] == 0)
    	rec_len = 0;
    else
	rec_len = (int)(res[1] << 8) + res[2];

    return rec_len;
}

/*********************************************************************************
  *Function           :  void UdpServer::receive(void)
  *Description        :  read data
  *Input              :
  *Output             :  none
  *Return             :  read data
  *author             :  chy
  *date               :  2015-12-01
  *Others             :
**********************************************************************************/
String UdpServer::receive()
{
    int buffer_len;
    get_remote_ip(); //set the remote_ip at every reading time

    buffer_len = available();
    if(buffer_len == 0)
	return "";

    uint8_t cmd[] =
    {
       'u',
        0,
        buffer_len
    };

    uint8_t res[250];
    if(buffer_len > 250)
	DEBUG("Not enough buffer for receiving data from UDPServer, please increase the buffer");

    bridge.transfer(cmd, 3, res, buffer_len);
    res[buffer_len] = '\0';  //Important! Jason does not add the '\0' automatically
    String received_str((char *)res);
    DEBUG("Receive data: %s", res);

    return received_str;
}

/*********************************************************************************
  *Function           :  void UdpServer::doBuffer(void)
  *Description        :  Set the available data length in the buffer
  *Input               :
  *Output             :
  *Return             :
  *author             :
  *date                :
  *Others             :
**********************************************************************************/
void UdpServer::doBuffer(void)
{
    uint8_t cmd[] =
    {
	'U',
         0
    };
    uint8_t res[3];
    bridge.transfer(cmd, 2, res, 3);

    if(res[0] == 0)
    	buffered = 0;
    else
	buffered = (int)(res[1] << 8) + res[2];
}

/*********************************************************************************
  *Function           :  void UdpServer::stop(void)
  *Description        :  Close all sockets.
  *Input              :  none
  *Output             :  none
  *Return             :  none
  *author             :  chy
  *date               :  2015-02-01
  *Others             :
**********************************************************************************/
void UdpServer::stop(void)
{
    uint8_t cmd[] = {'q', receive_socket.id};
    bridge.transfer(cmd, 2);
    cmd[0] = 'q';
    cmd[1] = send_socket.id;
    bridge.transfer(cmd, 2);
}


