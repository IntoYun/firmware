/**
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

#ifndef OPENWRT_CONN_H
#define OPENWRT_CONN_H

#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#include "pipe_hal.h"
#include "hw_config.h"
#include "enums_hal.h"
#include "wiring_bridge.h"


#undef putc
#undef getc

// management struture for sockets
typedef struct {
    int handle;
    volatile IpProtocol ipproto;
    volatile int local_port;
    volatile int remote_port;
    volatile uint8_t remote_ip[4];
    volatile bool connected;
    volatile int pending;
    volatile bool open;
    Pipe<char>* pipe;
} SockCtrl;


class OpenwrtConnClass
{
public:
    //! Constructor
    OpenwrtConnClass(BridgeClass &_b = Bridge);

    // ----------------------------------------------------------------
    // Sockets
    // ----------------------------------------------------------------

    /** Create a socket for a ip protocol (and optionaly bind)
        \param ipproto the protocol (UDP or TCP)
        \param port in case of UDP, this optional port where it is bind
        \return the socket handle if successful or SOCKET_ERROR on failure
    */
    int socketSocket(IpProtocol ipproto, int port = -1);

    /** make a socket connection
        \param socket the socket handle
        \param ip e.g. "192.168.8.1"
        \param port the port to connect
        \return true if successfully, false otherwise
    */
    bool socketConnect(int socket, const MDM_IP& ip, int port);
    bool socketConnect(int socket, const char * host, int port);

    /** make a socket connection
        \param socket the socket handle
        \return true if connected, false otherwise
    */
    bool socketIsConnected(int socket);

    /** Write socket data
        \param socket the socket handle
        \param buf the buffer to write
        \param len the size of the buffer to write
        \return the size written or SOCKET_ERROR on failure
    */
    int socketSend(int socket, const char * buf, int len);
    int socketSendTo(int socket, MDM_IP ip, int port, const char * buf, int len);

    /** Get the number of bytes pending for reading for this socket
        \param socket the socket handle
        \return the number of bytes pending or SOCKET_ERROR on failure
    */
    int socketReadable(int socket);

    /** Read this socket
        \param socket the socket handle
        \param buf the buffer to read into
        \param len the size of the buffer to read into
        \return the number of bytes read or SOCKET_ERROR on failure
    */
    int socketRecv(int socket, char* buf, int len);

    int socketRecvFrom(int socket, MDM_IP* ip, int* port, char* buf, int len);
    /** Close a connectied socket (that was connected with #socketConnect)
        \param socket the socket handle
        \return true if successfully, false otherwise
    */
    bool socketClose(int socket);

    /** Free the socket (that was allocated before by #socketSocket)
        \param socket the socket handle
        \return true if successfully, false otherwise
    */
    bool socketFree(int socket);


protected:
    // 5 TCP and UDP sockets total
    static SockCtrl _sockets[5];
    BridgeClass &bridge;

    static int _findSocket(int handle = MDM_SOCKET_ERROR/* = CREATE*/);
    static bool _socketFree(int socket);
};

#endif
