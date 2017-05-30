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

#ifndef DNS_HAL_H
#define	DNS_HAL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "static_assert.h"

/** 255.255.255.255 */
#define IPADDR_NONE         ((uint32_t)0xffffffffUL)
/** 127.0.0.1 */
#define IPADDR_LOOPBACK     ((uint32_t)0x7f000001UL)
/** 0.0.0.0 */
#define IPADDR_ANY          ((uint32_t)0x00000000UL)
/** 255.255.255.255 */
#define IPADDR_BROADCAST    ((uint32_t)0xffffffffUL)

#define HAL_IPv6 0

#if HAL_IPv6
typedef struct __attribute__((__packed__)) _HAL_IPAddress_t  {
    union {
        uint32_t ipv4;
        uint32_t ipv6[4];
    };
    uint8_t v;              // 4 for Ipv4, 6 for Ipv6
} HAL_IPAddress;

#define HAL_IPV4_SET(paddress, value)  ((paddress)->v = 4); ((paddress)->ipv4 = value)

STATIC_ASSERT(HAL_IPAddress_size, sizeof(HAL_IPAddress)==17);
#else
typedef struct __attribute__((__packed__)) _HAL_IPAddress_t {
    union {
        uint32_t ipv4;
    };
} HAL_IPAddress;
STATIC_ASSERT(HAL_IPAddress_size, sizeof(HAL_IPAddress)==4);
#define HAL_IPV4_SET(paddress, value)  (paddress->ipv4 = value)

#endif

inline bool is_ipv4(const HAL_IPAddress* address)
{
#if HAL_IPv6
	return address->v==4;
#else
	return true;
#endif
}

inline uint32_t ipv4_reverse(uint32_t ip_addr)
{
    // 转换处理的ip地址从低位开始，需要转换成高位开始。
    return ((ip_addr & 0x000000FF) << 24) |
        ((ip_addr & 0x0000FF00) << 8) |
        ((ip_addr & 0x00FF0000) >> 8) |
        ((ip_addr & 0xFF000000) >> 24) ;
}

typedef struct __attribute__((__packed__)) _NetworkConfig_t {
    HAL_IPAddress aucIP;             // byte 0 is MSB, byte 3 is LSB
    HAL_IPAddress aucSubnetMask;     // byte 0 is MSB, byte 3 is LSB
    HAL_IPAddress aucDefaultGateway; // byte 0 is MSB, byte 3 is LSB
    HAL_IPAddress aucDHCPServer;     // byte 0 is MSB, byte 3 is LSB
    HAL_IPAddress aucDNSServer;      // byte 0 is MSB, byte 3 is LSB
    uint8_t uaMacAddr[6];
} NetworkConfig;
STATIC_ASSERT(NetworkConfig_size, sizeof(HAL_IPAddress)*5+6);


typedef struct __attribute__((__packed__))  _IPConfig_t {
    uint16_t size;
    NetworkConfig nw;
} IPConfig;

typedef uint32_t network_interface_t;

/**
 *
 * @param hostname      buffer to receive the hostname
 * @param hostnameLen   length of the hostname buffer
 * @param out_ip_addr   The ip address in network byte order.
 * @return
 */
int inet_gethostbyname(const char* hostname, uint16_t hostnameLen, HAL_IPAddress* out_ip_addr,
        network_interface_t nif, void* reserved);


/**
 *
 * @param remoteIP  The IP address. MSB..LSB [0..3]
 * @param nTries
 * @return >0 on success. 0 on timeout? <0 on error.
 */
int inet_ping(const HAL_IPAddress* address, network_interface_t nif, uint8_t nTries,
        void* reserved);


#ifdef	__cplusplus
}
#endif

#endif	/* DNS_HAL_H */

