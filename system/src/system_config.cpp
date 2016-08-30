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
#include "system_rgbled.h"
#include "system_network.h"
#include "system_network_internal.h"
#include "params_hal.h"
#include "core_hal.h"
#include "inet_hal.h"
#include "wlan_hal.h"
#include "wiring_wifi.h"

using namespace intorobot;

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
        return DEVICE_CONFIG_GET_BASE_INFO;
    }
    else if(!strcmp(s,"getWifiList")) {
        return DEVICE_CONFIG_GET_WIFI_LIST;
    }
    else if(!strcmp(s,"sendWifiInfo")) {
        return DEVICE_CONFIG_SET_WIFI_CREDENTIALS;
    }
    else if(!strcmp(s,"sendDeviceInfo")) {
        return DEVICE_CONFIG_SET_BOUND_INFO;
    }
    else if(!strcmp(s,"checkWifi")) {
        return DEVICE_CONFIG_GET_WIFI_STATUS;
    }
    else if(!strcmp(s,"restartNetwork")) {
        return DEVICE_CONFIG_RESTART_NETWORK;
    }
    else if(!strcmp(s,"getInfo")) {
        return DEVICE_CONFIG_GET_INFO;
    }
    else if(!strcmp(s,"setInfo")) {
        return DEVICE_CONFIG_SET_INFO;
    }
    else if(!strcmp(s,"reset")) {
        return DEVICE_CONFIG_RESET_FAC;
    }
    else if(!strcmp(s,"reboot")) {
        return DEVICE_CONFIG_REBOOT;
    }
    else if(!strcmp(s,"clearKey")) {
        return DEVICE_CONFIG_CLEAR_SECURITY_INFO;
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
            case DEVICE_CONFIG_GET_BASE_INFO: //获取设备基础信息
                getDeviceBaseInfo();
                break;

            case DEVICE_CONFIG_GET_WIFI_LIST: //获取wifi列表
                getApScanList();
                break;

            case DEVICE_CONFIG_SET_WIFI_CREDENTIALS: //设置wifi信息
                value_Object = aJson.getObjectItem(root, "value");
                if (value_Object == NULL)
                {break;}
                setWifiCredentials(value_Object);
                break;

            case DEVICE_CONFIG_SET_BOUND_INFO:       //设置设备绑定信息
                value_Object = aJson.getObjectItem(root, "value");
                if (value_Object == NULL)
                {break;}
                setDeviceBoundInfo(value_Object);
                sendComfirm(200);
                _isConfigSuccessful=true;
                close();
                break;

            case DEVICE_CONFIG_GET_WIFI_STATUS:      //查询wifi连接状态
                getWifiStatus();
                break;

            case DEVICE_CONFIG_RESTART_NETWORK:      //重启网络
                restartNetwork();
                break;

            case DEVICE_CONFIG_GET_INFO:             //获取设备信息
                GETINFO:
                getDeviceInfo();
                break;

            case DEVICE_CONFIG_SET_INFO:             //设置设备信息
                value_Object = aJson.getObjectItem(root, "value");
                if (value_Object == NULL)
                {break;}
                setDeviceInfo(value_Object);
                break;

            case DEVICE_CONFIG_RESET_FAC:            //设备恢复出厂设置
                resetDeviceFac();
                break;

            case DEVICE_CONFIG_REBOOT:               //设备重启
                rebootDevice();
                break;

            case DEVICE_CONFIG_CLEAR_SECURITY_INFO:  //设备清除密钥
                clearSecurityInfo();
                break;

            case DEVICE_CONFIG_TEST:
                value_Object = aJson.getObjectItem(root, "value");
                testDevice(value_Object);
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

void DeviceConfig::getDeviceBaseInfo(void)
{
    aJsonObject* root = aJson.createObject();
    if (root == NULL)
    {return;}

    aJson.addNumberToObject(root, "status", 200);
    char device_id[32], board[8];
    if ( 0x01 != HAL_PARAMS_Get_System_at_mode() ) {
        HAL_Board_Type(board, sizeof(board), 1);
        aJson.addStringToObject(root, "board", (char *)board);
        aJson.addNumberToObject(root, "at_mode", 0);
    }
    else {
        HAL_Board_Type(board, sizeof(board), 0);
        HAL_PARAMS_Get_System_device_id(device_id, sizeof(device_id));
        if(!memcmp(board, device_id, 6)) {
            aJson.addStringToObject(root, "board", (char *)board);
        }
        else {
            HAL_Board_Type(board, sizeof(board), 1);
            aJson.addStringToObject(root, "board", (char *)board);
        }
        DEBUG_D("board = %s\r\n", board);
        aJson.addStringToObject(root, "device_id", (char *)device_id);
        aJson.addNumberToObject(root, "at_mode", 1);
    }
    char* string = aJson.print(root);
    write((unsigned char *)string, strlen(string));
    free(string);
    aJson.deleteItem(root);
}

void DeviceConfig::getApScanList(void)
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

bool DeviceConfig::setWifiCredentials(aJsonObject* value_Object)
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
            return true;
        }
    }
    sendComfirm(201);
    return false;
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
        HAL_PARAMS_Set_System_at_mode(0x01);
    }

    //domain and port
    Object = aJson.getObjectItem(value_Object, "sv_domain");
    Object1 = aJson.getObjectItem(value_Object, "sv_port");
    Object2 = aJson.getObjectItem(value_Object, "dw_domain");
    if (Object != (aJsonObject* )NULL && Object1 != (aJsonObject* )NULL && Object2 != (aJsonObject* )NULL) {
        HAL_PARAMS_Set_System_sv_domain(Object->valuestring);
        HAL_PARAMS_Set_System_sv_port(Object1->valueint);
        HAL_PARAMS_Set_System_dw_domain(Object2->valuestring);
        HAL_PARAMS_Set_System_sv_select(0x01);
    }
    HAL_PARAMS_Save_Params();
}

void DeviceConfig::getWifiStatus(void)
{
    aJsonObject* root = aJson.createObject();
    if (root == NULL)
    {return;}

    if(WiFi.ready())  //wifi连通
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
    free(string);
    aJson.deleteItem(root);
}

void DeviceConfig::restartNetwork(void)
{
    WiFi.off();
    delay(1000);
    WiFi.on();
    sendComfirm(200);
}

void DeviceConfig::getDeviceInfo(void)
{
    aJsonObject* root = aJson.createObject();
    if (root == NULL)
    {return;}

    aJson.addNumberToObject(root, "status", 200);

    aJsonObject* value_object = aJson.createObject();
    if (value_object == NULL)
    {return;}


    char device_id[32],board[8];
    if (0x01 != HAL_PARAMS_Get_System_at_mode()) {
        HAL_Board_Type(board, sizeof(board), 1);
        aJson.addStringToObject(value_object, "board", board);
        aJson.addNumberToObject(value_object, "at_mode", 0);
    }
    else {
        HAL_Board_Type(board, sizeof(board), 0);
        aJson.addStringToObject(value_object, "board", board);
        HAL_PARAMS_Get_System_device_id(device_id, sizeof(device_id));
        aJson.addStringToObject(value_object, "device_id", device_id);
        aJson.addNumberToObject(value_object, "at_mode", 1);
    }

    aJson.addNumberToObject(value_object, "zone", HAL_PARAMS_Get_System_zone());
    aJson.addNumberToObject(value_object, "sv_select", HAL_PARAMS_Get_System_sv_select());
    char sv_domain[50] = {0};
    HAL_PARAMS_Get_System_sv_domain(sv_domain, sizeof(sv_domain));
    aJson.addStringToObject(value_object, "sv_domain", sv_domain);
    aJson.addNumberToObject(value_object, "sv_port", HAL_PARAMS_Get_System_sv_port());
    char dw_domain[50] = {0};
    HAL_PARAMS_Get_System_dw_domain(dw_domain, sizeof(dw_domain));
    aJson.addStringToObject(value_object, "dw_domain", dw_domain);

    uint8_t stamac[20] = {0}, bssid[20] = {0};;

    aJson.addStringToObject(value_object, "stamac", (char *)WiFi.macAddress(stamac));
    aJson.addStringToObject(value_object, "apmac", (char *)WiFi.macAddress(stamac));
    aJson.addStringToObject(value_object, "ssid", WiFi.SSID());
    aJson.addStringToObject(value_object, "bssid", (char *)WiFi.BSSID(bssid));
    aJson.addNumberToObject(value_object, "rssi", WiFi.RSSI());

    aJson.addItemToObject(root, "value", value_object);

    char* string = aJson.print(root);
    write((unsigned char *)string, strlen(string));
    free(string);
    aJson.deleteItem(root);
}

/*设置板子信息*/
void DeviceConfig::setDeviceInfo(aJsonObject* value_object)
{
    aJsonObject* object = (aJsonObject* )NULL;
    aJsonObject* object1 = (aJsonObject* )NULL;
    aJsonObject* object2 = (aJsonObject* )NULL;

    char *valuestring = NULL;
    uint32_t len = 0;
    float valuefloat = 0.0;
    int valueint = 0;

    // device_id and access_token
    object = aJson.getObjectItem(value_object, "device_id");
    object1 = aJson.getObjectItem(value_object, "access_token");
    if (object != (aJsonObject* )NULL && object1 != (aJsonObject* )NULL)
    {
        HAL_PARAMS_Set_System_device_id(object->valuestring);
        HAL_PARAMS_Set_System_access_token(object1->valuestring);
        HAL_PARAMS_Set_System_at_mode(0x01);
    }

    //zone
    object = aJson.getObjectItem(value_object, "zone");
    if (object != (aJsonObject* )NULL)
    {
        valuefloat = (object->type==aJson_Int?(float)(object->valueint):object->valuefloat);
        if(valuefloat < -12 || valuefloat > 13)
        {valuefloat = 8.0;}
        HAL_PARAMS_Set_System_zone(valuefloat);
    }

    //domain and port
    object = aJson.getObjectItem(value_object, "sv_domain");
    object1 = aJson.getObjectItem(value_object, "sv_port");
    object2 = aJson.getObjectItem(value_object, "dw_domain");
    if (object != (aJsonObject* )NULL && object1 != (aJsonObject* )NULL && object2 != (aJsonObject* )NULL)
    {
        HAL_PARAMS_Set_System_sv_domain(object->valuestring);
        HAL_PARAMS_Set_System_sv_port(object1->valueint);
        HAL_PARAMS_Set_System_dw_domain(object2->valuestring);
        HAL_PARAMS_Set_System_sv_select(0x01);
    }

    object = aJson.getObjectItem(value_object, "stamac");
    object1 = aJson.getObjectItem(value_object, "apmac");
    if ((object != NULL)&&(object1 != NULL))
    {
/*
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
        */
        sendComfirm(200);
    }
    else
    {
        sendComfirm(202);
    }
}

void DeviceConfig::resetDeviceFac(void)
{
    HAL_PARAMS_Set_Boot_boot_flag(3);
    HAL_PARAMS_Save_Params();
    delay(1000);
    HAL_Core_System_Reset();
}

void DeviceConfig::rebootDevice(void)
{
    HAL_Core_System_Reset();
}

void DeviceConfig::clearSecurityInfo(void)
{
    HAL_PARAMS_Set_System_access_token("");
    HAL_PARAMS_Set_System_at_mode(0x00);
    HAL_PARAMS_Save_Params();
    sendComfirm(200);
}

void DeviceConfig::testDevice(aJsonObject* value_object)
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
    free(string);
    aJson.deleteItem(root);
}

void UsbDeviceConfig::close(void)
{
    //none do
}

void UsartDeviceConfig::init(void)
{
    serial.begin(115200);    //
    while (!serial); // wait for a serial connection
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
                network.connect();
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
}


UsbDeviceConfig DeviceConfigUsb;
//TcpDeviceConfig DeviceConfigTcp;
UdpDeviceConfig DeviceConfigUdp;
