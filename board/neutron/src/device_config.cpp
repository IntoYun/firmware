/**
******************************************************************************
* @file        : device_config.cpp
* @author   : robin
* @version  : V1.0.0
* @date      : 6-December-2014
* @brief      :
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
#include "device_config.h"
#include "lib_wifi.h"
#include "wiring_flash_memory.h"
#include "intorobot_api.h"

//#define DEBUG(fmt, ...)

#include "lib_system_all.h"

#include "device_config_hal.h"

#include "cmsis_os.h"

#include "lib_wifi_hal.h"

#include "lib_wifi_drv.h"



volatile uint8_t  smartconfig_over_flag = 0;
volatile uint8_t  udp_read_flag = 0;

/*********************************************************************************
 *Function		:   DeviceConfigType::Enum DeviceConfig::getMessageType(char *s)
 *Description	:   get the message type of device config
 *Input              :   s: the input data
 *Output		:   none
 *Return		:   the message type
 *author		:   robot
 *date			:   2015-02-01
 *Others		:
 **********************************************************************************/
DeviceConfigType::Enum DeviceConfig::getMessageType(char *s)
{
    if(0==strcmp(s,"hello"))
    {
        return DeviceConfigType::CHECKDEVICE;
    }
    else if(0==strcmp(s,"getWifiList"))
    {
        return DeviceConfigType::GETWIFILIST;
    }
    else if(0==strcmp(s,"sendWifiInfo"))
    {
        return DeviceConfigType::SENDWIFIINFO;
    }
    else if(0==strcmp(s,"sendDeviceInfo"))
    {
        return DeviceConfigType::SENDDEVICEINFO;
    }
    else if(0==strcmp(s,"restartNetwork"))
    {
        return DeviceConfigType::RESTARTNETWORK;
    }
    else if(0==strcmp(s,"checkWifi"))
    {
        return DeviceConfigType::CHECKWIFI;
    }
    else if(0==strcmp(s,"getInfo"))
    {
        return DeviceConfigType::GETINFO;
    }
    // else if(0==strcmp(s,"initPara"))
    // {
    //     return DeviceConfigType::INITPARA;
    // }
    // else if(0==strcmp(s,"setPara"))
    // {
    //     return DeviceConfigType::SETPARA;
    // }
    else if(0 == strcmp(s,"initInfo"))
    {
        return DeviceConfigType::INITDEVICEINFO;
    }
    else if(0==strcmp(s,"setInfo"))
    {
        return DeviceConfigType::SETINFO;
    }
    else if(0==strcmp(s,"reset"))
    {
        return DeviceConfigType::RESET;
    }
    else if(0==strcmp(s,"reboot"))
    {
        return DeviceConfigType::REBOOT;
    }
    else if(0==strcmp(s,"clearKey"))
    {
        return DeviceConfigType::CLEARKEY;
    }
    else if(0==strcmp(s,"test"))
    {
        return DeviceConfigType::TEST;
    }
    else
    {
        return DeviceConfigType::ERROR;
    }
}

/*********************************************************************************
 *Function		:   void DeviceConfig::process(void)
 *Description	:   the process of device config
 *Input              :   none
 *Output		:   none
 *Return		:   the result of the process
 *author		:   robot
 *date			:   2015-02-01
 *Others		:
 **********************************************************************************/
bool DeviceConfig::process( void )
{
    aJsonObject *root=NULL;
    aJsonObject* value_Object;

    bool _isConfigSuccessful=false;
    while(available())
    {
        String tmp=readString();
        MO_DEBUG(("config read data:%s\r\n",tmp.c_str()));
        root = aJson.parse((char *)tmp.c_str());
        if (root == NULL)
        {break;}
        aJsonObject* command_Object = aJson.getObjectItem(root, "command");

        if (command_Object == NULL)
        {break;}

        DeviceConfigType::Enum type = getMessageType(command_Object->valuestring);

        switch(type)
        {
        case DeviceConfigType::CHECKDEVICE://获取设备信息
            sendDeviceInfo();
            break;

        case DeviceConfigType::GETWIFILIST://获取wifi 列表
            sendApScanList();
            break;

        case DeviceConfigType::SENDWIFIINFO://设置wifi
            value_Object = aJson.getObjectItem(root, "value");
            if (value_Object == NULL)
            {break;}
            setWifiCredentials(value_Object);
            break;

        case DeviceConfigType::SENDDEVICEINFO:
            value_Object = aJson.getObjectItem(root, "value");
            if (value_Object == NULL)
            {break;}
            setDeviceBoundInfo(value_Object);
            sendComfirm(200);
            sendComfirm(200);
            _isConfigSuccessful=true;
            close();
            delay(500);
            break;

        case DeviceConfigType::INITDEVICEINFO:
            value_Object = aJson.getObjectItem(root, "value");
            if (value_Object == NULL)
            {break;}
            setDeviceBoundInfo(value_Object);
            sendComfirm(200);
            delay(500);
            break;

        case DeviceConfigType::CHECKWIFI:
            sendWifiStatus();
            break;

        case DeviceConfigType::RESTARTNETWORK:
            RestartNetwork();
            break;

        case DeviceConfigType::GETINFO:
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

        case DeviceConfigType::SETINFO:
            value_Object = aJson.getObjectItem(root, "value");
            if (value_Object == NULL)
            {break;}
            SetBoardPara(value_Object);
            break;

        case DeviceConfigType::RESET:
            resetDeviceFactory();
            break;

        case DeviceConfigType::REBOOT:
            rebootDevice();
            break;

        case DeviceConfigType::CLEARKEY:
            clearAccessToken();
            break;

        case DeviceConfigType::TEST:
            value_Object = aJson.getObjectItem(root, "value");
            testNeutron(value_Object);
            break;

        case DeviceConfigType::ERROR: //错误
            sendComfirm(201);
            break;

        default:
            break;
        }
    }
    if(root!=NULL)
    {
        aJson.deleteItem(root);
    }

    if(_isConfigSuccessful)
    {
        MO_DEBUG(("ConfigSuccessful"));
        return true;
    }
    else
    {return false;}
}

/*********************************************************************************
 *Function		:   void DeviceConfig::sendComfirm(int status)
 *Description	:   send the comfirm message
 *Input              :   the status of the comfirm message
 *Output		:   none
 *Return		:   none
 *author		:   robot
 *date			:   2015-02-01
 *Others		:
 **********************************************************************************/
void DeviceConfig::sendComfirm(int status)
{
    aJsonObject* root = aJson.createObject();
    if (root == NULL)
    {return;}

    aJson.addNumberToObject(root, "status", status);
    char* string = aJson.print(root);
    write((unsigned char *)string, strlen(string));
    MO_DEBUG(("sendComfirm: %s\r\n",string));
    vPortFree(string);
    aJson.deleteItem(root);
}

/*********************************************************************************
 *Function		:   void DeviceConfig::sendDeviceInfo(void)
 *Description	:   send the device info
 *Input              :   none
 *Output		:   none
 *Return		:   none
 *author		:   robot
 *date			:   2015-02-01
 *Others		:
 **********************************************************************************/
void DeviceConfig::sendDeviceInfo(void)
{
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
    vPortFree(string);
    aJson.deleteItem(root);
}

/*********************************************************************************
 *Function		:   void DeviceConfig::sendApScanList(void)
 *Description	:   send the ssid scan list
 *Input              :   none
 *Output		:   none
 *Return		:   none
 *author		:   robot
 *date			:   2015-02-01
 *Others		:
 **********************************************************************************/
void DeviceConfig::sendApScanList(void)
{
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
}

/*********************************************************************************
 *Function		:   bool DeviceConfig::setWifiCredentials(char *ssid, char *password, char *channel, char *security)
 *Description	:   set the wifi credentials
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
bool DeviceConfig::setWifiCredentials(aJsonObject* value_Object)
{
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
}

/*********************************************************************************
 *Function		:   bool DeviceConfig::setWrtTimezone(float time_zone)
 *Description	:   set the wrt time zone
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
bool DeviceConfig::setWrtTimezone(float time_zone)
{
    if(time_zone < -12 || time_zone > 13)  time_zone=8.0;
    int res=mo_DeviceConfig_setWrtTimezone(time_zone);

    if(res == 0)
    {
        return true;
    }
    return false;

}

/*********************************************************************************
 *Function		:   void DeviceConfig::setDeviceBoundInfo(char *device_id, char *access_token)
 *Description	:   get  the device bound info
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
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
        MO_DEBUG(("zone:%f", valuefloat));
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
            MO_DEBUG(("device_id:%s, length: %d", valuestring, len));
            if(len > 48) {len = 48;}
            memset(intorobot_system_param.device_id, 0, sizeof(intorobot_system_param.device_id));
            memcpy(intorobot_system_param.device_id, valuestring,len);
            //access_token
            valuestring = Object1->valuestring;
            len=strlen(valuestring);
            MO_DEBUG(("access_token:%s, length: %d", valuestring, len));
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
        MO_DEBUG(("sv_domain:%s", valuestring));
        if(len > 48) {len = 48;}
        memset(intorobot_system_param.sv_domain, 0, sizeof(intorobot_system_param.sv_domain));
        memcpy(intorobot_system_param.sv_domain, valuestring, len);
        //port
        valueint = Object1->valueint;
        MO_DEBUG(("sv_port:%d", valueint));
        intorobot_system_param.sv_port = valueint;
        //down domain
        valuestring = Object2->valuestring;
        len=strlen(valuestring);
        MO_DEBUG(("dw_domain:%s", valuestring));
        if(len > 48) {len = 48;}
        memset(intorobot_system_param.dw_domain, 0, sizeof(intorobot_system_param.dw_domain));
        memcpy(intorobot_system_param.dw_domain, valuestring, len);

        intorobot_system_param.sv_select = (char)0x01;
    }
    saveSystemParams(&intorobot_system_param);
}


/*********************************************************************************
 *Function		:   void DeviceConfig::sendWifiStatus(void)
 *Description	:   send the wifi status
 *Input              :   none
 *Output		:   none
 *Return		:   none
 *author		:   robot
 *date			:   2015-02-01
 *Others		:
 **********************************************************************************/
void DeviceConfig::sendWifiStatus(void)
{
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
}
/*********************************************************************************
 *Function		:  void DeviceConfig::RestartNetwork(void)
 *Description	:  Restart the network using "/etc/init.d/network restart"
 *Input              :  none
 *Output		:  none
 *Return		:  none
 *author		:  robot
 *date			:  2015-02-01
 *Others		:
 **********************************************************************************/
void DeviceConfig::RestartNetwork(void)
{
    WiFi.restartNetwork();
    sendComfirm(200);
    MO_DEBUG(("Network Restarted"));
}

/*获取核心板信息*/
void DeviceConfig::sendBoardInfo(void)
{
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
    aJsonObject* object = (aJsonObject* )NULL;
    aJsonObject* object1 = (aJsonObject* )NULL;
    aJsonObject* object2 = (aJsonObject* )NULL;

    char *valuestring = NULL;
    uint32_t len = 0;
    float valuefloat = 0.0;
    int valueint = 0;
    char device_id_test[48] = {0};

    // device_id and access_token
    MO_DEBUG(("Set device_id and access_token"));
    object = aJson.getObjectItem(value_object, "device_id");
    object1 = aJson.getObjectItem(value_object, "access_token");
    if (object != (aJsonObject* )NULL && object1 != (aJsonObject* )NULL)
    {
        //device_id
        valuestring = object->valuestring;
        len = strlen(valuestring);
        MO_DEBUG(("device_id:%s, length: %d", valuestring, len));
        if(len > 48) {len = 48;}
        memset(intorobot_system_param.device_id, 0, sizeof(intorobot_system_param.device_id));
        memcpy(intorobot_system_param.device_id, valuestring, len);
        memcpy(device_id_test, valuestring, len);
        intorobot_system_param.at_mode = 0x01;

        //access_token
        valuestring = object1->valuestring;
        len = strlen(valuestring);
        MO_DEBUG(("access_token:%s, length: %d", valuestring, len));
        if(len > 48) {len = 48;}
        memset(intorobot_system_param.access_token, 0, sizeof(intorobot_system_param.access_token));
        memcpy(intorobot_system_param.access_token, valuestring, len);
    }

    //zone
    MO_DEBUG(("Set zone"));
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
    MO_DEBUG(("Set sv_domain, sv_port and dw_domain"));
    object = aJson.getObjectItem(value_object, "sv_domain");
    object1 = aJson.getObjectItem(value_object, "sv_port");
    object2 = aJson.getObjectItem(value_object, "dw_domain");
    if (object != (aJsonObject* )NULL && object1 != (aJsonObject* )NULL && object2 != (aJsonObject* )NULL)
    {
        //domain
        valuestring = object->valuestring;
        len = strlen(valuestring);
        MO_DEBUG(("sv_domain:%s", valuestring));
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
}

/*********************************************************************************
 *Function		:   bool DeviceConfig::resetDeviceFactory(void)
 *Description	:
 *Input
 *Output		:
 *Return		:
 *author		:   robot
 *date			:   2015-02-01
 *Others		:
 **********************************************************************************/
void DeviceConfig::resetDeviceFactory(void)
{
    intorobot_boot_param.boot_flag = 3;
    saveBootParams(&intorobot_boot_param);
    MO_DEBUG(("Start reboot!!"));
    delay(1000);
    // restart stm32
    HAL_NVIC_SystemReset();
}

void DeviceConfig::rebootDevice(void)
{
    MO_DEBUG(("restart network!!"));
    // restart esp8266
    RestartNetwork(); // contain sendComfirm(200)

    // restart stm32
    HAL_NVIC_SystemReset();
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
}


/*********************************************************************************
 *Function		:   bool DeviceConfig::isNeedDeviceConfig(void)
 *Description	:   whether the device need config
 *Input              :   none
 *Output		:   none
 *Return		:   the result of device config
 *author		:   robot
 *date			:   2015-02-01
 *Others		:
 **********************************************************************************/
bool DeviceConfig::isNeedDeviceConfig(void)
{
  return intorobot_system_param.config_flag;
}

/*********************************************************************************
 *Function		:   bool DeviceConfig::setDeviceConfig(void)
 *Description	:   set the device config flag
 *Input              :   none
 *Output		:   none
 *Return		:   the result
 *author		:   robot
 *date			:   2015-02-01
 *Others		:
 **********************************************************************************/
bool DeviceConfig::setDeviceConfig(void)
{
    int res=mo_DeviceConfig_setDeviceConfig_hal();

    if(res == 0)
    {
        return true;
    }
    return false;
}

/*********************************************************************************
 *Function		:   bool DeviceConfig::clearDeviceConfig(void)
 *Description	:   clear the device config flag
 *Input              :   none
 *Output		:   none
 *Return		:   the result
 *author		:   robot
 *date			:   2015-02-01
 *Others		:
 **********************************************************************************/
bool DeviceConfig::clearDeviceConfig(void)
{
  intorobot_system_param.config_flag=0;     //清除配置标记
  saveSystemParams(&intorobot_system_param);
  return true;
}

/*********************************************************************************
 *Function		:   void UsbDeviceConfig::init(void)
 *Description	:   initial device config of the usb serail
 *Input              :   none
 *Output		:   none
 *Return		:   none
 *author		:   robot
 *date			:   2015-02-01
 *Others		:
 **********************************************************************************/
void UsbDeviceConfig::init(void)
{
    serialusb.begin(115200);    //
    while (!serialusb); // wait for a serial connection
}

/*********************************************************************************
 *Function		:   int UsbDeviceConfig::available(void)
 *Description	:   whether the data coming
 *Input              :   none
 *Output		:   none
 *Return		:   the result
 *author		:   robot
 *date			:   2015-02-01
 *Others		:
 **********************************************************************************/
int UsbDeviceConfig::available(void)
{
    return serialusb.available();
}

/*********************************************************************************
 *Function		:   int UsbDeviceConfig::read(void)
 *Description	:   read the data from the USB serial
 *Input              :   none
 *Output		:   none
 *Return		:   the data
 *author		:   robot
 *date			:   2015-02-01
 *Others		:
 **********************************************************************************/
int UsbDeviceConfig::read(void)
{
    return serialusb.read();
}

/*********************************************************************************
 *Function		:   String UsbDeviceConfig::readString(void)
 *Description	:   read string from the USB serial
 *Input              :   none
 *Output		:   none
 *Return		:   the data
 *author		:   robot
 *date			:   2015-02-01
 *Others		:

 **********************************************************************************/
String UsbDeviceConfig::readString(void)
{
    return serialusb.readString();
}



/*********************************************************************************
 *Function		:   size_t UsbDeviceConfig::write(const uint8_t *buffer, size_t size)
 *Description	:   write the data to the usb serial
 *Input              :
 *Output		:
 *Return		:   the sizeof the writed data
 *author		:   robot
 *date			:   2015-02-01
 *Others		:

 **********************************************************************************/
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
    MO_DEBUG(("sendComfirm: %s\r\n",string));
    vPortFree(string);
    aJson.deleteItem(root);
}


/*********************************************************************************
 *Function		:   void UsbDeviceConfig::close(void)
 *Description	:   close the USB serial device config
 *Input              :   none
 *Output		:   none
 *Return		:   none
 *author		:   robot
 *date			:   2015-02-01
 *Others		:
 **********************************************************************************/
void UsbDeviceConfig::close(void)
{
    //none do
}

/*********************************************************************************
 *Function		:   void UdpDeviceConfig::init(void)
 *Description	:   the init of the tcp device config
 *Input              :   none
 *Output		:   none
 *Return		:   none
 *author		:   robot
 *date			:   2015-02-01
 *Others		:
 **********************************************************************************/


extern void mo_smartconfig_start();

void UdpDeviceConfig::init(void)
{

	//延时否则太快 smartconfig无返回
    delay(2000);

    //smartconfig
    mo_smartconfig_start();
}

/*********************************************************************************
 *Function		:   void UdpDeviceConfig::sendComfirm(int status)
 *Description   	:   send comfirm message(override DeviceConfig cause UDP send mutli-message
 *Input                :   the status of the comfirm message
 *Output		:   none
 *Return		:   none
 *author		:   chy
 *date			:   2015-02-01
 *Others		:
 **********************************************************************************/
void UdpDeviceConfig::sendComfirm(int status)
{
    aJsonObject* root = aJson.createObject();
    if (root == NULL)
    {return;}

    system_rgb_blink(255, 0, 0, 200);
    aJson.addNumberToObject(root, "status", status);
    char* string = aJson.print(root);
    for(int i=0; i < 5; i++) //may be not enough
    {
        write((unsigned char *)string, strlen(string));
        delay(100);
        MO_DEBUG(("sendComfirm: %s\r\n",string));
    }
    vPortFree(string);
    aJson.deleteItem(root);
}

/*********************************************************************************
 *Function		:   int UdpDeviceConfig::available(void)
 *Description	:   whether the data coming
 *Input              :   none
 *Output		:   none
 *Return		:   the result
 *author		:   robot
 *date			:   2015-02-01
 *Others		:
 **********************************************************************************/
int UdpDeviceConfig::available(void)
{
    if( 1 == smartconfig_over_flag )
    {
        MO_DEBUG(("smart config over\r\n"));
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

}

/*********************************************************************************
 *Function		:   int UdpDeviceConfig::read(void)
 *Description	:   read the data from the tcp server
 *Input              :   none
 *Output		:   none
 *Return		:   the data
 *author		:   robot
 *date			:   2015-02-01
 *Others		:
 **********************************************************************************/
int UdpDeviceConfig::read(void)
{
    return Udp.read();
}

/*********************************************************************************
 *Function		:   String UdpDeviceConfig::readString(void)
 *Description	:   read string from the tcp server
 *Input              :   none
 *Output		:   none
 *Return		:   the data
 *author		:   robot
 *date			:   2015-02-01
 *Others		:
 **********************************************************************************/
String UdpDeviceConfig::readString(void)
{
    return Udp.readString();
}

/*********************************************************************************
 *Function		:   size_t UdpDeviceConfig::write(const uint8_t *buf, size_t size)
 *Description	:   write the data to the tcp server
 *Input              :
 *Output		:
 *Return		:   the sizeof the writed data
 *author		:   robot
 *date			:   2015-02-01
 *Others		:
 **********************************************************************************/
size_t UdpDeviceConfig::write(const uint8_t *buf, size_t size)
{

    //组包
    Udp.beginPacket("255.255.255.255",5557);
    Udp.write(buf,size);
    //发送
    return Udp.endPacket();
}

/*********************************************************************************
 *Function		:   void UdpDeviceConfig::close(void)
 *Description	:   close the tcp device config
 *Input              :   none
 *Output		:   none
 *Return		:   none
 *author		:   robot
 *date			:   2015-02-01
 *Others		:
 **********************************************************************************/
void UdpDeviceConfig::close(void)
{
    Udp.stop();
}


UsbDeviceConfig DeviceConfigUsb;
// TcpDeviceConfig DeviceConfigTcp;
UdpDeviceConfig DeviceConfigUdp;
