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
#include "esp_wifi_types.h"
#include "esp32_wifi_generic.h"
// #include "esp_wifi.h"
// #include "esp_smartconfig.h"
// #include "esp_err.h"
// #include "WString.h"

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
    return -1;
}

int wlan_connected_rssi(void)
{
    // return esp32_getRSSI();
}

int wlan_set_credentials(WLanCredentials* c)
{
    return -1;
}

void wlan_Imlink_start()
{
    // esp32_beginSmartConfig();
}

imlink_status_t wlan_Imlink_get_status()
{
    // if(!esp32_smartConfigDone())
    //     return IMLINK_DOING;
    // else
        return IMLINK_SUCCESS;
}

void wlan_Imlink_stop()
{
   // esp32_stopSmartConfig();
}

void wlan_setup()
{
    // esp32_setMode(WIFI_MODE_STA);
    // esp32_setDHCP(true);
    // esp32_setAutoConnect(true);
    // esp32_setAutoReconnect(true);
}

void wlan_fetch_ipconfig(WLanConfig* config)
{
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
    #if 0
    if(!esp_wifi_get_mac(WIFI_IF_STA, stamacaddr))
    {
        return -1;
    }

    if(!esp_wifi_get_mac(WIFI_IF_AP, apmacaddr))
    {
        return -1;
    }
    #endif
    #if 0
    if(!esp32_getMacAddress(stamacaddr))
    {
        return -1;
    }

    if(!esp32_getMacAddress(apmacaddr))
    {
        return -1;
    }
    #endif
    return 0;
}
