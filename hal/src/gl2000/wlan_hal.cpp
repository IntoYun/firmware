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

#include <stdlib.h>
#include "wlan_hal.h"
#include "delay_hal.h"
#include "parser.h"

//=======net notify===========
static HAL_NET_Callbacks netCallbacks = { 0 };

void HAL_NET_SetCallbacks(const HAL_NET_Callbacks* callbacks, void* reserved)
{
    netCallbacks.notify_connected = callbacks->notify_connected;
    netCallbacks.notify_disconnected = callbacks->notify_disconnected;
    netCallbacks.notify_dhcp = callbacks->notify_dhcp;
    netCallbacks.notify_can_shutdown = callbacks->notify_can_shutdown;
}

void HAL_NET_notify_connected()
{
    if (netCallbacks.notify_connected) {
        netCallbacks.notify_connected();
    }
}

void HAL_NET_notify_disconnected()
{
    if (netCallbacks.notify_disconnected) {
        netCallbacks.notify_disconnected();
    }
}

void HAL_NET_notify_dhcp(bool dhcp)
{
    if (netCallbacks.notify_dhcp) {
        netCallbacks.notify_dhcp(dhcp); // dhcp dhcp
    }
}

void HAL_NET_notify_can_shutdown()
{
    if (netCallbacks.notify_can_shutdown) {
        netCallbacks.notify_can_shutdown();
    }
}

uint32_t HAL_NET_SetNetWatchDog(uint32_t timeOutInMS)
{
    return 0;
}

//=======wifi activate/deactivate===========
wlan_result_t HAL_WLAN_On(void)
{
    return 0;
}

wlan_result_t HAL_WLAN_Off(void)
{
    return 0;
}

void HAL_WLAN_Setup(void)
{
    esp8266MDM.reset();
    esp8266MDM.init();
}

//=======wifi connect===========
wlan_result_t HAL_WLAN_Connect(void)
{
    return 0;
}

wlan_result_t HAL_WLAN_Disconnect(void)
{
    return 0;
}

void HAL_WLAN_Connect_Cancel(bool called_from_isr)
{

}

int HAL_WLAN_Connected_Rssi(void)
{
    wifi_info_t wifiInfo;

    if( true == esp8266MDM.getWifiInfo(&wifiInfo) ) {
        return wifiInfo.rssi;
    }
    return 0;
}

void HAL_WLAN_Drive(void)
{
    esp8266MDM.drive();
}

//================credentials======================
int HAL_WLAN_Clear_Credentials(void)
{
    return 0;
}

int HAL_WLAN_Has_Credentials(void)
{
    return 0;
}

bool HAL_WLAN_Reset_Credentials_Store_Required(void)
{
    return false;
}

wlan_result_t HAL_WLAN_Reset_Credentials_Store(void)
{
    HAL_WLAN_Clear_Credentials();
    return 0;
}

int HAL_WLAN_Get_Credentials(wlan_scan_result_t callback, void* callback_data)
{
    return 0;
}

int HAL_WLAN_Set_Credentials(WLanCredentials* c)
{
    if(JOINAP_SUCCESS == esp8266MDM.wifiJoinAp(c->ssid, c->password)) {
        return 0;
    }
    return -1;
}

//==============imlink==================
void HAL_WLAN_Imlink_Start(void)
{
    //esp8266MDM.reset();
    //esp8266MDM.init();
    esp8266MDM.startSmartconfig(SMARTCONFIGTYPE_ESPTOUCH);
}

imlink_status_t HAL_WLAN_Imlink_Get_Status(void)
{
    return (imlink_status_t)esp8266MDM.getSmartconfigStatus();
}

void HAL_WLAN_Imlink_Stop(void)
{
    esp8266MDM.stopSmartconfig();
}

void HAL_WLAN_Fetch_Ipconfig(WLanConfig* config)
{
    wifi_addr_t addr;
    wifi_info_t wifiInfo;

    memset(config, 0, sizeof(WLanConfig));
    config->size = sizeof(WLanConfig);

    if( true == esp8266MDM.getAddress(&addr) ) {
        config->nw.aucIP.ipv4 = addr.IpAddr;
        memcpy(config->nw.uaMacAddr, addr.MacAddr, 6);
    }
    if( true == esp8266MDM.getWifiInfo(&wifiInfo) ) {
        memcpy(config->uaSSID, wifiInfo.ssid, 33);
        memcpy(config->BSSID, wifiInfo.bssid, 6);
    }
}

void HAL_WLAN_Set_Error_Count(uint32_t errorCount)
{
}

/**
 * Sets the IP source - static or dynamic.
 */
void HAL_WLAN_Set_Ipaddress_Source(IPAddressSource source, bool persist, void* reserved)
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
void HAL_WLAN_Set_Ipaddress(const HAL_IPAddress* device, const HAL_IPAddress* netmask,
        const HAL_IPAddress* gateway, const HAL_IPAddress* dns1, const HAL_IPAddress* dns2, void* reserved)
{

}

WLanSecurityType toSecurityType(uint8_t security)
{
    switch(security)
    {
        case 0: //AUTH_OPEN
            return WLAN_SEC_UNSEC;
            break;
        case 1: //AUTH_WEP
            return WLAN_SEC_WEP;
            break;
        case 2: //AUTH_WPA_PSK
            return WLAN_SEC_WPA;
            break;
        case 3: //AUTH_WPA2_PSK
        case 4: //AUTH_WPA_WPA2_PSK
            return WLAN_SEC_WPA2;
            break;
        default:
            return WLAN_SEC_NOT_SET;
            break;
    }
}

WLanSecurityCipher toCipherType(uint8_t security)
{
    switch(security)
    {
        case 1: //AUTH_WEP
            return WLAN_CIPHER_AES;
            break;
        case 2: //AUTH_WPA_PSK
            return WLAN_CIPHER_TKIP;
            break;
        default:
            break;
    }
    return WLAN_CIPHER_NOT_SET;
}

#define WLAN_SCAN_AP_NUM 25
int HAL_WLAN_Scan(wlan_scan_result_t callback, void* cookie)
{
    //申请内存
    wifi_ap_t *aps = (wifi_ap_t *)malloc(sizeof(wifi_ap_t)*WLAN_SCAN_AP_NUM);
    if(aps == NULL) {
        return -1;
    }

    int result = esp8266MDM.apScan(aps, WLAN_SCAN_AP_NUM);
    //填充ap 列表
    if(result) {
        WiFiAccessPoint data;
        for(int n = 0; n < result; n++)
        {
            memset(&data, 0, sizeof(WiFiAccessPoint));
            memcpy(data.ssid, aps[n].ssid, aps[n].ssid_len);
            data.ssidLength = aps[n].ssid_len;
            memcpy(data.bssid, aps[n].bssid, 6);
            data.security = toSecurityType(aps[n].security);
            data.cipher = toCipherType(aps[n].security);
            data.channel = aps[n].channel;
            data.rssi = aps[n].rssi;
            callback(&data, cookie);
        }
    }
    free(aps);
    return result;
}

/**
 * wifi get station and ap mac addr
 */
int HAL_WLAN_Get_Macaddr(uint8_t *stamacaddr, uint8_t *apmacaddr)
{
    return 0;
}
