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

#include "cellular_hal.h"
#include "modem/inc/mdm_hal.h"

static CellularCredentials cellularCredentials;

static HAL_NET_Callbacks netCallbacks = { 0 };

//=======net notify===========
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

// Todo rename me, and allow the different connect, disconnect etc. timeouts be set by the HAL
uint32_t HAL_NET_SetNetWatchDog(uint32_t timeOutInuS)
{
    return 0;
}

int cellular_on(void* reserved)
{
    if(CellularMDM.powerOn()) {
        return 0;
    }
    return -1;
}

int cellular_off(void* reserved)
{
    if(CellularMDM.powerOff()) {
        return 0;
    }
    return -1;
}

void cellular_drive_now(void)
{
    CellularMDM.drive();
}

int cellular_connect(void* reserved)
{
    const CellularCredentials& cred = cellularCredentials;
    const char* apn = cred.apn;

    if(CellularMDM.connect(cred.apn, cred.username, cred.password)) {
        return 0;
    }
    return -1;
}

int cellular_disconnect(void* reserved)
{
    if(CellularMDM.disconnect()) {
        return 0;
    }
    return -1;
}

int cellular_device_info(CellularDevice* device, void* reserved)
{
    const DevStatus* status = CellularMDM.getDevStatus();
    if (!*status->ccid) {
        CellularMDM.init(); // attempt to fetch the info again in case the SIM card has been inserted.
    }
    // this would benefit from an unsolicited event to call CellularMDM.init() automatically on sim card insert)
    strncpy(device->imei, status->imei, sizeof(device->imei));
    strncpy(device->iccid, status->ccid, sizeof(device->iccid));
    return 0;
}

int cellular_fetch_ipconfig(CellularConfig* config, void* reserved)
{
    uint32_t ip_addr = CellularMDM.getIpAddress(); // Local IP
    if (ip_addr > 0) {
        memcpy(&config->nw.aucIP, &ip_addr, 4);
        return 0;
    }
    return -1;
}

int cellular_credentials_set(const char* apn, const char* username, const char* password, void* reserved)
{
    cellularCredentials.apn = apn;
    cellularCredentials.username = username;
    cellularCredentials.password = password;
    return 0;
}

// todo - better to have the caller pass CellularCredentials and copy the details out according to the size of the struct given.
CellularCredentials* cellular_credentials_get(void* reserved)
{
    return &cellularCredentials;
}

bool cellular_sim_ready(void* reserved)
{
    const DevStatus* status = CellularMDM.getDevStatus();
    return status->sim == SIM_READY;
}

void cellular_cancel(bool cancel, bool calledFromISR, void*)
{
    if (cancel) {
        CellularMDM.cancel();
    } else {
        CellularMDM.resume();
    }
}

int cellular_signal(CellularSignalHal &signal, void* reserved)
{
    NetStatus status;

    if(CellularMDM.getSignalStrength(status)) {
        signal.rssi = status.rssi;
        signal.qual = status.qual;
        return 0;
    }
    return -1;
}

int cellular_command(_CALLBACKPTR_MDM cb, void* param, system_tick_t timeout_ms, const char* format, ...)
{
    char buf[256];
    va_list args;

    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);

    CellularMDM.sendFormated(buf);
    return CellularMDM.waitFinalResp((MDMParser::_CALLBACKPTR)cb, (void*)param, timeout_ms);
}

int cellular_sms_received_handler_set(_CELLULAR_SMS_CB_MDM cb, void* data, void* reserved)
{
    if (cb) {
        CellularMDM.setSMSreceivedHandler((MDMParser::_CELLULAR_SMS_CB)cb, (void*)data);
        return 0;
    }
    return -1;
}

int cellular_pause(void* reserved)
{
    CellularMDM.pause();
    return 0;
}

int cellular_resume(void* reserved)
{
    CellularMDM.resume();
    return 0;
}

