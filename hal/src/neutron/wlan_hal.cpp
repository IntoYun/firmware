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

#include "wlan_hal.h"
#include "parser.h"

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
    return 0;
}

wlan_result_t wlan_disconnect()
{
    return 0;
}

int wlan_status()
{
    if(IPSTATUS_NOTCONNECTWIFI != esp8266MDM.getIpStatus()) {
        return 0;
    }
    return -1;
}

int wlan_connected_rssi(void)
{
    wifi_info_t wifiInfo;

    if( true == esp8266MDM.getWifiInfo(&wifiInfo) ){
        return wifiInfo.rssi;
    }
    return 0;
}

int wlan_set_credentials(WLanCredentials* c)
{
    if(JOINAP_SUCCESS == esp8266MDM.wifiJoinAp(c->ssid, c->password)) {
        return 0;
    }
    return -1;
}

#include "delay_hal.h"
void wlan_Imlink_start()
{
    //esp8266MDM.reset();
    //esp8266MDM.init();
    esp8266MDM.startSmartconfig(SMARTCONFIGTYPE_ESPTOUCH);
}

imlink_status_t wlan_Imlink_get_status()
{
    return (imlink_status_t)esp8266MDM.getSmartconfigStatus();
}

void wlan_Imlink_stop()
{
    esp8266MDM.stopSmartconfig();
}

void wlan_setup()
{
    //esp8266MDM.reset();
    //esp8266MDM.init();
}

void wlan_fetch_ipconfig(WLanConfig* config)
{
    wifi_addr_t addr;
    wifi_info_t wifiInfo;

    memset(config, 0, sizeof(WLanConfig));
    config->size = sizeof(WLanConfig);

    if( true == esp8266MDM.getAddress(&addr) ) {
        config->nw.aucIP.ipv4 = addr.IpAddr;
        memcpy(config->nw.uaMacAddr, addr.MacAddr, 6);
    }
    if( true == esp8266MDM.getWifiInfo(&wifiInfo) ){
        memcpy(config->uaSSID, wifiInfo.ssid, 33);
        memcpy(config->BSSID, wifiInfo.bssid, 6);
    }
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

/**
 * wifi set station and ap mac addr
 */
int wlan_set_macaddr(uint8_t *stamacaddr, uint8_t *apmacaddr)
{
    return 0;
}

/**
 * wifi get station and ap mac addr
 */
int wlan_get_macaddr(uint8_t *stamacaddr, uint8_t *apmacaddr)
{
    return 0;
}