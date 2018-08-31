/**
 ******************************************************************************
  Copyright (c) 2015 IntoRobot Industries, Inc.  All rights reserved.

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

#ifndef SYSTEM_NETWORK_CELLULAR_H
#define SYSTEM_NETWORK_CELLULAR_H


#include "intorobot_config.h"
#ifdef configWIRING_CELLULAR_ENABLE

#include "system_network_internal.h"
#include "cellular_hal.h"
#include "interrupts_hal.h"
#include "wiring_interrupts.h"

void network_notify_connected();
void network_notify_disconnected();
void network_notify_can_shutdown();
void network_notify_dhcp(bool dhcp);

class CellularNetworkInterface : public ManagedIPNetworkInterface<CellularConfig, CellularNetworkInterface>
{
    volatile bool connect_cancelled = false;
    volatile bool connecting = false;

protected:
    virtual void connect_init() override { /* n/a */ }

    void connect_finalize() override {
        ATOMIC_BLOCK() { connecting = true; }

        HAL_Cellular_Connect(nullptr);

        bool require_resume = false;

        ATOMIC_BLOCK() {
            // ensure after connection exits the cancel flag is cleared if it was set during connection
            if (connect_cancelled) {
                require_resume = true;
            }
            connecting = false;
        }
        if (require_resume) {
            HAL_Cellular_Cancel(false, HAL_IsISR(), NULL);
        }
    }

    void drive_now() override {
        HAL_Cellular_Drive();
    }

    void on_now() override {
        HAL_Cellular_On(NULL);
    }

    void off_now() override {
        HAL_Cellular_Disconnect(nullptr);
        HAL_Cellular_Off(NULL);
    }

    void disconnect_now() override {
        HAL_Cellular_Disconnect(nullptr);
    }

public:

    CellularNetworkInterface() {
        HAL_NET_Callbacks cb;
        cb.size = sizeof(HAL_NET_Callbacks);
        cb.notify_connected = network_notify_connected;
        cb.notify_disconnected = network_notify_disconnected;
        cb.notify_dhcp = network_notify_dhcp;
        cb.notify_can_shutdown = network_notify_can_shutdown;
        HAL_NET_SetCallbacks(&cb, nullptr);
    }

    void fetch_ipconfig(CellularConfig* target)  {
        HAL_Cellular_Fetch_Ipconfig(target, NULL);
    }

    void setup() override { /* n/a */ }

    // todo - associate credentials with presense of SIM card??
    bool clear_credentials() override { /* n/a */ return true; }
    bool has_credentials() override { /* n/a */ return true; }
    int set_credentials(NetworkCredentials* creds) override { /* n/a */ return -1; }

    void connect_cancel(bool cancel) override {
        // only cancel if presently connecting
        bool require_cancel = false;
        ATOMIC_BLOCK() {
            if (connecting)
            {
                if (cancel!=connect_cancelled) {
                    require_cancel = true;
                    connect_cancelled = cancel;
                }
            }
        }
        if (require_cancel) {
            HAL_Cellular_Cancel(cancel, HAL_IsISR(), NULL);
        }
    }

    void set_error_count(unsigned count) override { /* n/a */ }
};

#endif

#endif
