/**
******************************************************************************
* @file    socket_hal.c
* @author  Matthew McGowan
* @version V1.0.0
* @date    25-Sept-2014
* @brief
******************************************************************************
Copyright (c) 2013-2015 Particle Industries, Inc.  All rights reserved.

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
#define LWIP_SOCKET 1
#include "lwip/sockets.h"


const sock_handle_t SOCKET_MAX = (sock_handle_t)7; // 7 total sockets, handle 0-6
const sock_handle_t SOCKET_INVALID = (sock_handle_t)-1;

//extern int lwip_socket(int domain, int type, int protocol);
sock_handle_t socket_create(uint8_t family, uint8_t type, uint8_t protocol, uint16_t port, network_interface_t nif)
{
    // here family is not use, because LWIP_UNUSED_ARG();
    sock_handle_t handle = lwip_socket(family, type, protocol);
    return handle;
}

int32_t socket_connect(sock_handle_t sd, const sockaddr_t *addr, long addrlen)
{
    // here maybe some bugs, the struct sockaddr has sa_len member, but sockaddr_t don't
    struct sockaddr* addr_tmp;
    for (int i = 0; i < sizeof(addr->sa_data); i++){
        addr_tmp->sa_data[i] = addr->sa_data[i];
    }
    addr_tmp->sa_family = addr->sa_family;
    return lwip_connect(sd, addr_tmp, addrlen); // return the connect result
}
// not implement yet
sock_result_t socket_reset_blocking_call()
{
    return 0;
}

sock_result_t socket_receive(sock_handle_t sd, void* buffer, socklen_t len, system_tick_t _timeout)
{
    // here _timeout is not used, maybe a bug.
    return lwip_recv(sd, buffer, len, 0); // recv input param flags set 0, need consideration.
}

// not implement yet.
sock_result_t socket_create_nonblocking_server(sock_handle_t sock, uint16_t port)
{
    return 0;
}

sock_result_t socket_receivefrom(sock_handle_t sock, void* buffer, socklen_t bufLen, uint32_t flags, sockaddr_t* addr, socklen_t* addrsize)
{
    // here maybe some bugs, the struct sockaddr has sa_len member, but sockaddr don't
    struct sockaddr* addr_tmp;
    for (int i = 0; i < sizeof(addr->sa_data); i++){
        addr_tmp->sa_data[i] = addr->sa_data[i];
    }
    addr_tmp->sa_family = addr->sa_family;
    return lwip_recvfrom(sock, buffer, bufLen, 0, addr_tmp, addrsize); // flags set to 0, need more consideration.
}

sock_result_t socket_bind(sock_handle_t sock, uint16_t port)
{
}

sock_result_t socket_accept(sock_handle_t sock)
{
    return 0;
}


// not implement yet
// maybe need reconstruct the code
uint8_t socket_active_status(sock_handle_t socket)
{
    return 0;
}

sock_result_t socket_close(sock_handle_t sock)
{
    return lwip_close(sock);
}


sock_result_t socket_send(sock_handle_t sd, const void* buffer, socklen_t len)
{
    return lwip_send(sd, buffer, len, 0); // flags set to 0, need more consideration.
}

sock_result_t socket_sendto(sock_handle_t sd, const void* buffer, socklen_t len, uint32_t flags, sockaddr_t* addr, socklen_t addr_size)
{
    // here maybe some bugs, the struct sockaddr has sa_len member, but sockaddr don't
    struct sockaddr* addr_tmp;
    for (int i = 0; i < sizeof(addr->sa_data); i++){
        addr_tmp->sa_data[i] = addr->sa_data[i];
    }
    addr_tmp->sa_family = addr->sa_family;
    return lwip_sendto(sd, buffer, len, 0, addr_tmp, addr_size);
}

// const sock_handle_t SOCKET_MAX = (sock_handle_t)0xFFFE;
// const sock_handle_t SOCKET_INVALID = (sock_handle_t)-1;

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
