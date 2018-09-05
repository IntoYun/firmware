/**
 ******************************************************************************
  Copyright (c) 2013-2015 IntoRobot Industries, Inc.  All rights reserved.

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

#ifndef SYSTEM_NETWORK_WIFI_H
#define	SYSTEM_NETWORK_WIFI_H

#include "firmware_platform_config.h"
#if FIRMWARE_CONFIG_WIRING_WIFI

#include "system_network_internal.h"
#include "wlan_hal.h"
#include "interrupts_hal.h"

void network_notify_connected();
void network_notify_disconnected();
void network_notify_can_shutdown();
void network_notify_dhcp(bool dhcp);

class WiFiNetworkInterface : public ManagedIPNetworkInterface<WLanConfig, WiFiNetworkInterface>
{
protected:
    void connect_init() override {}

    void connect_finalize() override
    {
        HAL_WLAN_Connect();
    }

    void disconnect_now() override
    {
        HAL_WLAN_Disconnect();
    }

    void drive_now() override
    {
        HAL_WLAN_Drive();
    }

    void on_now() override { HAL_WLAN_On(); }
    void off_now() override { HAL_WLAN_Off(); }

public:

    WiFiNetworkInterface() {
        HAL_NET_Callbacks cb;
        cb.size = sizeof(HAL_NET_Callbacks);
        cb.notify_connected = network_notify_connected;
        cb.notify_disconnected = network_notify_disconnected;
        cb.notify_dhcp = network_notify_dhcp;
        cb.notify_can_shutdown = network_notify_can_shutdown;
        HAL_NET_SetCallbacks(&cb, nullptr);
    }

    void connect_cancel(bool cancel) override
    {
        if (cancel)
            HAL_WLAN_Connect_Cancel(HAL_IsISR());
    }

    bool has_credentials() override
    {
        return HAL_WLAN_Has_Credentials()==0;
    }

    int set_credentials(NetworkCredentials* credentials) override
    {
        if (!SYSTEM_NETWORK_STARTED || !credentials) {
            return -1;
        }

        WLanSecurityType security = credentials->security;

        if (0 == credentials->password[0]) {
            security = WLAN_SEC_UNSEC;
        }

        credentials->security = security;

        int result = HAL_WLAN_Set_Credentials(credentials);
        if (!result) {
            //system_notify_event(network_credentials, network_credentials_added, credentials);
        }
        return result;
    }

    bool clear_credentials() override
    {
        return HAL_WLAN_Clear_Credentials() == 0;
    }

    void setup() override
    {
        HAL_WLAN_Setup();

        if (HAL_WLAN_Reset_Credentials_Store_Required()) {
            HAL_WLAN_Reset_Credentials_Store();
        }
    }

    void fetch_ipconfig(WLanConfig* target)
    {
        HAL_WLAN_Fetch_Ipconfig(target);
    }

    void set_error_count(unsigned count) override
    {
        HAL_WLAN_Set_Error_Count(count);
    }
};

#endif

#endif
