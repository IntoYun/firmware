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
#include "system_config.h"
#include "system_rgbled.h"
#include "system_cloud_def.h"
#include "system_network.h"
#include "params_hal.h"
#include "core_hal.h"
#include "inet_hal.h"
#include "wlan_hal.h"
#include "wiring_wifi.h"
#include "string_convert.h"
#include "system_mode.h"
#include "system_task.h"
#include "system_test.h"

#ifndef configNO_NETWORK
using namespace intorobot;
#endif

typedef size_t (*writeHandler)(const uint8_t *buf, size_t size);

static volatile uint32_t config_timeout_start;
static volatile uint32_t config_timeout_duration;

inline void ARM_CONFIG_TIMEOUT(uint32_t dur) {
    config_timeout_start = HAL_Timer_Get_Milli_Seconds();
    config_timeout_duration = dur;
    DEBUG("CONFIG WD Set %d",(dur));
}
inline bool IS_CONFIG_TIMEOUT() {
    return config_timeout_duration && ((HAL_Timer_Get_Milli_Seconds()-config_timeout_start)>config_timeout_duration);
}

inline void CLR_CONFIG_TIMEOUT() {
    config_timeout_duration = 0;
    DEBUG("CONFIG WD Cleared, was %d", config_timeout_duration);
}

DeviceConfigCmdType DeviceConfig::getMessageType(char *s) {
    if(!strcmp(s,"hello")) {
        return DEVICE_CONFIG_HELLO;
    }
    else if(!strcmp(s,"checkWifi")) {
        return DEVICE_CONFIG_CHECK_WIFI;
    }
    else if(!strcmp(s,"getNetworkStatus")) {
        return DEVICE_CONFIG_GET_NETWORK_STATUS;
    }
    else if(!strcmp(s,"getWifiList")) {
        return DEVICE_CONFIG_GET_WIFI_LIST;
    }
    else if(!strcmp(s,"getInfo")) {
        return DEVICE_CONFIG_GET_INFO;
    }
    else if(!strcmp(s,"sendWifiInfo")) {
        return DEVICE_CONFIG_SEND_WIFI_INFO;
    }
    else if(!strcmp(s,"setNetowrkCredentials")) {
        return DEVICE_CONFIG_SET_NETWORK_CREDENTIALS;
    }
    else if(!strcmp(s,"sendDeviceInfo")) {
        return DEVICE_CONFIG_SET_DEVICE_INFO;
    }
    else if(!strcmp(s,"setSecurity")) {
        return DEVICE_CONFIG_SET_SECURITY;
    }
    else if(!strcmp(s,"setInfo")) {
        return DEVICE_CONFIG_SET_INFO;
    }
    else if(!strcmp(s,"restartNetwork")) {
        return DEVICE_CONFIG_RESTART_NETWORK;
    }
    else if(!strcmp(s,"reset")) {
        return DEVICE_CONFIG_RESET;
    }
    else if(!strcmp(s,"reboot")) {
        return DEVICE_CONFIG_REBOOT;
    }
    else if(!strcmp(s,"exit")) {
        return DEVICE_CONFIG_EXIT;
    }
    else if(!strcmp(s,"test")) {
        return DEVICE_CONFIG_TEST;
    }
    else {
        return DEVICE_CONFIG_ERROR;
    }
}

int DeviceConfig::process(void)
{
    aJsonObject *root=NULL;
    aJsonObject* value_Object;
    bool _isConfigSuccessful=false;

    while(available())
    {
        String tmp=readString();
        root = aJson.parse((char *)tmp.c_str());
        if (root == NULL)
        {break;}
        aJsonObject* command_Object = aJson.getObjectItem(root, "command");

        if (command_Object == NULL)
        {break;}

        DeviceConfigCmdType type = getMessageType(command_Object->valuestring);
        switch(type)
        {
            //查询类指令
            case DEVICE_CONFIG_HELLO:                   //获取设备基础信息
                dealHello();
                break;
            case DEVICE_CONFIG_CHECK_WIFI:              //获取wifi状态
                dealCheckWifi();
                break;
            case DEVICE_CONFIG_GET_WIFI_LIST:           //获取wifi列表
                dealGetWifiList();
                break;
            case DEVICE_CONFIG_GET_NETWORK_STATUS:      //查询网络状态
                dealGetNetworkStatus();
                break;
            case DEVICE_CONFIG_GET_INFO:                //获取设备信息
                dealGetInfo();
                break;
            //设置类指令
            case DEVICE_CONFIG_SEND_WIFI_INFO:          //设置wifi信息
            case DEVICE_CONFIG_SET_NETWORK_CREDENTIALS: //设置网络接入凭证
                value_Object = aJson.getObjectItem(root, "value");
                if (value_Object == NULL)
                {break;}
                dealSetNetworkCredentials(value_Object);
                break;
            case DEVICE_CONFIG_SET_DEVICE_INFO:         //设置设备信息
                value_Object = aJson.getObjectItem(root, "value");
                if (value_Object == NULL)
                {break;}
                dealSendDeviceInfo(value_Object);
                sendComfirm(200);
                _isConfigSuccessful=true;
                close();
                break;
            case DEVICE_CONFIG_SET_SECURITY:            //设置设备安全信息
                value_Object = aJson.getObjectItem(root, "value");
                if (value_Object == NULL)
                {break;}
                dealSetSecurity(value_Object);
                break;
            case DEVICE_CONFIG_SET_INFO:                //设置设备信息
                value_Object = aJson.getObjectItem(root, "value");
                if (value_Object == NULL)
                {break;}
                dealSetInfo(value_Object);
                break;
            //执行类指令
            case DEVICE_CONFIG_RESTART_NETWORK:         //重启网络
                dealRestartNetwork();
                break;
            case DEVICE_CONFIG_RESET:                   //设备恢复出厂设置
                dealReset();
                break;
            case DEVICE_CONFIG_REBOOT:                  //设备重启
                dealReboot();
                break;
            case DEVICE_CONFIG_EXIT:                    //退出配置模式
                sendComfirm(200);
                _isConfigSuccessful=true;
                close();
                break;
            //测试类指令
            case DEVICE_CONFIG_TEST:                    //设备测试
                value_Object = aJson.getObjectItem(root, "value");
                if (value_Object == NULL)
                {break;}
                dealTest(value_Object);
                break;
            case DEVICE_CONFIG_ERROR: //错误
                sendComfirm(201);
                break;
            default:
                break;
        }
        if(_isConfigSuccessful)
        {break;}
    }
    if(root!=NULL)
    {
        aJson.deleteItem(root);
    }

    if(_isConfigSuccessful)
    {
        return 0;
    }
    else
    {return 1;}
}

void DeviceConfig::sendComfirm(int status)
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

void DeviceConfig::dealHello(void)
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

void DeviceConfig::dealCheckWifi(void)
{
#ifdef configWIRING_WIFI_ENABLE
    aJsonObject* root = aJson.createObject();
    if (root == NULL)
    {return;}

    network_status(0, 0, NULL);
    if(WiFi.ready())  //wifi连通
    {
        manage_ip_config();
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
#endif
#ifdef configWIRING_CELLULAR_ENABLE || configWIRING_LORA_ENABLE
    sendComfirm(201);
#endif
}

void DeviceConfig::dealGetNetworkStatus(void)
{
#ifdef configWIRING_WIFI_ENABLE
    dealCheckWifi();
#endif
#ifdef configWIRING_CELLULAR_ENABLE
#endif
#ifdef configWIRING_LORA_ENABLE
#endif
}

void DeviceConfig::dealGetWifiList(void)
{
#ifdef configWIRING_WIFI_ENABLE
    WiFiAccessPoint ap[20];

    wlan_Imlink_stop();
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
#endif
#ifdef configWIRING_CELLULAR_ENABLE || configWIRING_LORA_ENABLE
    sendComfirm(201);
#endif
}

void DeviceConfig::dealGetInfo(void)
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
#ifdef configWIRING_WIFI_ENABLE
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
#endif
#ifdef configWIRING_CELLULAR_ENABLE
#endif
#ifdef configWIRING_LORA_ENABLE
#endif
    char* string = aJson.print(root);
    write((unsigned char *)string, strlen(string));
    free(string);
    aJson.deleteItem(root);
}

void DeviceConfig::dealSendWifiInfo(aJsonObject* value_Object)
{
#ifdef configWIRING_WIFI_ENABLE
    wlan_Imlink_stop();
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
        }
        sendComfirm(200);
        return;
    }
    sendComfirm(201);
#endif
#ifdef configWIRING_CELLULAR_ENABLE
    sendComfirm(201);
#endif
#ifdef configWIRING_LORA_ENABLE
    sendComfirm(201);
#endif
}

void DeviceConfig::dealSetNetworkCredentials(aJsonObject* value_Object)
{
#ifdef configWIRING_WIFI_ENABLE
    dealSendWifiInfo(value_Object);
#endif
#ifdef configWIRING_CELLULAR_ENABLE
    sendComfirm(201);
#endif
#ifdef configWIRING_LORA_ENABLE
    sendComfirm(201);
#endif
}

void DeviceConfig::dealSendDeviceInfo(aJsonObject* value_Object)
{
#ifdef configWIRING_WIFI_ENABLE
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
#endif
#ifdef configWIRING_CELLULAR_ENABLE || configWIRING_LORA_ENABLE
    sendComfirm(201);
#endif
}

void DeviceConfig::dealSetSecurity(aJsonObject* value_Object)
{
#ifdef configWIRING_WIFI_ENABLE || configWIRING_CELLULAR_ENABLE
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
#endif
#ifdef configWIRING_LORA_ENABLE
    sendComfirm(201);
#endif
}

void DeviceConfig::dealSetInfo(aJsonObject* value_object)
{
    aJsonObject* object = (aJsonObject* )NULL;
    aJsonObject* object1 = (aJsonObject* )NULL;
    aJsonObject* object2 = (aJsonObject* )NULL;

    char *valuestring = NULL;
    uint32_t len = 0;
    float valuefloat = 0.0;
    int valueint = 0;

#ifdef configWIRING_WIFI_ENABLE
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
#endif
#ifdef configWIRING_CELLULAR_ENABLE || configWIRING_LORA_ENABLE
    sendComfirm(201);
#endif
}

void DeviceConfig::dealRestartNetwork(void)
{
#ifdef configWIRING_WIFI_ENABLE
    WiFi.off();
    delay(1000);
    WiFi.on();
    sendComfirm(200);
#endif
#ifdef configWIRING_CELLULAR_ENABLE || configWIRING_LORA_ENABLE
    sendComfirm(201);
#endif
}

void DeviceConfig::dealReset(void)
{
    HAL_Core_Enter_Factory_Reset_Mode();
}

void DeviceConfig::dealReboot(void)
{
    HAL_Core_System_Reset();
}

void DeviceConfig::dealTest(aJsonObject* value_object)
{
    uint16_t pinNum;
    uint8_t pinLevel;
    testItem_t testItem;

    if(value_object == NULL)
    {
        return;
    }

    aJsonObject* itemObject = aJson.getObjectItem(value_object, "item");
    if(itemObject == NULL)
    {
        aJson.deleteItem(value_object);
        return ;
    }

    if(strcmp(itemObject->valuestring,"digitalWrite") == 0)
    {
        aJsonObject* pinObject = aJson.getObjectItem(value_object,"pin");
        if(pinObject == NULL)
        {
            aJson.deleteItem(pinObject);
            return;
        }
        pinNum = pinObject->valueint;

        aJsonObject* valObject = aJson.getObjectItem(value_object,"val");
        if(valObject == NULL)
        {
            aJson.deleteItem(valObject);
            return;
        }

        if(strcmp(valObject->valuestring,"HIGH") == 0)
        {
            pinLevel = HIGH;
        }
        else
        {
            pinLevel = LOW;
        }
        testItem = TEST_DIGITAL_WRITE;
    }
    else if(strcmp(itemObject->valuestring,"analogRead") == 0)
    {
        aJsonObject* pinObject = aJson.getObjectItem(value_object,"pin");
        if(pinObject == NULL)
        {
            aJson.deleteItem(pinObject);
            return;
        }
        pinNum = pinObject->valueint;
        testItem = TEST_ANALOG_READ;
    }
    else if(strcmp(itemObject->valuestring,"selfTest") == 0)
    {
        testItem = TEST_SELF_TEST;
    }
    else if(strcmp(itemObject->valuestring,"rfCheck") == 0)
    {
        testItem = TEST_RF_CHECK;
    }
    else if(strcmp(itemObject->valuestring,"sensorData") == 0)
    {
        testItem = TEST_SENSOR_DATA;
    }

    switch(testItem)
    {
        case TEST_DIGITAL_WRITE:
            {
                testDigitalWrite(pinNum,pinLevel,this);
            }
            break;

        case TEST_ANALOG_READ:
            {
                testAnalogRead(pinNum,this);
            }
            break;

        case TEST_SELF_TEST:
            {
                testSelfTest(this);
            }
            break;

        case TEST_RF_CHECK:
            {
                testRfCheck(this);
            }
            break;

        case TEST_SENSOR_DATA:
            {
                testSensorData(this);
            }
            break;

        default:
            break;
    }
}

#ifdef configSETUP_USBSERIAL_ENABLE
void UsbDeviceConfig::init(void)
{
    serialusb.begin(115200);
    while (!serialusb);
}

int UsbDeviceConfig::available(void)
{
    return serialusb.available();
}

int UsbDeviceConfig::read(void)
{
    return serialusb.read();
}

String UsbDeviceConfig::readString(void)
{
    return serialusb.readString();
}

size_t UsbDeviceConfig::write(const uint8_t *buffer, size_t size)
{
    for(size_t i = 0;i<size;i++)
    {
        serialusb.write(buffer[i]);
    }
    return size;
}

void UsbDeviceConfig::sendComfirm(int status)
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

void UsbDeviceConfig::close(void)
{
    //none do
}
#endif

#ifdef configSETUP_USARTSERIAL_ENABLE
void UsartDeviceConfig::init(void)
{
    serial.begin(115200);
    while (!serial);
}

int UsartDeviceConfig::available(void)
{
    return serial.available();
}

int UsartDeviceConfig::read(void)
{
    return serial.read();
}

String UsartDeviceConfig::readString(void)
{
    return serial.readString();
}

size_t UsartDeviceConfig::write(const uint8_t *buffer, size_t size)
{
    for(size_t i = 0;i<size;i++)
    {
        serial.write(buffer[i]);
    }
    return size;
}

void UsartDeviceConfig::sendComfirm(int status)
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

void UsartDeviceConfig::close(void)
{
    //none do
}
#endif

#ifdef configSETUP_TCP_ENABLE
void TcpDeviceConfig::init(void)
{
    server.begin();
}

void TcpDeviceConfig::sendComfirm(int status)
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

int TcpDeviceConfig::available(void)
{
    client_bak = server.available();
    if(client_bak)
    {
        if(client)
        {
            client.stop();
        }
        client=client_bak;
    }

    if(client)
    {
        if(client.connected())
        {
            return client.available();
        }
        client.stop();
    }
    return false;
}

int TcpDeviceConfig::read(void)
{
    return client.read();
}

String TcpDeviceConfig::readString(void)
{
    return client.readString();
}

size_t TcpDeviceConfig::write(const uint8_t *buf, size_t size)
{
    return client.write(buf,size);
}

void TcpDeviceConfig::close(void)
{
    client.stop();
}
#endif

#ifdef configSETUP_UDP_ENABLE
void UdpDeviceConfig::init(void)
{
    wlan_Imlink_start();
}

void UdpDeviceConfig::sendComfirm(int status)
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

int UdpDeviceConfig::available(void)
{
    static volatile uint8_t step = 0;
    switch(step)
    {
        case 0:
            if( IMLINK_SUCCESS == wlan_Imlink_get_status() ) {
                wlan_Imlink_stop();
                network_connect(0, 0, 0, NULL);
                ARM_CONFIG_TIMEOUT(5000);
                step = 1;
            }
            break;
        case 1:
            if(IS_CONFIG_TIMEOUT()) {
                wlan_Imlink_start();
                CLR_CONFIG_TIMEOUT();
                step = 0;
            }
            if( network_status(0, 0, NULL) ) {
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

int UdpDeviceConfig::read(void)
{
    return Udp.read();
}

String UdpDeviceConfig::readString(void)
{
    return Udp.readString();
}

size_t UdpDeviceConfig::write(const uint8_t *buf, size_t size)
{
    Udp.beginPacket(IPADDR_BROADCAST,5557);
    Udp.write(buf,size);
    return Udp.endPacket();
}

void UdpDeviceConfig::close(void)
{
    Udp.stop();
    wlan_Imlink_stop();
}
#endif

#ifdef configSETUP_USBSERIAL_ENABLE
UsbDeviceConfig DeviceSetupSerial;
#endif
#ifdef configSETUP_USARTSERIAL_ENABLE
UsartDeviceConfig DeviceSetupSerial;
#endif
#ifdef configSETUP_TCP_ENABLE
TcpDeviceConfig DeviceSetupAp;
#endif
#ifdef configSETUP_UDP_ENABLE
UdpDeviceConfig DeviceSetupImlink;
#endif

static system_config_mode_t current_system_config_mode = SYSTEM_CONFIG_MODE_AUTOMATIC;  //配置模式处理方式
static system_config_type_t current_system_config_type = SYSTEM_CONFIG_TYPE_NONE;       //配置类型
static uint8_t system_config_initial_flag = 0;

void set_system_config_mode(system_config_mode_t mode)
{
    current_system_config_mode = mode;
}

system_config_mode_t get_system_config_mode(void)
{
    return current_system_config_mode;
}

void set_system_config_type(system_config_type_t config_type)
{
    if(current_system_config_type == config_type)
    {
        return;
    }

    system_config_initial_flag = 0;
#ifdef configSETUP_UDP_ENABLE
    DeviceSetupImlink.close();
#endif
#ifdef configSETUP_TCP_ENABLE
    DeviceSetupAp.close();
#endif
    DeviceSetupSerial.close();

    current_system_config_type = config_type;
    switch(config_type)
    {
        case SYSTEM_CONFIG_TYPE_IMLINK_SERIAL:   //进入串口配置模式
            HAL_PARAMS_Set_System_config_flag(CONFIG_FLAG_IMLINK_SERIAL);
            break;
        case SYSTEM_CONFIG_TYPE_AP_SERIAL:      //进入ap+串口配置模式
            HAL_PARAMS_Set_System_config_flag(CONFIG_FLAG_AP_SERIAL);
            break;
        case SYSTEM_CONFIG_TYPE_SERIAL:         //串口配置模式
            HAL_PARAMS_Set_System_config_flag(CONFIG_FLAG_SERIAL);
            break;
        case SYSTEM_CONFIG_TYPE_IMLINK:         //进入imlink配置模式
            HAL_PARAMS_Set_System_config_flag(CONFIG_FLAG_IMLINK);
            break;
        case SYSTEM_CONFIG_TYPE_AP:             //进入ap配置模式
            HAL_PARAMS_Set_System_config_flag(CONFIG_FLAG_AP);
            break;
        default:   //退出配置模式
            HAL_PARAMS_Set_System_config_flag(CONFIG_FLAG_NONE);
            NEWORK_FN(Network_Setup(), (void)0);
            break;
    }
    HAL_PARAMS_Save_Params();
}

system_config_type_t get_system_config_type(void)
{
    return current_system_config_type;
}

void system_config_initial(void)
{
    switch(get_system_config_type())
    {
        case SYSTEM_CONFIG_TYPE_IMLINK_SERIAL:   //进入串口配置模式
#ifdef configSETUP_UDP_ENABLE
            DeviceSetupImlink.init();
#endif
            DeviceSetupSerial.init();
            break;
        case SYSTEM_CONFIG_TYPE_AP_SERIAL:      //进入ap+串口配置模式
#ifdef configSETUP_TCP_ENABLE
            DeviceSetupAp.init();
#endif
            DeviceSetupSerial.init();
            break;
        case SYSTEM_CONFIG_TYPE_SERIAL:         //串口配置模式
            DeviceSetupSerial.init();
            break;
        case SYSTEM_CONFIG_TYPE_IMLINK:         //进入imlink配置模式
#ifdef configSETUP_UDP_ENABLE
            DeviceSetupImlink.init();
#endif
            break;
        case SYSTEM_CONFIG_TYPE_AP:             //进入ap配置模式
#ifdef configSETUP_TCP_ENABLE
            DeviceSetupAp.init();
#endif
            break;
        default:
            break;
    }
}

int system_config_process(void)
{
    int result = 0;

    if(SYSTEM_CONFIG_TYPE_NONE == get_system_config_type())
    {
        return 0;
    }

    if(0 == system_config_initial_flag)
    {
        system_config_initial();
        system_config_initial_flag = 1;
    }

    switch(get_system_config_type())
    {
        case SYSTEM_CONFIG_TYPE_IMLINK_SERIAL:   //进入imlink+串口配置模式
#ifdef configSETUP_UDP_ENABLE
            result = DeviceSetupImlink.process();
            if(!result)
            {
                break;
            }
#endif
            result = DeviceSetupSerial.process();
            break;
        case SYSTEM_CONFIG_TYPE_AP_SERIAL:      //进入ap+串口配置模式
#ifdef configSETUP_TCP_ENABLE
            result = DeviceSetupAp.process();
            if(!result)
            {
                break;
            }
#endif
            result = DeviceSetupSerial.process();
            break;
        case SYSTEM_CONFIG_TYPE_SERIAL:         //串口配置模式
            result = DeviceSetupSerial.process();
            break;
        case SYSTEM_CONFIG_TYPE_IMLINK:         //进入imlink配置模式
#ifdef configSETUP_TCP_ENABLE
            result = DeviceSetupImlink.process();
#endif
            break;
        case SYSTEM_CONFIG_TYPE_AP:             //进入ap配置模式
#ifdef configSETUP_TCP_ENABLE
            result = DeviceSetupAp.process();
#endif
            break;
        default:
            break;
    }
    if(!result)
    {
        set_system_config_type(SYSTEM_CONFIG_TYPE_NONE);
    }
    return result;
}

// These are internal methods
void system_config_setup(void)
{
    switch(HAL_PARAMS_Get_System_config_flag())
    {
        case CONFIG_FLAG_IMLINK_SERIAL:   //进入imlink+串口配置模式
            set_system_config_type(SYSTEM_CONFIG_TYPE_IMLINK_SERIAL);
            break;
        case CONFIG_FLAG_AP_SERIAL:      //进入ap+串口配置模式
            set_system_config_type(SYSTEM_CONFIG_TYPE_AP_SERIAL);
            break;
        case CONFIG_FLAG_SERIAL:         //串口配置模式
            set_system_config_type(SYSTEM_CONFIG_TYPE_SERIAL);
            break;
        case CONFIG_FLAG_IMLINK:         //进入imlink配置模式
            set_system_config_type(SYSTEM_CONFIG_TYPE_IMLINK);
            break;
        case CONFIG_FLAG_AP:             //进入ap配置模式
            set_system_config_type(SYSTEM_CONFIG_TYPE_AP);
            break;
        default:
            set_system_config_type(SYSTEM_CONFIG_TYPE_NONE);
            break;
    }
}

void manage_system_config(void)
{
    if(SYSTEM_CONFIG_MODE_AUTOMATIC == get_system_config_mode()) //配置处理流程放后台处理,阻塞处理。
    {
        if(SYSTEM_CONFIG_TYPE_NONE != get_system_config_type())
        {
            system_rgb_blink(RGB_COLOR_RED, 1000);
            while(1)
            {
                if(0 == system_config_process())
                {break;}
                HAL_Core_System_Yield();
            }
        }
    }
}

#endif
