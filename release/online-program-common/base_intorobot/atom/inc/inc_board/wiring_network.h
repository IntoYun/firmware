/**
 ******************************************************************************
 * @file     : wiring_network.h
 * @author   : robin
 * @version	 : V1.0.0
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

#ifndef   WIRING_NETWORK_H_
#define   WIRING_NETWORK_H_

#include "wiring.h"

//Retained for compatibility and to flag compiler warnings as build errors
class NetworkClass
{
    public:
        uint8_t* macAddress(uint8_t* mac) __attribute__((deprecated("Please use WiFi.macAddress() instead")));
        IPAddress localIP() __attribute__((deprecated("Please use WiFi.localIP() instead")));
        IPAddress subnetMask() __attribute__((deprecated("Please use WiFi.subnetMask() instead")));
        IPAddress gatewayIP() __attribute__((deprecated("Please use WiFi.gatewayIP() instead")));
        char* SSID() __attribute__((deprecated("Please use WiFi.SSID() instead")));
        int8_t RSSI() __attribute__((deprecated("Please use WiFi.RSSI() instead")));
        uint32_t ping(IPAddress remoteIP) __attribute__((deprecated("Please use WiFi.ping() instead")));
        uint32_t ping(IPAddress remoteIP, uint8_t nTries) __attribute__((deprecated("Please use WiFi.ping() instead")));

        static void connect(void) __attribute__((deprecated("Please use WiFi.connect() instead")));
        static void disconnect(void) __attribute__((deprecated("Please use WiFi.disconnect() instead")));
        static bool connecting(void) __attribute__((deprecated("Please use WiFi.connecting() instead")));
        static bool ready(void) __attribute__((deprecated("Please use WiFi.ready() instead")));
};

extern NetworkClass Network;

#endif /*WIRING_NETWORK_H_*/

