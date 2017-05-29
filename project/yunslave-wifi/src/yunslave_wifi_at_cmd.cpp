/**
******************************************************************************
Copyright (c) 2013-2014 IntoRobot Team.  All right reserved.

robin  2017-05-03

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

#include "yunslave_wifi_config.h"
#include "yunslave_wifi_at_cmd.h"
#include "yunslave_wifi_at_parse.h"
#include "yunslave_wifi_at.h"
#include "yunslave_wifi_log.h"

mode_type_t modeType = MODE_NORMAL;
network_status_t networkStatus = NETWORK_STATUS_ROUTER_DISCONNECT;
uint16_t at_dataSendLen = 0;
bool g_productInfoSended = false;
bool g_productRegisterRequest = false;
bool g_productActivateRequest = false;
bool g_productActivateTimes = 0;
char g_signature[64] = {0};

at_cmd_t at_cmd[at_cmdNum]={
    {NULL,         0,  NULL, NULL, NULL, at_exeCmdNull},
    {"E", 1, NULL, NULL, at_setupCmdE, NULL},
    {"+RST",       4,  NULL, NULL, NULL, at_exeCmdRst},
    {"+RESTORE",   8,  NULL, NULL, NULL, at_exeCmdRestore},
    {"+INFO",      5,  NULL, at_queryCmdInfo, NULL, NULL},
    {"+DEVICE",    7,  NULL, at_queryCmdDevice, at_setupCmdDevice, NULL},
    {"+SECURITY",  9,  NULL, NULL, at_setupCmdSecurity, NULL},
    {"+GETTIME",   8,  NULL, at_queryGetTime, NULL, NULL},
    {"+REGISTER",  9,  NULL, NULL, at_setupRegister, NULL},
    {"+MODE",      5,  NULL, at_queryCmdMode, at_setupCmdMode, NULL},
    {"+NETSTATUS", 10, NULL, at_queryCmdNetStatus, NULL, NULL},
    {"+SENDDATA",  9,  NULL, NULL, at_setupCmdSendData, NULL},
};

void at_exeCmdNull(uint8_t id)
{
    log_v("at_exeCmdNull\r\n");
    at_response_ok();
}

void at_setupCmdE(uint8_t id, char *pPara)
{
    if(*pPara == '0') {
        echoFlag = FALSE;
    } else if(*pPara == '1') {
        echoFlag = TRUE;
    } else {
        at_response_error();
        return;
    }
    at_response_ok();
}

void at_exeCmdRst(uint8_t id)
{
    log_v("at_exeCmdRst\r\n");
    at_response_ok();
    System.reset();
}

void at_exeCmdRestore(uint8_t id)
{
    log_v("at_exeCmdRestore\r\n");
    at_response_ok();
    System.factoryReset();
}

void at_queryCmdInfo(uint8_t id)
{
    log_v("at_queryCmdInfo\r\n");
    String infoString = "";

    infoString = "+INFO:\"";
    infoString += YUNSLAVE_WIFI_VERSION;
    infoString += "\",\"";
    infoString += System.platformName();
    infoString += "\",\"";
    infoString += System.deviceID();
    infoString += "\",";
    infoString += System.securityMode();
    infoString += "\r\n";
    at_response(infoString.c_str());
    at_response_ok();
}

void at_queryCmdDevice(uint8_t id)
{
    log_v("at_queryCmdDevice\r\n");
    String infoString = "";

    infoString = "+DEVICE:\"";
    infoString += yunSlaveParams.product_id;
    infoString += "\",\"";
    infoString += yunSlaveParams.hardware_version;
    infoString += "\",\"";
    infoString += yunSlaveParams.soft_version;
    infoString += "\"\r\n";
    at_response(infoString.c_str());
    at_response_ok();
}

void at_setupCmdDevice(uint8_t id, char *pPara)
{
    log_v("at_setupCmdDevice\r\n");

    char product_id[33] = {0};
    char hardware_version[33] = {0};
    char soft_version[33] = {0};

    if (*pPara++ != '=') {// skip '='
        at_response_error();
        return;
    }

    //获取第1个参数 product_id
    if(at_data_str_copy(product_id, &pPara, sizeof(product_id)-1) <= 0) {
        at_response_error();
        return;
    }

    if (*pPara++ != ',') { // skip ','
        at_response_error();
        return;
    }

    //获取第2个参数 hardware_version
    if(at_data_str_copy(hardware_version, &pPara, sizeof(hardware_version)-1) <= 0) {
        at_response_error();
        return;
    }

    if (*pPara++ != ',') { // skip ','
        at_response_error();
        return;
    }

    //获取第3个参数 soft_version
    if(at_data_str_copy(soft_version, &pPara, sizeof(soft_version)-1) <= 0) {
        at_response_error();
        return;
    }

    memset(yunSlaveParams.product_id, 0, sizeof(yunSlaveParams.product_id));
    memset(yunSlaveParams.hardware_version, 0, sizeof(yunSlaveParams.hardware_version));
    memset(yunSlaveParams.soft_version, 0, sizeof(yunSlaveParams.soft_version));

    memcpy(yunSlaveParams.product_id, product_id, sizeof(yunSlaveParams.product_id));
    memcpy(yunSlaveParams.hardware_version, hardware_version, sizeof(yunSlaveParams.hardware_version));
    memcpy(yunSlaveParams.soft_version, soft_version, sizeof(yunSlaveParams.soft_version));

    log_v("product_id = %s\r\n", product_id);
    log_v("hardware_version = %s\r\n", hardware_version);
    log_v("soft_version = %s\r\n", soft_version);
    saveYunSlaveParams();
    g_productInfoSended = false;
    at_response_ok();
}

void at_setupCmdSecurity(uint8_t id, char *pPara)
{
    log_v("at_setupCmdSecurity\r\n");
    at_response_ok();
}

void at_queryGetTime(uint8_t id)
{
    log_v("at_queryGetTime\r\n");
    String timeString = "";

    timeString = "+GETTIME:";
    if (WiFi.ready()) {
        if(IntoRobot.syncTime()) {
            timeString += 1;
            timeString += ",";
            timeString += Time.now();
        } else {
            timeString += 3;
        }
    } else {
        timeString += 2;
    }
    timeString += "\r\n";
    at_response(timeString.c_str());
    at_response_ok();
}

void at_setupRegister(uint8_t id, char *pPara)
{
    log_v("at_exeRegister\r\n");
    char product_id[32];

    if (*pPara++ != '=') {// skip '='
        at_response_error();
        return;
    }

    //获取第1个参数 product_id
    if(at_data_str_copy(product_id, &pPara, sizeof(product_id)-1) <= 0) {
        at_response_error();
        return;
    }

    if (*pPara++ != ',') { // skip ','
        at_response_error();
        return;
    }

    //获取第2个参数 signature
    if(at_data_str_copy(g_signature, &pPara, sizeof(g_signature)-1) <= 0) {
        at_response_error();
        return;
    }

    switch(System.securityMode()) {
        case AT_MODE_FLAG_ABP:           //Activation By Personalization  //已经灌好密钥
        case AT_MODE_FLAG_OTAA_INACTIVE: //Over-The-Air Activation //灌装激活码  未激活
        case AT_MODE_FLAG_OTAA_ACTIVE:   //灌装激活码 已激活
            g_productRegisterRequest = false;
            at_response("\r\n+REGISTER:3"); //无需注册
            break;
        default:                         //没有密钥信息
            if (WiFi.ready()) {
                g_productRegisterRequest = true;
                at_response("\r\n+REGISTER:1");  //注册中
            } else {
                g_productRegisterRequest = false; //未联网，注册失败
                at_response("\r\n+REGISTER:2");
            }
            break;
    }
    at_response_ok();
}

void at_queryCmdMode(uint8_t id)
{
    log_v("at_queryCmdMode\r\n");
    String modeString = "";

    modeString = "+MODE:";
    modeString += modeType;
    modeString += "\r\n";
    at_response(modeString.c_str());
    at_response_ok();
}

void at_setupCmdMode(uint8_t id, char *pPara)
{
    log_v("at_setupCmdMode\r\n");
    int mode, time, err;

    if (*pPara++ != '=') { // skip '='
        at_response_error();
        return;
    }

    //get the first parameter digit
    if(!at_get_next_int_dec(&pPara, &mode, &err)) {
        at_response_error();
        return;
    }

    if (*pPara++ != ',') { // skip ','
        at_response_error();
        return;
    }

    //get the first parameter digit
    if(!at_get_next_int_dec(&pPara, &time, &err)) {
        at_response_error();
        return;
    }

    switch(mode) {
        case 1: //正常模式
        case 2: //ImLink配置模式
        case 3: //Ap配置模式
        case 4: //绑定模式（保留）
        case 5: //厂测模式（保留）
            at_response_ok();
            break;
        default:
            at_response_error();
            break;
    }

    switch(mode) {
        case 1: //正常模式
            modeType = MODE_NORMAL;
            System.configEnterMode(SYSTEM_CONFIG_TYPE_NONE);
            break;
        case 2: //ImLink配置模式
            modeType = MODE_IMLINK_CONFIG;
            System.configEnterMode(SYSTEM_CONFIG_TYPE_IMLINK);
            break;
        case 3: //Ap配置模式
            modeType = MODE_AP_CONFIG;
            System.configEnterMode(SYSTEM_CONFIG_TYPE_AP);
            break;
        case 4: //绑定模式（保留）
            modeType = MODE_BINDING;
            break;
        case 5: //厂测模式（保留）
            modeType = MODE_TEST;
            break;
        default:
            break;
    }
}

void at_queryCmdNetStatus(uint8_t id)
{
    log_v("at_queryCmdNetStatus\r\n");
    String netStatusString = "";

    netStatusString = "+NETSTATUS:";
    netStatusString += networkStatus;
    if(NETWORK_STATUS_ROUTER_DISCONNECT == networkStatus) {
        netStatusString += "\r\n";
    } else {
        netStatusString += ",\"";
        netStatusString += WiFi.SSID();
        netStatusString += "\",\"";
        netStatusString += String(WiFi.localIP());
        netStatusString += "\",";
        netStatusString += WiFi.RSSI();
        netStatusString += "\r\n";
    }
    at_response(netStatusString.c_str());
    at_response_ok();
}

void at_setupCmdSendData(uint8_t id, char *pPara)
{
    log_v("at_setupCmdSendData\r\n");
    int len, err;

    if (*pPara++ != '=') { // skip '='
        at_response_error();
        return;
    }

    //get the first parameter digit
    if(!at_get_next_int_dec(&pPara, &len, &err)) {
        at_response_error();
        return;
    }

    if((len <= 0)||(len > at_dataLenMax)) {
        at_response_error();
        return;
    }

    at_dataSendLen = len;
    AtSendDataCmd = true;
    at_response_ok();
    at_response("> ");
}

void at_DataSending(uint8_t *pAtRcvData)
{
    log_v("at_DataSending\r\n");
    log_v_dump(pAtRcvData, at_dataSendLen);
    String CloudDataTxTopic = "";
    String RecvString = "";

    CloudDataTxTopic = "v2/device/";
    CloudDataTxTopic += System.deviceID();
    CloudDataTxTopic += "/rx";

    RecvString = "\r\nRecv ";
    RecvString += at_dataSendLen;
    RecvString += " bytes\r\n";
    at_response(RecvString.c_str());

    if(IntoRobot.connected()) {
        if(IntoRobot.publish(CloudDataTxTopic.c_str(), pAtRcvData, at_dataSendLen, 0, true, TOPIC_VERSION_CUSTOM)) {
            at_response("\r\nSEND OK\r\n");
        } else {
            at_response_error();
        }
    } else {
        at_response_error();
    }
}

