/**
 ******************************************************************************
  Copyright (c) 2013-2015 Particle Industries, Inc.  All rights reserved.

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


#include "system_network_internal.h"
#include "wlan_hal.h"
#include "interrupts_hal.h"

class WiFiNetworkInterface : public ManagedIPNetworkInterface<WLanConfig, WiFiNetworkInterface>
{
protected:
    void connect_now() override { wlan_connect(); }
    void disconnect_now() override { wlan_disconnect(); }
    int status_now() override { return wlan_status(); }
    void on_now() override { wlan_activate(); }
    void off_now() override { wlan_deactivate(); }

public:
    bool has_credentials() override
    {
        return wlan_has_credentials()==0;
    }

    int set_credentials(NetworkCredentials* credentials) override
    {
        if (!INTOROBOT_WLAN_STARTED || !credentials)
        {
            return -1;
        }

        WLanSecurityType security = credentials->security;

        if (0 == credentials->password[0])
        {
            security = WLAN_SEC_UNSEC;
        }

        credentials->security = security;

        int result = wlan_set_credentials(credentials);
        if (!result)
            system_notify_event(network_credentials, network_credentials_added, credentials);
        return result;
    }

    bool clear_credentials() override
    {
        return wlan_clear_credentials() == 0;
    }

    void setup() override
    {
        wlan_setup();
    }

    void fetch_ipconfig(WLanConfig* target)
    {
        wlan_fetch_ipconfig(target);
    }

    void set_error_count(unsigned count) override
    {
        wlan_set_error_count(count);
    }
};

#endif
