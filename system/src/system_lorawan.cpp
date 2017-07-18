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
#include "system_product.h"
#include "system_utilities.h"
#include "string_convert.h"
#include "wiring_system.h"

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
volatile uint8_t INTOROBOT_CLOUD_SOCKETED = 0;           //网络连接状态 1连接 0断开
volatile uint8_t INTOROBOT_CLOUD_CONNECT_PREPARED = 0;   //平台链接预处理状态 1已经处理
volatile uint8_t INTOROBOT_CLOUD_CONNECTED = 0;          //平台连接状态 1连接上了


volatile uint8_t INTOROBOT_LORAWAN_JOINED = 0; //lorawan认证通过
volatile uint8_t INTOROBOT_LORAWAN_CONNECTED = 0; //lorawan发送版本信息完毕

void os_getDevEui(uint8_t *buf)
{
    char deveui[24]={0};
    HAL_PARAMS_Get_System_device_id(deveui, sizeof(deveui));
    string2hex(deveui, buf, 8, true);
}

void os_getAppEui(uint8_t *buf)
{
    char appeui[24]={0};
    HAL_PARAMS_Get_System_appeui(appeui, sizeof(appeui));
    string2hex(appeui, buf, 8, true);
}

void os_getAppKey(uint8_t *buf)
{
    char appkey[36]={0};
    HAL_PARAMS_Get_System_appkey(appkey, sizeof(appkey));
    string2hex(appkey, buf, 16, false);
}

bool intorobot_lorawan_flag_connected(void)
{
    if (INTOROBOT_LORAWAN_JOINED && INTOROBOT_LORAWAN_CONNECTED) {
        return true;
    } else {
        return false;
    }
}

void intorobot_lorawan_send_terminal_info(void)
{
    SLORAWAN_DEBUG("---------lorawan send termianal info--------");
    int32_t index = 0, len = 0;
    uint8_t buffer[256] = {0};
    char temp[32] = {0};

    buffer[index++] = BINARY_DATA_FORMAT;
    // product_id
    buffer[index++] = 0xFF;
    buffer[index++] = 0x01;
    buffer[index++] = 0x03;
    system_get_product_id(temp, sizeof(temp));
    len = strlen(temp);
    buffer[index++] = len;
    memcpy(&buffer[index], temp, len);
    index+=len;

    // productver
    buffer[index++] = 0xFF;
    buffer[index++] = 0x02;
    buffer[index++] = 0x03;
    system_get_product_software_version(temp, sizeof(temp));
    len = strlen(temp);
    buffer[index++] = len;
    memcpy(&buffer[index], temp, len);
    index+=len;

    // board
    buffer[index++] = 0xFF;
    buffer[index++] = 0x03;
    buffer[index++] = 0x03;
    system_get_board_id(temp, sizeof(temp));
    len = strlen(temp);
    buffer[index++] = len;
    memcpy(&buffer[index], temp, len);
    index+=len;

    for(int i=0; i<index; i++)
    {
        SLORAWAN_DEBUG_D("%02x ", buffer[i]);
    }

    LoRaWan.sendUnconfirmedFrame(buffer,index);
}

void intorobot_lorawan_send_data(char* buffer, uint16_t len)
{
    if(intorobot_lorawan_flag_connected())
    {
        LoRaWan.sendUnconfirmedFrame(buffer,len);
    }
}

void LoRaWanOnEvent(lorawan_event_t event)
{
    if(System.featureEnabled(SYSTEM_FEATURE_LORAMAC_MASTER_ENABLED))
    {
        switch(event)
        {
            case LORAWAN_EVENT_JOINING:
                {
                    SLORAWAN_DEBUG("--event joining--");
                }
                break;

            case LORAWAN_EVENT_JOINED:
                {
                    SLORAWAN_DEBUG("--event joined--");
                    char devaddr[16] = "", nwkskey[36] = "", appskey[36] = "";
                    uint32_t devAddr;
                    uint8_t nwkSkey[16],appSkey[16];
                    devAddr = LoRaWan.getMacDeviceAddr();
                    LoRaWan.getMacNwkSessionKey(nwkSkey);
                    LoRaWan.getMacAppSessionKey(appSkey);

                    //devaddr
                    hex2string((uint8_t *)&devAddr, 4, devaddr, true);
                    HAL_PARAMS_Set_System_devaddr(devaddr);
                    //nwkskey
                    hex2string(nwkSkey, 16, nwkskey, false);
                    HAL_PARAMS_Set_System_nwkskey(nwkskey);
                    //appskey
                    hex2string(appSkey, 16, appskey, false);
                    HAL_PARAMS_Set_System_appskey(appskey);
                    HAL_PARAMS_Set_System_at_mode(AT_MODE_FLAG_OTAA_ACTIVE);

                    HAL_PARAMS_Save_Params();

                    SLORAWAN_DEBUG("devaddr: %s", devaddr);
                    SLORAWAN_DEBUG("nwkskey: %s", nwkskey);
                    SLORAWAN_DEBUG("appskey: %s", appskey);
                    SLORAWAN_DEBUG("---------");
                    INTOROBOT_LORAWAN_JOINED = 1;
                    system_rgb_blink(RGB_COLOR_WHITE, 2000); //白灯闪烁
                }
            break;

            case LORAWAN_EVENT_JOIN_FAIL:
                SLORAWAN_DEBUG("--event join failed--");
                break;

            case LORAWAN_EVENT_TX_COMPLETE:
                // SLORAWAN_DEBUG("--event TX completed--");
                break;

            case LORAWAN_EVENT_RX_COMPLETE:
                {
                    SLORAWAN_DEBUG("--event RX completed--");
                    int len;
                    uint8_t buffer[256];
                    len = LoRaWan.receiveFrame(buffer);
                    if(len == -1)
                    {
                        return;
                    }
                    else
                    {
                        intorobotParseReceiveDatapoints(buffer,len);
                    }
                }
                break;

            case LORAWAN_EVENT_MLME_JOIN:
                break;

            case LORAWAN_EVENT_MLME_LINK_CHECK:
                SLORAWAN_DEBUG("--event MLME link check--");
                break;

            case LORAWAN_EVENT_MCPS_UNCONFIRMED:
                // SLORAWAN_DEBUG("--event MCPS unconfirmed--");
                break;

            case LORAWAN_EVENT_MCPS_CONFIRMED:
                SLORAWAN_DEBUG("--event MCPS confirmed--");
                break;

            case LORAWAN_EVENT_MCPS_PROPRIETARY:
                SLORAWAN_DEBUG("--event MCPS proprietary--");
                break;

            case LORAWAN_EVENT_MCPS_MULTICAST:
                SLORAWAN_DEBUG("--event MCPS multicast--");
                break;

            default:
                break;
        }
    }
}

#endif
