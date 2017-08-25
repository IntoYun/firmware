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

volatile bool INTOROBOT_LORAWAN_JOINED = false; //lorawan激活通过
volatile bool INTOROBOT_LORAWAN_CONNECTED = false; //lorawan发送版本信息完毕 已连接平台
volatile bool INTOROBOT_LORAWAN_JOIN_ENABLE = false; //入网使能 true使能
volatile bool INTOROBOT_LORAWAN_JOINING = false; //入网中
volatile bool INTOROBOT_LORAWAN_FIRST_ACTIVE = false;

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

void LoRaWanJoinEnable(bool enable)
{
    INTOROBOT_LORAWAN_JOIN_ENABLE = enable;
}

bool LoRaWanJoinIsEnabled(void)
{
    return INTOROBOT_LORAWAN_JOIN_ENABLE;
}

int8_t LoRaWanActiveStatus(void)
{
    if(INTOROBOT_LORAWAN_JOINED){
        return 1;
    }else{
        if(INTOROBOT_LORAWAN_JOINING){
            return 0;
        }
    }
    return -1;
}

void LoRaWanJoinOTAA(void)
{
    MlmeReq_t mlmeReq;
    uint8_t _devEui[8];
    uint8_t _appEui[8];

    os_getDevEui(_devEui);
    os_getAppEui(_appEui);
    os_getAppKey(LoRaWan.macParams.appKey);

    memcpyr(LoRaWan.macParams.devEui,_devEui,8);
    memcpyr(LoRaWan.macParams.appEui,_appEui,8);

    mlmeReq.Type = MLME_JOIN;
    mlmeReq.Req.Join.DevEui = LoRaWan.macParams.devEui;
    mlmeReq.Req.Join.AppEui = LoRaWan.macParams.appEui;
    mlmeReq.Req.Join.AppKey = LoRaWan.macParams.appKey;
    mlmeReq.Req.Join.NbTrials = LoRaWan._joinNbTrials;

    LoRaMacMlmeRequest( &mlmeReq );
}

void LoRaWanGetABPParams(uint32_t &devAddr, uint8_t *nwkSkey, uint8_t *appSkey)
{
    MibRequestConfirm_t mibReq;
    LoRaMacStatus_t status;

    mibReq.Type = MIB_DEV_ADDR;
    status = LoRaMacMibGetRequestConfirm( &mibReq );
    if(status == LORAMAC_STATUS_OK)
    {
        devAddr = mibReq.Param.DevAddr;
    }

    mibReq.Type = MIB_NWK_SKEY;
    status = LoRaMacMibGetRequestConfirm( &mibReq );
    if(status == LORAMAC_STATUS_OK)
    {
        memcpy1(nwkSkey,mibReq.Param.NwkSKey,16);
    }

    mibReq.Type = MIB_APP_SKEY;
    status = LoRaMacMibGetRequestConfirm( &mibReq );
    if(status == LORAMAC_STATUS_OK)
    {
        memcpy1(appSkey,mibReq.Param.AppSKey,16);
    }
}

//回复平台控制类型的数据点来通知服务器节点已收到
//发送空的确认包
void LoRaWanRespondServerConfirmedFrame(void)
{
    McpsReq_t mcpsReq;
    mcpsReq.Type = MCPS_UNCONFIRMED;
    mcpsReq.Req.Unconfirmed.fBuffer = NULL;
    mcpsReq.Req.Unconfirmed.fBufferSize = 0;
    mcpsReq.Req.Unconfirmed.Datarate = LORAWAN_DEFAULT_DATARATE;

    if( LoRaMacMcpsRequest( &mcpsReq ) == LORAMAC_STATUS_OK )
    {
        SLORAWAN_DEBUG("LoRaWan send empty frame status OK!!!");
        // return true;
    }
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
    SLORAWAN_DEBUG_D("\r\n");

    LoRaWan.sendUnconfirmedFrame(buffer,index);
}

void intorobot_lorawan_send_data(char* buffer, uint16_t len)
{
    if(intorobot_lorawan_flag_connected())
    {
        if(LoRaWan.getFrameType()){
            LoRaWan.sendConfirmedFrame(buffer,len);
        }else{
            LoRaWan.sendUnconfirmedFrame(buffer,len);
        }
    }
}

void LoRaWanOnEvent(lorawan_event_t event)
{
    //主从模式下都由内部存储参数
    switch(event)
    {
        case LORAWAN_EVENT_JOINED:
            {
                char devaddr[16] = "", nwkskey[36] = "", appskey[36] = "";
                uint32_t devAddr;
                uint8_t nwkSkey[16],appSkey[16];

                LoRaWanGetABPParams(devAddr,nwkSkey,appSkey);
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
                INTOROBOT_LORAWAN_JOINED = true;
                system_rgb_blink(RGB_COLOR_WHITE, 2000); //白灯闪烁
                //设置为C类
                MibRequestConfirm_t mibReq;
                mibReq.Type = MIB_DEVICE_CLASS;
                mibReq.Param.Class = CLASS_C;
                LoRaMacMibSetRequestConfirm( &mibReq );
                SLORAWAN_DEBUG("--LoRaWanOnEvent joined--");
            }
            break;

        case LORAWAN_EVENT_JOIN_FAIL:
            INTOROBOT_LORAWAN_JOINING = false;
            SLORAWAN_DEBUG("--LoRaWanOnEvent join failed--");
            break;

        case LORAWAN_EVENT_RX_COMPLETE:
            {
                if(System.featureEnabled(SYSTEM_FEATURE_DATAPOINT_ENABLED)) //数据点使能
                {
                    int len;
                    uint8_t buffer[256];
                    len = LoRaWan.receiveFrame(buffer);

                    #if 0
                    SLORAWAN_DEBUG_D("lorawan receive data:");
                    for(uint16_t i=0;i<len;i++)
                    {
                        SLORAWAN_DEBUG_D("0x%x ",buffer[i]);
                    }
                    SLORAWAN_DEBUG_D("\r\n");
                    #endif

                    if(len == -1){
                        return;
                    }else{
                        intorobotParseReceiveDatapoints(buffer,len);
                    }
                    SLORAWAN_DEBUG("--LoRaWanOnEvent RX Data--");
                }
            }
            break;

        case LORAWAN_EVENT_MCPSINDICATION_CONFIRMED:
            SLORAWAN_DEBUG("lorawan respond server ack");
            LoRaWanRespondServerConfirmedFrame();
            break;

        default:
            break;
    }
}

#endif
