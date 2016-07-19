/**
 ******************************************************************************
 * @file        : device_config.cpp
 * @authors     : robin, chy
 * @version     : V1.1.0
 * @date        : 6-December-2015
 * @brief       :
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
#include "device_config.h"
#include "lib_wifi.h"
#include "wiring_flash_memory.h"
#include "intorobot_api.h"


/*********************************************************************************
  *Function		:   DeviceConfigType::Enum DeviceConfig::getMessageType(char *s)
  *Description	:   get the message type of device config
  *Input        :   s: the input data
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
	else if(0  == strcmp(s,"getInfo"))
	{
		return DeviceConfigType::GETDEVICEINFO;
	}
	else if(0  == strcmp(s,"initInfo"))
	{
		return DeviceConfigType::INITDEVICEINFO;
	}
	else if(0  == strcmp(s,"cleanInfo"))
	{
		return DeviceConfigType::CLEANDEVICEINFO;
	}
    else if(0==strcmp(s,"restartNetwork"))
    {
        return DeviceConfigType::RESTARTNETWORK;
    }
    else if(0==strcmp(s,"checkWifi"))
    {
        return DeviceConfigType::CHECKWIFI;
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
    bool _isConfigSuccessful=false;
    while(available())
    {
        String tmp=readString();
        DEBUG("config read data:%s\r\n",tmp.c_str());
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

            case DeviceConfigType::SENDWIFIINFO://获取wifi 设置
                {
                    aJsonObject* value_Object = aJson.getObjectItem(root, "value");
                    if (value_Object == NULL)
                    {break;}
                    bool ret = setWifiCredentials(value_Object);
                    if (ret == true)
                    {sendComfirm(200);}
                    else
                    {sendComfirm(201);}
                }
                break;

            case DeviceConfigType::SENDDEVICEINFO://获取设备绑定信息
                {
                    aJsonObject* value_Object = aJson.getObjectItem(root, "value");
                    if (value_Object == NULL)
                    {break;}
                    setDeviceBoundInfo(value_Object);
                    sendComfirm(200);
                    _isConfigSuccessful=true;
                    close();
                }
                break;

            case DeviceConfigType::GETDEVICEINFO:
                sendInitInfo();
                break;

            case DeviceConfigType::INITDEVICEINFO://注入初始信息
                {
                    aJsonObject* value_Object = aJson.getObjectItem(root, "value");
                    if (value_Object == NULL)
                    {break;}
                    setDeviceBoundInfo(value_Object);
                    sendComfirm(200);
                    delay(500);
                    break;
                }

            case DeviceConfigType::CLEANDEVICEINFO: //清除设备信息
                {
                    cleanDeviceInfo();
                    sendComfirm(200);
                    delay(500);
                    break;
                }

            case DeviceConfigType::CHECKWIFI: //确定wifi  连接
                sendWifiStatus();
                break;

            case DeviceConfigType::ERROR: //错误
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
        DEBUG("ConfigSuccessful");
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
    DEBUG("sendComfirm: %s\r\n",string);
    free(string);
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
    aJson.addStringToObject(root, "product_id", intorobot_system_param.product_id);
    //intorobot atom
    char device_id[25], board[7];
    String serail_num = deviceSn();
    aJson.addStringToObject(root, "device_sn", serail_num.c_str());
    if ((char)0x1 != intorobot_system_param.at_mode) {
        aJson.addStringToObject(root, "board", INTOROBOT_BOARD_TYPE2);
        aJson.addNumberToObject(root, "at_mode", 0);
    } else {
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
    DEBUG("send device info: %s\r\n",string);
    free(string);
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
    DEBUG("num:%d\r\n", numOfNetworks);
    if(numOfNetworks<0)
    {
        sendComfirm(201);
    }
    else  //direct send all ap list
    {
        char aplist[AP_LIST_MAX_LENGTH]; //can be a bug here
        WiFi.getApList(aplist);
        write((unsigned char *)aplist, strlen(aplist));
        DEBUG("sendApScanList: %s\r\n",aplist);
    }
#if 0
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
            DEBUG("n = %d\r\n", n);
            aJsonObject* ssid_object = aJson.createObject();
            if (ssid_object == NULL)
            {
                aJson.deleteItem(root);
                return;
            }
            aJson.addItemToArray(ssidlistarray, ssid_object);
            DEBUG("n = %d\r\n", n);
            aJson.addStringToObject(ssid_object, "ssid", WiFi.SSID(n));
            DEBUG("n = %d\r\n", n);
            aJson.addNumberToObject(ssid_object, "entype", (int)WiFi.encryptionType(n));
            DEBUG("n = %d\r\n", n);
            aJson.addNumberToObject(ssid_object, "signal", (int)WiFi.RSSI(n));
        }
        char* string = aJson.print(root);
        write((unsigned char *)string, strlen(string));
        DEBUG("sendApScanList: %s\r\n",string);
        free(string);
        aJson.deleteItem(root);
    }
#endif
}

/*********************************************************************************
  *Function		:   bool DeviceConfig::setWifiCredentials(aJsonObject *value)
  *Description	:   set the wifi credentials
  *Input              :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
bool DeviceConfig::setWifiCredentials(aJsonObject *value_Object)
{
    char *ssid = NULL, *password = NULL, *channel = NULL, *security = NULL;
    aJsonObject *Object;

    //ssid
    Object = aJson.getObjectItem(value_Object, "ssid");
    if (Object == NULL) {
        return false;
    } else {
        ssid = Object->valuestring;
    }
    //passwd
    Object = aJson.getObjectItem(value_Object, "passwd");
    if (Object != (aJsonObject *)NULL) {
        password = Object->valuestring;
    }
    //channel
    Object = aJson.getObjectItem(value_Object, "channel");
    if (Object != (aJsonObject *)NULL) {
        channel = Object->valuestring;
    }
    //security
    aJsonObject* security_Object = aJson.getObjectItem(value_Object, "security");
    if (Object != (aJsonObject *)NULL) {
        security = Object->valuestring;
    }

    DEBUG("ssid:%s",ssid);
    if (password != (char *)NULL)
    {DEBUG("passwd:%s",password);}
    if (channel != (char *)NULL)
    {DEBUG("channel:%s",channel);}
    if (security != (char *)NULL)
    {DEBUG("security:%s",security);}

    if(password == (char *)NULL) { //密码为空
        WiFi.setCredentials(ssid);
    } else if (security == (char *)NULL) { //安全性为空
        WiFi.setCredentials(ssid, password);
    } else {
        WiFi.setCredentials(ssid, password, "auto", security);
    }
    system_rgb_blink(0, 255, 255, 1000);
    //WiFi.connect();
    //WiFi.restartNetwork();
    delay(5000);
    system_rgb_blink(255, 0, 0, 1000);
    return true;
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

    String str_temp;
    str_temp+=time_zone;


    Process Proc;
    Proc.begin("atom_set_time_zone");
    Proc.addParameter(str_temp.c_str());

    int res = Proc.run();

    if(res == 0)
    {
        DEBUG("config:set wrt time zone:%s \n",str_temp.c_str());
        return true;
    }
    return false;

}

/*********************************************************************************
  *Function		:   void DeviceConfig::setDeviceBoundInfo(aJsonObject* value_Object)
  *Description	:   get  the device bound info
  *Input              :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
void DeviceConfig::setDeviceBoundInfo(aJsonObject *value_Object)
{
    int valueint;
    char len, *valuestring, at_mode;
    float valuefloat;
    aJsonObject *Object, *Object1, *Object2;
    //zone
    Object = aJson.getObjectItem(value_Object, "zone");
    if (Object != (aJsonObject* )NULL) {
        valuefloat = (Object->type==aJson_Int?(float)(Object->valueint):Object->valuefloat);
        DEBUG("zone:%f", valuefloat);
        if(valuefloat < -12 || valuefloat > 13)
        {valuefloat = 8.0;}
        intorobot_system_param.zone = valuefloat;
        setWrtTimezone(valuefloat);     //设置wrt时区
    }
    //device_id and access_token
    at_mode = intorobot_system_param.at_mode;
    if ((char)0x1 != at_mode)
    {
        Object = aJson.getObjectItem(value_Object, "device_id");
        Object1 = aJson.getObjectItem(value_Object, "access_token");
        if (Object != (aJsonObject* )NULL && Object1 != (aJsonObject* )NULL) {
            //device_id
            valuestring = Object->valuestring;
            len=strlen(valuestring);
            DEBUG("device_id:%s, length: %d", valuestring, len);
            if(len > 48) {len = 48;}
            memset(intorobot_system_param.device_id, 0, sizeof(intorobot_system_param.device_id));
            memcpy(intorobot_system_param.device_id, valuestring,len);
            //access_token
            valuestring = Object1->valuestring;
            len=strlen(valuestring);
            DEBUG("access_token:%s, length: %d", valuestring, len);
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
        DEBUG("sv_domain:%s", valuestring);
        if(len > 48) {len = 48;}
        memset(intorobot_system_param.sv_domain, 0, sizeof(intorobot_system_param.sv_domain));
        memcpy(intorobot_system_param.sv_domain, valuestring, len);
        //port
        valueint = Object1->valueint;
        DEBUG("sv_port:%d", valueint);
        intorobot_system_param.sv_port = valueint;
         //down domain
        valuestring = Object2->valuestring;
        len=strlen(valuestring);
        DEBUG("dw_domain:%s", valuestring);
        if(len > 48) {len = 48;}
        memset(intorobot_system_param.dw_domain, 0, sizeof(intorobot_system_param.dw_domain));
        memcpy(intorobot_system_param.dw_domain, valuestring, len);

        intorobot_system_param.sv_select = (char)0x01;
    }
    writeSystemParam(&intorobot_system_param);
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
    DEBUG("send wifi status: %s\r\n",string);
    free(string);
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
    DEBUG("Network Restarted");
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
    Process Proc;
    Proc.begin("device_config");
    Proc.addParameter("CHECK");
    int res = Proc.run();
    DEBUG("config status: %d \r\n", res);
    if(res == 0)
    {
        return true;
    }
    return false;
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
    Process Proc;
    Proc.begin("device_config"); //in this script the openwrt Ap is restarted
    Proc.addParameter("SET");
    int res = Proc.run();
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
    Process Proc;
    Proc.begin("device_config");
    Proc.addParameter("CLEAR");
    int res = Proc.run();
    if(res == 0)
    {
        return true;
    }
    return false;
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
void DeviceConfig::sendInitInfo(void)
{
    aJsonObject* root = aJson.createObject();
    if (root == NULL)
    {return;}

    aJson.addNumberToObject(root, "status", 200);
    aJson.addStringToObject(root, "board", INTOROBOT_BOARD_TYPE1);
    //intorobot atom
    if ((char)0x01 == intorobot_system_param.at_mode) {
        aJson.addNumberToObject(root, "at_mode", 1);
    }
    else {
        aJson.addNumberToObject(root, "at_mode", 0);
    }
    char* string = aJson.print(root);
    write((unsigned char *)string, strlen(string));
    DEBUG("send device info: %s\r\n",string);
    free(string);
    aJson.deleteItem(root);
}

/*********************************************************************************
  *Function		:   void DeviceConfig::cleanDeviceInfo(void)
  *Description	:   clean the device info
  *Input              :   none
  *Output		:   none
  *Return		:   none
  *author		:   robot
  *date			:   2016-05-09
  *Others		:
**********************************************************************************/
void DeviceConfig::cleanDeviceInfo(void)
{
    memcpy(intorobot_system_param.access_token, "FFFFFFFFFFFFFFFFFFFFFFFF", 24);
    memcpy(intorobot_system_param.device_id, "FFFFFFFFFFFFFFFFFFFFFFFF", 24);
    intorobot_system_param.at_mode = 0xff;
    writeSystemParam(&intorobot_system_param);
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
    //serialusb.begin(115200);
    //while (!serialusb); // wait for a serial connection
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
  *Function		:   void TcpDeviceConfig::init(void)
  *Description	:   the init of the tcp device config
  *Input              :   none
  *Output		:   none
  *Return		:   none
  *author		:   robot
  *date			:   2015-02-01
  *Others		:
**********************************************************************************/
void TcpDeviceConfig::init(void)
{
    server.noListenOnLocalhost();
    server.begin();
    DEBUG("config: establish a TCP server");
}

/*********************************************************************************
  *Function		:   int TcpDeviceConfig::available(void)
  *Description	:   whether the data coming
  *Input              :   none
  *Output		:   none
  *Return		:   the result
  *author		:   robot
  *date			:   2015-02-01
  *Others		:
**********************************************************************************/
int TcpDeviceConfig::available(void)
{
    delay(300); // Poll every 300ms
    client_bak = server.accept();
    if(client_bak)
    {
        if(client)
        {
            client.stop();
            DEBUG("config: old client  stop");
        }
        client=client_bak;
        DEBUG("config: have new a client");
    }

    if(client)
    {
        if(client.connected())
        {
            return client.available();
        }
        DEBUG("config: client  stop");
        client.stop();
    }
    return false;
}

/*********************************************************************************
  *Function		:   int TcpDeviceConfig::read(void)
  *Description	:   read the data from the tcp server
  *Input              :   none
  *Output		:   none
  *Return		:   the data
  *author		:   robot
  *date			:   2015-02-01
  *Others		:
**********************************************************************************/
int TcpDeviceConfig::read(void)
{
    return client.read();
}

/*********************************************************************************
  *Function		:   String TcpDeviceConfig::readString(void)
  *Description	:   read string from the tcp server
  *Input              :   none
  *Output		:   none
  *Return		:   the data
  *author		:   robot
  *date			:   2015-02-01
  *Others		:
**********************************************************************************/
String TcpDeviceConfig::readString(void)
{
    return client.readString();
}

/*********************************************************************************
  *Function		:   size_t TcpDeviceConfig::write(const uint8_t *buf, size_t size)
  *Description	:   write the data to the tcp server
  *Input              :
  *Output		:
  *Return		:   the sizeof the writed data
  *author		:   robot
  *date			:   2015-02-01
  *Others		:
**********************************************************************************/
size_t TcpDeviceConfig::write(const uint8_t *buf, size_t size)
{
    return client.write(buf,size);
}

/*********************************************************************************
  *Function		:   void TcpDeviceConfig::close(void)
  *Description	:   close the tcp device config
  *Input              :   none
  *Output		:   none
  *Return		:   none
  *author		:   robot
  *date			:   2015-02-01
  *Others		:
**********************************************************************************/
void TcpDeviceConfig::close(void)
{
    client.stop();
}

//*************************************************************************************
/*********************************************************************************
  *Function		:   void UdpDeviceConfig::init(void)
  *Description  	:   Initialize UDP-based device confiig
  *Input                :   none
  *Output		:   none
  *Return		:   none
  *author		:   chy
  *date			:   2015-02-01
  *Others		:
**********************************************************************************/
void UdpDeviceConfig::init(void)
{
    end_imlink_config(); //kill the imlink processes for safty
    start_imlink_config();
    server.begin(5556, 5557);
    DEBUG("config: establish a UDP server");
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
    for(int i=0; i<15; i++) //may be not enough
    {
        write((unsigned char *)string, strlen(string));
        delay(100);
        DEBUG("sendComfirm: %s\r\n",string);
    }
    free(string);
    aJson.deleteItem(root);
}

/*********************************************************************************
  *Function		:   int UdpDeviceConfig::available(void)
  *Descriptioni  	:   check whether the data comes
  *Input                :   none
  *Output		:   none
  *Return		:   the result
  *author		:   chy
  *date			:   2015-02-01
  *Others		:
o**********************************************************************************/
int UdpDeviceConfig::available(void)
{
    return server.available();
}

/*********************************************************************************
  *Function		:   int UdpDeviceConfig::read(void)
  *Description	        :   read the data from the Udp server
  *Input                :   none
  *Output		:   none
  *Return		:   the data
  *author		:   chy
  *date			:   2015-02-01
  *Others		:
**********************************************************************************/
/*int UdpDeviceConfig::read(void)
{
    //return server.read_begin(0);
    return 1;
}
*/

/*********************************************************************************
  *Function		:   String UdpDeviceConfig::readString(void)
  *Description   	:   read string from the Udp server
  *Input                :   none
  *Output		:   none
  *Return		:   the data
  *author		:   chy
  *date			:   2015-02-01
  *Others		:
**********************************************************************************/
String UdpDeviceConfig::readString(void)
{
    return server.receive();
}

/*********************************************************************************
  *Function		:   size_t UdpDeviceConfig::write(const uint8_t *buf, size_t size)
  *Description   	:   write the data to the send_socket
  *Input                :
  *Output		:
  *Return		:   the sizeof the writed data
  *author		:   chy
  *date			:   2015-02-01
  *Others		:
**********************************************************************************/
size_t UdpDeviceConfig::write(const uint8_t *buf, size_t size)
{
    server.write(buf, size);
    return size;
}

/*********************************************************************************
  *Function		:   void UdpDeviceConfig::close(void)
  *Description  	:   close the udp device config
  *Input                :   none
  *Output		:   none
  *Return		:   none
  *author		:   robot
  *date			:   2015-02-01
  *Others		:
**********************************************************************************/
void UdpDeviceConfig::close(void)
{
    end_imlink_config(); //kill the imlink processes for safty
    server.stop();
}


UsbDeviceConfig DeviceConfigUsb;
TcpDeviceConfig DeviceConfigTcp;
UdpDeviceConfig DeviceConfigUdp;
