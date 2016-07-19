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
typedef enum {
    WIFIMODE_STATION            = 1,
    WIFIMODE_SOFTAP             = 2,
    WIFIMODE_STATION_AND_SOFTAP = 3
} wifi_mode_t;

typedef enum {
    SMARTCONFIGTYPE_ESPTOUCH         = 1,
    SMARTCONFIGTYPE_AIRKISS          = 2,
    SMARTCONFIGTYPE_ESPTOUCH_AIRKISS = 3
} smart_config_t;

typedef enum {
    IPSTATUS_ATERROR        = 1,
    IPSTATUS_GETAP          = 2 ,
    IPSTATUS_CONNECTED      = 3,
    IPSTATUS_DISCONNECTED   = 4,
    IPSTATUS_NOTCONNECTWIFI = 5
} ip_status_t;

typedef enum {
    JOINAP_ATERROR    = 0,
    JOINAP_TIMEOUT    = 1,
    JOINAP_PSWEEOR    = 2,
    JOINAP_NOFOUNDAP  = 3,
    JOINAP_CONNETFAIL = 4,
} wifi_join_ap_t;

typedef enum {
    DEALSTATUS_SUCCESS = 0,
    DEALSTATUS_FAIL = 1,
    DEALSTATUS_DOING = 2,
    DEALSTATUS_IDLE = 3,
} deal_status_t;

typedef struct {
   char ssid[33];
   uint8_t bssid[6];
   uint8_t security;
   uint8_t channel;
   int rssi;        // when scanning
} wifi_ap_t;


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

// ----------------------------------------------------------------
// Sockets
// ----------------------------------------------------------------

//! Type of IP protocol
typedef enum { MDM_IPPROTO_TCP = 0, MDM_IPPROTO_UDP = 1 } IpProtocol;

//! Socket error return codes
#define MDM_SOCKET_ERROR    (-1)

// ----------------------------------------------------------------
// Parsing
// ----------------------------------------------------------------

enum {
    // waitFinalResp Responses
    NOT_FOUND     =  0,
    WAIT          = -1, // TIMEOUT
    RESP_OK       = -2,
    RESP_ERROR    = -3,
    RESP_FAIL     = -4,
    RESP_PROMPT   = -5,
    RESP_ABORTED  = -6,

    // getLine Responses
    #define LENGTH(x)  (x & 0x00FFFF) //!< extract/mask the length
    #define TYPE(x)    (x & 0xFF0000) //!< extract/mask the type

    TYPE_UNKNOWN            = 0x000000,
    TYPE_OK                 = 0x110000,
    TYPE_ERROR              = 0x120000,
    TYPE_FAIL               = 0x130000,
    TYPE_CONNECT            = 0x210000,
    TYPE_BUSY               = 0x220000,
    TYPE_SMARTCONFIG        = 0x230000,
    TYPE_PROMPT             = 0x300000,
    TYPE_PLUS               = 0x400000,
    TYPE_TEXT               = 0x500000,
    TYPE_ABORTED            = 0x600000,

    // special timout constant
    TIMEOUT_BLOCKING = 0xffffffff
};

#define AT_ERROR  -1

#endif
