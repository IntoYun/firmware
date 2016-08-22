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
#include "esp8266_wifi_generic.h"

extern "C" {
#include "c_types.h"
#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "user_interface.h"
#include "lwip/err.h"
#include "lwip/dns.h"
}


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
    return esp8266_connect();
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
    esp8266_beginSmartConfig();
}

imlink_status_t wlan_Imlink_get_status()
{
    if(!esp8266_smartConfigDone)
    return IMLINK_DOING;
    else
    return IMLINK_SUCCESS;
}

void wlan_Imlink_stop()
{
    esp8266_stopSmartConfig();
}

void wlan_setup()
{
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