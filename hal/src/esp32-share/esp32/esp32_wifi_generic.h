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

#ifndef ESP32_WIFI_GENERIC_H_
#define ESP32_WIFI_GENERIC_H_

#include <stdint.h>
#include "esp_wifi_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
        WL_NO_SHIELD        = 255,   // for compatibility with WiFi Shield library
        WL_IDLE_STATUS      = 0,
        WL_NO_SSID_AVAIL    = 1,
        WL_SCAN_COMPLETED   = 2,
        WL_CONNECTED        = 3,
        WL_CONNECT_FAILED   = 4,
        WL_CONNECTION_LOST  = 5,
        WL_DISCONNECTED     = 6
} wl_status_t;

typedef void (*ScanDoneCb)(void);

void esp32_setScanDoneCb(ScanDoneCb cb);
bool esp32_setMode(wifi_mode_t m);
wifi_mode_t esp32_getMode();
bool esp32_enableSTA(bool enable);
bool esp32_enableAP(bool enable);
uint8_t* esp32_getMacAddress(uint8_t* mac);
bool esp32_setDHCP(char enable);
bool esp32_setAutoConnect(bool autoConnect);
int32_t esp32_getRSSI(void);
bool esp32_beginSmartConfig();
bool esp32_stopSmartConfig();
bool esp32_smartConfigDone();
int esp32_gethostbyname(const char* hostname, uint16_t hostnameLen, uint32_t &ip_addr);
int esp32_connect();
int esp32_disconnect();
wl_status_t esp32_status();
bool esp32_setAutoConnect(bool autoConnect);
bool esp32_getAutoConnect();
bool esp32_setAutoReconnect(bool autoReconnect);
int esp32_setConfig(wifi_config_t *conf);

#ifdef __cplusplus
}
#endif


#endif /* ESP32_WIFI_GENERIC_H_ */
