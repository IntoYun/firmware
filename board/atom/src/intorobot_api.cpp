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
#include "openwrt_pin.h"


//firmware  info
#define INTOROBOT_API_VER       "v1"

//firmware update address
#define INTOROBOT_FW_UPDATE_URL "/v1/bins/"
#define INTOROBOT_OW_UPDATE_URL "/downloads/atom/"
//sevice info
#define INTOROBOT_UPDATE_URL "www.intorobot.com"
#define INTOROBOT_SERVER_DOMAIN "iot.intorobot.com"
#define INTOROBOT_SERVER_PORT   1883


volatile uint8_t intorobot_cloud_connect_flag = 1;     //default connect intorobot cloud
volatile uint8_t intorobot_cloud_socketed_flag = 0;    //socket is not connected
volatile uint8_t intorobot_cloud_connected_flag = 0;   //socket is connected  but  intorobot cloud is not connected
volatile system_tick_t intorobot_mqttconnect_period_timer;            //mqtt connect period timer
volatile system_tick_t intorobot_mqttconnect_count;                   //mqtt connect count
volatile system_tick_t intorobot_mqttconnect_short_to_long_count;     //short to long count


volatile system_tick_t intorobot_wificheck_period_timer;       //wifi status check period timer



struct _callbacklist callbacklist;
IntorobotClass IntoRobot;
SystemClass System;

System_Mode_TypeDef SystemClass::_mode = MODE_AUTOMATIC;
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
    intorobot_cloud_connected_flag = 0;
    intorobot_cloud_connect_flag = 0;
}
uint8_t IntorobotClass::publish(const char *topic, bool payload)
{
    return publish(topic, (uint8_t)payload);
}

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
	IntoRobot.publish(topic, tmp);
}

uint8_t IntorobotClass::publish(const char *topic, double payload)
{
	char tmp[128];
	memset(tmp,0,sizeof(tmp));
	sprintf(tmp, "%.2f", payload);
	IntoRobot.publish(topic, tmp);

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
    DEBUG("%s",fulltopic.c_str());
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
    DEBUG("%s",fulltopic.c_str());
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
    DEBUG("%s",fulltopic.c_str());
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

    strcpy(product_id,intorobot_system_param.product_id);
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
void IntorobotClass::process(void)
{
#ifdef INTOROBOT_WLAN_ENABLE
    if(intorobot_cloud_connect_flag)
    {
        u32 wificheck_timer = INTOROBOT_WIFICHECK_FAIL_PERIOD_MILLIS;
        if(intorobot_cloud_socketed_flag)
        {wificheck_timer = INTOROBOT_WIFICHECK_SUCC_PERIOD_MILLIS;}
        //check wifi status
        if(timerIsEnd(intorobot_wificheck_period_timer, wificheck_timer))
        {
            intorobot_wificheck_period_timer = timerGetId();
            if(WiFi.status())
            {
                DEBUG("wifi status true");
                if(!intorobot_cloud_socketed_flag)
                {
                    system_rgb_blink(0, 0, 255, 1000);//plan to connect intorobot cloud. rgb blink blue.
                    intorobot_cloud_socketed_flag=1;
                    intorobot_mqttconnect_period_timer = timerGetId();
                }
            }
            else
            {
                DEBUG("wifi status false");
                if(intorobot_cloud_socketed_flag)
                {
                    system_rgb_blink(0, 255, 0, 1000);//plan to connect internet. rgb blink green.
                    intorobot_cloud_socketed_flag=0;
                    intorobot_cloud_connected_flag=0;
                }
            }
        }

        //check  intorobot Cloud is connected
        if(intorobot_cloud_socketed_flag&&(!intorobot_cloud_connected_flag))
        {
            u32 mqttconnect_timer = INTOROBOT_MQTTCONNECT_SHORT_PERIOD_MILLIS;
            if(intorobot_mqttconnect_short_to_long_count>=INTOROBOT_SHORT_TO_LONG_MAC_COUNT)
            {mqttconnect_timer = INTOROBOT_MQTTCONNECT_LONG_PERIOD_MILLIS;}

            if(timerIsEnd(intorobot_mqttconnect_period_timer, mqttconnect_timer))
            {
                DEBUG("cloud connecting");
                intorobot_mqttconnect_period_timer = timerGetId();

                if(intorobot_mqttconnect_short_to_long_count<INTOROBOT_SHORT_TO_LONG_MAC_COUNT)
                {intorobot_mqttconnect_short_to_long_count++;}

                DEBUG("cloud connecting");
                String fulltopic, clientid;
                char temp[64];

                //disconnect intorobot cloud
                if(ApiMqttClient.connected())
                {
                    ApiMqttClient.disconnect();
                }
                intorobot_info_debug();
                //connect intorobot cloud
                fill_mqtt_topic(fulltopic, INTOROBOT_MQTT_WILLTOPIC, NULL);
                //set intorobot server
                if ((char)0xff == intorobot_system_param.sv_select)
                { ApiMqttClient.setMqtt(INTOROBOT_SERVER_DOMAIN, INTOROBOT_SERVER_PORT); }
                else
                { ApiMqttClient.setMqtt(intorobot_system_param.sv_domain, intorobot_system_param.sv_port); }
                //mqtt connect
                memset(temp,0,sizeof(temp));
                clientid = clientId();
                strcpy(temp, clientid.c_str());
                DEBUG("clientid     =   %s", clientid.c_str());
                if(ApiMqttClient.connect(temp, intorobot_system_param.access_token, intorobot_system_param.device_id, fulltopic.c_str(), INTOROBOT_MQTT_WILLQOS, INTOROBOT_MQTT_WILLRETAIN, INTOROBOT_MQTT_WILLMESSAGE))
                {
                    DEBUG("cloud connected");
                    char sys_ver[32]="0.0.0.0";
                    firmwareupdate.openwrt_firmware_version(sys_ver);
                    DEBUG("sys_version  =  %s", sys_ver);
                    //publish  device info
                    memset(temp,0,sizeof(temp));
                    sprintf(temp,"{\"fw_ver\":\"%s\",\"sys_ver\":\"%s\"}", INTOROBOT_FIRMWARE_LIB_VER, sys_ver);
                    publish(INTOROBOT_MQTT_INFOTOPIC, (uint8_t*)temp, strlen(temp),true);
                    //stm32 firmware update
                    if(firmwareupdate.st_firmware_isupdate())
                    {
                        DEBUG("stm32 firmware is update");
                        firmwareupdate.st_firmware_clearupdate();
                        publish(INTOROBOT_MQTT_RESPONSE_TOPIC, (uint8_t *)INTOROBOT_MQTT_RESMES_ST_FWUPSUCC, strlen(INTOROBOT_MQTT_RESMES_ST_FWUPSUCC),false);
                    }
                    //server control system reset
                    if(firmwareupdate.st_system_isreset())
                    {
                        DEBUG("system is reset");
                        firmwareupdate.st_system_clearreset();
                        publish(INTOROBOT_MQTT_RESPONSE_TOPIC, (uint8_t *)INTOROBOT_MQTT_RESMES_SRSETSUCC, strlen(INTOROBOT_MQTT_RESMES_SRSETSUCC),false);
                    }
                    //subscibe topic
                    resubscribe();
                    intorobot_cloud_connected_flag=1;
                    system_rgb_blink(255, 255, 255, 2000);
                }
            }
        }

        //intorobot cloud process
        if(intorobot_cloud_socketed_flag&&intorobot_cloud_connected_flag)
        {
            if(false == ApiMqttClient.loop())
            {
                DEBUG("ApiMqttClient  false");
                intorobot_cloud_connected_flag=0;
                system_rgb_blink(0, 0, 255, 1000);
                intorobot_mqttconnect_period_timer = timerGetId();
            }
            else
            {sendDebug();}
        }
    }
#endif
}

/*********************************************************************************
  *Function		:    void IntorobotClass::sendDebug(void)
  *Description	:    send debug info to the platform system
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
void IntorobotClass::sendDebug(void)
{
    u32 n;
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
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
void IntorobotClass::receiveDebug(uint8_t *pIn, uint32_t len)
{
    u32 n;

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
  *Input		      :
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
  *Input		      :
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
  *Input		      :
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
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
String deviceSn(void)
{
    char hex_digit;
    char id[12];

    String deviceID = INTOROBOT_BOARD_TYPE1;
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
  *Input		      :
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
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
void addsubcallback(char *topic, char *device_id, void (*callback)(uint8_t *payload, uint32_t len), uint8_t qos)
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
  *Function		:    void addsubcallback()
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
  *Input		      :
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
  *Input		      :
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
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
void apiMqttClientCallBack(char *topic, uint8_t *payload, uint32_t length)
{
    DEBUG("apiMqttClientCallBack");
    uint8_t *pData=NULL;
    CB CallBack=getsubcallback(topic);
    if(CallBack!=NULL)
    {
        pData = (uint8_t *)malloc(length+1);
        memset(pData, 0, length+1);
        memcpy(pData, payload, length);
        CallBack(pData,length);
        free(pData);
    }
    WidgetBaseClass *pWidgetBase=getsubwcallback(topic);
    if(pWidgetBase!=NULL)
    {
        pData = (uint8_t *)malloc(length+1);
        memset(pData, 0, length+1);
        memcpy(pData, payload, length);
        pWidgetBase->widgetBaseCallBack(pData,length);
        free(pData);
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
    u32 n;
    String s_time="";

    DEBUG("default:SyncTimeCallback");
    for(n=0; n<len; n++)
    {s_time+=(char)payload[n];}
    Time.setTime(atol(s_time.c_str())); // set the time of stm32
    //unsubscribe the system time topic
    IntoRobot.unsubscribe(INTOROBOT_MQTT_TIMETOPIC, "service");
}

/*********************************************************************************
  *Function		:   void stm32FwUpdateCallback(uint8_t *payload, uint32_t len)
  *Description	:   the callback function of firmware update
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
void stm32FwUpdateCallback(uint8_t *payload, uint32_t len)
{
    u32 n;
    String s_payload="", url="";
    aJsonClass aJson;
    char flag=0;
    bool board_type_error = false;

    DEBUG("default:FirmwareUpdateCallback");

    for(n=0; n<len; n++)
    {s_payload+=(char)payload[n];}

    DEBUG("Payload: %s\n", s_payload.c_str());
    IntoRobot.publish(INTOROBOT_MQTT_RESPONSE_TOPIC, (uint8_t *)INTOROBOT_MQTT_RESMES_ST_FWUPREADY, strlen(INTOROBOT_MQTT_RESMES_ST_FWUPREADY),false);
    aJsonObject *root = aJson.parse((char *)s_payload.c_str());
    if (root == NULL)
    {flag=1;}
    aJsonObject* md5_Object = aJson.getObjectItem(root, "md5");
    if(md5_Object == NULL)
    {flag=1;}
    aJsonObject* dtoken_Object = aJson.getObjectItem(root, "dwn_token");
    if(dtoken_Object == NULL)
    {flag=1;}
/*
    aJsonObject* type_Object = aJson.getObjectItem(root, "board");

    if( !strcmp(type_Object->valuestring, INTOROBOT_BOARD_TYPE1) || !strcmp(type_Object->valuestring, INTOROBOT_BOARD_TYPE2) )
    {
        flag = 0;
        board_type_error = false;
    }
    else
    {
        flag = 1;
        board_type_error = true;
    }
*/

    if(0==flag)
    {
        if ((char)0xff == intorobot_system_param.sv_select)
        {
            url+=INTOROBOT_UPDATE_URL;
        }
        else
        {
            url+=intorobot_system_param.dw_domain;
        }
        url+=INTOROBOT_FW_UPDATE_URL;
        url+="?dwn_token=";
        url+=dtoken_Object->valuestring;
#if 0
        DEBUG("type=%s,md5=%s,url=%s\r\n",type_Object->valuestring, md5_Object->valuestring,url.c_str());
#else
        DEBUG("md5=%s,url=%s\r\n", md5_Object->valuestring,url.c_str());
#endif
        if(true==firmwareupdate.st_firmware_down(url.c_str(),  md5_Object->valuestring))
        {
            IntoRobot.publish(INTOROBOT_MQTT_RESPONSE_TOPIC, (uint8_t *)INTOROBOT_MQTT_RESMES_ST_FWDOWNSUCC, strlen(INTOROBOT_MQTT_RESMES_ST_FWDOWNSUCC),false);
            //update firmware
            firmwareupdate.st_firmware_update();
            //正常情况此处重启了
        }
        else
        {
            IntoRobot.publish(INTOROBOT_MQTT_RESPONSE_TOPIC, (uint8_t *)INTOROBOT_MQTT_RESMES_ST_FWDOWNFAIL, strlen(INTOROBOT_MQTT_RESMES_ST_FWDOWNFAIL),false);
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

    //恢复三色灯颜色
    delay(500);
    //reconnect
    IntoRobot.disconnect();
    IntoRobot.connect();
}

/*********************************************************************************
  *Function		:   void openwrtUpdateCallback(uint8_t *payload, uint32_t len)
  *Description	:   the callback function of firmware update
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
void openwrtUpdateCallback(uint8_t *payload, uint32_t len)
{
    u32 n;
    String s_payload="", url="";
    aJsonClass aJson;
    char temp[64],flag=0,result=1;
    bool board_type_error = false;

    DEBUG("default:openwrtUpdateCallback");

    for(n=0; n<len; n++)
    {s_payload+=(char)payload[n];}

    DEBUG("Payload: %s\n", s_payload.c_str());
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
            url+=INTOROBOT_UPDATE_URL;
        }
        else
        {
            url+=intorobot_system_param.dw_domain;
        }
        url+=INTOROBOT_OW_UPDATE_URL;
        url+=sys_Ver_Object->valuestring;
        /*
        if( !strcmp(type_Object->valuestring, INTOROBOT_BOARD_TYPE1) || !strcmp(type_Object->valuestring, INTOROBOT_BOARD_TYPE2) )
        */
        if (1)
        {
            DEBUG("type=%s,sys_ver=%s,url=%s\r\n",type_Object->valuestring,sys_Ver_Object->valuestring,url.c_str());
            if(true==firmwareupdate.openwrt_firmware_down(sys_Ver_Object->valuestring, url.c_str()))
            {
                DEBUG("down success\r\n");
                memset(temp,0,sizeof(temp));
                sprintf(temp,"{\"status\":\"%s\"}", INTOROBOT_MQTT_RESMES_OW_FWUPROCESS);
                IntoRobot.publish(INTOROBOT_MQTT_RESPONSE_JSON_TOPIC, (uint8_t*)temp, strlen(temp),false);
                delay(100);//等待数据发送完毕
                //update firmware
                firmwareupdate.openwrt_firmware_update();
                delay(1000);
                firmwareupdate.openwrt_reboot();
                //系统将重启
            }
            else
            {
                DEBUG("down fail\r\n");
                result=1;//下载失败
            }
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
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
void tokenUpdateCallback(uint8_t *payload, uint32_t len)
{
    u32 n;
    String s_access_token;

    DEBUG("default:TokenUpdateCallback");
    for(n=0; n<len; n++)
    {s_access_token+=(char)payload[n];}
    //send response topic
    IntoRobot.publish(INTOROBOT_MQTT_RESPONSE_TOPIC, (uint8_t *)INTOROBOT_MQTT_RESMES_TKUPSUCC, strlen(INTOROBOT_MQTT_RESMES_TKUPSUCC),false);
    //save access token
    DEBUG("before access_token = %s",intorobot_system_param.access_token);
    memset(intorobot_system_param.access_token, 0, sizeof(intorobot_system_param.access_token));
    memcpy(intorobot_system_param.access_token,s_access_token.c_str(),s_access_token.length()<48?s_access_token.length():48);
    DEBUG("after access_token = %s",intorobot_system_param.access_token);
    writeSystemParam(&intorobot_system_param);
    //delay 1000ms
    delay(1000);
    //reconnect
    IntoRobot.disconnect();
    IntoRobot.connect();
}

/*********************************************************************************
  *Function		:     bool start_imlink_config(void)
  *Description	:
  *Input		      :
  *Output		:     none
  *Return		:     none
  *author		:     robin
  *date			:     2015-03-27
  *Others		:
**********************************************************************************/
bool start_imlink_config(void)
{
    int res;
    Process Proc;
    Proc.begin("imlink_config");
    Proc.addParameter("SET");
    DEBUG("start imlink config");
    res = Proc.run();
    if(res == 0)
    {
        return true;
    }
    return false;
}
/*********************************************************************************
  *Function		:     bool end_imlink_config(void)
  *Description	:
  *Input		      :
  *Output		:     none
  *Return		:     none
  *author		:     robin
  *date			:     2015-03-27
  *Others		:
**********************************************************************************/
bool end_imlink_config(void)
{
    int res;
    Process Proc;
    Proc.begin("sc_config_kill");
    res = Proc.run();
    if(res == 0)
    {
        return true;
    }
    return false;
}

/*********************************************************************************
  *Function		:     void systemResetCallback(uint8_t *payload, uint32_t len)
  *Description	:     server control system reset
  *Input		      :     payload:  the point of payload   len
                               len: the len of payload
  *Output		:     none
  *Return		:     none
  *author		:     robin
  *date			:     2015-03-27
  *Others		:
**********************************************************************************/
void systemResetCallback(uint8_t *payload, uint32_t len)
{
    DEBUG("default:SystemResetCallback");
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
  *Input		      :     payload:  the point of payload   len
                               len: the len of payload
  *Output		:     none
  *Return		:     none
  *author		:     robin
  *date			:     2015-03-27
  *Others		:
**********************************************************************************/
void systemDebugCallback(uint8_t *payload, uint32_t len)
{
    DEBUG("default:SystemDebugCallback");
    IntoRobot.receiveDebug(payload, len);
}

/*********************************************************************************
  *Function		:     bool system_rgb_off(void)
  *Description	:     system control rgb led off
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
bool system_rgb_off(void)
{
    if(!RGB.controlled())
    {
        Process Proc;
        Proc.begin("rgb_control");
        Proc.addParameter("kill");
        Proc.run();
        return true;
    }
    return false;
}

/*********************************************************************************
  *Function		:    bool system_rgb_color(uint8_t red, uint8_t green, uint8_t blue)
  *Description	:    system control rgb color
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
bool system_rgb_color(uint8_t red, uint8_t green, uint8_t blue)
{
    if(!RGB.controlled())
    {
        Process Proc;
        Proc.begin("rgb_control");
        Proc.addParameter("color");
        Proc.addParameter(String(red));
        Proc.addParameter(String(green));
        Proc.addParameter(String(blue));
        Proc.run();
        return true;
    }
    return false;
}

/*********************************************************************************
  *Function		:    bool system_rgb_blink(uint8_t red, uint8_t green, uint8_t blue, uint16_t period)
  *Description	:    system control system rgb blink
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
bool system_rgb_blink(uint8_t red, uint8_t green, uint8_t blue, uint16_t period)
{
    if(!RGB.controlled())
    {
        Process Proc;
        Proc.begin("rgb_control");
        Proc.addParameter("blink");
        Proc.addParameter(String(red));
        Proc.addParameter(String(green));
        Proc.addParameter(String(blue));
        Proc.addParameter(String(period));
        Proc.run();
        return true;
    }
    return false;
}

/*********************************************************************************
  *Function		:   bool system_rgb_breath(uint8_t red, uint8_t green, uint8_t blue, uint16_t period)
  *Description	:   system control rgb led breath
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
bool system_rgb_breath(uint8_t red, uint8_t green, uint8_t blue, uint16_t period)
{
    if(!RGB.controlled())
    {
        Process Proc;
        Proc.begin("rgb_control");
        Proc.addParameter("breath");
        Proc.addParameter(String(red));
        Proc.addParameter(String(green));
        Proc.addParameter(String(blue));
        Proc.addParameter(String(period));
        Proc.run();
        return true;
    }
    return false;
}

/*********************************************************************************
  *Function		:      void intorobot_deviceconfig(void)
  *Description	:      device config
  *Input		      :      none
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

    if(true == DeviceConfigTcp.isNeedDeviceConfig()) //enter device config
    {
        DEBUG("enter device config");
        system_rgb_blink(255, 0, 0, 1000); //enter device config, rgb blink red
        DeviceConfigUsb.init();
        DeviceConfigTcp.init();
        DeviceConfigUdp.init();

        while(1)
        {
            if(DeviceConfigUsb.process()||DeviceConfigTcp.process()||DeviceConfigUdp.process())
            {
                break;
            }
        }
        DEBUG("exit device config");
        DeviceConfigTcp.setDeviceConfig();
        //firmwareupdate.st_system_reset();
    }
#endif
#endif
}

/*********************************************************************************
  *Function		:      void intorobot_info_reset(void)
  *Description	:      device info reset
  *Input		      :      none
  *Output		:      none
  *Return		:      none
  *author		:      robot
  *date			:      2015-02-01
  *Others		:
**********************************************************************************/
void intorobot_info_reset(void)
{
    readSystemParam(&intorobot_system_param);   //read intorobot_system_param
    memset(&intorobot_system_param,0,sizeof(intorobot_system_param));
    memcpy(intorobot_system_param.product_id, "553c6699084d55640e332113", 24);
    memcpy(intorobot_system_param.access_token, "FFFFFFFFFFFFFFFFFFFFFFFF", 24);
    memcpy(intorobot_system_param.device_id, "FFFFFFFFFFFFFFFFFFFFFFFF", 24);
    memcpy(intorobot_system_param.server, "CN", 2);
    intorobot_system_param.zone=8;
    writeSystemParam(&intorobot_system_param);
}

/*********************************************************************************
  *Function		:      void intorobot_deviceconfig(void)
  *Description	:      device config
  *Input		      :      none
  *Output		:      none
  *Return		:      none
  *author		:      robot
  *date			:      2015-02-01
  *Others		:
**********************************************************************************/
void intorobot_info_debug(void)
{
    DEBUG("----------------------device info------------------------");
    DEBUG("product_id     = %s",intorobot_system_param.product_id);
    DEBUG("access_token   = %s", intorobot_system_param.access_token);
    DEBUG("device_id      = %s",intorobot_system_param.device_id);
    DEBUG("server         = %s",intorobot_system_param.server);
    DEBUG("timezone       = %f",intorobot_system_param.zone);

    DEBUG("at_mode        = %02x",intorobot_system_param.at_mode);
    DEBUG("FIRMWARE_LIB_VER   = %s",INTOROBOT_FIRMWARE_LIB_VER);

    DEBUG("----------------------service info------------------------");
    DEBUG("sv_select       = %02x",intorobot_system_param.sv_select);
    if (intorobot_system_param.sv_select == (char)0xff) {
        DEBUG("service_domain   = %s",INTOROBOT_SERVER_DOMAIN);
        DEBUG("service_port     = %d",INTOROBOT_SERVER_PORT);
        DEBUG("download_domain  = %s",INTOROBOT_UPDATE_URL);
    } else {
        DEBUG("service_domain   = %s", intorobot_system_param.sv_domain);
        DEBUG("service_port     = %d", intorobot_system_param.sv_port);
        DEBUG("download_domain  = %s", intorobot_system_param.dw_domain);
    }
}

/*********************************************************************************
  *Function		:      void intorobot_loop(void)
  *Description	:
  *Input		      :
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
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
void intorobot_init(void)
{
    memset(&callbacklist, 0, sizeof(struct _callbacklist));

    //read intorobot_system_param
    readSystemParam(&intorobot_system_param);
    //open baudrate 115200
    SerialUSB.begin();
    while(!SerialUSB);
    //open bridge
    DEBUG("Bridge Connecting");
    Bridge.begin();
    DEBUG("Bridge Connected");
    //init openwrt pin
    OpenwrtPin.openwrt_pininit();
    //set time zone
    Time.zone(intorobot_system_param.zone);
    //device config
    if(intorobot_cloud_connect_flag)
    {
        intorobot_deviceconfig();
        system_rgb_blink(0, 255, 0, 1000);  //plan to connect intorobot cloud. rgb blink green.
        intorobot_wificheck_period_timer = timerGetId();
    }
    else
    {system_rgb_blink(0, 255, 255, 2000);} //not need to connect internet. rgb breath blue.

    //default subscribe topic
    if (System.mode() != MODE_MANUAL)
    {
        IntoRobot.subscribe(INTOROBOT_MQTT_ST_UPDATETOPIC, NULL, stm32FwUpdateCallback);
        IntoRobot.subscribe(INTOROBOT_MQTT_TIMETOPIC, "service", syncTimeCallback);
        IntoRobot.subscribe(INTOROBOT_MQTT_TOKENTOPIC, NULL, tokenUpdateCallback);
        IntoRobot.subscribe(INTOROBOT_MQTT_RESETTOPIC, NULL, systemResetCallback);
        IntoRobot.subscribe(INTOROBOT_MQTT_RECDEBUGTOPIC, NULL, systemDebugCallback);
        IntoRobot.subscribe(INTOROBOT_MQTT_OW_UPDATETOPIC, NULL, openwrtUpdateCallback);//add chenkaiyao 20160326
    }
}

