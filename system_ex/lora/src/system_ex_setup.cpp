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

}

void DeviceSetup::dealGetNetworkStatus(void)
{
    dealCheckWifi();
}

void DeviceSetup::dealGetWifiList(void)
{

}

void DeviceSetup::dealGetInfo(void)
{
    sendComfirm(200);
}

void DeviceSetup::dealSetNetworkCredentials(aJsonObject* value_Object)
{

}

void DeviceSetup::dealSendDeviceInfo(aJsonObject* value_Object)
{

}

void DeviceSetup::dealSetSecurity(aJsonObject* value_Object)
{

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
    HAL_PARAMS_Save_Params();
    sendComfirm(200);
}

void DeviceSetup::dealRestartNetwork(void)
{
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

void UsbDeviceSetup::init(void)
{
    serialusb.begin(115200);    //
    while (!serialusb); // wait for a serial connection
}

int UsbDeviceSetup::available(void)
{
    return serialusb.available();
}

int UsbDeviceSetup::read(void)
{
    return serialusb.read();
}

String UsbDeviceSetup::readString(void)
{
    return serialusb.readString();
}

size_t UsbDeviceSetup::write(const uint8_t *buffer, size_t size)
{
    for(size_t i = 0;i<size;i++)
    {
        serialusb.write(buffer[i]);
    }
    return size;
}

void UsbDeviceSetup::sendComfirm(int status)
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

void UsbDeviceSetup::close(void)
{
    //none do
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

UsbDeviceSetup DeviceSetupUsb;
UsartDeviceSetup DeviceSetupUsart;
#endif
