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
#include "system_datapoint.h"
#include "system_mode.h"
#include "system_task.h"
#include "system_rgbled.h"
#include "system_lorawan.h"
#include "system_lorawan.h"
#include "string_convert.h"

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

volatile uint8_t g_intorobot_lorawan_connected = 0;      //lorawan认证通过

size_t intorobot_debug_info_write(uint8_t byte)
{
    return 1;
}

int intorobot_debug_info_read(void)
{
    return -1;
}

int intorobot_debug_info_available(void)
{
    return -1;
}

void os_getDevEui(u1_t* buf)
{
    char deveui[24]={0};
    HAL_PARAMS_Get_System_device_id(deveui, sizeof(deveui));
    string2hex(deveui, buf, 8, true);
    //SLORAWAN_DEBUG("otta get devEui: %s", deveui);
}

void os_getArtEui(u1_t* buf)
{
    char appeui[24]={0};
    HAL_PARAMS_Get_System_appeui(appeui, sizeof(appeui));
    string2hex(appeui, buf, 8, true);
    //SLORAWAN_DEBUG("otta get appEui: %s", appeui);
}
void os_getDevKey(u1_t* buf)
{
    char appkey[36]={0};
    HAL_PARAMS_Get_System_appkey(appkey, sizeof(appkey));
    string2hex(appkey, buf, 16, false);
    //SLORAWAN_DEBUG("otta get appKey: %s", appkey);
}

static void lorawan_datapoint_receive_callback(uint8_t *payload, uint32_t len)
{
    SLORAWAN_DEBUG("Ok! receive datapoint form cloud!");
    //intorobotReceiveDataProcessBinary(payload, len);
}

void lorawanSendData(char* buffer, uint16_t len)
{
    if(1 == g_intorobot_lorawan_connected) {
        SLORAWAN_DEBUG("lorawan_send_datapoint len = %d", len);
        if (LMIC.opmode & OP_TXRXPEND) {
            SLORAWAN_DEBUG("op_txrxpend, not sending");
        } else {
            for(int i=0; i<len; i++)
            {
                SLORAWAN_DEBUG_D("%02x ", buffer[i]);
            }
            SLORAWAN_DEBUG_D("\r\n");
            LMIC_setTxData2(1, buffer, len, 0);
            SLORAWAN_DEBUG("packet queued");
        }
    }
}

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
            {
                SLORAWAN_DEBUG("--joined--");
                char devaddr[16] = "", nwkskey[36] = "", appskey[36] = "";

                hex2string((uint8_t *)&LMIC.devaddr, 4, devaddr, true);
                HAL_PARAMS_Set_System_devaddr(devaddr);

                hex2string(LMIC.nwkKey, 16, nwkskey, false);
                HAL_PARAMS_Set_System_nwkskey(nwkskey);

                hex2string(LMIC.artKey, 16, appskey, false);
                HAL_PARAMS_Set_System_appskey(appskey);

                HAL_PARAMS_Set_System_at_mode(AT_MODE_FLAG_OTAA_ACTIVE);
                HAL_PARAMS_Save_Params();

                SLORAWAN_DEBUG("devaddr: %s", devaddr);
                SLORAWAN_DEBUG("nwkskey: %s", nwkskey);
                SLORAWAN_DEBUG("appskey: %s", appskey);
                SLORAWAN_DEBUG("---------");
                g_intorobot_lorawan_connected = 1;
                system_rgb_blink(RGB_COLOR_WHITE, 2000); //白灯闪烁
            }
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

#endif

