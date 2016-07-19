/**
 ******************************************************************************
 * @file        : device_config.h
<<<<<<< HEAD
 * @authors     : robin & chy
 * @version	: V1.1.0
 * @date        : 6-December-2015
 * @brief       :
=======
 * @author   : robin
 * @version	: V1.0.0
 * @date       : 6-December-2014
 * @brief       :
>>>>>>> add parameter
 ******************************************************************************
  Copyright (c) 2015-2017 IntoRobot Team @ MOLMC LTD. Co.  All right reserved.

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
#ifndef   DEVICE_CONFIG_H_
#define	  DEVICE_CONFIG_H_

#include "wiring_usbserial.h"
#include "lib_tcpclient.h"
#include "lib_tcpserver.h"
#include "lib_udpserver.h"
#include "ajson.h"

//data type
namespace DeviceConfigType {
	enum Enum{
		CHECKDEVICE,    //check device exist
		GETWIFILIST,    //get wifi list
		SENDWIFIINFO,   //send wifi info
		SENDDEVICEINFO, //send device info
        GETDEVICEINFO,  //get initial info
		INITDEVICEINFO, //init device info
        CLEANDEVICEINFO,//clean device info
		RESTARTNETWORK, //restart network
		CHECKWIFI,      //check wifi connect
		ERROR
	};
}

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

	DeviceConfigType::Enum getMessageType(char *s);
	bool process(void);
	void RestartNetwork(void);
	virtual void sendComfirm(int status);
	void sendDeviceInfo(void);
	void sendApScanList(void);
	//bool setWifiCredentials(char *ssid, char *password, char *channel, char *security);
	bool setWifiCredentials(aJsonObject *value_Object);
	bool setWrtTimezone(float time_zone);
	void setDeviceBoundInfo(aJsonObject *value_Object);
	void sendWifiStatus(void);
	bool isNeedDeviceConfig(void);
	bool setDeviceConfig(void);
	bool clearDeviceConfig(void);
    void sendInitInfo(void);
    void cleanDeviceInfo(void);
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
};

class TcpDeviceConfig : public DeviceConfig
{
public:
	TcpServer server;
	TcpClient client;
	TcpClient client_bak;

public:
	TcpDeviceConfig(){}

    void init(void);
    virtual int available(void);
    virtual int read(void);
    virtual String readString(void);
    virtual size_t write(const uint8_t *buf, size_t size);
	virtual void close(void);
};

class UdpDeviceConfig : public DeviceConfig
{
public:
	UdpServer server;

public:
	UdpDeviceConfig(){};

	void init(void);
    virtual int available(void);
    virtual int read(void) {};
    virtual String readString(void);
    virtual size_t write(const uint8_t *buf, size_t size);
    virtual void close(void);
	virtual void sendComfirm(int status);
};

extern UsbDeviceConfig DeviceConfigUsb;
extern TcpDeviceConfig DeviceConfigTcp;
extern UdpDeviceConfig DeviceConfigUdp;


#endif /*WIFI_CREDENTIALS_READER_H_*/

