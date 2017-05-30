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

#ifndef YUNSLAVE_WIFI_AT_CMD_H__
#define YUNSLAVE_WIFI_AT_CMD_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    char *at_cmdName;
    int8_t at_cmdLen;
    void (*at_testCmd)(uint8_t id);
    void (*at_queryCmd)(uint8_t id);
    void (*at_setupCmd)(uint8_t id, char *pPara);
    void (*at_exeCmd)(uint8_t id);
}at_cmd_t;

#define at_cmdNum   32

extern at_cmd_t at_cmd[at_cmdNum];
extern uint16_t at_dataSendLen;

void at_exeCmdNull(uint8_t id);
void at_setupCmdE(uint8_t id, char *pPara);
void at_exeCmdRst(uint8_t id);
void at_exeCmdRestore(uint8_t id);
void at_queryCmdInfo(uint8_t id);
void at_queryCmdDevice(uint8_t id);
void at_setupCmdDevice(uint8_t id, char *pPara);
void at_setupCmdSecurity(uint8_t id, char *pPara);
void at_queryGetTime(uint8_t id);
void at_setupRegister(uint8_t id, char *pPara);
void at_queryCmdMode(uint8_t id);
void at_setupCmdMode(uint8_t id, char *pPara);
void at_queryCmdNetStatus(uint8_t id);
void at_setupCmdSendData(uint8_t id, char *pPara);
void at_DataSending(uint8_t *pAtRcvData);

typedef enum
{
    MODE_NORMAL = 1,                             //正常工作模式
    MODE_IMLINK_CONFIG,                          //ImLink配置模式
    MODE_AP_CONFIG,                              //AP配置模式
    MODE_BINDING,                                //绑定模式
    MODE_TEST                                    //产测模式
}mode_type_t;

typedef enum
{
    NETWORK_STATUS_ROUTER_DISCONNECT = 1,       //未连接路由器
    NETWORK_STATUS_ROUTER_CONNECTED,            //连接路由器, 未连接平台
    NETWORK_STATUS_CLOUD_CONNECTED,             //连接路由器，连接平台
}network_status_t;

typedef enum
{
    EVENT_NET_DISCON_ROUTER = 1,                //YunSlave已断开连接路由器
    EVENT_NET_CON_ROUTER,                       //YunSlave已连接路由器
    EVENT_NET_DISCON_SERVER,                    //YunSlave已断开连服务器
    EVENT_NET_CON_SERVER,                       //YunSlave已连服务器
}event_net_type_t;

#define  PRODUCT_ACTIVATE_TIMES                 3
#define  PRODUCT_ACTIVATE_INTERVAL              60*1000  //单位: ms

extern mode_type_t modeType;
extern network_status_t networkStatus;
extern bool g_productInfoSended;
extern bool g_productRegisterRequest;
extern bool g_productActivateRequest;
extern bool g_productActivateTimes;
extern char g_signature[64];


#ifdef __cplusplus
}
#endif

#endif /* YUNSLAVE_WIFI_AT_CMD_H__*/

