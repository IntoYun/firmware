/*
 ******************************************************************************
  Copyright (c) 2014-2015 IntoRobot Industries, Inc.  All rights reserved.

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

#ifndef WIRING_CELLULAR_H_
#define WIRING_CELLULAR_H_

#include "firmware_config.h"

#ifdef FIRMWARE_CONFIG_WIRING_CELLULAR

#include "wiring_network.h"
#include "system_network.h"
#include "cellular_hal.h"
#include "inet_hal.h"
#include "wiring_cellular_printable.h"

namespace intorobot {

class CellularClass : public NetworkClass
{
    CellularDevice device;

public:

    IPAddress localIP() {
        return IPAddress(((CellularConfig*)network_config(*this, 0, NULL))->nw.aucIP);
    }
    void on() {
        network_on(*this, 0, 0, NULL);
    }
    void off() {
        network_off(*this, 0, 0, NULL);
    }
    void connect(unsigned flags=0) {
        network_connect(*this, flags, 0, NULL);
    }
    bool connecting(void) {
        return network_connecting(*this, 0, NULL);
    }

    void disconnect() {
        network_disconnect(*this, 0, NULL);
    }

    void setCredentials(const char* apn) {
        setCredentials(apn, "", "");
    }

    void setCredentials(const char* username, const char* password) {
        setCredentials("", username, password);
    }

    void setCredentials(const char* apn, const char* username, const char* password) {
        HAL_Cellular_Set_Credentials(apn, username, password, nullptr);
    }

    bool ready()
    {
        return network_ready(*this, 0,  NULL);
    }

    CellularSignal RSSI();

    template<typename... Targs>
    inline int command(const char* format, Targs... Fargs)
    {
        return HAL_Cellular_Command(NULL, NULL, 10000, format, Fargs...);
    }

    template<typename... Targs>
    inline int command(system_tick_t timeout_ms, const char* format, Targs... Fargs)
    {
        return HAL_Cellular_Command(NULL, NULL, timeout_ms, format, Fargs...);
    }

    template<typename T, typename... Targs>
    inline int command(int (*cb)(int type, const char* buf, int len, T* param),
            T* param, const char* format, Targs... Fargs)
    {
        return HAL_Cellular_Command((_CALLBACKPTR_MDM)cb, (void*)param, 10000, format, Fargs...);
    }

    template<typename T, typename... Targs>
    inline int command(int (*cb)(int type, const char* buf, int len, T* param),
            T* param, system_tick_t timeout_ms, const char* format, Targs... Fargs)
    {
        return HAL_Cellular_Command((_CALLBACKPTR_MDM)cb, (void*)param, timeout_ms, format, Fargs...);
    }

    IPAddress resolve(const char* name)
    {
        HAL_IPAddress ip;
        return (inet_gethostbyname(name, strlen(name), &ip, *this, NULL)<0) ?
                IPAddress(uint32_t(0)) : IPAddress(ip);
    }
};


extern CellularClass Cellular;

}   // namespace intorobot

#endif  // Wiring_Cellular
#endif	/* WIRING_CELLULAR_H_ */

