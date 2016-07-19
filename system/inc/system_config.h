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
#ifndef SYSTEM_CONFIG_H_
#define SYSTEM_CONFIG_H_

#include "wiring_usbserial.h"
#include "wiring_tcpclient.h"
#include "wiring_tcpserver.h"
#include "wiring_udp.h"
#include "ajson.h"


//data type
typedef enum{
    DEVICE_CONFIG_CHECK_EXIST,          //检查设备
    DEVICE_CONFIG_GET_WIFI_LIST,        //获取wifi列表
    DEVICE_CONFIG_SET_WIFI_INFO,        //设置wifi列表
    DEVICE_CONFIG_SET_BOUND_INFO,       //设置设备绑定信息
    DEVICE_CONFIG_CHECK_WIFI,           //检查wifi链接状态
    DEVICE_CONFIG_RESTART_NETWORK,      //重启网络
    DEVICE_CONFIG_INIT_DEFAULT_INFO,    //初始化设备默认参数
    DEVICE_CONFIG_GET_INFO,             //获取设备信息
    DEVICE_CONFIG_SET_INFO,             //设置设备信息
    DEVICE_CONFIG_RESET,                //设备复位
    DEVICE_CONFIG_REBOOT,               //设备重启
    DEVICE_CONFIG_CLEARKEY,             //设备清除密钥
    DEVICE_CONFIG_TEST,                 //设备测试
    DEVICE_CONFIG_ERROR
}DeviceConfigCmdType;


class DeviceConfig
{
public:
	aJsonClass aJson;

public:
	DeviceConfig(void){}

	virtual int available(void)=0;
	virtual int read(void)=0;
	virtual String readString(void)=0;
	virtual size_t write(const uint8_t *buffer, size_t size)=0;
	virtual void close(void)=0;
    virtual void sendComfirm(int status);

	DeviceConfigCmdType getMessageType(char *s);

	bool process(void);
	void sendDeviceExistInfo(void);
	void sendApScanList(void);
	bool setWifiCredentials(aJsonObject* value_Object);
	bool setWrtTimezone(float time_zone);
    void setDeviceBoundInfo(aJsonObject* value_Object);
    void sendWifiStatus(void);
    void restartNetwork(void);
    void sendDeviceInfo(void);
    void initDeviceInfo(void);
    void initBoardPara(void);
    void sendBoardInfo(void);
    void SetDeviceInfo(aJsonObject* value_Object);
    void SetBoardPara(aJsonObject* value_object);
    void resetDeviceFactory(void);
    void rebootDevice(void);
    void clearAccessToken(void);
    void test(aJsonObject* value_Object);
    void testNeutron(aJsonObject* value_object);
};

class UsbDeviceConfig : public DeviceConfig
{
public:
	USBSerial &serialusb;

public:
	UsbDeviceConfig(USBSerial &_s = SerialUSB):serialusb(_s){}
    void init();
    virtual int available(void);
    virtual int read(void);
    virtual String readString(void);
    virtual size_t write(const uint8_t *buf, size_t size);
    virtual void close(void);
    virtual void sendComfirm(int status);

};

class TcpDeviceConfig : public DeviceConfig
{
public:
    TCPServer server;
    TCPClient client,client_bak;

public:
	//TcpDeviceConfig(){}

    void init(void);
    virtual int available(void);
    virtual int read(void);
    virtual String readString(void);
    virtual size_t write(const uint8_t *buf, size_t size);
    virtual void close(void);
    virtual void sendComfirm(int status);
};

class UdpDeviceConfig : public DeviceConfig
{
public:
    UDP Udp;

public:
    UdpDeviceConfig(){}

    void init(void);
    virtual int available(void);
    virtual int read(void);
    virtual String readString(void);
    virtual size_t write(const uint8_t *buf, size_t size);
    virtual void close(void);
    virtual void sendComfirm(int status);
};

extern UsbDeviceConfig DeviceConfigUsb;
extern TcpDeviceConfig DeviceConfigTcp;
extern UdpDeviceConfig DeviceConfigUdp;


#endif /*SYSTEM_CONFIG_H_*/

