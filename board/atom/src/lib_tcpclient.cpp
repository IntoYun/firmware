/**
 ******************************************************************************
 * @file     : lib_tcpclient.cpp
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

#include "lib_tcpclient.h"

/*********************************************************************************
  *Function           :  TcpClient::TcpClient(int _h, BridgeClass &_b)
  *Description       :  constructor function
  *Input               :
  *Output             :
  *Return             :
  *author             :
  *date                :
  *Others             :
**********************************************************************************/
TcpClient::TcpClient(int _h, BridgeClass &_b) :
    bridge(_b), handle(_h), opened(true), buffered(0)
{
}


/*********************************************************************************
  *Function           :  TcpClient::TcpClient(BridgeClass &_b)
  *Description       :  constructor function
  *Input               :
  *Output             :
  *Return             :
  *author             :
  *date                :
  *Others             :
**********************************************************************************/
TcpClient::TcpClient(BridgeClass &_b) :
    bridge(_b), handle(0), opened(false), buffered(0)
{
}

/*********************************************************************************
  *Function           :  TcpClient::~TcpClient()
  *Description       :  constructor function
  *Input               :
  *Output             :
  *Return             :
  *author             :
  *date                :
  *Others             :
**********************************************************************************/
TcpClient::~TcpClient()
{
}

/*********************************************************************************
  *Function           :  TcpClient& TcpClient::operator=(const TcpClient &_x)
  *Description       :
  *Input               :
  *Output             :
  *Return             :
  *author             :
  *date                :
  *Others             :
**********************************************************************************/
TcpClient& TcpClient::operator=(const TcpClient &_x)
{
    opened = _x.opened;
    handle = _x.handle;
    return *this;
}

/*********************************************************************************
  *Function           :  void TcpClient::stop(void)
  *Description       :  Disconnect from the server.
  *Input               :  none
  *Output             :  none
  *Return             :  none
  *author             :  robot
  *date                :  2015-02-01
  *Others             :
**********************************************************************************/
void TcpClient::stop(void)
{
    if (opened)
    {
        uint8_t cmd[] = {'j', handle};
        bridge.transfer(cmd, 2);
    }
    opened = false;
}

/*********************************************************************************
  *Function           :  void TcpClient::doBuffer(void)
  *Description       :
  *Input               :
  *Output             :
  *Return             :
  *author             :
  *date                :
  *Others             :
**********************************************************************************/
void TcpClient::doBuffer(void)
{
    // If there are already char in buffer exit
    if (buffered > 0)
    return;

    // Try to buffer up to 32 characters
    readPos = 0;
    uint8_t cmd[] = {'K', handle, sizeof(buffer)};
    buffered = bridge.transfer(cmd, 3, buffer, sizeof(buffer));
}

/*********************************************************************************
  *Function           :  int TcpClient::available(void)
  *Description       :  Returns the number of bytes available for reading (that is, the amount of data
                              that has been written to the client by the server it is connected to).
  *Input               :  none
  *Output             :  none
  *Return             :  The number of bytes available.
  *author             :  robot
  *date                :  2015-02-01
  *Others             :
**********************************************************************************/
int TcpClient::available(void)
{
    // Look if there is new data available
    doBuffer();
    return buffered;
}

/*********************************************************************************
  *Function           :  int TcpClient::read(void)
  *Description       :  Read the next byte received from the server the client is connected to (after the last call to read()).
                              read() inherits from the Stream utility class.
  *Input               :   none
  *Output             :   none
  *Return             :   The next byte (or character), or -1 if none is available.
  *author             :   robot
  *date                :   2015-02-01
  *Others             :
**********************************************************************************/
int TcpClient::read(void)
{
    doBuffer();
    if (buffered == 0)
    return -1; // no chars available
    else
    {
        buffered--;
        return buffer[readPos++];
    }
}

/*********************************************************************************
  *Function           :  int TcpClient::read(uint8_t *buff, size_t size)
  *Description       :
  *Input               :
  *Output             :
  *Return             :
  *author             :
  *date                :
  *Others             :
**********************************************************************************/
int TcpClient::read(uint8_t *buff, size_t size)
{
    size_t readed = 0;
    do
    {
        if (buffered == 0)
        {
        doBuffer();
        if (buffered == 0)
        return readed;
        }
        buff[readed++] = buffer[readPos++];
        buffered--;
    } while (readed < size);
    return readed;
}

/*********************************************************************************
  *Function           :  int TcpClient::peek(void)
  *Description       :  Read a byte from the file without advancing to the next one.
                             That is, successive calls to peek() will return the same value,
                             as will the next call to read().
  *Input               :  none
  *Output             :  none
  *Return             :  int : the first byte of incoming data available (or -1 if no data is available)
  *author             :  robot
  *date                :  2015-02-01
  *Others             :
**********************************************************************************/
int TcpClient::peek(void)
{
    doBuffer();
    if (buffered == 0)
        return -1; // no chars available
    else
        return buffer[readPos];
}

/*********************************************************************************
  *Function           :  size_t TcpClient::write(uint8_t c)
  *Description       :  Write data to the server the client is connected to.
  *Input               :  data: the byte or char to write
  *Output             :  none
  *Return             :  byte: the number of characters written. it is not necessary to read this value.
  *author             :  robot
  *date                :  2015-02-01
  *Others             :
**********************************************************************************/
size_t TcpClient::write(uint8_t c)
{
    if (!opened)
        return 0;
    uint8_t cmd[] = {'l', handle, c};
    bridge.transfer(cmd, 3);
    return 1;
}

/*********************************************************************************
  *Function           :  size_t TcpClient::write(const uint8_t *buf, size_t size)
  *Description       :
  *Input               :
  *Output             :
  *Return             :
  *author             :
  *date                :
  *Others             :
**********************************************************************************/
size_t TcpClient::write(const uint8_t *buf, size_t size)
{
    if (!opened)
        return 0;
    uint8_t cmd[] = {'l', handle};
    bridge.transfer(cmd, 2, buf, size, NULL, 0);
    return size;
}

/*********************************************************************************
  *Function           :  void TcpClient::flush(void)
  *Description       :  Discard any bytes that have been written to the client but not yet read.
  *Input               :  none
  *Output             :  none
  *Return             :  none
  *author             :  robot
  *date                :  2015-02-01
  *Others             :
**********************************************************************************/
void TcpClient::flush(void)
{
}

/*********************************************************************************
  *Function           :  uint8_t TcpClient::connected(void)
  *Description       :  Whether or not the client is connected. Note that a client is considered connected
                              if the connection has been closed but there is still unread data.
  *Input               :   none
  *Output             :   none
  *Return             :   Returns true if the client is connected, false if not.
  *author             :   robot
  *date                :   2015-02-01
  *Others             :
**********************************************************************************/
uint8_t TcpClient::connected(void)
{
    if (!opened)
        return false;
    uint8_t cmd[] = {'L', handle};
    uint8_t res[1];
    bridge.transfer(cmd, 2, res, 1);
    return (res[0] == 1);
}

/*********************************************************************************
  *Function           :  int TcpClient::connect(IPAddress ip, uint16_t port)
  *Description       :  Connects to a specified IP address and port. The return value indicates success or failure.
                              Also supports DNS lookups when using a domain name.
  *Input               :   ip: the IP address that the client will connect to (array of 4 bytes)
                              port: the port that the client will connect to (int)
  *Output             :   none
  *Return             :   Returns true if the connection succeeds, false if not.
  *author             :   robot
  *date                :   2015-02-01
  *Others             :
**********************************************************************************/
int TcpClient::connect(IPAddress ip, uint16_t port)
{
    String address;
    address.reserve(18);
    address += ip[0];
    address += '.';
    address += ip[1];
    address += '.';
    address += ip[2];
    address += '.';
    address += ip[3];
    return connect(address.c_str(), port);
}

/*********************************************************************************
  *Function           :  int TcpClient::connect(const char *host, uint16_t port)
  *Description       :  Connects to a specified IP address and port. The return value indicates success or failure.
                              Also supports DNS lookups when using a domain name.
  *Input               :  host:  the domain name the client will connect to (string, ex.:"arduino.cc")
                             port: the port that the client will connect to (int)
  *Output             :  none
  *Return             :  Returns true if the connection succeeds, false if not.
  *author             :  robot
  *date                :  2015-02-01
  *Others             :
**********************************************************************************/
int TcpClient::connect(const char *host, uint16_t port)
{
    uint8_t tmp[] =
    {
        'C',
        (uint8_t)((port >> 8) & 0xFF),
        (uint8_t)(port & 0xFF)
    };
    uint8_t res[1];
    int l = bridge.transfer(tmp, 3, (const uint8_t *)host, strlen(host), res, 1);
    if (l == 0)
    return 0;
    handle = res[0];

    // wait for connection
    uint8_t tmp2[] = { 'c', handle };
    uint8_t res2[1];
    while (true)
    {
        bridge.transfer(tmp2, 2, res2, 1);
        if (res2[0] == 0)
        break;
        delay(1);
    }
    opened = true;

    // check for successful connection
    if (connected())
    return 1;

    opened = false;
    handle = 0;
    return 0;
}

