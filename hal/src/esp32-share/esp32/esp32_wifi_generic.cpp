/*
 ESP8266WiFiGeneric.cpp - WiFi library for esp8266

 Copyright (c) 2014 Ivan Grokhotkov. All rights reserved.
 This file is part of the esp8266 core for Arduino environment.

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

 Reworked on 28 Dec 2015 by Markus Sattler

 */

#include "esp32_wifi_generic.h"

extern "C" {
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include <esp_err.h>
#include <esp_wifi.h>
#include <esp_event_loop.h>
#include "lwip/ip_addr.h"
#include "lwip/opt.h"
#include "lwip/err.h"
#include "lwip/dns.h"
#include "esp_ipc.h"

#include "esp_smartconfig.h"
// #include "esp_wifi_types.h"
// #include "tcpip_adapter.h"
}

#include "timer_hal.h"
#include "service_debug.h"
#include "delay_hal.h"
#include <vector>

typedef void (*WiFiEventCb)(system_event_id_t event);

typedef struct {
    WiFiEventCb cb;
    system_event_id_t event;
} WiFiEventCbList_t;


static std::vector<WiFiEventCbList_t> cbEventList;

// WiFiEventCbList_t cbEventList;


volatile uint8_t _dns_founded=0;
volatile uint8_t wifiStatus;
volatile uint16_t _scanCount;
void *_scanResult = NULL;

void *_getScanInfoByIndex(int i)
{
    if(!_scanResult || (size_t) i > _scanCount)
    {
        return 0;
    }
    return reinterpret_cast<wifi_ap_record_t*>(_scanResult) + i;
}

bool getNetworkInfo(uint8_t i, String &ssid, uint8_t &encType, int8_t &rssi, uint8_t* bssid, int32_t &channel)
{
    wifi_ap_record_t* it = reinterpret_cast<wifi_ap_record_t*>(_getScanInfoByIndex(i));
    if(!it)
    {
        return false;
    }
    ssid = (const char*) it->ssid;
    encType = it->authmode;
    rssi = it->rssi;
    bssid = it->bssid;
    channel = it->primary;
    return true;
}

uint16_t WiFi_getScanCount()
{
    return _scanCount;
}

void WiFi_setStatus(wl_status_t status)
{
    wifiStatus = status;
    //log_i("wifi status: %d", status);
}

wl_status_t WiFi_getStatus()
{
    return wifiStatus;
}

void WiFi_scanDone()
{
    // WiFiScanClass::_scanComplete = true;
    // WiFiScanClass::_scanStarted = false;
    esp_wifi_scan_get_ap_num(&_scanCount);
    if(_scanCount) {
        _scanResult = new wifi_ap_record_t[_scanCount];
        if(_scanResult) {
            esp_wifi_scan_get_ap_records(&_scanCount, (wifi_ap_record_t*)_scanResult);
        } else {
            //no memory
            _scanCount = 0;
        }
    }
}

esp_err_t _eventCallback(void *arg, system_event_t *event)
{
    // log_d("Event: %d - %s", event->event_id, system_event_names[event->event_id]);

    if(event->event_id == SYSTEM_EVENT_SCAN_DONE)
    {
        WiFi_scanDone();
    }
    else if(event->event_id == SYSTEM_EVENT_STA_DISCONNECTED)
    {
        uint8_t reason = event->event_info.disconnected.reason;
        // log_w("Reason: %u - %s", reason, reason2str(reason));
        if(reason == WIFI_REASON_NO_AP_FOUND)
        {
            WiFi_setStatus(WL_NO_SSID_AVAIL);
        }
        else if(reason == WIFI_REASON_AUTH_FAIL || reason == WIFI_REASON_ASSOC_FAIL)
        {
            WiFi_setStatus(WL_CONNECT_FAILED);
        }
        else if(reason == WIFI_REASON_BEACON_TIMEOUT || reason == WIFI_REASON_HANDSHAKE_TIMEOUT)
        {
            WiFi_setStatus(WL_CONNECTION_LOST);
        }
        else
        {
            WiFi_setStatus(WL_DISCONNECTED);
        }
    }
    else if(event->event_id == SYSTEM_EVENT_STA_START)
    {
        WiFi_setStatus(WL_DISCONNECTED);
    }
    else if(event->event_id == SYSTEM_EVENT_STA_STOP)
    {
        WiFi_setStatus(WL_NO_SHIELD);
    }
    else if(event->event_id == SYSTEM_EVENT_STA_GOT_IP)
    {
        WiFi_setStatus(WL_CONNECTED);
    }

    for(uint32_t i = 0; i < cbEventList.size(); i++)
    {
        WiFiEventCbList_t entry = cbEventList[i];
        if(entry.cb)
        {
            if(entry.event == (system_event_id_t) event->event_id || entry.event == SYSTEM_EVENT_MAX)
            {
                entry.cb((system_event_id_t) event->event_id);
            }
        }
    }
    return ESP_OK;
}


static bool wifiLowLevelInit(){
    static bool lowLevelInitDone = false;
    if(!lowLevelInitDone){
        tcpip_adapter_init();
        esp_event_loop_init(&_eventCallback, NULL);
        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        esp_err_t err = esp_wifi_init(&cfg);
        if(err){
            // log_e("esp_wifi_init %d", err);
            return false;
        }
        esp_wifi_set_storage(WIFI_STORAGE_FLASH);
        esp_wifi_set_mode(WIFI_MODE_NULL);
        lowLevelInitDone = true;
    }
    return true;
}

static bool wifiLowLevelDeinit(){
    //deinit not working yet!
    esp_wifi_deinit();
    return true;
}

static bool _esp_wifi_started = false;

static bool espWiFiStart(){
    if(_esp_wifi_started){
        return true;
    }
    if(!wifiLowLevelInit()){
        return false;
    }
    esp_err_t err = esp_wifi_start();
    if (err != ESP_OK) {
        // log_e("esp_wifi_start %d", err);
        wifiLowLevelDeinit();
        return false;
    }
    _esp_wifi_started = true;
    return true;
}

static bool espWiFiStop(){
    esp_err_t err;
    if(!_esp_wifi_started){
        return true;
    }
    err = esp_wifi_stop();
    if(err){
        // log_e("Could not stop WiFi! %u", err);
        return false;
    }
    _esp_wifi_started = false;
    return wifiLowLevelDeinit();
}

/**
 * set new mode
 * @param m WiFiMode_t
 */
bool esp32_setMode(wifi_mode_t m)
{
    wifi_mode_t cm = esp32_getMode();
    if(cm == WIFI_MODE_MAX)
    {
        return false;
    }

    if(cm == m)
    {
        return true;
    }

    esp_err_t err;
    err = esp_wifi_set_mode(m);

    if(err)
    {
        // log_e("Could not set mode! %u", err);
        return false;
    }

    if(m)
    {
        return espWiFiStart();
    }
    return espWiFiStop();

}

/**
 * get WiFi mode
 * @return WiFiMode
 */
wifi_mode_t esp32_getMode()
{
    if(!wifiLowLevelInit()){
        return WIFI_MODE_MAX;
    }

    uint8_t mode;
    esp_wifi_get_mode((wifi_mode_t*)&mode);
    return (wifi_mode_t)mode;
}

/**
 * control STA mode
 * @param enable bool
 * @return ok
 */
bool esp32_enableSTA(bool enable)
{
    wifi_mode_t currentMode = esp32_getMode();
    bool isEnabled = ((currentMode & WIFI_MODE_STA) != 0);

    if(isEnabled != enable)
    {
        if(enable)
        {
            return esp32_setMode((wifi_mode_t)(currentMode | WIFI_MODE_STA));
        }
        else
        {
            return esp32_setMode((wifi_mode_t)(currentMode & (~WIFI_MODE_STA)));
        }
    }
    else
    {
        return true;
    }
}

/**
 * control AP mode
 * @param enable bool
 * @return ok
 */
bool esp32_enableAP(bool enable)
{
    wifi_mode_t currentMode = esp32_getMode();
    bool isEnabled = ((currentMode & WIFI_MODE_AP) != 0);

    if(isEnabled != enable)
    {
        if(enable)
        {
            return esp32_setMode((wifi_mode_t)(currentMode | WIFI_MODE_AP));
        }
        else
        {
            return esp32_setMode((wifi_mode_t)(currentMode & (~WIFI_MODE_AP)));
        }
    }
    else
    {
        return true;
    }
}

/**
 * Get the station interface MAC address.
 * @param mac   pointer to uint8_t array with length WL_MAC_ADDR_LENGTH
 * @return      pointer to uint8_t *
 */
uint8_t* esp32_getMacAddress(uint8_t* mac)
{
    esp_wifi_get_mac(WIFI_IF_STA, mac);
    return mac;

#if 0
    uint8_t mac[6];
    char macStr[18] = { 0 };
    esp_wifi_get_mac(WIFI_IF_STA, mac);
    // sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    // return String(macStr);
    return macStr;
#endif
}

bool esp32_setDHCP(char enable)
{
    // if(true == enable)
    //     wifi_station_dhcpc_start();
    // else
    //     wifi_station_dhcpc_stop();
    return true;
}

/**
 * Setting the ESP8266 station to connect to the AP (which is recorded)
 * automatically or not when powered on. Enable auto-connect by default.
 * @param autoConnect bool
 * @return if saved
 */
bool esp32_setAutoConnect(bool autoConnect)
{
    bool ret;
    ret = esp_wifi_set_auto_connect(autoConnect);
    return ret;
}

/**
 * Checks if ESP8266 station mode will connect to AP
 * automatically or not when it is powered on.
 * @return auto connect
 */
bool esp32_getAutoConnect()
{
    bool autoConnect;
    esp_wifi_get_auto_connect(&autoConnect);
    return autoConnect;
}

/**
 * Set whether reconnect or not when the ESP8266 station is disconnected from AP.
 * @param autoReconnect
 * @return
 */
bool esp32_setAutoReconnect(bool autoReconnect)
{
    if((esp32_getMode() & WIFI_MODE_STA) != 0)
    {
        if(esp_wifi_disconnect() == ESP_OK)
        {
            return esp_wifi_connect() == ESP_OK;
        }
    }
    return false;
}

/**
 * Return the current network RSSI.
 * @return  RSSI value
 */
int32_t esp32_getRSSI(void)
{
    wifi_ap_record_t info;
    if(!esp_wifi_sta_get_ap_info(&info))
    {
        return info.rssi;
    }
    return 0;
}


bool _smartConfigStarted = false;
bool _smartConfigDone = false;

/**
 * _smartConfigCallback
 * @param st
 * @param result
 */
void smartConfigCallback(uint32_t st, void* result)
{
    smartconfig_status_t status = (smartconfig_status_t) st;
    if (status == SC_STATUS_LINK)
    {
        wifi_sta_config_t *sta_conf = reinterpret_cast<wifi_sta_config_t *>(result);

        esp_wifi_set_config(WIFI_IF_AP, (wifi_config_t *)sta_conf);
        esp_wifi_disconnect();
        esp_wifi_connect();

        _smartConfigDone = true;
    }
    else if (status == SC_STATUS_LINK_OVER)
    {
        esp32_stopSmartConfig();
    }
}

/**
 * Start SmartConfig
 */
bool esp32_beginSmartConfig()
{
    if (_smartConfigStarted)
    {
        return false;
    }

    // if (!WiFi.mode(WIFI_STA)) {
    if (!esp32_setMode(WIFI_MODE_STA))
    {
        return false;
    }

    esp_err_t err;
    err = esp_smartconfig_start(reinterpret_cast<sc_callback_t>(&smartConfigCallback), 1);
    if (err == ESP_OK)
    {
        _smartConfigStarted = true;
        _smartConfigDone = false;
        return true;
    }
    return false;
}


/**
 *  Stop SmartConfig
 */
bool esp32_stopSmartConfig()
{
    if (!_smartConfigStarted)
    {
        return true;
    }

    if (esp_smartconfig_stop() == ESP_OK)
    {
        _smartConfigStarted = false;
        return true;
    }

    return false;
}

/**
 * Query SmartConfig status, to decide when stop config
 * @return smartConfig Done
 */
bool esp32_smartConfigDone()
{
    if(!_smartConfigStarted)
    {
        return false;
    }
    return _smartConfigDone;
}

static bool _dns_busy = false;

/**
 * DNS callback
 * @param name
 * @param ipaddr
 * @param callback_arg
 */
static void wifi_dns_found_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg)
{
    if(ipaddr) {
        // (*reinterpret_cast<IPAddress*>(callback_arg)) = ipaddr->u_addr.ip4.addr;
        (*reinterpret_cast<uint32_t*>(callback_arg)) = ipaddr->u_addr.ip4.addr;
    }
    _dns_busy = false;
}

// int esp32_gethostbyName(const char* aHostname, IPAddress& aResult)
int esp32_gethostbyName(const char* aHostname, uint32_t &aResult)
{
    ip_addr_t addr;
    aResult = static_cast<uint32_t>(0);

    _dns_busy = true;
    err_t err = dns_gethostbyname(aHostname, &addr, &wifi_dns_found_callback, &aResult);
    if(err == ERR_OK && addr.u_addr.ip4.addr) {
        aResult = addr.u_addr.ip4.addr;
        _dns_busy = false;
    } else if(err == ERR_INPROGRESS) {
        while(_dns_busy){
            // delay(1);
            HAL_Delay_Milliseconds(100);
        }
    } else {
        _dns_busy = false;
        return 0;
    }
    return 1;
}

#if 0
void wifi_dns_found_callback(const char *name, ip_addr_t *ipaddr, void *callback_arg) {
    if(ipaddr) {
        (*reinterpret_cast<uint32_t*>(callback_arg)) = ipaddr->addr;
    }
    _dns_founded = 1;
}

int esp32_gethostbyname(const char* hostname, uint16_t hostnameLen, uint32_t &ip_addr)
{
    ip_addr_t addr;

    if(!strcmp(hostname, "255.255.255.255"))
    {
        ip_addr = IPADDR_NONE;
        return 0;
    }

    addr.addr = ipaddr_addr(hostname);
    if (addr.addr != IPADDR_NONE)
    {
        ip_addr = addr.addr;
        return 0;
    }

    ip_addr = 0;
    err_t err = dns_gethostbyname(hostname, &addr, &wifi_dns_found_callback, &ip_addr);
    if(err == ERR_OK) {
        ip_addr = addr.addr;
    } else if(err == ERR_INPROGRESS) {
        ARM_WIFI_TIMEOUT(2000);
        _dns_founded =0;
        while (!_dns_founded) {
            optimistic_yield(100);
            if(IS_WIFI_TIMEOUT()) {
                CLR_WIFI_TIMEOUT();
                break;
            }
        }
        // will return here when dns_found_callback fires
        if(ip_addr != 0) {
            err = ERR_OK;
        }
    }
    return (err == ERR_OK) ? 0 : 1;
}
#endif

int esp32_connect()
{
    esp_wifi_connect();

    // ETS_UART_INTR_DISABLE();
    // wifi_station_connect();
    // ETS_UART_INTR_ENABLE();
    // return 0;
}

int esp32_disconnect()
{
    bool ret;
    wifi_config_t conf;
    *conf.sta.ssid = 0;
    *conf.sta.password = 0;

    esp_wifi_set_config(WIFI_IF_STA, &conf);
    ret = esp_wifi_set_config(WIFI_IF_STA, &conf) == ESP_OK;
    // if(wifioff) {
        esp32_enableSTA(false);
    // }

    return ret;

    // ETS_UART_INTR_DISABLE();
    // wifi_station_disconnect();
    // ETS_UART_INTR_ENABLE();
    // return 0;
}

wl_status_t esp32_status()
{
    return WiFi_getStatus();
    #if  0 
    station_status_t status = WiFi_getStatus();
    switch(status) {
        case STATION_GOT_IP:
            return WL_CONNECTED;
        case STATION_NO_AP_FOUND:
            return WL_NO_SSID_AVAIL;
        case STATION_CONNECT_FAIL:
        case STATION_WRONG_PASSWORD:
            return WL_CONNECT_FAILED;
        case STATION_IDLE:
            return WL_IDLE_STATUS;
        default:
            return WL_DISCONNECTED;
    }
    #endif
    return 0;
}

