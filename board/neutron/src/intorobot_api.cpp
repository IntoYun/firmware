/**
******************************************************************************
* @file     : intorobot_api.cpp
* @author   : robin
* @version  : V1.0.0
* @date     : 6-December-2014
* @brief    :
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
#include "intorobot_api.h"
#include "lib_wifi.h"
#include "lib_rgb.h"
#include "device_config.h"
#include "firmware_update.h"

#include "wiring_flash_memory.h"
#include "wiring_time.h"
#include "lib_system_all.h"
#include "lib_rgb.h"
#include "lib_wifi_drv.h"
#include "cmsis_os.h"

//firmware  info
#define INTOROBOT_API_VER       "v1"

//firmware update address
#define INTOROBOT_FW_UPDATE_URL "/v1/bins/"
#define INTOROBOT_OW_UPDATE_URL "/downloads/neutron/"

//sevice info
#define INTOROBOT_UPDATE_URL "www.intorobot.com"
#define INTOROBOT_SERVER_DOMAIN "iot.intorobot.com"

#define INTOROBOT_SERVER_PORT   1883

volatile uint8_t intorobot_cloud_connect_flag = 1;     //需要连接平台				1连接           0不连接
volatile uint8_t intorobot_cloud_socketed_flag = 0;    //wifi连接状态               1 连接			0断开
volatile uint8_t intorobot_cloud_connected_flag = 0;   //平台连接状态               1连接上了
volatile system_tick_t intorobot_mqttconnect_period_timer;            //mqtt connect period timer
volatile system_tick_t intorobot_mqttconnect_count;                   //mqtt connect count
volatile system_tick_t intorobot_mqttconnect_short_to_long_count;     //快速重连平台次数


volatile system_tick_t intorobot_wificheck_period_timer;       //wifi status check period timer


void mo_system_reboot_hal();

struct _callbacklist callbacklist;  //回调结构体
IntorobotClass IntoRobot;
SystemClass System;
boot_params_t intorobot_boot_param;  	//bootloader参数
system_params_t intorobot_system_param;  	//设备参数

System_Mode_TypeDef SystemClass::_mode = MODE_AUTOMATIC;

extern RTC_HandleTypeDef RtcHandle;
/*********************************************************************************
 *Function		:     SystemClass::SystemClass()
 *Description	:     constructor function
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
SystemClass::SystemClass()
{
}

/*********************************************************************************
 *Function		:    SystemClass::SystemClass(System_Mode_TypeDef mode)
 *Description	:    constructor function
 *Input              :    the mode of system
 *Output		:    none
 *Return		:    none
 *author		:    robot
 *date			:    2015-02-01
 *Others		:
 **********************************************************************************/
SystemClass::SystemClass(System_Mode_TypeDef mode)
{
    switch(mode)
    {
    case MODE_AUTOMATIC:
        _mode = MODE_AUTOMATIC;
        intorobot_cloud_connect_flag = 1;
        break;

    case MODE_SEMI_AUTOMATIC:
        _mode = MODE_SEMI_AUTOMATIC;
        intorobot_cloud_connect_flag = 0;
        break;

    case MODE_MANUAL:
        _mode = MODE_MANUAL;
        intorobot_cloud_connect_flag = 0;
        break;
    }
}

/*********************************************************************************
 *Function		:     System_Mode_TypeDef SystemClass::mode(void)
 *Description	:     return the mode of system
 *Input              :     none
 *Output		:     none
 *Return		:     the mode of system
 *author		:     robot
 *date			:     2015-02-01
 *Others		:
 **********************************************************************************/
System_Mode_TypeDef SystemClass::mode(void)
{
    return _mode;
}

/*********************************************************************************
 *Function		:   IntorobotClass::IntorobotClass(void)
 *Description	:   consturctor function
 *Input              :   none
 *Output		:   none
 *Return		:   none
 *author		:   robot
 *date			:   2015-02-01
 *Others		:
 **********************************************************************************/
IntorobotClass::IntorobotClass(void)
{
    Cloud_Debug_Buffer  Debug_tx_buffer;
    Cloud_Debug_Buffer  Debug_rx_buffer;

    memset(&Debug_tx_buffer,0,sizeof(Debug_tx_buffer));
    memset(&Debug_rx_buffer,0,sizeof(Debug_rx_buffer));

    ApiMqttClient = MqttClientClass((char *)INTOROBOT_SERVER_DOMAIN, INTOROBOT_SERVER_PORT, apiMqttClientCallBack, mqtttcpclient);
}

/*********************************************************************************
 *Function		:    bool IntorobotClass::connected(void)
 *Description	:    whether board connected intorobot system
 *Input              :    none
 *Output		:    none
 *Return		:    the result fo connecting
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
bool IntorobotClass::connected(void)
{
    if(intorobot_cloud_socketed_flag && intorobot_cloud_connected_flag)
    {return true;}
    else
    {return false;}
}

/*********************************************************************************
 *Function		:    void IntorobotClass::connect(void)
 *Description	:    connect the  intorobot system
 *Input              :    none
 *Output		:    none
 *Return		:    none
 *author		:    robot
 *date			:    2015-02-01
 *Others		:
 **********************************************************************************/
void IntorobotClass::connect(void)
{
    intorobot_cloud_connect_flag = 1;
}

/*********************************************************************************
 *Function		:    void IntorobotClass::disconnect(void)
 *Description	:   disconnect the intorobot system
 *Input              :    none
 *Output		:    none
 *Return		:    none
 *author		:    robot
 *date			:    2015-02-01
 *Others		:
 **********************************************************************************/
void IntorobotClass::disconnect(void)
{
    ApiMqttClient.disconnect();
    intorobot_cloud_connect_flag = 0;
    intorobot_cloud_connected_flag = 0;
}
/*
  uint8_t IntorobotClass::publish(const char *topic, bool payload)
  {
  return publish(topic, (uint8_t)payload);
  }
*/
uint8_t IntorobotClass::publish(const char *topic, char payload)
{
    return publish(topic, (uint8_t)payload);
}

uint8_t IntorobotClass::publish(const char *topic, uint8_t payload)
{
    String tmp;
    tmp += payload;
    return publish(topic, (uint8_t *)tmp.c_str(), strlen(tmp.c_str()), 0, true);
}

uint8_t IntorobotClass::publish(const char *topic, short int payload)
{
    String tmp;
    tmp += payload;
    return publish(topic, (uint8_t *)tmp.c_str(), strlen(tmp.c_str()), 0, true);
}

uint8_t IntorobotClass::publish(const char *topic, unsigned short int payload)
{
    String tmp;
    tmp += payload;
    return publish(topic, (uint8_t *)tmp.c_str(), strlen(tmp.c_str()), 0, true);
}

uint8_t IntorobotClass::publish(const char *topic, int payload)
{
    String tmp;
    tmp += payload;
    return publish(topic, (uint8_t *)tmp.c_str(), strlen(tmp.c_str()), 0, true);
}

uint8_t IntorobotClass::publish(const char *topic, unsigned int payload)
{
    String tmp;
    tmp += payload;
    return publish(topic, (uint8_t *)tmp.c_str(), strlen(tmp.c_str()), 0, true);
}

uint8_t IntorobotClass::publish(const char *topic, long payload)
{
    String tmp;
    tmp += payload;
    return publish(topic, (uint8_t *)tmp.c_str(), strlen(tmp.c_str()), 0, true);
}

uint8_t IntorobotClass::publish(const char *topic, unsigned long payload)
{
    String tmp;
    tmp += payload;
    return publish(topic, (uint8_t *)tmp.c_str(), strlen(tmp.c_str()), 0, true);

}

uint8_t IntorobotClass::publish(const char *topic, float payload)
{
    char tmp[128];
    memset(tmp,0,sizeof(tmp));
    sprintf(tmp, "%.2f", payload);
    return IntoRobot.publish(topic, tmp);
}

uint8_t IntorobotClass::publish(const char *topic, double payload)
{
    char tmp[128];
    memset(tmp,0,sizeof(tmp));
    sprintf(tmp, "%.2f", payload);
    return IntoRobot.publish(topic, tmp);

}

uint8_t IntorobotClass::publish(const char *topic, String payload)
{
    return publish(topic, (uint8_t *)payload.c_str(), strlen(payload.c_str()), 0, true);
}

/*********************************************************************************
 *Function		:    uint8_t IntorobotClass::publish(const char *topic, char *payload)
 *Description	:    publish the topic
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
uint8_t IntorobotClass::publish(const char *topic, char *payload)
{
    return publish(topic, (uint8_t *)payload, strlen(payload), 0, true);
}

/*********************************************************************************
 *Function		:   uint8_t IntorobotClass::publish()
 *Description	:
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
uint8_t IntorobotClass::publish(const char* topic, uint8_t* payload, unsigned int plength)
{
    return publish(topic, payload, plength, 0, true);
}

/*********************************************************************************
 *Function		:     uint8_t IntorobotClass::publish()
 *Description	:
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
uint8_t IntorobotClass::publish(const char* topic, uint8_t* payload, unsigned int plength, uint8_t retained)
{
    return publish(topic, payload, plength, 0, retained);
}

/*********************************************************************************
 *Function		:      uint8_t IntorobotClass::publish()
 *Description	:
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
uint8_t IntorobotClass::publish(const char* topic, uint8_t* payload, unsigned int plength, uint8_t qos, uint8_t retained)
{
    String fulltopic;
    fill_mqtt_topic(fulltopic, topic, NULL);
    MO_DEBUG(("%s",fulltopic.c_str()));
    return ApiMqttClient.publish(fulltopic.c_str(), payload, plength, qos, retained);
}


/*********************************************************************************
  *Function		:   uint8_t IntorobotClass::subscribe()
  *Description	:
  *Input              :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
uint8_t IntorobotClass::subscribe(const char* topic, const char *device_id, WidgetBaseClass *pWidgetBase)
{
    return subscribe(topic, device_id, pWidgetBase, 0);
}

/*********************************************************************************
  *Function		:     uint8_t IntorobotClass::subscribe()
  *Description	:
  *Input              :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
uint8_t IntorobotClass::subscribe(const char* topic, const char *device_id, WidgetBaseClass *pWidgetBase, uint8_t qos)
{
    String fulltopic;

    addsubwcallback((char *)topic, (char *)device_id, pWidgetBase, qos);
    fill_mqtt_topic(fulltopic, topic, device_id);
    MO_DEBUG(("%s",fulltopic.c_str()));
    return ApiMqttClient.subscribe(fulltopic.c_str(), qos);
}


/*********************************************************************************
 *Function		:   uint8_t IntorobotClass::subscribe()
 *Description	:
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
uint8_t IntorobotClass::subscribe(const char* topic, const char *device_id, void (*callback)(uint8_t*, uint32_t))
{
    return subscribe(topic, device_id, callback, 0);
}

/*********************************************************************************
 *Function		:     uint8_t IntorobotClass::subscribe()
 *Description	:
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
uint8_t IntorobotClass::subscribe(const char* topic, const char *device_id, void (*callback)(uint8_t*, uint32_t), uint8_t qos)
{
    String fulltopic;
    addsubcallback((char *)topic, (char *)device_id, callback, qos);
    fill_mqtt_topic(fulltopic, topic, device_id);
    return ApiMqttClient.subscribe(fulltopic.c_str(), qos);
}

/*********************************************************************************
 *Function		:       uint8_t IntorobotClass::unsubscribe()
 *Description	:
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
uint8_t IntorobotClass::unsubscribe(const char *topic, const char *device_id)
{
    String fulltopic;

    delsubcallback((char *)topic, (char *)device_id);
    fill_mqtt_topic(fulltopic, topic, device_id);
    return ApiMqttClient.unsubscribe(fulltopic.c_str());
}

/*********************************************************************************
 *Function		:    int IntorobotClass::deviceInfo(char *product_id, char *device_id, char *access_token)
 *Description	:    get the device info
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
int IntorobotClass::deviceInfo(char *product_id, char *device_id, char *access_token, char *device_sn)
{
    if((product_id==NULL) || (device_id==NULL) || (access_token==NULL))
    {
        return false;
    }

    strcpy(device_id,intorobot_system_param.device_id);
    strcpy(access_token,intorobot_system_param.access_token);
    String serail_num = deviceSn();
    strcpy(device_sn, serail_num.c_str());
    return true;
}

/*********************************************************************************
 *Function		:     void IntorobotClass::syncTime(void)
 *Description	:     sync the board time through the intorobot system
 *Input              :     none
 *Output		:     none
 *Return		:     none
 *author		:     robot
 *date			:     2015-02-01
 *Others		:
 **********************************************************************************/
void IntorobotClass::syncTime(void)
{
    subscribe(INTOROBOT_MQTT_TIMETOPIC, NULL, syncTimeCallback);
}

/*********************************************************************************
 *Function		:   void IntorobotClass::process(void)
 *Description	:
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/

/*

  wifi断开查询周期1秒/wifi连接查询周期15秒

  wifi状态断开
  打开红蓝灯闪烁
  wifi断开标志
  平台断开标志

  wifi状态连接
  蓝灯闪烁
  wifi连接标志

  wifi状态连接
  蓝灯闪烁
  wifi连接标志

  如果wifi连接 且 平台断开
  前二十次快速连接/以后慢速连接

  重新连接平台

  重新连接平台


  如果wifi状态连接 且 平台连接
  执行MQTT loop

*/





void IntorobotClass::process(void)
{

#ifdef INTOROBOT_WLAN_ENABLE
    if(intorobot_cloud_connect_flag)	//需要连接平台(使用模式宏控制)
    {
        //wifi状态查询周期  断开1s  连接10s
        uint32_t wificheck_timer;
        if(intorobot_cloud_socketed_flag)
        {
            wificheck_timer = INTOROBOT_WIFICHECK_SUCC_PERIOD_MILLIS;
        }
        else
        {
            wificheck_timer = INTOROBOT_WIFICHECK_FAIL_PERIOD_MILLIS;
        }

        //查询wifi状态
        if(timerIsEnd(intorobot_wificheck_period_timer, wificheck_timer))
        {
            intorobot_wificheck_period_timer = timerGetId();
            if(WiFi.status())
            {
                //wifi状态置1  蓝灯闪烁
                //MO_DEBUG(("wifi status true"));
                if(!intorobot_cloud_socketed_flag)//保证执行一次
                {
                    system_rgb_blink(0, 0, 255, 1000);//蓝灯闪烁
                    intorobot_cloud_socketed_flag=1;	//网络连接标志
                    intorobot_mqttconnect_period_timer = timerGetId();
                }
            }
            else
            {
                //wifi状态置0	平台状态置0	绿灯闪烁
                MO_DEBUG(("wifi status false"));
                if(intorobot_cloud_socketed_flag)//保证执行一次
                {
                    system_rgb_blink(0, 255, 0, 1000);	//尝试联网绿灯闪烁
                    intorobot_cloud_socketed_flag=0;    //网络断开标志
                    intorobot_cloud_connected_flag=0;   //平台断开标志
                }
            }
        }

        //如果wifi连接      并且    平台断开  则重连
        if(intorobot_cloud_socketed_flag&&(!intorobot_cloud_connected_flag))
        {
            //前20次重连间隔1秒		后面15秒
            uint32_t mqttconnect_timer;
            if(intorobot_mqttconnect_short_to_long_count>=INTOROBOT_SHORT_TO_LONG_MAC_COUNT)
            {
                mqttconnect_timer = INTOROBOT_MQTTCONNECT_LONG_PERIOD_MILLIS;
            }
            else
            {
                mqttconnect_timer = INTOROBOT_MQTTCONNECT_SHORT_PERIOD_MILLIS;
            }

            //重连平台
            if(timerIsEnd(intorobot_mqttconnect_period_timer, mqttconnect_timer))
            {
                intorobot_mqttconnect_period_timer = timerGetId();

                if(intorobot_mqttconnect_short_to_long_count<INTOROBOT_SHORT_TO_LONG_MAC_COUNT)
                { intorobot_mqttconnect_short_to_long_count++; }

                MO_DEBUG(("cloud connecting"));
                String fulltopic, clientid;
                char temp[64];

                //断开重连
#if 0
                if(ApiMqttClient.connected())
                {
                    ApiMqttClient.disconnect();
                }
#endif

                ApiMqttClient.stop();

                //打印连接信息
                intorobot_info_debug();

                //信息填充
                fill_mqtt_topic(fulltopic, INTOROBOT_MQTT_WILLTOPIC, NULL);
                //set intorobot server
                if ((char)0x01 == intorobot_system_param.sv_select)
                { ApiMqttClient.setMqtt(intorobot_system_param.sv_domain, intorobot_system_param.sv_port); }
                else
                { ApiMqttClient.setMqtt(INTOROBOT_SERVER_DOMAIN, INTOROBOT_SERVER_PORT); }
                // mqtt connect
                memset(temp,0,sizeof(temp));
                clientid = clientId();
                strcpy(temp, clientid.c_str());
                MO_DEBUG(("clientid     =   %s", clientid.c_str()));

                //mqtt连接平台
                if(ApiMqttClient.connect(temp, intorobot_system_param.access_token, intorobot_system_param.device_id, fulltopic.c_str(), INTOROBOT_MQTT_WILLQOS, INTOROBOT_MQTT_WILLRETAIN, INTOROBOT_MQTT_WILLMESSAGE))
                {
                    MO_DEBUG(("cloud connected"));
                    memset(temp,0,sizeof(temp));
                    sprintf(temp,"{\"fw_ver\":\"%s\",\"sys_ver\":\"%s\"}", intorobot_system_param.fw_version, intorobot_system_param.net_version);
                    publish(INTOROBOT_MQTT_INFOTOPIC, (uint8_t*)temp, strlen(temp), true);

                    //升级标志清零(如果是升级重启)  并发送平台
                    /*if(firmwareupdate.st_firmware_isupdate())
                    {
                        MO_DEBUG(("stm32 firmware is update"));
                        firmwareupdate.st_firmware_clearupdate();
                        publish(INTOROBOT_MQTT_RESPONSE_TOPIC, (uint8_t *)INTOROBOT_MQTT_RESMES_ST_FWUPSUCC, strlen(INTOROBOT_MQTT_RESMES_ST_FWUPSUCC),false);
                    }*/
                    //复位 标志清零(如果是复位重启)  并上传给平台
                    if(firmwareupdate.st_system_isreset())
                    {
                        MO_DEBUG(("system is reset"));
                        firmwareupdate.st_system_clearreset();
                        publish(INTOROBOT_MQTT_RESPONSE_TOPIC, (uint8_t *)INTOROBOT_MQTT_RESMES_SRSETSUCC, strlen(INTOROBOT_MQTT_RESMES_SRSETSUCC),false);
                    }

                    //重新订阅
                    resubscribe();
                    intorobot_cloud_connected_flag=1;
                    system_rgb_blink(255, 255, 255, 2000);          //白灯闪烁

                    intorobot_mqttconnect_short_to_long_count=0;	//快速重连平台次数清零
                }

            }
        }

        //如果wifi信号正常	并且连接着平台  则执行MQTT loop
        if(intorobot_cloud_socketed_flag&&intorobot_cloud_connected_flag)
        {
            if(false == ApiMqttClient.loop())
            {
                //平台数据接收异常		平台状态置0		绿灯闪烁
                MO_DEBUG(("ApiMqttClient  false"));
                intorobot_cloud_connected_flag=0;
                system_rgb_blink(0, 0, 255, 1000);
                intorobot_mqttconnect_period_timer = timerGetId();
            }
            else
            {sendDebug();}	//发送IntoRobot.printf打印到平台
        }
    }
#endif
}

/*********************************************************************************
 *Function		:    void IntorobotClass::sendDebug(void)
 *Description	:    send debug info to the platform system
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
void IntorobotClass::sendDebug(void)
{
    uint32_t n;
    String s_debug_info="";

    for(n=0; (n<MQTT_MAX_PACKET_SIZE)&&(Debug_tx_buffer.tail!=Debug_tx_buffer.head); n++)
    {
        s_debug_info += (char)Debug_tx_buffer.buffer[Debug_tx_buffer.tail];
        Debug_tx_buffer.tail = (unsigned int)(Debug_tx_buffer.tail + 1) % Cloud_DEBUG_BUFFER_SIZE;
    }

    if(n)
    {
        publish(INTOROBOT_MQTT_SENDBUGTOPIC, (uint8_t *)s_debug_info.c_str(), n, false);
    }
}

/*********************************************************************************
 *Function		:    void IntorobotClass::receiveDebug(void)
 *Description	:    send debug info to the platform system
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
void IntorobotClass::receiveDebug(uint8_t *pIn, uint32_t len)
{
    uint32_t n;

    for( n=0; n<len; n++)
    {
        Debug_rx_buffer.buffer[Debug_rx_buffer.head] = pIn[n];
        Debug_rx_buffer.head++;
        /* To avoid buffer overflow */
        if(Debug_rx_buffer.head == Cloud_DEBUG_BUFFER_SIZE)
        {
            Debug_rx_buffer.head = 0;
        }
    }
}

/*********************************************************************************
 *Function		:   void IntorobotClass::fill_mqtt_topic(String &fulltopic, const char *topic, const char *device_id)
 *Description	:   fill mqtt topic
 *Input		:
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
void IntorobotClass::fill_mqtt_topic(String &fulltopic, const char *topic, const char *device_id)
{
    char temp[128];

    memset(temp,0,sizeof(temp));
    if(device_id == NULL)
    {sprintf(temp,"%s/%s/", INTOROBOT_API_VER, intorobot_system_param.device_id);}
    else
    {sprintf(temp,"%s/%s/", INTOROBOT_API_VER, device_id);}
    fulltopic=temp;
    fulltopic+=topic;
}

/*********************************************************************************
 *Function		:    size_t IntorobotClass::write(uint8_t byte)
 *Description	:
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
size_t IntorobotClass::write(uint8_t byte)
{
    Debug_tx_buffer.buffer[Debug_tx_buffer.head] = byte;
    Debug_tx_buffer.head++;
    /* To avoid buffer overflow */
    if(Debug_tx_buffer.head == Cloud_DEBUG_BUFFER_SIZE)
    {
        Debug_tx_buffer.head = 0;
    }
    return 1;
}

/*********************************************************************************
 *Function		:    int IntorobotClass::read(void)
 *Description	:
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
int IntorobotClass::read(void)
{
    // if the head isn't ahead of the tail, we don't have any characters
    if (Debug_rx_buffer.head == Debug_rx_buffer.tail)
    {
        return -1;
    }
    else
    {
        unsigned char c = Debug_rx_buffer.buffer[Debug_rx_buffer.tail];
        Debug_rx_buffer.tail = (unsigned int)(Debug_rx_buffer.tail + 1) % Cloud_DEBUG_BUFFER_SIZE;
        return c;
    }
}

/*********************************************************************************
 *Function		:    int IntorobotClass::available(void)
 *Description	:
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
int IntorobotClass::available(void)
{
    return (unsigned int)(Cloud_DEBUG_BUFFER_SIZE + Debug_rx_buffer.head - Debug_rx_buffer.tail) % Cloud_DEBUG_BUFFER_SIZE;
}

/*********************************************************************************
  *Function		:    String clientId(void)
  *Description	:    get a random clientId of 32 characters
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
String clientId(void)
{
    char charset[62] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                        'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
                        'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3',
                        '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D',
                        'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
                        'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                        'Y', 'Z'};
    String clientid = "";
    int randnum;

    randomSeed(millis());
    for (int i = 0; i < 32; i++) {
         randnum = random(0, 62);
         clientid.concat(charset[randnum]);
    }

    return clientid;
}

/*********************************************************************************
 *Function		:    String deviceSn(void)
 *Description	:    get the unique device serial number
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
//Device Serial
//#define         ID1          (0x1FFFF7E8)
//#define         ID2          (0x1FFFF7EC)
//#define         ID3          (0x1FFFF7F0)


#define         ID1          (0x1FFF7A10)
#define         ID2          (0x1FFF7A14)
#define         ID3          (0x1FFF7A18)



String deviceSn(void)
{
    String deviceID;
    char hex_digit;
    char id[12];

    memcpy(id, (char *)ID1, 12);
    for (int i = 0; i < 12; ++i)
    {
        hex_digit = 48 + (id[i] >> 4);
        if (57 < hex_digit)
        {
            hex_digit += 39;
        }
        deviceID.concat(hex_digit);

        hex_digit = 48 + (id[i] & 0xf);
        if (57 < hex_digit)
        {
            hex_digit += 39;
        }
        deviceID.concat(hex_digit);
    }
    return deviceID;
}

/*********************************************************************************
 *Function		:     CB getsubcallback(char * fulltopic)
 *Description	:
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
CB getsubcallback(char * fulltopic)
{
    char topictmp[128]={0};

    for (int i = 0 ; i < callbacklist.total_callbacks; i++)
    {
        memset(topictmp, 0, sizeof(topictmp));
        if(callbacklist.callbacknode[i].device_id == NULL)
        {sprintf(topictmp,"%s/%s/", INTOROBOT_API_VER, intorobot_system_param.device_id);}
        else
        {sprintf(topictmp,"%s/%s/", INTOROBOT_API_VER, callbacklist.callbacknode[i].device_id);}
        strcat(topictmp,callbacklist.callbacknode[i].topic);
        if (strcmp(fulltopic, topictmp) == 0)
        {
            return callbacklist.callbacknode[i].callback;
        }
    }
    return NULL;
}

/*********************************************************************************
 *Function		:     CB getsubcallback(char * fulltopic)
 *Description	:
 *Input		      :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
WidgetBaseClass *getsubwcallback(char * fulltopic)
{
    char topictmp[128]={0};

    for (int i = 0 ; i < callbacklist.total_wcallbacks; i++)
    {
        memset(topictmp, 0, sizeof(topictmp));
        if(callbacklist.wcallbacknode[i].device_id == NULL)
        {sprintf(topictmp,"%s/%s/", INTOROBOT_API_VER, intorobot_system_param.device_id);}
        else
        {sprintf(topictmp,"%s/%s/", INTOROBOT_API_VER, callbacklist.wcallbacknode[i].device_id);}
        strcat(topictmp,callbacklist.wcallbacknode[i].topic);
        if (strcmp(fulltopic, topictmp) == 0)
        {
            return callbacklist.wcallbacknode[i].pWidgetBase;
        }
    }
    return NULL;
}
/*********************************************************************************
 *Function		:    void addsubcallback()
 *Description	:
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
void addsubcallback(char *topic, char *device_id, void (*callback)(uint8_t*, uint32_t), uint8_t qos)
{
    int if_found_topic = 0;
    int i = 0;

    for (i = 0 ; i < callbacklist.total_callbacks; i++)
    {
        if ((topic == callbacklist.callbacknode[i].topic)&&(device_id == callbacklist.callbacknode[i].device_id))
        {
            if_found_topic = 1;
            break;
        }
    }

    if (if_found_topic)
    {
        callbacklist.callbacknode[i].callback = callback;
        callbacklist.callbacknode[i].qos = qos;
    }
    else
    {
        if (callbacklist.total_callbacks == MAX_CALLBACK_NUM)
        {
            return;
        }
        else
        {
            callbacklist.callbacknode[callbacklist.total_callbacks].callback = callback;
            callbacklist.callbacknode[callbacklist.total_callbacks].qos = qos;
            callbacklist.callbacknode[callbacklist.total_callbacks].topic = topic;
            callbacklist.callbacknode[callbacklist.total_callbacks].device_id= device_id;
            callbacklist.total_callbacks ++;
        }
    }
}

/*********************************************************************************
  *Function		:    void addsubwcallback()
  *Description	:
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
void addsubwcallback(char *topic, char *device_id, WidgetBaseClass *pWidgetBase, uint8_t qos)
{
    int if_found_topic = 0;
    int i = 0;

    for (i = 0 ; i < callbacklist.total_wcallbacks; i++)
    {
        if ((topic == callbacklist.wcallbacknode[i].topic)&&(device_id == callbacklist.wcallbacknode[i].device_id))
        {
            if_found_topic = 1;
            break;
        }
    }

    if (if_found_topic)
    {
        callbacklist.wcallbacknode[i].pWidgetBase = pWidgetBase;
        callbacklist.wcallbacknode[i].qos = qos;
    }
    else
    {
        if (callbacklist.total_wcallbacks == MAX_CALLBACK_NUM)
        {
            return;
        }
        else
        {
            callbacklist.wcallbacknode[callbacklist.total_wcallbacks].pWidgetBase = pWidgetBase;
            callbacklist.wcallbacknode[callbacklist.total_wcallbacks].qos = qos;
            callbacklist.wcallbacknode[callbacklist.total_wcallbacks].topic = topic;
            callbacklist.wcallbacknode[callbacklist.total_wcallbacks].device_id= device_id;
            callbacklist.total_wcallbacks ++;
        }
    }
}

/*********************************************************************************
 *Function		:    void delsubcallback(char * topic, char *device_id)
 *Description	:
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
void delsubcallback(char * topic, char *device_id)
{
    for (int i = 0 ; i < callbacklist.total_callbacks; i++)
    {
        if ((topic == callbacklist.callbacknode[i].topic) && (device_id == callbacklist.callbacknode[i].device_id))
        {
            memcpy(&callbacklist.callbacknode[i], &callbacklist.callbacknode[i+1], (callbacklist.total_callbacks - 1 - i) * sizeof(struct _callbacknode));
            memset(&callbacklist.callbacknode[callbacklist.total_callbacks-1], 0, sizeof(struct _callbacknode));
            callbacklist.total_callbacks--;
            return;
        }
    }

    for (int i = 0 ; i < callbacklist.total_wcallbacks; i++)
    {
        if ((topic == callbacklist.wcallbacknode[i].topic) && (device_id == callbacklist.wcallbacknode[i].device_id))
        {
            memcpy(&callbacklist.wcallbacknode[i], &callbacklist.wcallbacknode[i+1], (callbacklist.total_wcallbacks - 1 - i) * sizeof(struct _wcallbacknode));
            memset(&callbacklist.wcallbacknode[callbacklist.total_wcallbacks-1], 0, sizeof(struct _wcallbacknode));
            callbacklist.total_wcallbacks--;
            return;
        }
    }
}

/*********************************************************************************
 *Function		:       void resubscribe(void)
 *Description	:
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
void resubscribe(void)
{
    for (int i = 0 ; i < callbacklist.total_callbacks; i++)
    {
        IntoRobot.subscribe(callbacklist.callbacknode[i].topic, callbacklist.callbacknode[i].device_id,
                            callbacklist.callbacknode[i].callback, callbacklist.callbacknode[i].qos);
    }

    for (int i = 0 ; i < callbacklist.total_wcallbacks; i++)
    {
        IntoRobot.subscribe(callbacklist.wcallbacknode[i].topic, callbacklist.wcallbacknode[i].device_id,
                            callbacklist.wcallbacknode[i].pWidgetBase, callbacklist.wcallbacknode[i].qos);
    }
}

/*********************************************************************************
 *Function		:    void apiMqttClientCallBack(char *topic, uint8_t *payload, uint32_t length)
 *Description	:    the mqtt client callbck function
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
void apiMqttClientCallBack(char *topic, uint8_t *payload, uint32_t length)
{
    MO_DEBUG(("apiMqttClientCallBack"));
    uint8_t *pData = NULL;
    CB CallBack=getsubcallback(topic);
    if(CallBack!=NULL)
    {
        pData = (uint8_t *)pvPortMalloc(length+1);
        memset(pData, 0, length+1);
        memcpy(pData, payload, length);
        CallBack(pData,length);
        vPortFree(pData);
    }


    WidgetBaseClass *pWidgetBase=getsubwcallback(topic);
    if(pWidgetBase!=NULL)
    {
        pData = (uint8_t *)pvPortMalloc(length+1);
        memset(pData, 0, length+1);
        memcpy(pData, payload, length);
        pWidgetBase->widgetBaseCallBack(pData,length);
        vPortFree(pData);
    }
}

/*********************************************************************************
 *Function		:    void syncTimeCallback(uint8_t *payload, uint32_t len)
 *Description	:    the callback function of synctime
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
void syncTimeCallback(uint8_t *payload, uint32_t len)
{
    uint32_t n;
    String s_time="";

    MO_DEBUG(("default:SyncTimeCallback"));
    for(n=0; n<len; n++)
    {s_time+=(char)payload[n];}
    Time.setTime(atol(s_time.c_str())); // set the time of stm32
    //unsubscribe the system time topic
    IntoRobot.unsubscribe(INTOROBOT_MQTT_TIMETOPIC, "service");
}

/*********************************************************************************
 *Function		:   void stm32FwUpdateCallback(uint8_t *payload, uint32_t len)
 *Description	:   the callback function of firmware update
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
void stm32FwUpdateCallback(uint8_t *payload, uint32_t len)
{
    uint32_t n;
    String s_payload="", domain="", param="";
    aJsonClass aJson;

    MO_DEBUG(("default:FirmwareUpdateCallback"));

    system_rgb_blink(255, 255, 0, 50);
    for(n=0; n<len; n++)
    {s_payload+=(char)payload[n];}

    IntoRobot.publish(INTOROBOT_MQTT_RESPONSE_TOPIC, (uint8_t *)INTOROBOT_MQTT_RESMES_ST_FWUPREADY, strlen(INTOROBOT_MQTT_RESMES_ST_FWUPREADY),false);
    aJsonObject *root = aJson.parse((char *)s_payload.c_str());
    bool board_type_error = false;
    if (root != NULL)
    {
        /*aJsonObject* type_Object = aJson.getObjectItem(root, "board");
        MO_DEBUG(("board_type: %s", type_Object->valuestring));
        if( !strcmp(type_Object->valuestring, INTOROBOT_BOARD_TYPE1) || !strcmp(type_Object->valuestring, INTOROBOT_BOARD_TYPE2) )*/
        if (1)
        {
            board_type_error = false;
            aJsonObject* md5_Object = aJson.getObjectItem(root, "md5");
            if(md5_Object != NULL)
            {
                aJsonObject* dtoken_Object = aJson.getObjectItem(root, "dwn_token");
                if(dtoken_Object != NULL)
                {
                    //down the firmware
                    if ((char)0xff == intorobot_system_param.sv_select)
                    {
                        domain+=INTOROBOT_UPDATE_URL;
                    }
                    else
                    {
                        domain+=intorobot_system_param.dw_domain;
                    }
                    param+=INTOROBOT_FW_UPDATE_URL;
                    param+="?dwn_token=";
                    param+=dtoken_Object->valuestring;
                    MO_DEBUG(("md5=%s,url=%s\r\n", md5_Object->valuestring,param.c_str()));
                    if(true==firmwareupdate.st_firmware_down(domain.c_str(), param.c_str(),  md5_Object->valuestring))
                    {
                        IntoRobot.publish(INTOROBOT_MQTT_RESPONSE_TOPIC, (uint8_t *)INTOROBOT_MQTT_RESMES_ST_FWUPSUCC1, strlen(INTOROBOT_MQTT_RESMES_ST_FWUPSUCC1),false);
                        //IntoRobot.publish(INTOROBOT_MQTT_RESPONSE_TOPIC, (uint8_t *)INTOROBOT_MQTT_RESMES_ST_FWDOWNSUCC, strlen(INTOROBOT_MQTT_RESMES_ST_FWDOWNSUCC),false);
                        // Set boot_flag '4' for update the firmware
                        intorobot_boot_param.boot_flag = 4;
                        saveBootParams(&intorobot_boot_param);
                        if (root != NULL)
                        {
                            aJson.deleteItem(root);
                        }
                        // restart the stm32
                        mo_system_reboot_hal();
                        //HAL_NVIC_SystemReset();

                        // if(false==firmwareupdate.st_firmware_update())
                        // {
                        //     //delay(1200);
                        //     IntoRobot.publish(INTOROBOT_MQTT_RESPONSE_TOPIC, (uint8_t *)INTOROBOT_MQTT_RESMES_ST_FWUPFAIL, strlen(INTOROBOT_MQTT_RESMES_ST_FWUPFAIL),false);
                        // }
                        //return;
                    }
                }
            }
        }
        else
        {
            //board_type_error = true;
        }
    }
    if (root != NULL)
    {aJson.deleteItem(root);}
    if (board_type_error)
    {
        // board type error
        IntoRobot.publish(INTOROBOT_MQTT_RESPONSE_TOPIC, (uint8_t *)INTOROBOT_MQTT_RESMES_ST_FWUPTYPEEEOR, strlen(INTOROBOT_MQTT_RESMES_ST_FWUPTYPEEEOR),false);
    }
    else
    {
        //download fall
        IntoRobot.publish(INTOROBOT_MQTT_RESPONSE_TOPIC, (uint8_t *)INTOROBOT_MQTT_RESMES_ST_FWDOWNFAIL, strlen(INTOROBOT_MQTT_RESMES_ST_FWDOWNFAIL),false);
    }
    delay(500);
    IntoRobot.disconnect();
    IntoRobot.connect();
}

/*********************************************************************************
  *Function		:   void netUpdateCallback(uint8_t *payload, uint32_t len)
  *Description	:   the callback function of firmware update
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
void netUpdateCallback(uint8_t *payload, uint32_t len)
{
    u32 n;
    String s_payload="", domain="", param="";
    aJsonClass aJson;
    char temp[64],flag=0,result=1;
    bool board_type_error = false;

    MO_DEBUG(("default:netUpdateCallback"));

    for(n=0; n<len; n++)
    {s_payload+=(char)payload[n];}

    //MO_DEBUG("Payload: %s\n", s_payload.c_str());
    aJsonObject *root = aJson.parse((char *)s_payload.c_str());
    if(root == NULL)
    {flag=1;}
/*
    aJsonObject* type_Object = aJson.getObjectItem(root, "board");
    if( !strcmp(type_Object->valuestring, INTOROBOT_BOARD_TYPE1) || !strcmp(type_Object->valuestring, INTOROBOT_BOARD_TYPE2) )
    {
        board_type_error = false;
    }
    else
    {
        flag = 1;
        board_type_error = true;
    }
*/

    aJsonObject* sys_Ver_Object = aJson.getObjectItem(root, "sys_ver");
    if(sys_Ver_Object == NULL)
    {flag=1;}

    if(0==flag)
    {
        if ((char)0xff == intorobot_system_param.sv_select)
        {
            domain+=INTOROBOT_UPDATE_URL;
        }
        else
        {
            domain+=intorobot_system_param.dw_domain;
        }
        param+=INTOROBOT_OW_UPDATE_URL;
        param+=sys_Ver_Object->valuestring;
        //if( !strcmp(type_Object->valuestring, INTOROBOT_BOARD_TYPE1) || !strcmp(type_Object->valuestring, INTOROBOT_BOARD_TYPE2) )
        if(1)
        {
            if(strcmp(intorobot_system_param.net_version, sys_Ver_Object->valuestring))
            {
                if(true==firmwareupdate.net_firmware_down(domain.c_str(), param.c_str()))
                {
                    MO_DEBUG(("down success\r\n"));
                    delay(1000);//等待数据发送完毕
                    memset(temp,0,sizeof(temp));
                    sprintf(temp,"{\"status\":\"%s\"}", INTOROBOT_MQTT_RESMES_OW_FWUPROCESS);
                    IntoRobot.publish(INTOROBOT_MQTT_RESPONSE_JSON_TOPIC, (uint8_t*)temp, strlen(temp),false);
                    system_rgb_color(255, 255, 0);
                    firmwareupdate.net_firmware_update();
                    delay(20000);
                    mo_system_reboot_hal();
                    //系统将重启
                }
            }
            MO_DEBUG(("down fail\r\n"));
            result=1;//下载失败
        }
    }
    if (root != NULL)
    {aJson.deleteItem(root);}

    memset(temp,0,sizeof(temp));
    if (board_type_error)
    {
        // board type error
        sprintf(temp,"{\"status\":\"%s\"}", INTOROBOT_MQTT_RESMES_OW_FWUPTYPEEEOR);
    }
    else
    {
        memset(temp,0,sizeof(temp));
        if(0==result)
        {
            sprintf(temp,"{\"status\":\"%s\"}", INTOROBOT_MQTT_RESMES_OW_FWUPSUCC);
        }
        else if(1==result)
        {
            sprintf(temp,"{\"status\":\"%s\"}", INTOROBOT_MQTT_RESMES_OW_FWDOWNFAIL);
        }
        else
        {
            sprintf(temp,"{\"status\":\"%s\"}", INTOROBOT_MQTT_RESMES_OW_FWUPFAIL);
        }
    }
    IntoRobot.publish(INTOROBOT_MQTT_RESPONSE_JSON_TOPIC, (uint8_t*)temp, strlen(temp),false);
    //恢复三色灯颜色
    delay(500);
    //reconnect
    IntoRobot.disconnect();
    IntoRobot.connect();
}


/*********************************************************************************
 *Function		:    void tokenUpdateCallback(uint8_t *payload, uint32_t len)
 *Description	:    the callback function of token update
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
void tokenUpdateCallback(uint8_t *payload, uint32_t len)
{
    uint32_t n;
    String s_access_token;

    MO_DEBUG(("default:TokenUpdateCallback"));
    for(n=0; n<len; n++)
    {s_access_token+=(char)payload[n];}
    //send response topic
    IntoRobot.publish(INTOROBOT_MQTT_RESPONSE_TOPIC, (uint8_t *)INTOROBOT_MQTT_RESMES_TKUPSUCC, strlen(INTOROBOT_MQTT_RESMES_TKUPSUCC),false);
    //save access token
    MO_DEBUG(("before access_token = %s",intorobot_system_param.access_token));
    memcpy(intorobot_system_param.access_token, 0, sizeof(intorobot_system_param.access_token));
    memcpy(intorobot_system_param.access_token,s_access_token.c_str(),s_access_token.length()<48?s_access_token.length():48);
    MO_DEBUG(("after access_token = %s",intorobot_system_param.access_token));
    saveSystemParams(&intorobot_system_param);
    //delay 1000ms
    delay(1000);
    //reconnect
    IntoRobot.disconnect();
    IntoRobot.connect();
}

/*********************************************************************************
 *Function		:     void systemResetCallback(uint8_t *payload, uint32_t len)
 *Description	:     server control system reset
 *Input              :     payload:  the point of payload   len
                               len: the len of payload
                               *Output		:     none
                               *Return		:     none
                               *author		:     robin
                               *date			:     2015-03-27
                               *Others		:
                               **********************************************************************************/
void systemResetCallback(uint8_t *payload, uint32_t len)
{
    MO_DEBUG(("default:SystemResetCallback"));
    //system reset ready
    IntoRobot.publish(INTOROBOT_MQTT_RESPONSE_TOPIC, (uint8_t *)INTOROBOT_MQTT_RESMES_SRSETREADY, strlen(INTOROBOT_MQTT_RESMES_SRSETREADY),false);
    //system reset
    if(false==firmwareupdate.st_system_reset())
    {
        //delay(1200);
        IntoRobot.publish(INTOROBOT_MQTT_RESPONSE_TOPIC, (uint8_t *)INTOROBOT_MQTT_RESMES_SRSTTFAIL, strlen(INTOROBOT_MQTT_RESMES_SRSTTFAIL),false);
    }
}

/*********************************************************************************
 *Function		:     void systemDebugCallback(uint8_t *payload, uint32_t len)
 *Description	:     server control system reset
 *Input              :     payload:  the point of payload   len
                               len: the len of payload
                               *Output		:     none
                               *Return		:     none
                               *author		:     robin
                               *date			:     2015-03-27
                               *Others		:
                               **********************************************************************************/
void systemDebugCallback(uint8_t *payload, uint32_t len)
{
    MO_DEBUG(("default:SystemDebugCallback"));
    IntoRobot.receiveDebug(payload, len);
}

/*********************************************************************************
 *Function		:     bool system_rgb_off(void)
 *Description	:     system control rgb led off
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
bool system_rgb_off(void)
{
    if(RGB.controlled())
    {
        RGB.off();
        return true;
    }
    return false;
}

/*********************************************************************************
 *Function		:    bool system_rgb_color(uint8_t red, uint8_t green, uint8_t blue)
 *Description	:    system control rgb color
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
bool system_rgb_color(uint8_t red, uint8_t green, uint8_t blue)
{
    if(RGB.controlled())
    {
        RGB.color(red,green,blue);
        return true;
    }
    return false;
}

/*********************************************************************************
 *Function		:    bool system_rgb_blink(uint8_t red, uint8_t green, uint8_t blue, uint16_t period)
 *Description	:    system control system rgb blink
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
bool system_rgb_blink(uint8_t red, uint8_t green, uint8_t blue, uint16_t period)
{
    if(RGB.controlled())
    {
        RGB.blink(red,green,blue, period);
        return true;
    }
    return false;
}

/*********************************************************************************
 *Function		:   bool system_rgb_breath(uint8_t red, uint8_t green, uint8_t blue, uint16_t period)
 *Description	:   system control rgb led breath
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
bool system_rgb_breath(uint8_t red, uint8_t green, uint8_t blue, uint16_t period)
{
    if(RGB.controlled())
    {
        RGB.breath(red,green,blue, period);
        return true;
    }
    return false;
}

/*********************************************************************************
 *Function		:      void intorobot_deviceconfig(void)
 *Description	:      device config
 *Input              :      none
 *Output		:      none
 *Return		:      none
 *author		:      robot
 *date			:      2015-02-01
 *Others		:
 **********************************************************************************/
void intorobot_deviceconfig(void)
{
#ifdef INTOROBOT_WLAN_ENABLE
#ifdef INTOROBOT_CONFIG_ENABLE
    if(true == DeviceConfigUdp.isNeedDeviceConfig()) //获取配置状态
    {
        MO_DEBUG(("enter device config"));
        system_rgb_blink(255, 0, 0, 1000); //进入配置模式红灯闪烁
        DeviceConfigUsb.init();
        DeviceConfigUdp.init();     //建立ucp server

        while(1)
        {
            if(DeviceConfigUsb.process()||DeviceConfigUdp.process())        //有一个配置成功则完成
            {
                break;
            }
        }
        MO_DEBUG(("exit  device config"));
        DeviceConfigUdp.clearDeviceConfig();
        //firmwareupdate.st_system_reset();           //重启系统  不重启
    }
#endif
#endif
}

/*********************************************************************************
 *Function		:      void intorobot_deviceconfig(void)
 *Description	:      device config
 *Input              :      none
 *Output		:      none
 *Return		:      none
 *author		:      robot
 *date			:      2015-02-01
 *Others		:
 **********************************************************************************/
void intorobot_info_debug(void)
{
    MO_DEBUG(("----------------------device info------------------------"));
    MO_DEBUG(("access_token   = %s",intorobot_system_param.access_token));
    MO_DEBUG(("device_id      = %s",intorobot_system_param.device_id));
    MO_DEBUG(("timezone       = %f",intorobot_system_param.zone));


    MO_DEBUG(("at_mode        = %02x",intorobot_system_param.at_mode));
    MO_DEBUG(("FIRMWARE_LIB_VER   = %s",INTOROBOT_FIRMWARE_LIB_VER));

    MO_DEBUG(("----------------------service info------------------------"));
    MO_DEBUG(("sv_select       = %02x",intorobot_system_param.sv_select));
    if (intorobot_system_param.sv_select == (char)0x01)
    {
        MO_DEBUG(("service_domain   = %s", intorobot_system_param.sv_domain));
        MO_DEBUG(("service_port     = %d", intorobot_system_param.sv_port));
        MO_DEBUG(("download_domain  = %s", intorobot_system_param.dw_domain));
    }
    else
    {
        MO_DEBUG(("service_domain   = %s",INTOROBOT_SERVER_DOMAIN));
        MO_DEBUG(("service_port     = %d",INTOROBOT_SERVER_PORT));
        MO_DEBUG(("download_domain  = %s",INTOROBOT_UPDATE_URL));
    }
}

/*********************************************************************************
 *Function		:      void intorobot_loop(void)
 *Description	:
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
void intorobot_loop(void)
{
    if (System.mode() != MODE_MANUAL)
    {
        IntoRobot.process();
    }
}

/*********************************************************************************
 *Function		:       void intorobot_init(void)
 *Description	:
 *Input              :
 *Output		:
 *Return		:
 *author		:
 *date			:
 *Others		:
 **********************************************************************************/
void intorobot_init(void)
{
    memset(&callbacklist, 0, sizeof(struct _callbacklist));     //回调结构体清零

    loadBootParams(&intorobot_boot_param); //读取bootloader参数
    loadSystemParams(&intorobot_system_param); //读取系统参数
    if(intorobot_boot_param.initparam_flag == 1) //初始化参数 保留密钥
    {
        MO_DEBUG(("init fac\r\n"));
        initFacSystemParams(&intorobot_system_param);
    }
    else if(intorobot_boot_param.initparam_flag == 2) //初始化所有参数
    {
        MO_DEBUG(("init all\r\n"));
        initSystemParams(&intorobot_system_param);
    }
    if(intorobot_boot_param.initparam_flag != 0) //初始化参数 保留密钥
    {
        intorobot_boot_param.initparam_flag = 0;
        saveBootParams(&intorobot_boot_param); //读取bootloader参数
    }
    //保存固件版本号
    if(strcmp(INTOROBOT_FIRMWARE_LIB_VER,intorobot_system_param.fw_version))
    {
        memset(intorobot_system_param.fw_version, 0, sizeof(intorobot_system_param.fw_version));
        strcpy(intorobot_system_param.fw_version, INTOROBOT_FIRMWARE_LIB_VER);
        saveSystemParams(&intorobot_system_param);
    }
    //保存net程序版本号
    char sys_ver[32] = {0};
    if(firmwareupdate.net_firmware_version(sys_ver))
    {
        if(strcmp(sys_ver,intorobot_system_param.net_version))
        {
            memset(intorobot_system_param.net_version, 0, sizeof(intorobot_system_param.net_version));
            strcpy(intorobot_system_param.net_version, sys_ver);
            saveSystemParams(&intorobot_system_param);
        }
    }
    //open baudrate 115200
    SerialUSB.begin();  //暂时不支持
    while(!SerialUSB);

    //set time zone
    Time.zone(intorobot_system_param.zone);  //设置时间偏移   时区

    //device config
    if(intorobot_cloud_connect_flag)  //连接平台模式
    {
        intorobot_deviceconfig();           //配置模式 (暂时直接返回)
        system_rgb_blink(0, 255, 0, 1000);  //连接平台模式绿灯闪烁
        intorobot_wificheck_period_timer = timerGetId();
    }
    else
    {
        system_rgb_blink(0, 255, 255, 2000);
    } //脱机模式  浅蓝闪烁

    //default subscribe topic
    if (System.mode() != MODE_MANUAL)       //平台模式  订阅5个回调	(只是填充入结构体并未push的平台)
    {
        IntoRobot.subscribe(INTOROBOT_MQTT_ST_UPDATETOPIC, NULL, stm32FwUpdateCallback);          //固件升级
        IntoRobot.subscribe(INTOROBOT_MQTT_TIMETOPIC, "service", syncTimeCallback);               //同步时间
        IntoRobot.subscribe(INTOROBOT_MQTT_TOKENTOPIC, NULL, tokenUpdateCallback);                //更新access token
        IntoRobot.subscribe(INTOROBOT_MQTT_RESETTOPIC, NULL, systemResetCallback);              //stm32重启
        IntoRobot.subscribe(INTOROBOT_MQTT_RECDEBUGTOPIC, NULL, systemDebugCallback);           //从平台获取调试信息
        IntoRobot.subscribe(INTOROBOT_MQTT_OW_UPDATETOPIC, NULL, netUpdateCallback);//add chenkaiyao 20160326
    }

}

void enter_dfu(bool persist)
{
    // true  - DFU mode persist if firmware upgrade is not completed
    // false - Briefly enter DFU bootloader mode (works with latest bootloader only )
    //         Subsequent reset or power off-on will execute normal firmware
    if (persist)
    {
        intorobot_boot_param.boot_flag=6;
		saveBootParams(&intorobot_boot_param);
    }
    else
    {
        HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR1, 0x7DEA);
    }
    HAL_NVIC_SystemReset();
}

static osThreadId handle_intorobot_loop;			//intorobot_loop;主任务句柄

extern void mo_setup_loop_init();


void task_mo_intorobot_loop(void const *argument)
{
    system_rgb_color( 0, 255, 255);
    //delay(1000);
    //wifi硬件初始化
    mo_drv_wifi_set_default_mode();

    intorobot_init();

    //创建setup loop任务
    mo_setup_loop_init();

    while(1)
    {
        intorobot_loop();
    }

}

extern void mo_RGBClass_hal();

void mo_intorobot_init()
{

#ifdef WIFI_HARDWARE_ENABLE
    osThreadDef(INB_LOOP, task_mo_intorobot_loop, osPriorityNormal, 0, 1024);
    handle_intorobot_loop = osThreadCreate(osThread(INB_LOOP),NULL);
    MO_ASSERT((handle_intorobot_loop!=NULL));
#else
    //创建setup loop任务
    mo_setup_loop_init();
#endif
    mo_RGBClass_hal();
}
