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


#ifndef WIRING_PLATFORM_H_
#define WIRING_PLATFORM_H_

#include "inet_hal.h"

/**
 *  This header file maps platform ID to compile-time switches for the Wiring API.
 */

// This is my code (mdma), but on second thoughts I feel this should be driven bottom up for
// components of the platform. (I.e. platform  defines comes from the HAL)


#if PLATFORM_ID==0 || PLATFORM_ID==1     // atom / neutron
#define Wiring_WiFi 1
#define Wiring_IPv6 0
#endif

#if PLATFORM_ID==2     // gcc
#define Wiring_WiFi 1
#define Wiring_IPv6 0
#define Wiring_SPI1 1
#endif

#if PLATFORM_ID==4      // nut
#define Wiring_WiFi 1
#define Wiring_IPv6 1
#define Wiring_SPI1 1
#endif

#if PLATFORM_ID==5      //lora
#define Wiring_WiFi 1
#define Wiring_IPv6 1
#define Wiring_SPI1 1
#define Wiring_Serial2 1
#endif

#if PLATFORM_ID==6      // photon
#define Wiring_WiFi 1
#define Wiring_IPv6 1
#define Wiring_SPI1 1
#define Wiring_Serial2 1
#endif

#if PLATFORM_ID==7
#define Wiring_WiFi 1
#define Wiring_IPv6 1
#define Wiring_SPI1 1
#define Wiring_Serial2 1
#endif

#if PLATFORM_ID==8      // P1 / bm14
#define Wiring_WiFi 1
#define Wiring_IPv6 1
#define Wiring_SPI1 1
#define Wiring_Serial2 1
#endif

#if PLATFORM_ID==9      // ethernet
#define Wiring_IPv6 1
#endif

#if PLATFORM_ID==10      // electron
#define Wiring_Cellular 1
#define Wiring_Serial2 1
#define Wiring_Serial3 0 // Used internally by modem
#define Wiring_Serial4 1
#define Wiring_Serial5 1
#define Wiring_SPI1 1
#define Wiring_SPI2 1

#define Wiring_Wire1 1
#define Wiring_Wire3 1 /* System PMIC and Fuel Guage I2C3 */
#define Wiring_SetupButtonUX 1
#endif

#ifndef Wiring_SPI1
#define Wiring_SPI1 0
#endif

#ifndef Wiring_SPI2
#define Wiring_SPI2 0
#endif

#ifndef Wiring_Wire1
#define Wiring_Wire1 0
#endif

#ifndef Wiring_Wire3
#define Wiring_Wire3 0
#endif

#ifndef Wiring_WiFi
#define Wiring_WiFi 0
#endif

#ifndef Wiring_Cellular
#define Wiring_Cellular 0
#endif

#ifndef Wiring_Serial2
#define Wiring_Serial2 0
#endif

#ifndef Wiring_Serial3
#define Wiring_Serial3 0
#endif

#ifndef Wiring_Serial4
#define Wiring_Serial4 0
#endif

#ifndef Wiring_Serial5
#define Wiring_Serial5 0
#endif

#ifndef Wiring_SetupButtonUX
#define Wiring_SetupButtonUX 0
#endif

#endif	/* WIRING_PLATFORM_H_ */

