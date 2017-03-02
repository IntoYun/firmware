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

#include "WString.h"
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "esp_wifi_types.h"

#define WIFI_SCAN_RUNNING   (-1)
#define WIFI_SCAN_FAILED    (-2)

#define WiFiMode_t   wifi_mode_t
#define WIFI_OFF     WIFI_MODE_NULL
#define WIFI_STA     WIFI_MODE_STA
#define WIFI_AP      WIFI_MODE_AP
#define WIFI_AP_STA  WIFI_MODE_APSTA

#define WiFiEvent_t  system_event_id_t

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

typedef enum {
        STATION_IDLE = 0,
        STATION_CONNECTING,
        STATION_WRONG_PASSWORD,
        STATION_NO_AP_FOUND,
        STATION_CONNECT_FAIL,
        STATION_GOT_IP
} station_status_t;


typedef struct bss_info {
        // struct bss_info* next;
    STAILQ_ENTRY(bss_info)     next;
        uint8_t bssid[6];
        uint8_t ssid[32];
        uint8_t ssid_len;
        uint8_t channel;
        int8_t rssi;
        wifi_auth_mode_t authmode;
        uint8_t is_hidden;
        int16_t freq_offset;
        int16_t freqcal_val;
        uint8_t *esp_mesh_ie;
        uint8_t simple_pair;
} bss_info_t;


#define FLASH_MAC_HEADER      0x5aa5f88f

typedef struct __attribute__((__packed__))  _mac_param_t {
        uint32_t header;
        uint8_t stamac_addrs[6];
        uint8_t apmac_addrs[6];
} mac_param_t;



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
wl_status_t WiFi_getStatus();
bool getNetworkInfo(uint8_t i, String &ssid, uint8_t &encType, int8_t &rssi, uint8_t* bssid, int32_t &channel);
uint16_t WiFi_getScanCount();
#ifdef __cplusplus
}
#endif


#endif /* ESP32_WIFI_GENERIC_H_ */
