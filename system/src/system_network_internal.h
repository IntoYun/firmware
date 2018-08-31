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

#ifndef SYSTEM_NETWORK_INTERNAL_H
#define SYSTEM_NETWORK_INTERNAL_H

#include "intorobot_config.h"
#ifndef configNO_NETWORK

#include "wiring_ticks.h"
#include "system_event.h"
#include "system_network.h"
#include "system_rgbled.h"
#include "system_cloud.h"
#include "system_threading.h"

const static char *GTAG = "sys-network";

enum eWanTimings
{
    CONNECT_TO_ADDRESS_MAX = S2M(30),
    DISCONNECT_TO_RECONNECT = S2M(2),
};

extern volatile uint8_t SYSTEM_NETWORK_STARTED;

extern uint32_t network_watchdog_duration;
extern uint32_t network_watchdog_base;

inline void ARM_NETWORK_WD(uint32_t x) {
    network_watchdog_base = HAL_Tick_Get_Milli_Seconds();
    network_watchdog_duration = x;
    MOLMC_LOGD(GTAG, "Network WD Set %d",(x));
}

inline bool IS_NETWORK_TIMEOUT() {
    return network_watchdog_duration && ((HAL_Tick_Get_Milli_Seconds()-network_watchdog_base)>network_watchdog_duration);
}

inline void CLR_NETWORK_WD() {
    network_watchdog_duration = 0;
    MOLMC_LOGD(GTAG, "Network WD Cleared, was %d",network_watchdog_duration);
}

/**
 * Internal network interface class to provide polymorphic behavior for each
 * network type.  This is not part of the dynalib so functions can freely evolve.
 */
struct NetworkInterface
{
    virtual network_interface_t network_interface()=0;
    virtual void setup()=0;

    virtual void on()=0;
    virtual void off(bool disconnect_cloud=false)=0;
    virtual void connect(bool listen_enabled=true)=0;
    virtual bool connecting()=0;
    virtual bool connected()=0;
    virtual void connect_cancel(bool cancel)=0;
    /**
     * Force a manual disconnct.
     */
    virtual void disconnect()=0;

    /**
     * @return {@code true} if this connection was manually taken down.
     */
    virtual bool manual_disconnect()=0;
    /**
     * Perform the 10sec press command, e.g. clear credentials.
     */
    virtual bool ready()=0;

    virtual bool clear_credentials()=0;
    virtual bool has_credentials()=0;
    virtual int set_credentials(NetworkCredentials* creds)=0;

    virtual void config_clear()=0;
    virtual void update_config(bool force=false)=0;
    virtual void* config()=0;       // not really happy about lack of type
};


class ManagedNetworkInterface : public NetworkInterface
{
    volatile uint8_t NETWORK_DISCONNECT;
    volatile uint8_t NETWORK_DELETE_PROFILES;
    volatile uint8_t NETWORK_CONNECTED;
    volatile uint8_t NETWORK_CONNECTING;
    volatile uint8_t NETWORK_DHCP;
    volatile uint8_t NETWORK_CAN_SHUTDOWN;

protected:
    virtual network_interface_t network_interface() override { return 0; }

    virtual void connect_init()=0;
    virtual void connect_finalize()=0;
    virtual void disconnect_now()=0;
    virtual void drive_now()=0;

    virtual void on_now()=0;
    virtual void off_now()=0;

public:

    virtual void get_ipconfig(IPConfig* config)=0;

    virtual void set_error_count(unsigned count)=0;

    bool manual_disconnect() override
    {
        return NETWORK_DISCONNECT;
    }

    void set_manual_disconnect(bool disconnect)
    {
        NETWORK_DISCONNECT = disconnect;
    }

    bool connected() override
    {
        return NETWORK_CONNECTED;
    }

    void connect(bool listen_enabled=true) override
    {
        MOLMC_LOGD(GTAG, "ready(): %d; connecting(): %d", (int)ready(), (int)connecting());
        if (!ready() && !connecting()) {
            on(); // activate WiFi

            NETWORK_DISCONNECT = 0;
            connect_init();
            SYSTEM_NETWORK_STARTED = 1;

            if (!has_credentials()) {
                disconnect();
            } else {
                NETWORK_CONNECTING = 1;
                MOLMC_LOGD(GTAG, "ARM_NETWORK_WD 1");
                ARM_NETWORK_WD(CONNECT_TO_ADDRESS_MAX);    // reset the network if it doesn't connect within the timeout
                system_notify_event(event_network_status, ep_network_status_connecting);
                connect_finalize();
            }
        }
    }

    void disconnect() override
    {
        const bool was_connected = NETWORK_CONNECTED;
        const bool was_connecting = NETWORK_CONNECTING;

        if (SYSTEM_NETWORK_STARTED) {
            NETWORK_DISCONNECT = 1; //Do not ARM_NETWORK_WD() in WLAN_Async_Callback()
            NETWORK_CONNECTING = 0;
            NETWORK_CONNECTED = 0;
            NETWORK_DHCP = 0;

            CLOUD_FN(cloud_disconnect(), (void)0);
            if (was_connected) {
                // "Disconnecting" event is generated only for a successfully established connection
                system_notify_event(event_network_status, ep_network_status_disconnecting);
            }
            disconnect_now();
            config_clear();
            if (was_connected || was_connecting) {
                system_notify_event(event_network_status, ep_network_status_disconnected);
            }
            system_rgb_blink(RGB_COLOR_GREEN, 1000);//绿灯闪烁
        }
    }

    bool ready() override
    {
        drive_now();
        update_config(false);
        return (SYSTEM_NETWORK_STARTED && NETWORK_DHCP);
    }

    bool connecting() override
    {
        drive_now();
        update_config(false);
        return (SYSTEM_NETWORK_STARTED && NETWORK_CONNECTING);
    }

    void on() override
    {
        if (!SYSTEM_NETWORK_STARTED) {
            system_notify_event(event_network_status, ep_network_status_powering_on);
            config_clear();
            on_now();
            //update_config(true);
            SYSTEM_NETWORK_STARTED = 1;
            system_notify_event(event_network_status, ep_network_status_on);
        }
    }

    void off(bool disconnect_cloud=false) override
    {
        if (SYSTEM_NETWORK_STARTED) {
            disconnect();
            system_notify_event(event_network_status, ep_network_status_powering_off);
            off_now();

            if (disconnect_cloud) {
                CLOUD_FN(intorobot_cloud_flag_disconnect(), (void)0);
            }

            SYSTEM_NETWORK_STARTED = 0;
            NETWORK_DHCP = 0;
            NETWORK_CONNECTED = 0;
            NETWORK_CONNECTING = 0;
            system_notify_event(event_network_status, ep_network_status_off);
        }
    }

    void notify_connected()
    {
        NETWORK_CONNECTED = 1;
        NETWORK_CONNECTING = 0;

        /* If DHCP has completed, don't re-arm WD due to spurious notify_connected()
         * from WICED on loss of internet and reconnect
         */
        if (!NETWORK_DISCONNECT && !NETWORK_DHCP) {
            MOLMC_LOGD(GTAG, "ARM_NETWORK_WD 2");
            ARM_NETWORK_WD(CONNECT_TO_ADDRESS_MAX);
        }
        MOLMC_LOGD(GTAG, "notify_connected");
        system_notify_event(event_network_status, ep_network_status_connected);
        system_rgb_blink(RGB_COLOR_BLUE, 1000);//蓝灯闪烁
    }

    void notify_disconnected()
    {
        CLOUD_FN(cloud_disconnect(), (void)0);
        if (NETWORK_CONNECTED) {
            //Breathe blue if established connection gets disconnected
            if (!NETWORK_DISCONNECT) {
                //if WiFi.disconnect called, do not enable wlan watchdog
                MOLMC_LOGD(GTAG, "ARM_NETWORK_WD 3");
                ARM_NETWORK_WD(DISCONNECT_TO_RECONNECT);
            }
        }
        NETWORK_CONNECTED = 0;
        NETWORK_CONNECTING = 0;
        NETWORK_DHCP = 0;
        system_notify_event(event_network_status, ep_network_status_disconnected);
        system_rgb_blink(RGB_COLOR_GREEN, 1000);//绿灯闪烁
    }

    void notify_dhcp(bool dhcp)
    {
        NETWORK_CONNECTING = 0;
        if (dhcp) {
            MOLMC_LOGD(GTAG, "CLR_NETWORK_WD 1, DHCP success");
            CLR_NETWORK_WD();
            NETWORK_DHCP = 1;
        } else {
            config_clear();
            NETWORK_DHCP = 0;
            MOLMC_LOGD(GTAG, "DHCP fail, ARM_NETWORK_WD 5");
            ARM_NETWORK_WD(DISCONNECT_TO_RECONNECT);
        }
    }

    void notify_can_shutdown()
    {
        NETWORK_CAN_SHUTDOWN = 1;
    }

    void notify_cannot_shutdown()
    {
        NETWORK_CAN_SHUTDOWN = 0;
    }

    inline bool hasDHCP()
    {
        return NETWORK_DHCP;
    }
};

extern ManagedNetworkInterface& network;

template <typename Config, typename C>
class ManagedIPNetworkInterface : public ManagedNetworkInterface
{
    Config ip_config;

public:
    void get_ipconfig(IPConfig* config) override
    {
        update_config(true);
        memcpy(config, this->config(), config->size);
    }

    void update_config(bool force=false) override
    {
        // todo - IPv6 may not set this field.
        bool fetched_config = ip_config.nw.aucIP.ipv4!=0;
        if (hasDHCP() || force) {
            if (!fetched_config || force) {
                memset(&ip_config, 0, sizeof(ip_config));
                ip_config.size = sizeof(ip_config);
                reinterpret_cast<C*>(this)->fetch_ipconfig(&ip_config);
            }
        } else if (fetched_config) {
            config_clear();
        }
    }

    void config_clear() override
    {
        memset(&ip_config, 0, sizeof(ip_config));
    }

    void* config() override  { return &ip_config; }

};

#endif

#endif/* SYSTEM_NETWORK_INTERNAL_H */

