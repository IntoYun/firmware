/**
 ******************************************************************************
 * @file    socket_hal.c
 * @author  Matthew McGowan
 * @version V1.0.0
 * @date    25-Sept-2014
 * @brief
 ******************************************************************************
  Copyright (c) 2013-2015 IntoRobot Industries, Inc.  All rights reserved.

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

#include "socket_hal.h"

#include <lwip/sockets.h>
#include <lwip/netdb.h>
#include <errno.h>

// return 0, success
int32_t socket_connect(sock_handle_t sd, const sockaddr_t *addr, long addrlen)
{
    struct sockaddr *serveraddr;
    // !!! serveraddr the sa_len have not fuzhi
    serveraddr->sa_family = addr->sa_family;
    // include first two bytes (port), then four bytes, ipv4 address
    for (int i = 0; i< 14; i++) {
        serveraddr->sa_data[i] = addr->sa_data[i];
    }

    int res = lwip_connect_r(sd, (struct sockaddr *)&addr, sizeof(serveraddr));
    return res;
}

sock_result_t socket_reset_blocking_call()
{
    return 0;
}

// ret > 0, receive something
sock_result_t socket_receive(sock_handle_t sd, void* buffer, socklen_t len, system_tick_t _timeout)
{
    // no need to check the available data
    // int count;
    // int res = ioctl(sd, FIONREAD, &count);
    // if (res < 0) {
    //     return 0;
    // }
    int res = recv(sd, buffer, len, MSG_DONTWAIT);
    if (res < 0) {
        return 0;
    }
    return res;
}

sock_result_t socket_create_nonblocking_server(sock_handle_t sock, uint16_t port)
{
    return 0;
}

sock_result_t socket_receivefrom(sock_handle_t sock, void* buffer, socklen_t bufLen, uint32_t flags, sockaddr_t* addr, socklen_t* addrsize)
{
    return 0;
}

sock_result_t socket_bind(sock_handle_t sock, uint16_t port)
{
    return 0;
}

sock_result_t socket_accept(sock_handle_t sock)
{
    return 0;
}

uint8_t socket_active_status(sock_handle_t sock)
{
    return 0;
}

sock_result_t socket_close(sock_handle_t sock)
{
    close(sock);
    return 0;
}

sock_handle_t socket_create(uint8_t family, uint8_t type, uint8_t protocol, uint16_t port, network_interface_t nif)
{
    sock_handle_t sockfd = socket(family, type, 0);
    return sockfd;
}

// ? the return value meaning
sock_result_t socket_send(sock_handle_t sd, const void* buffer, socklen_t len)
{
    int res = send(sd, buffer, len, MSG_DONTWAIT);
    return res;
}

sock_result_t socket_sendto(sock_handle_t sd, const void* buffer, socklen_t len, uint32_t flags, sockaddr_t* addr, socklen_t addr_size)
{
    return 0;
}

const sock_handle_t SOCKET_MAX = (sock_handle_t)0xFFFE;
const sock_handle_t SOCKET_INVALID = (sock_handle_t)-1;

inline bool is_valid(sock_handle_t handle) {
    return handle<SOCKET_MAX;
}

uint8_t socket_handle_valid(sock_handle_t handle) {
    return is_valid(handle);
}

sock_handle_t socket_handle_invalid()
{
    return SOCKET_INVALID;
}


sock_result_t socket_join_multicast(const HAL_IPAddress* addr, network_interface_t nif, void* reserved)
{
    return -1;
}

sock_result_t socket_leave_multicast(const HAL_IPAddress* addr, network_interface_t nif, void* reserved)
{
	return -1;
}

sock_result_t socket_peer(sock_handle_t sd, sock_peer_t* peer, void* reserved)
{
    return -1;
}
