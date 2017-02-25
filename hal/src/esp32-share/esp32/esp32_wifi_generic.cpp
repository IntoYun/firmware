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

volatile uint8_t _dns_founded=0;

static volatile uint32_t esp32_wifi_timeout_start;
static volatile uint32_t esp32_wifi_timeout_duration;

inline void ARM_WIFI_TIMEOUT(uint32_t dur) {
    esp32_wifi_timeout_start = HAL_Timer_Get_Milli_Seconds();
    esp32_wifi_timeout_duration = dur;
    //DEBUG("esp8266 WIFI WD Set %d",(dur));
}
inline bool IS_WIFI_TIMEOUT() {
    return esp32_wifi_timeout_duration && ((HAL_Timer_Get_Milli_Seconds()-esp32_wifi_timeout_start)>esp32_wifi_timeout_duration);
}

inline void CLR_WIFI_TIMEOUT() {
    esp32_wifi_timeout_duration = 0;
    //DEBUG("esp8266 WIFI WD Cleared, was %d", esp8266_wifi_timeout_duration);
}

#if 1
static bool wifiLowLevelInit(){
    static bool lowLevelInitDone = false;
    if(!lowLevelInitDone){
        tcpip_adapter_init();
        // esp_event_loop_init(&WiFiGenericClass::_eventCallback, NULL);
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
    //esp_wifi_deinit();
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
#endif

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

// int esp32_getMacAddress(uint8_t* mac)
// {
//     if(!esp_wifi_get_mac(WIFI_IF_STA, mac))
//     {
//         return -1;
//     }

//     return 1;
// }

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
    #if 0
    station_status_t status = wifi_station_get_connect_status();
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

