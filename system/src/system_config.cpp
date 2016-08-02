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
#include <stdio.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_params.h"
#include "wiring_wifi.h"


volatile uint8_t  smartconfig_over_flag = 0;
volatile uint8_t  udp_read_flag = 0;


DeviceConfigCmdType DeviceConfig::getMessageType(char *s) {
    if(!strcmp(s,"hello")) {
        return DEVICE_CONFIG_CHECK_EXIST;
    }
    else if(!strcmp(s,"getWifiList")) {
        return DEVICE_CONFIG_GET_WIFI_LIST;
    }
    else if(!strcmp(s,"sendWifiInfo")) {
        return DEVICE_CONFIG_SET_WIFI_INFO;
    }
    else if(!strcmp(s,"sendDeviceInfo")) {
        return DEVICE_CONFIG_SET_BOUND_INFO;
    }
    else if(!strcmp(s,"restartNetwork")) {
        return DEVICE_CONFIG_RESTART_NETWORK;
    }
    else if(!strcmp(s,"checkWifi")) {
        return DEVICE_CONFIG_CHECK_WIFI;
    }
    else if(!strcmp(s,"getInfo")) {
        return DEVICE_CONFIG_GET_INFO;
    }
    else if(!strcmp(s,"initInfo")) {
        return DEVICE_CONFIG_INIT_DEFAULT_INFO;
    }
    else if(!strcmp(s,"setInfo")) {
        return DEVICE_CONFIG_SET_INFO;
    }
    else if(!strcmp(s,"reset")) {
        return DEVICE_CONFIG_RESET;
    }
    else if(!strcmp(s,"reboot")) {
        return DEVICE_CONFIG_REBOOT;
    }
    else if(!strcmp(s,"clearKey")) {
        return DEVICE_CONFIG_CLEARKEY;
    }
    else if(!strcmp(s,"test")) {
        return DEVICE_CONFIG_TEST;
    }
    else {
        return DEVICE_CONFIG_ERROR;
    }
}

bool DeviceConfig::process(void)
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
            case DEVICE_CONFIG_CHECK_EXIST://获取设备信息
                sendDeviceInfo();
                break;

            case DEVICE_CONFIG_GET_WIFI_LIST://获取wifi 列表
                sendApScanList();
                break;

            case DEVICE_CONFIG_SET_WIFI_INFO://设置wifi
                value_Object = aJson.getObjectItem(root, "value");
                if (value_Object == NULL)
                {break;}
                setWifiCredentials(value_Object);
                break;

            case DEVICE_CONFIG_SET_BOUND_INFO:
                value_Object = aJson.getObjectItem(root, "value");
                if (value_Object == NULL)
                {break;}
                setDeviceBoundInfo(value_Object);
                sendComfirm(200);
                _isConfigSuccessful=true;
                close();
                delay(500);
                break;
#if 0
            case INITDEVICEINFO:
                value_Object = aJson.getObjectItem(root, "value");
                if (value_Object == NULL)
                {break;}
                setDeviceBoundInfo(value_Object);
                sendComfirm(200);
                delay(500);
                break;
#endif
            case DEVICE_CONFIG_CHECK_WIFI:
                sendWifiStatus();
                break;

            case DEVICE_CONFIG_RESTART_NETWORK:
                restartNetwork();
                break;
#if 0
            case GETINFO:
                sendBoardInfo();
                break;

                // case DeviceConfigType::INITPARA:
                //     initBoardPara();
                //     break;

                // case DeviceConfigType::SETPARA:
                //     value_Object = aJson.getObjectItem(root, "value");
                //     if (value_Object == NULL)
                //     {break;}
                //     SetBoardPara(value_Object);
                //     break;

            case SETINFO:
                value_Object = aJson.getObjectItem(root, "value");
                if (value_Object == NULL)
                {break;}
                SetBoardPara(value_Object);
                break;
#endif
            case DEVICE_CONFIG_RESET:
                resetDeviceFactory();
                break;

            case DEVICE_CONFIG_REBOOT:
                rebootDevice();
                break;

            case DEVICE_CONFIG_CLEARKEY:
                clearAccessToken();
                break;

            case DEVICE_CONFIG_TEST:
                value_Object = aJson.getObjectItem(root, "value");
                //testNeutron(value_Object);
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
        return true;
    }
    else
    {return false;}
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

void DeviceConfig::sendDeviceInfo(void)
{
#if 0
    aJsonObject* root = aJson.createObject();
    if (root == NULL)
    {return;}

    aJson.addNumberToObject(root, "status", 200);
    //intorobot atom
    char device_id[25], board[7];
    if ((char)0x1 != intorobot_system_param.at_mode) {
        aJson.addStringToObject(root, "board", INTOROBOT_BOARD_TYPE2);
        aJson.addNumberToObject(root, "at_mode", 0);
    }
    else {
        memset(board, 0, sizeof(board));
        memcpy(board, intorobot_system_param.device_id, 6);
        if(0 == strcmp(board, INTOROBOT_BOARD_TYPE2)) {
            aJson.addStringToObject(root, "board", INTOROBOT_BOARD_TYPE2);
        } else {
            aJson.addStringToObject(root, "board", INTOROBOT_BOARD_TYPE1);
        }
        memset(device_id, 0, sizeof(device_id));
        memcpy(device_id, intorobot_system_param.device_id, 24);
        aJson.addStringToObject(root, "device_id", device_id);
        aJson.addNumberToObject(root, "at_mode", 1);
    }
    char* string = aJson.print(root);
    write((unsigned char *)string, strlen(string));
    MO_DEBUG(("send device info: %s\r\n",string));
    free(string);
    aJson.deleteItem(root);
#endif
}

void DeviceConfig::sendApScanList(void)
{
#if 0 
    int8_t numOfNetworks = WiFi.scanNetworks();
    if(numOfNetworks < 1)
    {
        sendComfirm(201);
    }
    else
    {
        aJsonObject* root = aJson.createObject();
        if (root == NULL)
        {return;}
        aJson.addNumberToObject(root, "status",200);
        aJson.addNumberToObject(root, "listnum",numOfNetworks);
        aJsonObject* ssidlistarray = aJson.createArray();
        if (ssidlistarray == NULL)
        {
            aJson.deleteItem(root);
            return;
        }
        aJson.addItemToObject(root, "ssidlist", ssidlistarray);

        for(int n = 0; n < numOfNetworks; n++)
        {
            aJsonObject* ssid_object = aJson.createObject();
            if (ssid_object == NULL)
            {
                aJson.deleteItem(root);
                return;
            }
            aJson.addItemToArray(ssidlistarray, ssid_object);
            aJson.addStringToObject(ssid_object, "ssid", WiFi.SSID(n));
            aJson.addNumberToObject(ssid_object, "entype", (int)WiFi.encryptionType(n));
            aJson.addNumberToObject(ssid_object, "signal", (int)WiFi.RSSI(n));
        }
        char* string = aJson.print(root);
        write((unsigned char *)string, strlen(string));
        MO_DEBUG(("sendApScanList: %s\r\n",string));
        vPortFree(string);
        aJson.deleteItem(root);
    }
#endif
}

bool DeviceConfig::setWifiCredentials(aJsonObject* value_Object)
{
#if 0
    aJsonObject* ssid_Object = aJson.getObjectItem(value_Object, "ssid");
    aJsonObject* passwd_Object = aJson.getObjectItem(value_Object, "passwd");
    // TODO: channel security bssid
    // stop the smartconfig
    if(mo_drv_wifi_run_cmd("AT+CWSTOPSMART","OK",10))
    {
        MO_INFO(("STOP SMART OK!\r\n"));
    }
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
            setWifiFlag = WiFi.setCredentials(pSsid, pPasswd);
            if (setWifiFlag == 0)
            {
                MO_DEBUG(("Set wifi OK!"));
                sendComfirm(200);
                return true;
            }
        }
    }
    sendComfirm(201);
    MO_ERROR(("Set wifi Failed!"));
    return false;
#endif
    return true;
}

bool DeviceConfig::setWrtTimezone(float time_zone)
{
#if 0 
    if(time_zone < -12 || time_zone > 13)  time_zone=8.0;
    int res=mo_DeviceConfig_setWrtTimezone(time_zone);

    if(res == 0)
    {
        return true;
    }
    return false;
#endif
return true;
}

void DeviceConfig::setDeviceBoundInfo(aJsonObject* value_Object)
{
    int valueint;
    char len, *valuestring, at_mode;
    float valuefloat;
    aJsonObject* Object, *Object1, *Object2;
    //zone
    Object = aJson.getObjectItem(value_Object, "zone");
    if (Object != (aJsonObject* )NULL) {
        valuefloat = (Object->type==aJson_Int?(float)(Object->valueint):Object->valuefloat);
        //MO_DEBUG(("zone:%f", valuefloat));
        if(valuefloat < -12 || valuefloat > 13)
        {valuefloat = 8.0;}
        intorobot_system_param.zone = valuefloat;
        setWrtTimezone(valuefloat);     //设置wrt时区
    }
    //device_id and access_token
    at_mode = intorobot_system_param.at_mode;
    //if ((char)0x1 != at_mode)
    {
        Object = aJson.getObjectItem(value_Object, "device_id");
        Object1 = aJson.getObjectItem(value_Object, "access_token");
        if (Object != (aJsonObject* )NULL && Object1 != (aJsonObject* )NULL) {
            //device_id
            valuestring = Object->valuestring;
            len=strlen(valuestring);
            //MO_DEBUG(("device_id:%s, length: %d", valuestring, len));
            if(len > 48) {len = 48;}
            memset(intorobot_system_param.device_id, 0, sizeof(intorobot_system_param.device_id));
            memcpy(intorobot_system_param.device_id, valuestring,len);
            //access_token
            valuestring = Object1->valuestring;
            len=strlen(valuestring);
           // MO_DEBUG(("access_token:%s, length: %d", valuestring, len));
            if(len > 48) {len = 48;}
            memset(intorobot_system_param.access_token, 0, sizeof(intorobot_system_param.access_token));
            memcpy(intorobot_system_param.access_token, valuestring,len);
            intorobot_system_param.at_mode = (char)0x1;
        }
    }
    //domain and port
    Object = aJson.getObjectItem(value_Object, "sv_domain");
    Object1 = aJson.getObjectItem(value_Object, "sv_port");
    Object2 = aJson.getObjectItem(value_Object, "dw_domain");
    if (Object != (aJsonObject* )NULL && Object1 != (aJsonObject* )NULL && Object2 != (aJsonObject* )NULL) {
        //domain
        valuestring = Object->valuestring;
        len=strlen(valuestring);
        //MO_DEBUG(("sv_domain:%s", valuestring));
        if(len > 48) {len = 48;}
        memset(intorobot_system_param.sv_domain, 0, sizeof(intorobot_system_param.sv_domain));
        memcpy(intorobot_system_param.sv_domain, valuestring, len);
        //port
        valueint = Object1->valueint;
        //MO_DEBUG(("sv_port:%d", valueint));
        intorobot_system_param.sv_port = valueint;
        //down domain
        valuestring = Object2->valuestring;
        len=strlen(valuestring);
        //MO_DEBUG(("dw_domain:%s", valuestring));
        if(len > 48) {len = 48;}
        memset(intorobot_system_param.dw_domain, 0, sizeof(intorobot_system_param.dw_domain));
        memcpy(intorobot_system_param.dw_domain, valuestring, len);

        intorobot_system_param.sv_select = (char)0x01;
    }
    saveSystemParams(&intorobot_system_param);
}

void DeviceConfig::sendWifiStatus(void)
{
#if 0
    aJsonObject* root = aJson.createObject();
    if (root == NULL)
    {return;}

    if(WiFi.status())  //wifi连通
    {
        aJson.addNumberToObject(root, "status", 200);
        aJson.addStringToObject(root, "ssid", WiFi.SSID());
    }
    else
    {
        aJson.addNumberToObject(root, "status", 201);
        aJson.addStringToObject(root, "ssid", WiFi.SSID());
    }

    char* string = aJson.print(root);
    write((unsigned char *)string, strlen(string));
    MO_DEBUG(("send wifi status: %s\r\n",string));
    vPortFree(string);
    aJson.deleteItem(root);
#endif
}

void DeviceConfig::restartNetwork(void)
{
#if 0
    WiFi.restartNetwork();
    sendComfirm(200);
    MO_DEBUG(("Network Restarted"));
#endif
}

void DeviceConfig::sendBoardInfo(void)
{
#if 0
    aJsonObject* root = aJson.createObject();
    if (root == NULL)
    {return;}

    aJson.addNumberToObject(root, "status", 200);

    aJsonObject* value_object = aJson.createObject();
    if (value_object == NULL)
    {return;}

    if ((char)0x01 != intorobot_system_param.at_mode) {
        aJson.addStringToObject(value_object, "board", INTOROBOT_BOARD_TYPE2);
        aJson.addNumberToObject(value_object, "at_mode", 0);
    }
    else {
        char board[8];
        memset(board, 0, sizeof(board));
        memcpy(board, intorobot_system_param.device_id, 6);
        if(0 == strcmp(board, INTOROBOT_BOARD_TYPE1)) {
            aJson.addStringToObject(value_object, "board", INTOROBOT_BOARD_TYPE1);
        } else {
            aJson.addStringToObject(value_object, "board", INTOROBOT_BOARD_TYPE2);
        }
        char device_id[28];
        memset(device_id, 0, sizeof(device_id));
        memcpy(device_id, intorobot_system_param.device_id, 24);
        aJson.addStringToObject(value_object, "device_id", device_id);
        aJson.addNumberToObject(value_object, "at_mode", 1);
    }

    aJson.addNumberToObject(value_object, "zone", intorobot_system_param.zone);
    aJson.addNumberToObject(value_object, "sv_select", intorobot_system_param.sv_select);
    char sv_domain[50] = {0};
    // memset(sv_domain, 0x00, sizeof(sv_domain));
    // memcpy(sv_domain, intorobot_system_param.sv_domain, 48);
    aJson.addStringToObject(value_object, "sv_domain", sv_domain);
    aJson.addNumberToObject(value_object, "sv_port", intorobot_system_param.sv_port);
    char dw_domain[50] = {0};
    // memset(dw_domain, 0x00, sizeof(dw_domain));
    // memcpy(dw_domain, intorobot_system_param.dw_domain, 48);
    aJson.addStringToObject(value_object, "dw_domain", dw_domain);

    char stamac[20] = {0}, apmac[20] = {0}, ssid[30] = {0}, bssid[20] = {0};
    int rssi = 0, channel = 0;
    mo_get_mac_hal(stamac, apmac);
    mo_get_ssi_hal(ssid, bssid, channel, rssi);

    aJson.addStringToObject(value_object, "stamac", stamac);
    aJson.addStringToObject(value_object, "apmac", apmac);
    aJson.addStringToObject(value_object, "ssid", ssid);
    aJson.addStringToObject(value_object, "bssid", bssid);
    aJson.addNumberToObject(value_object, "rssi", rssi);

    aJson.addItemToObject(root, "value", value_object);

    char* string = aJson.print(root);
    write((unsigned char *)string, strlen(string));
    MO_DEBUG(("send device info: %s\r\n",string));
    vPortFree(string);
    // aJson.deleteItem(value_object);  // if not comment it, it die (usbserial)
    aJson.deleteItem(root);
#endif
}

/*初始化参数*/
void DeviceConfig::initBoardPara(void)
{
    intorobot_system_param.at_mode = (char)0x00;
    intorobot_system_param.sv_select = (char)0x00;
    saveSystemParams(&intorobot_system_param);
}

/*设置板子信息*/
void DeviceConfig::SetBoardPara(aJsonObject* value_object)
{
#if 0
    aJsonObject* object = (aJsonObject* )NULL;
    aJsonObject* object1 = (aJsonObject* )NULL;
    aJsonObject* object2 = (aJsonObject* )NULL;

    char *valuestring = NULL;
    uint32_t len = 0;
    float valuefloat = 0.0;
    int valueint = 0;
    char device_id_test[48] = {0};

    // device_id and access_token
    //MO_DEBUG(("Set device_id and access_token"));
    object = aJson.getObjectItem(value_object, "device_id");
    object1 = aJson.getObjectItem(value_object, "access_token");
    if (object != (aJsonObject* )NULL && object1 != (aJsonObject* )NULL)
    {
        //device_id
        valuestring = object->valuestring;
        len = strlen(valuestring);
        //MO_DEBUG(("device_id:%s, length: %d", valuestring, len));
        if(len > 48) {len = 48;}
        memset(intorobot_system_param.device_id, 0, sizeof(intorobot_system_param.device_id));
        memcpy(intorobot_system_param.device_id, valuestring, len);
        memcpy(device_id_test, valuestring, len);
        intorobot_system_param.at_mode = 0x01;

        //access_token
        valuestring = object1->valuestring;
        len = strlen(valuestring);
        //MO_DEBUG(("access_token:%s, length: %d", valuestring, len));
        if(len > 48) {len = 48;}
        memset(intorobot_system_param.access_token, 0, sizeof(intorobot_system_param.access_token));
        memcpy(intorobot_system_param.access_token, valuestring, len);
    }

    //zone
    //MO_DEBUG(("Set zone"));
    object = aJson.getObjectItem(value_object, "zone");
    if (object != (aJsonObject* )NULL)
    {
        valuefloat = (object->type==aJson_Int?(float)(object->valueint):object->valuefloat);
        MO_DEBUG(("zone:%f", valuefloat));
        if(valuefloat < -12 || valuefloat > 13)
        {valuefloat = 8.0;}
        intorobot_system_param.zone = valuefloat;
        setWrtTimezone(valuefloat);     //设置wrt时区
    }

    //domain and port
    //MO_DEBUG(("Set sv_domain, sv_port and dw_domain"));
    object = aJson.getObjectItem(value_object, "sv_domain");
    object1 = aJson.getObjectItem(value_object, "sv_port");
    object2 = aJson.getObjectItem(value_object, "dw_domain");
    if (object != (aJsonObject* )NULL && object1 != (aJsonObject* )NULL && object2 != (aJsonObject* )NULL)
    {
        //domain
        valuestring = object->valuestring;
        len = strlen(valuestring);
        //MO_DEBUG(("sv_domain:%s", valuestring));
        if(len > 48) {len = 48;}
        memset(intorobot_system_param.sv_domain, 0, sizeof(intorobot_system_param.sv_domain));
        memcpy(intorobot_system_param.sv_domain, valuestring, len);
        //port
        valueint = object1->valueint;
        MO_DEBUG(("sv_port:%d", valueint));
        intorobot_system_param.sv_port = valueint;
        //down domain
        valuestring = object2->valuestring;
        len=strlen(valuestring);
        MO_DEBUG(("dw_domain:%s", valuestring));
        if(len > 48) {len = 48;}
        memset(intorobot_system_param.dw_domain, 0, sizeof(intorobot_system_param.dw_domain));
        memcpy(intorobot_system_param.dw_domain, valuestring, len);
        intorobot_system_param.sv_select = (char)0x01;
    }
    saveSystemParams(&intorobot_system_param);

    loadSystemParams(&intorobot_system_param);
    // test for Device id for factory
    if (0 == strcmp(device_id_test, intorobot_system_param.device_id))
    {
        MO_DEBUG(("device_id:%s", intorobot_system_param.device_id));
        MO_DEBUG(("Set device id OK"));
        // sendConfirm(200);
    }
    else
    {
        MO_ERROR(("Set device id error"));
        sendComfirm(201);

    }
    MO_DEBUG(("device_id:%s", intorobot_system_param.device_id));

    object = aJson.getObjectItem(value_object, "stamac");
    object1 = aJson.getObjectItem(value_object, "apmac");

    if ((object != NULL)&&(object1 != NULL))
    {
        MO_DEBUG(("Set stamac and apmac"));
        uint8_t setMacFlag = 1; // 0 success, 1 fail
        setMacFlag = mo_set_mac_hal(object->valuestring, object1->valuestring);
        if (setMacFlag == 0)
        {
            MO_DEBUG(("Set Macaddress OK!"));
            sendComfirm(200);
        }
        else
        {
            MO_DEBUG(("Set Macaddress Failed!"));
            sendComfirm(202);
        }
    }
    else
    {
        MO_DEBUG(("Set Macaddress Failed!"));
        sendComfirm(202);
    }

    // stop the smartconfig
    if(mo_drv_wifi_run_cmd("AT+CWSTOPSMART","OK",10))
    {
        MO_INFO(("STOP SMART OK!\r\n"));
    }

    MO_DEBUG(("SetInfo End!!"));
#endif
}

void DeviceConfig::resetDeviceFactory(void)
{
    intorobot_boot_param.boot_flag = 3;
    saveBootParams(&intorobot_boot_param);
    //MO_DEBUG(("Start reboot!!"));
    delay(1000);
    // restart stm32
    //HAL_NVIC_SystemReset();
}

void DeviceConfig::rebootDevice(void)
{
    //MO_DEBUG(("restart network!!"));
    // restart esp8266
    restartNetwork(); // contain sendComfirm(200)

    // restart stm32
    //HAL_NVIC_SystemReset();
}

void DeviceConfig::clearAccessToken(void)
{
    memcpy(intorobot_system_param.access_token, "FFFFFFFFFFFFFFFFFFFFFFFF", 24);
    intorobot_system_param.at_mode = (char)0x0;
    saveSystemParams(&intorobot_system_param);
    sendComfirm(200);
}

void DeviceConfig::testNeutron(aJsonObject* value_object)// uart
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

void UsbDeviceConfig::init(void)
{
    serialusb.begin(115200);    //
    while (!serialusb); // wait for a serial connection
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
    //MO_DEBUG(("sendComfirm: %s\r\n",string));
    free(string);
    aJson.deleteItem(root);
}

void UsbDeviceConfig::close(void)
{
    //none do
}

void UdpDeviceConfig::init(void)
{
	//延时否则太快 smartconfig无返回
    delay(2000);
    //smartconfig
    //mo_smartconfig_start();
}

void UdpDeviceConfig::sendComfirm(int status)
{
#if 0
    aJsonObject* root = aJson.createObject();
    if (root == NULL)
    {return;}

    system_rgb_blink(255, 0, 0, 200);
    aJson.addNumberToObject(root, "status", status);
    char* string = aJson.print(root);
    for(int i=0; i < 15; i++) //may be not enough
    {
        write((unsigned char *)string, strlen(string));
        delay(100);
        MO_DEBUG(("sendComfirm: %s\r\n",string));
    }
    free(string);
    aJson.deleteItem(root);
#endif
}

int UdpDeviceConfig::available(void)
{
#if 0
    if( 1 == smartconfig_over_flag )
    {
        //MO_DEBUG(("smart config over\r\n"));
        smartconfig_over_flag = 0;
        udp_read_flag = 1;
        mo_smartconfig_stop();
        //监听端口
        Udp.begin(5556);
    }

    if(udp_read_flag)
    {
        // MO_DEBUG(("udp read\r\n"));
        delay(300); // Poll every 300ms
        return Udp.available();
    }
    else
    {return 0;}
#endif
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
    //组包
    Udp.beginPacket("255.255.255.255",5557);
    Udp.write(buf,size);
    //发送
    return Udp.endPacket();
}

void UdpDeviceConfig::close(void)
{
    Udp.stop();
}


UsbDeviceConfig DeviceConfigUsb;
//TcpDeviceConfig DeviceConfigTcp;
UdpDeviceConfig DeviceConfigUdp;
