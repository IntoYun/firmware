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

#ifndef ENUMS_HAL_H
#define ENUMS_HAL_H

#include <string.h>

// ----------------------------------------------------------------
// Types
// ----------------------------------------------------------------

//! An IP v4 address
typedef uint32_t MDM_IP;
#define NOIP ((MDM_IP)0) //!< No IP address
// IP number formating and conversion
#define IPSTR           "%d.%d.%d.%d"
#define IPNUM(ip)       ((ip)>>24)&0xff, \
                        ((ip)>>16)&0xff, \
                        ((ip)>> 8)&0xff, \
                        ((ip)>> 0)&0xff
#define IPADR(a,b,c,d) ((((uint32_t)(a))<<24) | \
                        (((uint32_t)(b))<<16) | \
                        (((uint32_t)(c))<< 8) | \
                        (((uint32_t)(d))<< 0))

#ifndef MACSTR
#define MACSTR           "%x:%x:%x:%x:%x:%x"
#endif

// ----------------------------------------------------------------
// Sockets
// ----------------------------------------------------------------

//! Type of IP protocol
typedef enum {
    MDM_IPPROTO_TCP = 0,
    MDM_IPPROTO_UDP = 1,
    MDM_IPPROTO_TCP_SERVER = 2
} IpProtocol;

//! Socket error return codes
#define MDM_SOCKET_ERROR    (-1)

#endif
