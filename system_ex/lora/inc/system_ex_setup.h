/**
 ******************************************************************************
 * @file        : device_config.h
 * @author   : robin
 * @version	: V1.0.0
 * @date       : 6-December-2014
 * @brief       :
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
#ifndef SYSTEM_EX_SETUP_H_
#define SYSTEM_EX_SETUP_H_

#include "intorobot_config.h"

#ifdef configSETUP_ENABLE

#include "wiring_usbserial.h"
#include "wiring_usartserial.h"
#include "wiring_tcpclient.h"
#include "wiring_tcpserver.h"
#include "wiring_udp.h"
#include "ajson.h"

//设备配置指令
typedef enum{
    //查询类
    DEVICE_SETUP_HELLO,                   //获取设备基础信息
    DEVICE_SETUP_CHECK_WIFI,              //获取wifi状态
    DEVICE_SETUP_GET_WIFI_LIST,           //获取wifi列表
    DEVICE_SETUP_GET_NETWORK_STATUS,      //查询网络状态
    DEVICE_SETUP_GET_INFO,                //获取设备信息
    //设置类
    DEVICE_SETUP_SEND_WIFI_INFO,          //设置wifi信息
    DEVICE_SETUP_SET_NETWORK_CREDENTIALS, //设置网络接入凭证
    DEVICE_SETUP_SET_DEVICE_INFO,         //设置设备信息
    DEVICE_SETUP_SET_SECURITY,            //设置设备安全信息
    DEVICE_SETUP_SET_INFO,                //设置设备信息
    //执行类
    DEVICE_SETUP_RESTART_NETWORK,         //重启网络
    DEVICE_SETUP_RESET,                   //设备恢复出厂设置
    DEVICE_SETUP_REBOOT,                  //设备重启
    DEVICE_SETUP_EXIT,                    //退出配置模式
    //测试类
    DEVICE_SETUP_TEST,                    //设备测试
    DEVICE_SETUP_ERROR
}DeviceSetupCmdType;

class DeviceSetup
{
public:
    aJsonClass aJson;

public:
    DeviceSetup(void){}

    virtual int available(void)=0;
    virtual int read(void)=0;
    virtual String readString(void)=0;
    virtual size_t write(const uint8_t *buffer, size_t size)=0;
    virtual void close(void)=0;
    virtual void sendComfirm(int status);

    DeviceSetupCmdType getMessageType(char *s);

    bool process(void);

    void dealHello(void);
    void dealCheckWifi(void);
    void dealGetNetworkStatus(void);
    void dealGetWifiList(void);
    void dealGetInfo(void);

    void dealSendWifiInfo(aJsonObject* value_Object);
    void dealSetNetworkCredentials(aJsonObject* value_Object);
    void dealSendDeviceInfo(aJsonObject* value_Object);
    void dealSetSecurity(aJsonObject* value_Object);
    void dealSetInfo(aJsonObject* value_Object);

    void dealRestartNetwork(void);
    void dealReset(void);
    void dealReboot(void);
    void dealTest(aJsonObject* value_Object);
};

class UsbDeviceSetup : public DeviceSetup
{
public:
    USBSerial &serialusb;

public:
    UsbDeviceSetup(USBSerial &_s = SerialUSB):serialusb(_s){}
    void init();
    virtual int available(void);
    virtual int read(void);
    virtual String readString(void);
    virtual size_t write(const uint8_t *buf, size_t size);
    virtual void close(void);
    virtual void sendComfirm(int status);
};

class UsartDeviceSetup: public DeviceSetup
{
public:
	USARTSerial &serial;

public:
	UsartDeviceSetup(USARTSerial &_s = Serial):serial(_s){}
    void init();
    virtual int available(void);
    virtual int read(void);
    virtual String readString(void);
    virtual size_t write(const uint8_t *buf, size_t size);
    virtual void close(void);
    virtual void sendComfirm(int status);
};

void manage_setup_config();

#endif
#endif

