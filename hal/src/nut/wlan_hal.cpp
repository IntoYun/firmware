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

#include "hw_config.h"
#include "wlan_hal.h"
#include "esp8266_wifi_generic.h"
#include "flash_map.h"
#include "memory_hal.h"
#include "macaddr_hal.h"

uint32_t HAL_NET_SetNetWatchDog(uint32_t timeOutInMS)
{
    return 0;
}

int wlan_clear_credentials()
{
    return 0;
}

int wlan_has_credentials()
{
    return 0;
}

wlan_result_t wlan_activate()
{
    return 0;
}

wlan_result_t wlan_deactivate()
{
    return 0;
}

int wlan_connect()
{
    if(wlan_status()) {
        return esp8266_connect();
    }
    else {
        return 0;
    }
}

wlan_result_t wlan_disconnect()
{
    return esp8266_disconnect();
}

int wlan_status()
{
    wl_status_t status = esp8266_status();
    switch(status) {
        case WL_CONNECTED:
            return 0;
        default:
            return 1;
    }
    return 0;
}

int wlan_connected_rssi(void)
{
    return esp8266_getRSSI();
}

int wlan_set_credentials(WLanCredentials* c)
{
    struct station_config conf;
    strcpy(reinterpret_cast<char*>(conf.ssid), c->ssid);

    if(c->password) {
        if (strlen(c->password) == 64) // it's not a passphrase, is the PSK
            memcpy(reinterpret_cast<char*>(conf.password), c->password, 64);
        else
            strcpy(reinterpret_cast<char*>(conf.password), c->password);
    } else {
        *conf.password = 0;
    }
    conf.bssid_set = 0;

    ETS_UART_INTR_DISABLE();
    // workaround for #1997: make sure the value of ap_number is updated and written to flash
    // to be removed after SDK update
    wifi_station_ap_number_set(2);
    wifi_station_ap_number_set(1);

    wifi_station_set_config(&conf);
    ETS_UART_INTR_ENABLE();
    return 0;
}

void wlan_Imlink_start()
{
    esp8266_setAutoReconnect(false);
    wlan_disconnect();
    esp8266_beginSmartConfig();
}

imlink_status_t wlan_Imlink_get_status()
{
    if(!esp8266_smartConfigDone())
    return IMLINK_DOING;
    else
    return IMLINK_SUCCESS;
}

void wlan_Imlink_stop()
{
    esp8266_stopSmartConfig();
    esp8266_setAutoReconnect(true);
}

void wlan_setup()
{
}

void wlan_fetch_ipconfig(WLanConfig* config)
{
    memset(config, 0, sizeof(WLanConfig));
    config->size = sizeof(WLanConfig);

    struct ip_info ip;
    wifi_get_ip_info(STATION_IF, &ip);
    config->nw.aucIP.ipv4 = ip.ip.addr;
    config->nw.aucSubnetMask.ipv4 = ip.netmask.addr;
    config->nw.aucDefaultGateway.ipv4 = ip.gw.addr;

    ip_addr_t dns_ip = dns_getserver(0);
    config->nw.aucDNSServer.ipv4 = dns_ip.addr;
    wifi_get_macaddr(STATION_IF, config->nw.uaMacAddr);

    struct station_config conf;
    wifi_station_get_config(&conf);
    memcpy(config->uaSSID, conf.ssid, 32);
    memcpy(config->BSSID, conf.bssid, 6);
}

void wlan_set_error_count(uint32_t errorCount)
{
}


/**
 * Sets the IP source - static or dynamic.
 */
void wlan_set_ipaddress_source(IPAddressSource source, bool persist, void* reserved)
{
}

/**
 * Sets the IP Addresses to use when the device is in static IP mode.
 * @param device
 * @param netmask
 * @param gateway
 * @param dns1
 * @param dns2
 * @param reserved
 */
void wlan_set_ipaddress(const HAL_IPAddress* device, const HAL_IPAddress* netmask,
        const HAL_IPAddress* gateway, const HAL_IPAddress* dns1, const HAL_IPAddress* dns2, void* reserved)
{

}

int wlan_scan(wlan_scan_result_t callback, void* cookie)
{
    return -1;
}

/**
 * Lists all WLAN credentials currently stored on the device
 */
int wlan_get_credentials(wlan_scan_result_t callback, void* callback_data)
{
    // Reading credentials from the CC3000 is not possible
    return 0;
}

#define STATION_IF      0x00
#define SOFTAP_IF       0x01


/**
 * wifi set station and ap mac addr
 */
int wlan_set_macaddr(uint8_t *stamacaddr, uint8_t *apmacaddr)
{
    if (stamacaddr != NULL && apmacaddr != NULL){
        mac_param_t mac_addrs;
        mac_addrs.header = FLASH_MAC_HEADER;

        memset(mac_addrs.stamac_addrs, 0, sizeof(mac_addrs.stamac_addrs));
        memcpy(mac_addrs.stamac_addrs, stamacaddr, sizeof(mac_addrs.stamac_addrs));

        memset(mac_addrs.apmac_addrs, 0, sizeof(mac_addrs.apmac_addrs));
        memcpy(mac_addrs.apmac_addrs, apmacaddr, sizeof(mac_addrs.apmac_addrs));

        uint32_t len = sizeof(mac_addrs);
        HAL_FLASH_Interminal_Erase(HAL_FLASH_Interminal_Get_Sector(FLASH_MAC_START_ADDR));
        HAL_FLASH_Interminal_Write(FLASH_MAC_START_ADDR, (uint32_t *)&mac_addrs, len);
        return 0;
    }
    return -1;
}

/**
 * wifi get station and ap mac addr
 */
int wlan_get_macaddr(uint8_t *stamacaddr, uint8_t *apmacaddr)
{
    if(!wifi_get_macaddr(STATION_IF, stamacaddr))
    {
        return -1;
    }
    if(!wifi_get_macaddr(SOFTAP_IF, apmacaddr))
    {
        return -1;
    }
    return 0;
}

int wlan_set_macaddr_from_flash(uint8_t *stamacaddr, uint8_t *apmacaddr)
{
    if(!wifi_set_macaddr(STATION_IF, stamacaddr))
        {
            return -1;
        }
    if(!wifi_set_macaddr(SOFTAP_IF, apmacaddr))
        {
            return -1;
        }
    return 0;
}

int wlan_set_macaddr_when_init(void)
{
    mac_param_t mac_addrs;
    HAL_FLASH_Interminal_Read(FLASH_MAC_START_ADDR, (uint32_t *)&mac_addrs, sizeof(mac_addrs));
    if (FLASH_MAC_HEADER == mac_addrs.header){
        esp8266_setMode(WIFI_AP_STA);
        wlan_set_macaddr_from_flash(mac_addrs.stamac_addrs, mac_addrs.apmac_addrs);
        esp8266_setMode(WIFI_STA);
        // for (int i = 0; i < 6; i++){
        //     DEBUG("stamac: %x", mac_addrs.stamac_addrs[i]);
        // }
        // HAL_FLASH_Interminal_Erase(HAL_FLASH_Interminal_Get_Sector(FLASH_MAC_START_ADDR));
    }
}

