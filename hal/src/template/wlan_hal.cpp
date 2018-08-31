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

uint32_t HAL_NET_SetNetWatchDog(uint32_t timeOutInMS)
{
    return 0;
}

int HAL_WLAN_Clear_Credentials(void)
{
    return 0;
}

int HAL_WLAN_Has_Credentials(void)
{
    return 0;
}

wlan_result_t HAL_WLAN_Activate(void)
{
    return 0;
}

wlan_result_t HAL_WLAN_Deactivate(void)
{
    return 0;
}

int wlan_connect(void)
{
    return 0;
}

wlan_result_t wlan_disconnect(void)
{
    return 0;
}

int HAL_WLAN_Connected_Rssi(void)
{
    return 0;
}

int HAL_WLAN_Set_Credentials(WLanCredentials* c)
{
    return -1;
}

void HAL_WLAN_Imlink_Start(void)
{
}

imlink_status_t HAL_WLAN_Imlink_Get_Status(void)
{
    return IMLINK_SUCCESS;
}

void HAL_WLAN_Imlink_Stop(void)
{
}

void HAL_WLAN_Setup(void)
{
}

void HAL_WLAN_Fetch_Ipconfig(WLanConfig* config)
{
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

int HAL_WLAN_Scan(wlan_scan_result_t callback, void* cookie)
{
    return -1;
}

/**
 * Lists all WLAN credentials currently stored on the device
 */
int HAL_WLAN_Get_Credentials(wlan_scan_result_t callback, void* callback_data)
{
    // Reading credentials from the CC3000 is not possible
    return 0;
}

/**
 * wifi get station and ap mac addr
 */
int HAL_WLAN_Get_Macaddr(uint8_t *stamacaddr, uint8_t *apmacaddr)
{
    return 0;
}
