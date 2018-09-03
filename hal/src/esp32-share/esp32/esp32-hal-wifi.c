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

#include <string.h>
#include "molmc_log.h"
#include "lwip/dns.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_smartconfig.h"
#include "freertos/event_groups.h"
#include "esp32-hal-wifi.h"
#include "net_hal.h"

const static char *TAG = "hal-wifi";

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

static xQueueHandle _network_event_queue;
static TaskHandle_t _network_event_task_handle = NULL;
static EventGroupHandle_t _network_event_group = NULL;

static esp_err_t _eventCallback(void *arg, system_event_t *event);
static ScanDoneCb _scanDoneCb = NULL;

static void _network_event_task(void * arg)
{
    system_event_t *event = NULL;
    for (;;) {
        if(xQueueReceive(_network_event_queue, &event, portMAX_DELAY) == pdTRUE){
            _eventCallback(arg, event);
        }
    }
    vTaskDelete(NULL);
    _network_event_task_handle = NULL;
}

static esp_err_t _network_event_cb(void *arg, system_event_t *event)
{
    if (xQueueSend(_network_event_queue, &event, portMAX_DELAY) != pdPASS) {
        return ESP_FAIL;
    }
    return ESP_OK;
}

static bool _start_network_event_task(void)
{
    if(!_network_event_group){
        _network_event_group = xEventGroupCreate();
        if(!_network_event_group){
            MOLMC_LOGD(TAG, "Network Event Group Create Failed!");
            return false;
        }
        xEventGroupSetBits(_network_event_group, WIFI_DNS_IDLE_BIT);
    }
    if(!_network_event_queue){
        _network_event_queue = xQueueCreate(32, sizeof(system_event_t *));
        if(!_network_event_queue){
            return false;
        }
    }
    if(!_network_event_task_handle){
        xTaskCreatePinnedToCore(_network_event_task, "network_event", 4096, NULL, 2, &_network_event_task_handle, ARDUINO_RUNNING_CORE);
        if(!_network_event_task_handle){
            return false;
        }
    }
    return esp_event_loop_init(&_network_event_cb, NULL) == ESP_OK;
}

void tcpipInit(void)
{
    static bool initialized = false;
    if(!initialized && _start_network_event_task()){
        initialized = true;
        tcpip_adapter_init();
    }
}

static bool lowLevelInitDone = false;
static bool wifiLowLevelInit(bool persistent)
{
    if(!lowLevelInitDone){
        tcpipInit();
        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        esp_err_t err = esp_wifi_init(&cfg);
        if(err){
            return false;
        }
        if(!persistent){
          esp_wifi_set_storage(WIFI_STORAGE_RAM);
        }
        lowLevelInitDone = true;
    }
    return true;
}

static bool wifiLowLevelDeinit(void)
{
    //deinit not working yet!
    //esp_wifi_deinit();
    return true;
}

static bool _esp_wifi_started = false;

static bool espWiFiStart(bool persistent)
{
    if(_esp_wifi_started){
        return true;
    }
    if(!wifiLowLevelInit(persistent)){
        return false;
    }
    esp_err_t err = esp_wifi_start();
    if (err != ESP_OK) {
        wifiLowLevelDeinit();
        return false;
    }
    _esp_wifi_started = true;
    system_event_t event;
    event.event_id = SYSTEM_EVENT_WIFI_READY;
    _eventCallback(NULL, &event);

    return true;
}

static bool espWiFiStop(void)
{
    esp_err_t err;
    if(!_esp_wifi_started) {
        return true;
    }
    _esp_wifi_started = false;
    err = esp_wifi_stop();
    if(err) {
        _esp_wifi_started = true;
        return false;
    }
    return wifiLowLevelDeinit();
}

// -----------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------- Generic WiFi function -----------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------
static bool _persistent = true;

int esp32_setStatusBits(int bits)
{
    if(!_network_event_group){
        return 0;
    }
    return xEventGroupSetBits(_network_event_group, bits);
}

int esp32_clearStatusBits(int bits)
{
    if(!_network_event_group){
        return 0;
    }
    return xEventGroupClearBits(_network_event_group, bits);
}

int esp32_getStatusBits(void)
{
    if(!_network_event_group){
        return 0;
    }
    return xEventGroupGetBits(_network_event_group);
}

int esp32_waitStatusBits(int bits, uint32_t timeout_ms)
{
    if(!_network_event_group){
        return 0;
    }
    return xEventGroupWaitBits(
        _network_event_group,    // The event group being tested.
        bits,  // The bits within the event group to wait for.
        pdFALSE,         // BIT_0 and BIT_4 should be cleared before returning.
        pdTRUE,        // Don't wait for both bits, either bit will do.
        timeout_ms / portTICK_PERIOD_MS ) & bits; // Wait a maximum of 100ms for either bit to be set.
}

#ifdef HAL_WIFI_DEBUG
const char * system_event_reasons[] = { "UNSPECIFIED", "AUTH_EXPIRE", "AUTH_LEAVE", "ASSOC_EXPIRE", "ASSOC_TOOMANY", "NOT_AUTHED", "NOT_ASSOCED", "ASSOC_LEAVE", "ASSOC_NOT_AUTHED", "DISASSOC_PWRCAP_BAD", "DISASSOC_SUPCHAN_BAD", "IE_INVALID", "MIC_FAILURE", "4WAY_HANDSHAKE_TIMEOUT", "GROUP_KEY_UPDATE_TIMEOUT", "IE_IN_4WAY_DIFFERS", "GROUP_CIPHER_INVALID", "PAIRWISE_CIPHER_INVALID", "AKMP_INVALID", "UNSUPP_RSN_IE_VERSION", "INVALID_RSN_IE_CAP", "802_1X_AUTH_FAILED", "CIPHER_SUITE_REJECTED", "BEACON_TIMEOUT", "NO_AP_FOUND", "AUTH_FAIL", "ASSOC_FAIL", "HANDSHAKE_TIMEOUT" };
#define reason2str(r) ((r>176)?system_event_reasons[r-176]:system_event_reasons[r-1])
#endif

static esp_err_t _eventCallback(void *arg, system_event_t *event)
{
    if(event->event_id == SYSTEM_EVENT_SCAN_DONE) {
        MOLMC_LOGD(TAG, "SYSTEM_EVENT_SCAN_DONE\r\n");
        _scanDoneCb();
    } else if(event->event_id == SYSTEM_EVENT_STA_START) {
        //MOLMC_LOGD(TAG, "SYSTEM_EVENT_STA_START\r\n");
        esp32_setStatusBits(STA_STARTED_BIT);
    } else if(event->event_id == SYSTEM_EVENT_STA_STOP) {
        //MOLMC_LOGD(TAG, "SYSTEM_EVENT_STA_STOP\r\n");
        esp32_clearStatusBits(STA_STARTED_BIT | STA_CONNECTED_BIT | STA_HAS_IP_BIT | STA_HAS_IP6_BIT);
    } else if(event->event_id == SYSTEM_EVENT_STA_CONNECTED) {
        esp32_setStatusBits(STA_CONNECTED_BIT);
    } else if(event->event_id == SYSTEM_EVENT_STA_DISCONNECTED) {
        uint8_t reason = event->event_info.disconnected.reason;
        MOLMC_LOGD(TAG, "SYSTEM_EVENT_STA_DISCONNECTED reason = %u - %s\r\n", reason, reason2str(reason));
        if(reason == WIFI_REASON_NO_AP_FOUND) {
        } else if(reason == WIFI_REASON_AUTH_FAIL || reason == WIFI_REASON_ASSOC_FAIL) {
        } else if(reason == WIFI_REASON_BEACON_TIMEOUT || reason == WIFI_REASON_HANDSHAKE_TIMEOUT) {
        } else if(reason == WIFI_REASON_AUTH_EXPIRE) {
        } else {
        }
        esp32_clearStatusBits(STA_CONNECTED_BIT | STA_HAS_IP_BIT | STA_HAS_IP6_BIT);
        if(((reason == WIFI_REASON_AUTH_EXPIRE) ||
            (reason >= WIFI_REASON_BEACON_TIMEOUT && reason != WIFI_REASON_AUTH_FAIL)) &&
            esp32_getAutoReconnect())
        {
            esp32_enableSTA(false);
            esp32_enableSTA(true);
            esp32_begin();
        }
        HAL_NET_notify_disconnected();
    } else if(event->event_id == SYSTEM_EVENT_STA_GOT_IP) {
        MOLMC_LOGD(TAG, "SYSTEM_EVENT_STA_GOT_IP\r\n");
        esp32_setStatusBits(STA_HAS_IP_BIT | STA_CONNECTED_BIT);
        HAL_NET_notify_dhcp(true);
        HAL_NET_notify_connected();
    } else if(event->event_id == SYSTEM_EVENT_STA_LOST_IP) {
        esp32_clearStatusBits(STA_HAS_IP_BIT);
    } else if(event->event_id == SYSTEM_EVENT_AP_START) {
        esp32_setStatusBits(AP_STARTED_BIT);
    } else if(event->event_id == SYSTEM_EVENT_AP_STOP) {
        esp32_clearStatusBits(AP_STARTED_BIT | AP_HAS_CLIENT_BIT);
    } else if(event->event_id == SYSTEM_EVENT_AP_STACONNECTED) {
        esp32_setStatusBits(AP_HAS_CLIENT_BIT);
    } else if(event->event_id == SYSTEM_EVENT_AP_STADISCONNECTED) {
        wifi_sta_list_t clients;
        if(esp_wifi_ap_get_sta_list(&clients) != ESP_OK || !clients.num){
            esp32_clearStatusBits(AP_HAS_CLIENT_BIT);
        }
    } else if(event->event_id == SYSTEM_EVENT_ETH_START) {
        esp32_setStatusBits(ETH_STARTED_BIT);
    } else if(event->event_id == SYSTEM_EVENT_ETH_STOP) {
        esp32_clearStatusBits(ETH_STARTED_BIT | ETH_CONNECTED_BIT | ETH_HAS_IP_BIT | ETH_HAS_IP6_BIT);
    } else if(event->event_id == SYSTEM_EVENT_ETH_CONNECTED) {
        esp32_setStatusBits(ETH_CONNECTED_BIT);
    } else if(event->event_id == SYSTEM_EVENT_ETH_DISCONNECTED) {
        esp32_clearStatusBits(ETH_CONNECTED_BIT | ETH_HAS_IP_BIT | ETH_HAS_IP6_BIT);
    } else if(event->event_id == SYSTEM_EVENT_ETH_GOT_IP) {
        esp32_setStatusBits(ETH_CONNECTED_BIT | ETH_HAS_IP_BIT);
    } else if(event->event_id == SYSTEM_EVENT_GOT_IP6) {
        if(event->event_info.got_ip6.if_index == TCPIP_ADAPTER_IF_AP){
            esp32_setStatusBits(AP_HAS_IP6_BIT);
        } else if(event->event_info.got_ip6.if_index == TCPIP_ADAPTER_IF_STA){
            esp32_setStatusBits(STA_CONNECTED_BIT | STA_HAS_IP6_BIT);
        } else if(event->event_info.got_ip6.if_index == TCPIP_ADAPTER_IF_ETH){
            esp32_setStatusBits(ETH_CONNECTED_BIT | ETH_HAS_IP6_BIT);
        }
    }
    return ESP_OK;
}

void esp32_setScanDoneCb(ScanDoneCb cb)
{
    if (cb != NULL) {
        _scanDoneCb = cb;
    }
}

/**
 * Return the current channel associated with the network
 * @return channel (1-13)
 */
int32_t esp32_getChannel(void)
{
    uint8_t primaryChan = 0;
    wifi_second_chan_t secondChan = WIFI_SECOND_CHAN_NONE;
    if(!lowLevelInitDone){
        return primaryChan;
    }
    esp_wifi_get_channel(&primaryChan, &secondChan);
    return primaryChan;
}

/**
 * store WiFi config in SDK flash area
 * @param persistent
 */
void esp32_setPersistent(bool persistent)
{
    _persistent = persistent;
}

bool esp32_setMode(wifi_mode_t m)
{
    wifi_mode_t cm = esp32_getMode();
    if(cm == m) {
        return true;
    }
    if(!cm && m){
        if(!espWiFiStart(_persistent)){
            return false;
        }
    } else if(cm && !m){
        return espWiFiStop();
    }

    esp_err_t err;
    err = esp_wifi_set_mode(m);
    if(err) {
        return false;
    }
    return true;
}
/**
 * get WiFi mode
 * @return WiFiMode
 */
wifi_mode_t esp32_getMode(void)
{
    if(!_esp_wifi_started){
        return WIFI_MODE_NULL;
    }
    wifi_mode_t mode;
    if(esp_wifi_get_mode(&mode) == ESP_ERR_WIFI_NOT_INIT){
        MOLMC_LOGD(TAG, "WiFi not started");
        return WIFI_MODE_NULL;
    }
    return mode;
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

    if(isEnabled != enable) {
        if(enable) {
            return esp32_setMode((wifi_mode_t)(currentMode | WIFI_MODE_STA));
        }
        return esp32_setMode((wifi_mode_t)(currentMode & (~WIFI_MODE_STA)));
    }
    return true;
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

    if(isEnabled != enable) {
        if(enable) {
            return esp32_setMode((wifi_mode_t)(currentMode | WIFI_MODE_AP));
        }
        return esp32_setMode((wifi_mode_t)(currentMode & (~WIFI_MODE_AP)));
    }
    return true;
}

/**
 * control modem sleep when only in STA mode
 * @param enable bool
 * @return ok
 */
bool esp32_setSleep(bool enable)
{
    if((esp32_getMode() & WIFI_MODE_STA) == 0) {
        MOLMC_LOGD(TAG, "STA has not been started");
        return false;
    }
    return esp_wifi_set_ps(enable?WIFI_PS_MIN_MODEM:WIFI_PS_NONE) == ESP_OK;
}

/**
 * get modem sleep enabled
 * @return true if modem sleep is enabled
 */
bool esp32_getSleep(void)
{
    wifi_ps_type_t ps;
    if((esp32_getMode() & WIFI_MODE_STA) == 0) {
        MOLMC_LOGD(TAG, "STA has not been started");
        return false;
    }
    if(esp_wifi_get_ps(&ps) == ESP_OK) {
        return ps == WIFI_PS_MIN_MODEM;
    }
    return false;
}

/**
 * control wifi tx power
 * @param power enum maximum wifi tx power
 * @return ok
 */
bool esp32_setTxPower(wifi_power_t power)
{
    if((esp32_getStatusBits() & (STA_STARTED_BIT | AP_STARTED_BIT)) == 0) {
        MOLMC_LOGD(TAG, "Neither AP or STA has been started");
        return false;
    }
    return esp_wifi_set_max_tx_power(power) == ESP_OK;
}

wifi_power_t esp32_getTxPower(void)
{
    int8_t power;
    if((esp32_getStatusBits() & (STA_STARTED_BIT | AP_STARTED_BIT)) == 0) {
        MOLMC_LOGD(TAG, "Neither AP or STA has been started");
        return WIFI_POWER_19_5dBm;
    }
    if(esp_wifi_get_max_tx_power(&power)) {
        return WIFI_POWER_19_5dBm;
    }
    return (wifi_power_t)power;
}

uint8_t* esp32_getMacAddress(uint8_t* mac)
{
    esp_wifi_get_mac(WIFI_IF_STA, mac);
    return mac;
}

bool esp32_setDHCP(char enable)
{
    if(true == enable) {
        tcpip_adapter_dhcpc_start(TCPIP_ADAPTER_IF_STA);
    } else {
        tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA);
    }
    return true;
}

bool esp32_setAutoConnect(bool autoConnect)
{
    bool ret;
    ret = esp_wifi_set_auto_connect(autoConnect);
    return ret;
}

bool esp32_getAutoConnect(void)
{
    bool autoConnect;
    esp_wifi_get_auto_connect(&autoConnect);
    return autoConnect;
}

static bool _autoReconnect = false;
bool esp32_setAutoReconnect(bool autoReconnect)
{
    _autoReconnect = autoReconnect;
    return true;
}

bool esp32_getAutoReconnect(void)
{
    return _autoReconnect;
}

int32_t esp32_getRSSI(void)
{
    wifi_ap_record_t info;
    if(!esp_wifi_sta_get_ap_info(&info)) {
        return info.rssi;
    }
    return 0;
}

int esp32_setConfig(wifi_config_t *conf)
{
    esp_wifi_set_config(WIFI_IF_STA, conf);
    return 0;
}

bool esp32_begin(void)
{
    if(!esp32_enableSTA(true)) {
        return false;
    }
    esp_wifi_start();
    esp_wifi_connect();
    esp32_setDHCP(true);
    return true;
}

int esp32_connect(void)
{
    if ( esp_wifi_connect() == ESP_OK ) {
        return 0;
    }
    return 1;
}

int esp32_disconnect(void)
{
    if ( esp_wifi_disconnect() == ESP_OK ) {
        return 0;
    }
    return 1;
}

static bool _smartConfigStarted = false;
static bool _smartConfigDone = false;

static void smartConfigCallback(uint32_t st, void *result)
{
    smartconfig_status_t status = (smartconfig_status_t) st;
    MOLMC_LOGD(TAG, "beginSmartConfig status = %d\r\n", status);
    if (status == SC_STATUS_LINK) {
        wifi_sta_config_t *sta_conf = (wifi_sta_config_t *)result;
        MOLMC_LOGD(TAG, "ssid     = %s\r\n", sta_conf->ssid);
        MOLMC_LOGD(TAG, "password = %s\r\n", sta_conf->password);
        esp_wifi_set_config(WIFI_IF_STA, (wifi_config_t *)sta_conf);
        esp_wifi_connect();
    } else if (status == SC_STATUS_LINK_OVER) {
        _smartConfigDone = true;
    }
}

bool esp32_beginSmartConfig(void)
{
    MOLMC_LOGD(TAG, "esp32_beginSmartConfig\r\n");
    if (_smartConfigStarted) {
        return false;
    }

    if (!esp32_setMode(WIFI_MODE_STA)) {
        return false;
    }

    esp_wifi_disconnect();

    esp_err_t err;
    err = esp_smartconfig_start((sc_callback_t)(&smartConfigCallback), 1);
    if (err == ESP_OK) {
        _smartConfigStarted = true;
        _smartConfigDone = false;
        return true;
    }
    return false;
}

bool esp32_stopSmartConfig(void)
{
    MOLMC_LOGD(TAG, "esp8266_stopSmartConfig\r\n");
    if (!_smartConfigStarted) {
        return true;
    }

    if (esp_smartconfig_stop() == ESP_OK) {
        _smartConfigStarted = false;
        return true;
    }

    return false;
}

bool esp32_smartConfigDone(void)
{
    if(!_smartConfigStarted) {
        return false;
    }
    return _smartConfigDone;
}

// -----------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------ Generic Network function ---------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

/**
 * DNS callback
 * @param name
 * @param ipaddr
 * @param callback_arg
 */
static void wifi_dns_found_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg)
{
    if(ipaddr) {
        (*(uint32_t*)(callback_arg)) = ipaddr->u_addr.ip4.addr;
    }
    xEventGroupSetBits(_network_event_group, WIFI_DNS_DONE_BIT);
}

/**
 * Resolve the given hostname to an IP address.
 * @param hostname     Name to be resolved
 * @param ip_addr       IPAddress structure to store the returned IP address
 * @return 1 if aIPAddrString was successfully converted to an IP address,
 *          else error code
 */
int esp32_gethostbyname(const char* hostname, uint16_t hostnameLen, uint32_t *ip_addr)
{
    ip_addr_t addr;

    if(!strcmp(hostname, "255.255.255.255")) {
        *ip_addr = IPADDR_NONE;
        return 0;
    }

    addr.u_addr.ip4.addr = ipaddr_addr(hostname);
    if (addr.u_addr.ip4.addr != IPADDR_NONE) {
        *ip_addr = addr.u_addr.ip4.addr;
        return 0;
    }

    esp32_waitStatusBits(WIFI_DNS_IDLE_BIT, 5000);
    esp32_clearStatusBits(WIFI_DNS_IDLE_BIT);
    err_t err = dns_gethostbyname(hostname, &addr, &wifi_dns_found_callback, ip_addr);
    if(err == ERR_OK && addr.u_addr.ip4.addr) {
        *ip_addr = addr.u_addr.ip4.addr;
    } else if(err == ERR_INPROGRESS) {
        esp32_waitStatusBits(WIFI_DNS_DONE_BIT, 4000);
        esp32_clearStatusBits(WIFI_DNS_DONE_BIT);
    }
    esp32_setStatusBits(WIFI_DNS_IDLE_BIT);
    return ((*ip_addr) == 0) ? -1 : 0;
}

