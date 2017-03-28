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

#include "intorobot_config.h"

#include <stdlib.h>
#include <stdio.h>

#include "hw_config.h"
#include "core_hal.h"
#include "params_hal.h"
#include "ota_flash_hal.h"
#include "wiring_string.h"
#include "system_clouddata.h"
#include "system_mode.h"
#include "system_task.h"
#include "system_threading.h"
#include "system_update.h"
#include "system_rgbled.h"
#include "wiring_time.h"
#include "wiring_ticks.h"
#include "wiring_random.h"
#include "system_product.h"
#include "ajson.h"

/*debug switch*/
#define SYSTEM_LORAWAN_DEBUG

#ifdef SYSTEM_LORAWAN_DEBUG
#define SLORAWAN_DEBUG(...)  do {DEBUG(__VA_ARGS__);}while(0)
#define SLORAWAN_DEBUG_D(...)  do {DEBUG_D(__VA_ARGS__);}while(0)
#else
#define SLORAWAN_DEBUG(...)
#define SLORAWAN_DEBUG_D(...)
#endif

using namespace intorobot;

#ifndef configNO_LORAWAN

bool intorobot_sync_time(void)
{
    SLORAWAN_DEBUG("---------device syncTime begin---------");
    return false;
}

bool intorobot_device_activate(void)
{
    SLORAWAN_DEBUG("---------device otta activate begin---------");
    return false;
}

void cloud_datapoint_receive_callback(uint8_t *payload, uint32_t len)
{
    SLORAWAN_DEBUG("Ok! receive datapoint form cloud!");
    //intorobotReceiveDataProcessJson(payload, len);
}

/*
void onEvent (ev_t ev) {
    SLORAWAN_DEBUG("onEvent");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            SLORAWAN_DEBUG("scan timeout");
            break;
        case EV_BEACON_FOUND:
            SLORAWAN_DEBUG("beacon found");
            break;
        case EV_BEACON_MISSED:
            SLORAWAN_DEBUG("beacon missed");
            break;
        case EV_BEACON_TRACKED:
            SLORAWAN_DEBUG("beacon tracked");
            break;
        case EV_JOINING:
            SLORAWAN_DEBUG("joining");
            break;
        case EV_JOINED:
            SLORAWAN_DEBUG("joined");
            break;
        case EV_RFU1:
            SLORAWAN_DEBUG("rfu1");
            break;
        case EV_JOIN_FAILED:
            SLORAWAN_DEBUG("join failed");
            break;
        case EV_REJOIN_FAILED:
            SLORAWAN_DEBUG("rejoin failed");
            break;
        case EV_TXCOMPLETE:
            SLORAWAN_DEBUG("tx complete(includes waiting for RX windows)");
            if (LMIC.txrxFlags & TXRX_ACK)
                SLORAWAN_DEBUG("Received ack");
            if (LMIC.dataLen) {
                SLORAWAN_DEBUG("Received %d  bytes of payload", LMIC.dataLen);
            }
            break;
        case EV_LOST_TSYNC:
            SLORAWAN_DEBUG("lost tsync");
            break;
        case EV_RESET:
            SLORAWAN_DEBUG("reset");
            break;
        case EV_RXCOMPLETE:
            SLORAWAN_DEBUG("rx complete");
            // data received in ping slot
            break;
        case EV_LINK_DEAD:
            SLORAWAN_DEBUG("link dead");
            break;
        case EV_LINK_ALIVE:
            SLORAWAN_DEBUG("link alive");
            break;
        default:
            SLORAWAN_DEBUG("unknow event");
            break;
    }
}
*/
#endif

