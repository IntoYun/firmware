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

#ifndef ESP32_HAL_WIFI_H_
#define ESP32_HAL_WIFI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "esp_wifi_types.h"
#include "soc/soc.h"

typedef enum {
    WIFI_POWER_19_5dBm = 78,// 19.5dBm
    WIFI_POWER_19dBm = 76,// 19dBm
    WIFI_POWER_18_5dBm = 74,// 18.5dBm
    WIFI_POWER_17dBm = 68,// 17dBm
    WIFI_POWER_15dBm = 60,// 15dBm
    WIFI_POWER_13dBm = 52,// 13dBm
    WIFI_POWER_11dBm = 44,// 11dBm
    WIFI_POWER_8_5dBm = 34,// 8.5dBm
    WIFI_POWER_7dBm = 28,// 7dBm
    WIFI_POWER_5dBm = 20,// 5dBm
    WIFI_POWER_2dBm = 8,// 2dBm
    WIFI_POWER_MINUS_1dBm = -4// -1dBm
} wifi_power_t;

static const int AP_STARTED_BIT    = BIT0;
static const int AP_HAS_IP6_BIT    = BIT1;
static const int AP_HAS_CLIENT_BIT = BIT2;
static const int STA_STARTED_BIT   = BIT3;
static const int STA_CONNECTED_BIT = BIT4;
static const int STA_HAS_IP_BIT    = BIT5;
static const int STA_HAS_IP6_BIT   = BIT6;
static const int ETH_STARTED_BIT   = BIT7;
static const int ETH_CONNECTED_BIT = BIT8;
static const int ETH_HAS_IP_BIT    = BIT9;
static const int ETH_HAS_IP6_BIT   = BIT10;
static const int WIFI_SCANNING_BIT = BIT11;
static const int WIFI_SCAN_DONE_BIT= BIT12;
static const int WIFI_DNS_IDLE_BIT = BIT13;
static const int WIFI_DNS_DONE_BIT = BIT14;

typedef void (*ScanDoneCb)(void);

int esp32_setStatusBits(int bits);
int esp32_clearStatusBits(int bits);
int esp32_getStatusBits(void);
int esp32_waitStatusBits(int bits, uint32_t timeout_ms);
void esp32_setScanDoneCb(ScanDoneCb cb);
int32_t esp32_getChannel(void);
void esp32_setPersistent(bool persistent);
bool esp32_setMode(wifi_mode_t m);
wifi_mode_t esp32_getMode(void);
bool esp32_enableSTA(bool enable);
bool esp32_enableAP(bool enable);
uint8_t* esp32_getMacAddress(uint8_t* mac);
bool esp32_setDHCP(char enable);
bool esp32_setAutoConnect(bool autoConnect);
int32_t esp32_getRSSI(void);
bool esp32_setAutoConnect(bool autoConnect);
bool esp32_getAutoConnect(void);
bool esp32_setAutoReconnect(bool autoReconnect);
bool esp32_getAutoReconnect(void);
int esp32_setConfig(wifi_config_t *conf);
bool esp32_begin(void);
int esp32_connect(void);
int esp32_disconnect(void);
bool esp32_beginSmartConfig(void);
bool esp32_stopSmartConfig(void);
bool esp32_smartConfigDone(void);
int esp32_gethostbyname(const char* hostname, uint16_t hostnameLen, uint32_t *ip_addr);

#ifdef __cplusplus
}
#endif


#endif /* ESP32_WIFI_GENERIC_H_ */
