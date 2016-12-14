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
#ifdef configSETUP_ENABLE

#include <stdio.h>
#include <stdlib.h>
#include "system_ex_setup.h"
#include "system_rgbled.h"
#include "system_cloud_def.h"
#include "system_network.h"
#include "system_network_internal.h"
#include "params_hal.h"
#include "core_hal.h"
#include "inet_hal.h"
#include "wlan_hal.h"
#include "wiring_wifi.h"
#include "string_convert.h"
#include "system_mode.h"

#ifndef configNO_NETWORK
using namespace intorobot;
#endif

static volatile uint32_t setup_timeout_start;
static volatile uint32_t setup_timeout_duration;

inline void ARM_SETUP_TIMEOUT(uint32_t dur) {
    setup_timeout_start = HAL_Timer_Get_Milli_Seconds();
    setup_timeout_duration = dur;
    DEBUG("SETUP WD Set %d",(dur));
}
inline bool IS_SETUP_TIMEOUT() {
    return setup_timeout_duration && ((HAL_Timer_Get_Milli_Seconds()-setup_timeout_start)>setup_timeout_duration);
}

inline void CLR_SETUP_TIMEOUT() {
    setup_timeout_duration = 0;
    DEBUG("SETUP WD Cleared, was %d", setup_timeout_duration);
}

DeviceSetupCmdType DeviceSetup::getMessageType(char *s) {
    if(!strcmp(s,"hello")) {
        return DEVICE_SETUP_HELLO;
    }
    else if(!strcmp(s,"checkWifi")) {
        return DEVICE_SETUP_CHECK_WIFI;
    }
    else if(!strcmp(s,"getNetworkStatus")) {
        return DEVICE_SETUP_GET_NETWORK_STATUS;
    }
    else if(!strcmp(s,"getWifiList")) {
        return DEVICE_SETUP_GET_WIFI_LIST;
    }
    else if(!strcmp(s,"getInfo")) {
        return DEVICE_SETUP_GET_INFO;
    }
    else if(!strcmp(s,"sendWifiInfo")) {
        return DEVICE_SETUP_SEND_WIFI_INFO;
    }
    else if(!strcmp(s,"setNetowrkCredentials")) {
        return DEVICE_SETUP_SET_NETWORK_CREDENTIALS;
    }
    else if(!strcmp(s,"sendDeviceInfo")) {
        return DEVICE_SETUP_SET_DEVICE_INFO;
    }
    else if(!strcmp(s,"setSecurity")) {
        return DEVICE_SETUP_SET_SECURITY;
    }
    else if(!strcmp(s,"setInfo")) {
        return DEVICE_SETUP_SET_INFO;
    }
    else if(!strcmp(s,"restartNetwork")) {
        return DEVICE_SETUP_RESTART_NETWORK;
    }
    else if(!strcmp(s,"reset")) {
        return DEVICE_SETUP_RESET;
    }
    else if(!strcmp(s,"reboot")) {
        return DEVICE_SETUP_REBOOT;
    }
    else if(!strcmp(s,"exit")) {
        return DEVICE_SETUP_EXIT;
    }
    else if(!strcmp(s,"test")) {
        return DEVICE_SETUP_TEST;
    }
    else {
        return DEVICE_SETUP_ERROR;
    }
}

bool DeviceSetup::process(void)
{
    aJsonObject *root=NULL;
    aJsonObject* value_Object;
    bool _isSetupSuccessful=false;

    while(available())
    {
        String tmp=readString();
        root = aJson.parse((char *)tmp.c_str());
        if (root == NULL)
        {break;}
        aJsonObject* command_Object = aJson.getObjectItem(root, "command");

        if (command_Object == NULL)
        {break;}

        DeviceSetupCmdType type = getMessageType(command_Object->valuestring);
        switch(type)
        {
            //查询类指令
            case DEVICE_SETUP_HELLO:                   //获取设备基础信息
                dealHello();
                break;
            case DEVICE_SETUP_CHECK_WIFI:              //获取wifi状态
                dealCheckWifi();
                break;
            case DEVICE_SETUP_GET_WIFI_LIST:           //获取wifi列表
                dealGetWifiList();
                break;
            case DEVICE_SETUP_GET_NETWORK_STATUS:      //查询网络状态
                dealGetNetworkStatus();
                break;
            case DEVICE_SETUP_GET_INFO:                //获取设备信息
                dealGetInfo();
                break;
            //设置类指令
            case DEVICE_SETUP_SEND_WIFI_INFO:          //设置wifi信息
            case DEVICE_SETUP_SET_NETWORK_CREDENTIALS: //设置网络接入凭证
                value_Object = aJson.getObjectItem(root, "value");
                if (value_Object == NULL)
                {break;}
                dealSetNetworkCredentials(value_Object);
                break;
            case DEVICE_SETUP_SET_DEVICE_INFO:         //设置设备信息
                value_Object = aJson.getObjectItem(root, "value");
                if (value_Object == NULL)
                {break;}
                dealSendDeviceInfo(value_Object);
                sendComfirm(200);
                _isSetupSuccessful=true;
                close();
                break;
            case DEVICE_SETUP_SET_SECURITY:            //设置设备安全信息
                value_Object = aJson.getObjectItem(root, "value");
                if (value_Object == NULL)
                {break;}
                dealSetSecurity(value_Object);
                break;
            case DEVICE_SETUP_SET_INFO:                //设置设备信息
                value_Object = aJson.getObjectItem(root, "value");
                if (value_Object == NULL)
                {break;}
                dealSetInfo(value_Object);
                break;
            //执行类指令
            case DEVICE_SETUP_RESTART_NETWORK:         //重启网络
                dealRestartNetwork();
                break;
            case DEVICE_SETUP_RESET:                   //设备恢复出厂设置
                dealReset();
                break;
            case DEVICE_SETUP_REBOOT:                  //设备重启
                dealReboot();
                break;
            case DEVICE_SETUP_EXIT:                    //退出配置模式
                sendComfirm(200);
                _isSetupSuccessful=true;
                close();
                break;
            //测试类指令
            case DEVICE_SETUP_TEST:                    //设备测试
                value_Object = aJson.getObjectItem(root, "value");
                if (value_Object == NULL)
                {break;}
                dealTest(value_Object);
                break;
            case DEVICE_SETUP_ERROR: //错误
                sendComfirm(201);
                break;
            default:
                break;
        }
        if(_isSetupSuccessful)
        {break;}
    }
    if(root!=NULL)
    {
        aJson.deleteItem(root);
    }

    if(_isSetupSuccessful)
    {
        return true;
    }
    else
    {return false;}
}

void DeviceSetup::sendComfirm(int status)
{
    aJsonObject* root = aJson.createObject();
    if (root == NULL)
    {return;}

    aJson.addNumberToObject(root, "status", status);
    char* string = aJson.print(root);
    write((unsigned char *)string, strlen(string));
    free(string);
    aJson.deleteItem(root);
}

void DeviceSetup::dealHello(void)
{
    aJsonObject* root = aJson.createObject();
    if (root == NULL)
    {return;}

    aJson.addNumberToObject(root, "status", 200);
    aJson.addNumberToObject(root, "version", 2);  //v2版本配置协议
    char device_id[32], board[8];
    if ( AT_MODE_FLAG_NONE == HAL_PARAMS_Get_System_at_mode() )
    {
        HAL_Board_Type(board, sizeof(board), 1);
        aJson.addStringToObject(root, "board", (char *)board);
        aJson.addNumberToObject(root, "at_mode", 0);
    }
    else
    {
        HAL_Board_Type(board, sizeof(board), 0);
        HAL_PARAMS_Get_System_device_id(device_id, sizeof(device_id));
        if(!memcmp(board, device_id, 6)) {
            aJson.addStringToObject(root, "board", (char *)board);
        }
        else {
            HAL_Board_Type(board, sizeof(board), 1);
            aJson.addStringToObject(root, "board", (char *)board);
        }
        aJson.addStringToObject(root, "device_id", (char *)device_id);
        aJson.addNumberToObject(root, "at_mode", HAL_PARAMS_Get_System_at_mode());
    }
    char* string = aJson.print(root);
    write((unsigned char *)string, strlen(string));
    free(string);
    aJson.deleteItem(root);
}

void DeviceSetup::dealCheckWifi(void)
{
    aJsonObject* root = aJson.createObject();
    if (root == NULL)
    {return;}

    if(WiFi.ready())  //wifi连通
    {
        aJson.addNumberToObject(root, "status", 200);
        aJson.addStringToObject(root, "ssid", WiFi.SSID());
        aJson.addNumberToObject(root, "rssi", WiFi.RSSI());
    }
    else
    {
        aJson.addNumberToObject(root, "status", 201);
    }

    char* string = aJson.print(root);
    write((unsigned char *)string, strlen(string));
    free(string);
    aJson.deleteItem(root);
}

void DeviceSetup::dealGetNetworkStatus(void)
{
    dealCheckWifi();
}

void DeviceSetup::dealGetWifiList(void)
{
    WiFiAccessPoint ap[20];

    int found = WiFi.scan(ap, 20);
    if(found < 1)
    {
        sendComfirm(201);
    }
    else
    {
        aJsonObject* root = aJson.createObject();
        if (root == NULL)
        {return;}
        aJson.addNumberToObject(root, "status",200);
        aJson.addNumberToObject(root, "listnum",found);
        aJsonObject* ssidlistarray = aJson.createArray();
        if (ssidlistarray == NULL)
        {
            aJson.deleteItem(root);
            return;
        }
        aJson.addItemToObject(root, "ssidlist", ssidlistarray);

        for(int n = 0; n < found; n++)
        {
            aJsonObject* ssid_object = aJson.createObject();
            if (ssid_object == NULL)
            {
                aJson.deleteItem(root);
                return;
            }
            aJson.addItemToArray(ssidlistarray, ssid_object);
            aJson.addStringToObject(ssid_object, "ssid", ap[n].ssid);
            aJson.addNumberToObject(ssid_object, "entype", ap[n].security);
            aJson.addNumberToObject(ssid_object, "signal", ap[n].rssi);
        }
        char* string = aJson.print(root);
        write((unsigned char *)string, strlen(string));
        free(string);
        aJson.deleteItem(root);
    }
}

void DeviceSetup::dealGetInfo(void)
{
    aJsonObject* root = aJson.createObject();
    if (root == NULL)
    {return;}

    aJson.addNumberToObject(root, "status", 200);

    aJsonObject* value_object = aJson.createObject();
    if (value_object == NULL)
    {return;}

    char device_id[32],board[8];
    HAL_Board_Type(board, sizeof(board), 0);
    aJson.addStringToObject(value_object, "board", board);
    HAL_PARAMS_Get_System_device_id(device_id, sizeof(device_id));
    aJson.addStringToObject(value_object, "device_id", device_id);
    aJson.addNumberToObject(value_object, "at_mode", HAL_PARAMS_Get_System_at_mode());

    aJson.addNumberToObject(value_object, "zone", HAL_PARAMS_Get_System_zone());
    aJson.addNumberToObject(value_object, "sv_select", HAL_PARAMS_Get_System_sv_select());

    char domain[50] = {0};
    HAL_PARAMS_Get_System_sv_domain(domain, sizeof(domain));
    if (strlen(domain))
    {
        aJson.addStringToObject(value_object, "sv_domain", domain);
        aJson.addNumberToObject(value_object, "sv_port", HAL_PARAMS_Get_System_sv_port());
    }
    else
    {
        aJson.addStringToObject(value_object, "sv_domain", INTOROBOT_SERVER_DOMAIN);
        aJson.addNumberToObject(value_object, "sv_port", INTOROBOT_SERVER_PORT);
    }

    memset(domain, 0, sizeof(domain));
    HAL_PARAMS_Get_System_http_domain(domain, sizeof(domain));
    if (strlen(domain))
    {
        aJson.addStringToObject(value_object, "http_domain", domain);
        aJson.addNumberToObject(value_object, "http_port", HAL_PARAMS_Get_System_http_port());
    }
    else
    {
        aJson.addStringToObject(value_object, "http_domain", INTOROBOT_HTTP_DOMAIN);
        aJson.addNumberToObject(value_object, "http_port", INTOROBOT_HTTP_PORT);
    }

    memset(domain, 0, sizeof(domain));
    HAL_PARAMS_Get_System_dw_domain(domain, sizeof(domain));
    if (strlen(domain))
    {
        aJson.addStringToObject(value_object, "dw_domain", domain);
    }
    else
    {
        aJson.addStringToObject(value_object, "dw_domain", INTOROBOT_UPDATE_DOMAIN);
    }

    uint8_t stamac[6] = {0}, apmac[6] = {0}, bssid[20] = {0};;
    char macStr[20] = {0};

    wlan_get_macaddr(stamac, apmac);
    memset(macStr, 0, sizeof(macStr));
    sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", stamac[0], stamac[1], stamac[2], stamac[3], stamac[4], stamac[5]);
    aJson.addStringToObject(value_object, "stamac", macStr);
    memset(macStr, 0, sizeof(macStr));
    sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", apmac[0], apmac[1], apmac[2], apmac[3], apmac[4], apmac[5]);
    aJson.addStringToObject(value_object, "apmac", macStr);
    aJson.addStringToObject(value_object, "ssid", WiFi.SSID());
    aJson.addStringToObject(value_object, "bssid", (char *)WiFi.BSSID(bssid));
    aJson.addNumberToObject(value_object, "rssi", WiFi.RSSI());

    aJson.addItemToObject(root, "value", value_object);

    char* string = aJson.print(root);
    write((unsigned char *)string, strlen(string));
    free(string);
    aJson.deleteItem(root);
}

void DeviceSetup::dealSendWifiInfo(aJsonObject* value_Object)
{
    aJsonObject* ssid_Object = aJson.getObjectItem(value_Object, "ssid");
    aJsonObject* passwd_Object = aJson.getObjectItem(value_Object, "passwd");

    if ((ssid_Object != NULL) && (passwd_Object != NULL))
    {
        char *pSsid,*pPasswd;
        pSsid = ssid_Object->valuestring;
        pPasswd = passwd_Object->valuestring;

        if(0==strcmp(pPasswd,""))  //密码为空
        {
            WiFi.setCredentials(pSsid);
        }
        else
        {
            int setWifiFlag = 1; // 0: success; 1 fail
            WiFi.setCredentials(pSsid, pPasswd);
            sendComfirm(200);
        }
    }
    sendComfirm(201);
}

void DeviceSetup::dealSetNetworkCredentials(aJsonObject* value_Object)
{
    dealSendWifiInfo(value_Object);
}

void DeviceSetup::dealSendDeviceInfo(aJsonObject* value_Object)
{
    float valuefloat;
    aJsonObject* Object, *Object1, *Object2;
    //zone
    Object = aJson.getObjectItem(value_Object, "zone");
    if (Object != (aJsonObject* )NULL) {
        valuefloat = (Object->type==aJson_Int?(float)(Object->valueint):Object->valuefloat);
        if(valuefloat < -12 || valuefloat > 13)
        {valuefloat = 8.0;}
        HAL_PARAMS_Set_System_zone(valuefloat);
    }

    //device_id  and access_token
    Object = aJson.getObjectItem(value_Object, "device_id");
    Object1 = aJson.getObjectItem(value_Object, "access_token");
    if (Object != (aJsonObject* )NULL && Object1 != (aJsonObject* )NULL) {
        HAL_PARAMS_Set_System_device_id(Object->valuestring);
        HAL_PARAMS_Set_System_access_token(Object1->valuestring);
        HAL_PARAMS_Set_System_at_mode(AT_MODE_FLAG_ABP);
    }

    //domain and port
    Object = aJson.getObjectItem(value_Object, "sv_domain");
    Object1 = aJson.getObjectItem(value_Object, "sv_port");
    Object2 = aJson.getObjectItem(value_Object, "dw_domain");
    if (Object != (aJsonObject* )NULL && Object1 != (aJsonObject* )NULL && Object2 != (aJsonObject* )NULL) {
        HAL_PARAMS_Set_System_sv_domain(Object->valuestring);
        HAL_PARAMS_Set_System_sv_port(Object1->valueint);
        HAL_PARAMS_Set_System_dw_domain(Object2->valuestring);
        HAL_PARAMS_Set_System_sv_select(SV_SELECT_FLAG_CUSTOM);
    }
    HAL_PARAMS_Save_Params();
}

void DeviceSetup::dealSetSecurity(aJsonObject* value_Object)
{
    int valueint;
    char len, *valuestring, at_mode;
    aJsonObject* Object, *Object1, *Object2;

    //device_id  and activation_code
    Object = aJson.getObjectItem(value_Object, "device_id");
    Object1 = aJson.getObjectItem(value_Object, "activation_code");
    if (Object != (aJsonObject* )NULL && Object1 != (aJsonObject* )NULL) {
        HAL_PARAMS_Set_System_device_id(Object->valuestring);
        HAL_PARAMS_Set_System_activation_code(Object1->valuestring);
        HAL_PARAMS_Set_System_at_mode(AT_MODE_FLAG_OTAA_INACTIVE);
        HAL_PARAMS_Save_Params();
    }
    else
    {
        //device_id  and access_token
        Object = aJson.getObjectItem(value_Object, "device_id");
        Object1 = aJson.getObjectItem(value_Object, "access_token");
        if (Object != (aJsonObject* )NULL && Object1 != (aJsonObject* )NULL) {
            HAL_PARAMS_Set_System_device_id(Object->valuestring);
            HAL_PARAMS_Set_System_access_token(Object1->valuestring);
            HAL_PARAMS_Set_System_at_mode(AT_MODE_FLAG_ABP);
            HAL_PARAMS_Save_Params();
        }
    }
}

void DeviceSetup::dealSetInfo(aJsonObject* value_object)
{
    aJsonObject* object = (aJsonObject* )NULL;
    aJsonObject* object1 = (aJsonObject* )NULL;
    aJsonObject* object2 = (aJsonObject* )NULL;

    char *valuestring = NULL;
    uint32_t len = 0;
    float valuefloat = 0.0;
    int valueint = 0;

    //zone
    object = aJson.getObjectItem(value_object, "zone");
    if (object != (aJsonObject* )NULL)
    {
        valuefloat = (object->type==aJson_Int?(float)(object->valueint):object->valuefloat);
        if(valuefloat < -12 || valuefloat > 13)
        {valuefloat = 8.0;}
        HAL_PARAMS_Set_System_zone(valuefloat);
    }

    //mqtt domain and port
    object = aJson.getObjectItem(value_object, "sv_domain");
    object1 = aJson.getObjectItem(value_object, "sv_port");
    if (object != (aJsonObject* )NULL && object1 != (aJsonObject* )NULL)
    {
        HAL_PARAMS_Set_System_sv_domain(object->valuestring);
        HAL_PARAMS_Set_System_sv_port(object1->valueint);
        HAL_PARAMS_Set_System_sv_select(SV_SELECT_FLAG_CUSTOM);
    }

    //http domain and port
    object = aJson.getObjectItem(value_object, "http_domain");
    object1 = aJson.getObjectItem(value_object, "http_port");
    if (object != (aJsonObject* )NULL && object1 != (aJsonObject* )NULL)
    {
        HAL_PARAMS_Set_System_http_domain(object->valuestring);
        HAL_PARAMS_Set_System_http_port(object1->valueint);
        HAL_PARAMS_Set_System_sv_select(SV_SELECT_FLAG_CUSTOM);
    }

    //dw_domain
    object = aJson.getObjectItem(value_object, "dw_domain");
    if (object != (aJsonObject* )NULL)
    {
        HAL_PARAMS_Set_System_dw_domain(object->valuestring);
        HAL_PARAMS_Set_System_sv_select(SV_SELECT_FLAG_CUSTOM);
    }

    uint8_t stamac[6] = {0}, apmac[6] = {0};
    object = aJson.getObjectItem(value_object, "stamac");
    object1 = aJson.getObjectItem(value_object, "apmac");
    if ((object != NULL)&&(object1 != NULL))
    {
        mac_str_to_bin(object->valuestring, stamac);
        mac_str_to_bin(object1->valuestring, apmac);
        int ret = wlan_set_macaddr(stamac, apmac);
        if(!ret)
        {
            sendComfirm(200);
        }
        else
        {
            sendComfirm(201);
        }
    }
    else
    {
        sendComfirm(202);
    }
    HAL_PARAMS_Save_Params();
}

void DeviceSetup::dealRestartNetwork(void)
{
    WiFi.off();
    delay(1000);
    WiFi.on();
    sendComfirm(200);
}

void DeviceSetup::dealReset(void)
{
    HAL_Core_Enter_Factory_Reset_Mode();
}

void DeviceSetup::dealReboot(void)
{
    HAL_Core_System_Reset();
}

void DeviceSetup::dealTest(aJsonObject* value_object)
{
#if 0
    aJsonObject* object = (aJsonObject* )NULL;
    aJsonObject* object1 = (aJsonObject* )NULL;

    char* itemstr = NULL;
    char* switchstr = NULL;
    uint32_t len = 0;

    object = aJson.getObjectItem(value_object, "item");
    object1 = aJson.getObjectItem(value_object, "switch");
    if (object != (aJsonObject* )NULL && object1 != (aJsonObject* )NULL)
    {
        // item
        itemstr = object->valuestring;
        len = strlen(itemstr);
        MO_DEBUG(("item:%s", itemstr));
        // switch
        switchstr = object1->valuestring;
        MO_DEBUG(("switch:%s", switchstr));
        if ( 0 == strcmp(itemstr, "gpio"))
        {
            bool serialEnableFlag = Serial.isEnabled(); // disable others later on, for example spi
            if ( 0 == strcmp(switchstr, "on"))
            {
                // turn on all the pin
                // if (serialEnableFlag == true)
                // {
                //     // disable Serial
                //     Serial.end();
                // }
                testNeutronPinOn();
                aJsonObject* root = aJson.createObject();
                if (root == NULL)
                {return;}

                aJson.addNumberToObject(root, "status", 200);
                char* string = aJson.print(root);
                write((unsigned char *)string, strlen(string));
                MO_DEBUG(("status: %s\r\n",string));
                vPortFree(string);
                // aJson.deleteItem(value_object1);  // if not commented it, it die
                aJson.deleteItem(root);
            }
            else if ( 0 == strcmp(switchstr, "off") )
            {
                testNeutronPinOff();
                // if (serialEnableFlag)
                // {
                //     Serial.begin();
                // }

                aJsonObject* root = aJson.createObject();
                if (root == NULL)
                {return;}

                aJson.addNumberToObject(root, "status", 200);
                char* string = aJson.print(root);
                write((unsigned char *)string, strlen(string));
                MO_DEBUG(("status: %s\r\n",string));
                vPortFree(string);
                // aJson.deleteItem(value_object1);  // if not commented it, it die
                aJson.deleteItem(root);
            }
            else
            {
                MO_ERROR(("GPIO ERROR"));
            }
        }
        else if ( 0 == strcmp(itemstr, "sensor"))
        {
            if ( 0 == strcmp(switchstr, "on"))
            {
                MO_DEBUG(("SENSOR ON"));

                Neutron.begin();
                int16_t accel[3] = {0};
                int16_t gyro[3] = {0};
                int16_t mag[3] = {0};
                double temperature, pressure, alt;
                uint16_t lightData;
                Neutron.getAllDataExceptMic(accel, gyro, mag, temperature, pressure, alt, lightData);

                aJsonObject* root = aJson.createObject();
                if (root == NULL)
                {return;}

                aJson.addNumberToObject(root, "status", 200);

                aJsonObject* value_object1 = aJson.createObject();
                if (value_object1 == NULL)
                {return;}
                aJson.addNumberToObject(value_object1, "acc_x", accel[0]);
                aJson.addNumberToObject(value_object1, "acc_y", accel[1]);
                aJson.addNumberToObject(value_object1, "acc_z", accel[2]);
                aJson.addNumberToObject(value_object1, "gyr_x", gyro[0]);
                aJson.addNumberToObject(value_object1, "gyr_y", gyro[1]);
                aJson.addNumberToObject(value_object1, "gyr_z", gyro[2]);
                aJson.addNumberToObject(value_object1, "mag_x", mag[0]);
                aJson.addNumberToObject(value_object1, "mag_y", mag[1]);
                aJson.addNumberToObject(value_object1, "mag_z", mag[2]);
                aJson.addNumberToObject(value_object1, "light", lightData);
                aJson.addNumberToObject(value_object1, "temp",  temperature);
                aJson.addNumberToObject(value_object1, "pres",  pressure);
                aJson.addNumberToObject(value_object1, "atd",   alt);

                aJson.addItemToObject(root, "value", value_object1);

                char* string = aJson.print(root);
                write((unsigned char *)string, strlen(string));
                MO_DEBUG(("send sensors data: %s\r\n",string));
                vPortFree(string);
                // aJson.deleteItem(value_object1);  // if not commented it, it die
                aJson.deleteItem(root);
            }
            else if ( 0 == strcmp(switchstr, "off") )
            {
                MO_DEBUG(("SENSOR OFF"));
                aJsonObject* root = aJson.createObject();
                if (root == NULL)
                {return;}

                aJson.addNumberToObject(root, "status", 200);
                char* string = aJson.print(root);
                write((unsigned char *)string, strlen(string));
                MO_DEBUG(("status: %s\r\n",string));
                vPortFree(string);
                // aJson.deleteItem(value_object1);  // if not commented it, it die
                aJson.deleteItem(root);
            }
            else
            {
                MO_ERROR(("SENSOR ERROR"));
            }
        }
        else if ( 0 == strcmp(itemstr, "mic"))
        {
            if ( 0 == strcmp(switchstr, "on"))
            {
                // Neutron.begin();
                micSensor.begin();
                // MicData
                MO_DEBUG(("Begin to transfer mic data"));

                // for 5 seconds data
                // for (int i = 0; i< 4000; i++)
                //while(1)
                uint32_t timeId = timerGetId();
                SerialUSB.begin();
                while (timerIsEnd(timeId, 2000) != true ) // 2 seconds
                {
                    micSensor.getMICDataToUSBSerial(SerialUSB);
                }
                MO_DEBUG(("mic times  seconds end"));
            }
            else if ( 0 == strcmp(switchstr, "off") )
            {
                MO_DEBUG(("MIC OFF"));
                aJsonObject* root = aJson.createObject();
                if (root == NULL)
                {return;}

                aJson.addNumberToObject(root, "status", 200);
                char* string = aJson.print(root);
                write((unsigned char *)string, strlen(string));
                MO_DEBUG(("status: %s\r\n",string));
                vPortFree(string);
                // aJson.deleteItem(value_object1);  // if not commented it, it die
                aJson.deleteItem(root);

            }
            else
            {
                MO_ERROR(("MIC ERROR"));
            }
        }
        else
        {
            MO_ERROR(("ITEM ERROR"));
        }
    }

    // below comment them, or it will die
    // vPortFree(itemstr);
    // vPortFree(switchstr);
    // aJson.deleteItem(object);
    // aJson.deleteItem(object1);
#endif
}

void UsartDeviceSetup::init(void)
{
    serial.begin(115200);    //
    while (!serial); // wait for a serial connection
}

int UsartDeviceSetup::available(void)
{
    return serial.available();
}

int UsartDeviceSetup::read(void)
{
    return serial.read();
}

String UsartDeviceSetup::readString(void)
{
    return serial.readString();
}

size_t UsartDeviceSetup::write(const uint8_t *buffer, size_t size)
{
    for(size_t i = 0;i<size;i++)
    {
        serial.write(buffer[i]);
    }
    return size;
}

void UsartDeviceSetup::sendComfirm(int status)
{
    aJsonObject* root = aJson.createObject();
    if (root == NULL)
    {return;}

    aJson.addNumberToObject(root, "status", status);
    char* string = aJson.print(root);
    write((unsigned char *)string, strlen(string));
    free(string);
    aJson.deleteItem(root);
}

void UsartDeviceSetup::close(void)
{
    //none do
}

void UdpDeviceSetup::init(void)
{
    wlan_Imlink_start();
}

void UdpDeviceSetup::sendComfirm(int status)
{
    aJsonObject* root = aJson.createObject();
    if (root == NULL)
    {return;}

    aJson.addNumberToObject(root, "status", status);
    char* string = aJson.print(root);
    for(int i=0; i < 10; i++) //may be not enough
    {
        write((unsigned char *)string, strlen(string));
        delay(100);
    }
    free(string);
    aJson.deleteItem(root);
}

int UdpDeviceSetup::available(void)
{
    static volatile uint8_t step = 0;
    switch(step)
    {
        case 0:
            if( IMLINK_SUCCESS == wlan_Imlink_get_status() ) {
                wlan_Imlink_stop();
                network.connect();
                ARM_SETUP_TIMEOUT(5000);
                step = 1;
            }
            break;
        case 1:
            if(IS_SETUP_TIMEOUT()) {
                wlan_Imlink_start();
                CLR_SETUP_TIMEOUT();
                step = 0;
            }
            if( network.status() ) {
                system_rgb_blink(RGB_COLOR_RED, 200);
                Udp.begin(5556);
                step = 2;
            }
            break;
        case 2:
            return Udp.parsePacket();
            break;
        default:
            break;
    }
    return 0;
}

int UdpDeviceSetup::read(void)
{
    return Udp.read();
}

String UdpDeviceSetup::readString(void)
{
    return Udp.readString();
}

size_t UdpDeviceSetup::write(const uint8_t *buf, size_t size)
{
    Udp.beginPacket(IPADDR_BROADCAST,5557);
    Udp.write(buf,size);
    return Udp.endPacket();
}

void UdpDeviceSetup::close(void)
{
    Udp.stop();
}

UsartDeviceSetup DeviceSetupUsart;
UdpDeviceSetup DeviceSetupUdp;

#endif
