/*
 ESP8266WiFiGeneric.h - esp8266 Wifi support.
 Based on WiFi.h from Ardiono WiFi shield library.
 Copyright (c) 2011-2014 Arduino.  All right reserved.
 Modified by Ivan Grokhotkov, December 2014
 Reworked by Markus Sattler, December 2015

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef ESP8266_WIFI_GENERIC_H_
#define ESP8266_WIFI_GENERIC_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "ota_flash_hal.h"

down_status_t esp8266_downOnlineApp(const char *host, const char *param, const char * md5);
down_status_t esp8266_downDefaultApp(const char *host, const char *param);
down_status_t esp8266_getDownOnlineAppStatus(void);
down_status_t esp8266_getDownDefaultAppStatus(void);
uint8_t esp8266_getDownloadProgress(void);

#ifdef __cplusplus
}
#endif


#endif /* ESP8266_WIFI_GENERIC_H_ */
