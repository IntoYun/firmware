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

#include "wiring_string.h"
#include "wiring_tcpclient.h"
#include "system_mqttclient.h"
#include "system_cloud_def.h"
#include "system_cloud.h"
#include "system_mode.h"
#include "system_task.h"
#include "system_threading.h"
#include "system_update.h"
#include "params_hal.h"
#include <stdlib.h>
#include <stdio.h>

//#ifndef INTOROBOT_NO_CLOUD

volatile uint8_t g_intorobot_network_connected = 0;    //网络连接状态 1连接 0断开
volatile uint8_t g_intorobot_cloud_connected = 0;      //平台连接状态 1连接上了

struct CallBackList g_callback_list;  //回调结构体
struct CloudDebugBuffer  g_debug_tx_buffer;
struct CloudDebugBuffer  g_debug_rx_buffer;

TCPClient g_mqtt_tcp_client;
MqttClientClass g_mqtt_client;


pCallBack get_subscribe_callback(char * fulltopic);
WidgetBaseClass *get_widget_subscribe_callback(char * fulltopic);
void add_subscribe_callback(char *topic, char *device_id, void (*callback)(uint8_t*, uint32_t), uint8_t qos);
void add_widget_subscibe_callback(char *topic, char *device_id, WidgetBaseClass *pWidgetBase, uint8_t qos);
void del_subscribe_callback(char * topic, char *device_id);


void mqtt_client_callback(char *topic, uint8_t *payload, uint32_t length)
{
    uint8_t *pdata = NULL;
    pCallBack pcallback=get_subscribe_callback(topic);
    if(pcallback!=NULL)
    {
        pdata = (uint8_t *)malloc(length+1);
        memset(pdata, 0, length+1);
        memcpy(pdata, payload, length);
        pcallback(pdata,length);
        free(pdata);
    }

    WidgetBaseClass *pwidgetbase=get_widget_subscribe_callback(topic);
    if(pwidgetbase!=NULL)
    {
        pdata = (uint8_t *)malloc(length+1);
        memset(pdata, 0, length+1);
        memcpy(pdata, payload, length);
        pwidgetbase->widgetBaseCallBack(pdata,length);
        free(pdata);
    }
}

void sync_time_callback(uint8_t *payload, uint32_t len)
{

}

void ota_update_callback(uint8_t *payload, uint32_t len)
{

}

void subsys_update_callback(uint8_t *payload, uint32_t len)
{

}

void accesstoken_update_callback(uint8_t *payload, uint32_t len)
{

}

void system_reset_callback(uint8_t *payload, uint32_t len)
{

}

void system_debug_callback(uint8_t *payload, uint32_t len)
{

}

void fill_mqtt_topic(String &fulltopic, const char *topic, const char *device_id)
{
    char temp[128];
    String sdevice_id=intorobot_deviceID();

    memset(temp,0,sizeof(temp));
    if(device_id == NULL)
    {sprintf(temp,"%s/%s/", INTOROBOT_API_VER, sdevice_id.c_str());}
    else
    {sprintf(temp,"%s/%s/", INTOROBOT_API_VER, device_id);}
    fulltopic=temp;
    fulltopic+=topic;
}

String intorobot_deviceID(void)
{
    char device_id[48]={0};

    HAL_PARAMS_Get_DeviceID(device_id, sizeof(device_id));
    return device_id;
}

bool intorobot_cloud_init(void)
{
    memset(&g_debug_tx_buffer,0,sizeof(g_debug_tx_buffer));
    memset(&g_debug_rx_buffer,0,sizeof(g_debug_rx_buffer));

    g_mqtt_client = MqttClientClass((char *)INTOROBOT_SERVER_DOMAIN, INTOROBOT_SERVER_PORT, mqtt_client_callback, g_mqtt_tcp_client);
}

uint8_t intorobot_publish(const char* topic, uint8_t* payload, unsigned int plength, uint8_t qos, uint8_t retained)
{
    String fulltopic;

    fill_mqtt_topic(fulltopic, topic, NULL);
    return g_mqtt_client.publish(fulltopic.c_str(), payload, plength, retained);
}

uint8_t intorobot_subscribe(const char* topic, const char *device_id, void (*callback)(uint8_t*, uint32_t), uint8_t qos)
{
    String fulltopic;

    add_subscribe_callback((char *)topic, (char *)device_id, callback, qos);
    fill_mqtt_topic(fulltopic, topic, device_id);
    return g_mqtt_client.subscribe(fulltopic.c_str(), qos);
}

uint8_t intorobot_widget_subscribe(const char* topic, const char *device_id, WidgetBaseClass *pWidgetBase, uint8_t qos)
{
    String fulltopic;

    add_widget_subscibe_callback((char *)topic, (char *)device_id, pWidgetBase, qos);
    fill_mqtt_topic(fulltopic, topic, device_id);
    return g_mqtt_client.subscribe(fulltopic.c_str(), qos);
}

uint8_t intorobot_unsubscribe(const char *topic, const char *device_id)
{
    String fulltopic;

    del_subscribe_callback((char *)topic, (char *)device_id);
    fill_mqtt_topic(fulltopic, topic, device_id);
    return g_mqtt_client.unsubscribe(fulltopic.c_str());
}

void intorobot_sync_time(void)
{
    intorobot_subscribe(INTOROBOT_MQTT_SUB_SYNC_TIME_TOPIC, NULL, sync_time_callback, 0);
}

size_t intorobot_debug_info_write(uint8_t byte)
{
    g_debug_tx_buffer.buffer[g_debug_tx_buffer.head] = byte;
    g_debug_tx_buffer.head++;
    /* To avoid buffer overflow */
    if(g_debug_tx_buffer.head == CLOUD_DEBUG_BUFFER_SIZE)
    {
        g_debug_tx_buffer.head = 0;
    }
    return 1;
}

int intorobot_debug_info_read(void)
{
    // if the head isn't ahead of the tail, we don't have any characters
    if (g_debug_rx_buffer.head == g_debug_rx_buffer.tail)
    {
        return -1;
    }
    else
    {
        unsigned char c = g_debug_rx_buffer.buffer[g_debug_rx_buffer.tail];
        g_debug_rx_buffer.tail = (unsigned int)(g_debug_rx_buffer.tail + 1) % CLOUD_DEBUG_BUFFER_SIZE;
        return c;
    }
}

int intorobot_debug_info_available(void)
{
    return (unsigned int)(CLOUD_DEBUG_BUFFER_SIZE + g_debug_rx_buffer.head - g_debug_rx_buffer.tail) % CLOUD_DEBUG_BUFFER_SIZE;
}


pCallBack get_subscribe_callback(char * fulltopic)
{
    char topictmp[128]={0};
    char device_id[48]={0};

    for (int i = 0 ; i < g_callback_list.total_callbacks; i++)
    {
        memset(topictmp, 0, sizeof(topictmp));
        if(g_callback_list.callback_node[i].device_id == NULL) {
            HAL_PARAMS_Get_DeviceID(device_id, sizeof(device_id));
            sprintf(topictmp,"%s/%s/", INTOROBOT_API_VER, device_id);
        }
        else {
            sprintf(topictmp,"%s/%s/", INTOROBOT_API_VER, g_callback_list.callback_node[i].device_id);
        }
        strcat(topictmp,g_callback_list.callback_node[i].topic);
        if (strcmp(fulltopic, topictmp) == 0) {
            return g_callback_list.callback_node[i].callback;
        }
    }
    return NULL;
}

void add_subscribe_callback(char *topic, char *device_id, void (*callback)(uint8_t*, uint32_t), uint8_t qos)
{
    int if_found_topic = 0;
    int i = 0;

    for (i = 0 ; i < g_callback_list.total_callbacks; i++)
    {
        if ((topic == g_callback_list.callback_node[i].topic)&&(device_id == g_callback_list.callback_node[i].device_id))
        {
            if_found_topic = 1;
            break;
        }
    }

    if (if_found_topic)
    {
        g_callback_list.callback_node[i].callback = callback;
        g_callback_list.callback_node[i].qos = qos;
    }
    else
    {
        if (g_callback_list.total_callbacks == MAX_CALLBACK_NUM)
        {
            return;
        }
        else
        {
            g_callback_list.callback_node[g_callback_list.total_callbacks].callback = callback;
            g_callback_list.callback_node[g_callback_list.total_callbacks].qos = qos;
            g_callback_list.callback_node[g_callback_list.total_callbacks].topic = topic;
            g_callback_list.callback_node[g_callback_list.total_callbacks].device_id= device_id;
            g_callback_list.total_callbacks ++;
        }
    }
}

WidgetBaseClass *get_widget_subscribe_callback(char * fulltopic)
{
    char topictmp[128]={0};
    char device_id[48]={0};

    for (int i = 0 ; i < g_callback_list.total_wcallbacks; i++)
    {
        memset(topictmp, 0, sizeof(topictmp));
        if(g_callback_list.widget_callback_node[i].device_id == NULL) {
            HAL_PARAMS_Get_DeviceID(device_id, sizeof(device_id));
            sprintf(topictmp,"%s/%s/", INTOROBOT_API_VER, device_id);
        }
        else {
            sprintf(topictmp,"%s/%s/", INTOROBOT_API_VER, g_callback_list.widget_callback_node[i].device_id);
        }
        strcat(topictmp,g_callback_list.widget_callback_node[i].topic);
        if (strcmp(fulltopic, topictmp) == 0)
        {
            return g_callback_list.widget_callback_node[i].pWidgetBase;
        }
    }
    return NULL;
}

void add_widget_subscibe_callback(char *topic, char *device_id, WidgetBaseClass *pWidgetBase, uint8_t qos)
{
    int if_found_topic = 0;
    int i = 0;

    for (i = 0 ; i < g_callback_list.total_wcallbacks; i++)
    {
        if ((topic == g_callback_list.widget_callback_node[i].topic)&&(device_id == g_callback_list.widget_callback_node[i].device_id))
        {
            if_found_topic = 1;
            break;
        }
    }

    if (if_found_topic)
    {
        g_callback_list.widget_callback_node[i].pWidgetBase = pWidgetBase;
        g_callback_list.widget_callback_node[i].qos = qos;
    }
    else
    {
        if (g_callback_list.total_wcallbacks == MAX_CALLBACK_NUM)
        {
            return;
        }
        else
        {
            g_callback_list.widget_callback_node[g_callback_list.total_wcallbacks].pWidgetBase = pWidgetBase;
            g_callback_list.widget_callback_node[g_callback_list.total_wcallbacks].qos = qos;
            g_callback_list.widget_callback_node[g_callback_list.total_wcallbacks].topic = topic;
            g_callback_list.widget_callback_node[g_callback_list.total_wcallbacks].device_id= device_id;
            g_callback_list.total_wcallbacks ++;
        }
    }
}

void del_subscribe_callback(char * topic, char *device_id)
{
    for (int i = 0 ; i < g_callback_list.total_callbacks; i++)
    {
        if ((topic == g_callback_list.callback_node[i].topic) && (device_id == g_callback_list.callback_node[i].device_id))
        {
            memcpy(&g_callback_list.callback_node[i], &g_callback_list.callback_node[i+1], (g_callback_list.total_callbacks - 1 - i) * sizeof(struct CallBackNode));
            memset(&g_callback_list.callback_node[g_callback_list.total_callbacks-1], 0, sizeof(struct CallBackNode));
            g_callback_list.total_callbacks--;
            return;
        }
    }

    for (int i = 0 ; i < g_callback_list.total_wcallbacks; i++)
    {
        if ((topic == g_callback_list.widget_callback_node[i].topic) && (device_id == g_callback_list.widget_callback_node[i].device_id))
        {
            memcpy(&g_callback_list.widget_callback_node[i], &g_callback_list.widget_callback_node[i+1], (g_callback_list.total_wcallbacks - 1 - i) * sizeof(struct WidgetCallBackNode));
            memset(&g_callback_list.widget_callback_node[g_callback_list.total_wcallbacks-1], 0, sizeof(struct WidgetCallBackNode));
            g_callback_list.total_wcallbacks--;
            return;
        }
    }
}

void resubscribe(void)
{
    for (int i = 0 ; i < g_callback_list.total_callbacks; i++)
    {
        intorobot_subscribe(g_callback_list.callback_node[i].topic, g_callback_list.callback_node[i].device_id,
                            g_callback_list.callback_node[i].callback, g_callback_list.callback_node[i].qos);
    }

    for (int i = 0 ; i < g_callback_list.total_wcallbacks; i++)
    {
        intorobot_widget_subscribe(g_callback_list.widget_callback_node[i].topic, g_callback_list.widget_callback_node[i].device_id,
                            g_callback_list.widget_callback_node[i].pWidgetBase, g_callback_list.widget_callback_node[i].qos);
    }
}

void mqtt_send_debug_info(void)
{
    uint32_t n;
    String s_debug_info="";

    for(n=0; (n<MQTT_MAX_PACKET_SIZE)&&(g_debug_tx_buffer.tail!=g_debug_tx_buffer.head); n++)
    {
        s_debug_info += (char)g_debug_tx_buffer.buffer[g_debug_tx_buffer.tail];
        g_debug_tx_buffer.tail = (unsigned int)(g_debug_tx_buffer.tail + 1) % CLOUD_DEBUG_BUFFER_SIZE;
    }

    if(n)
    {
        g_mqtt_client.publish(INTOROBOT_MQTT_SEND_DEBUG_TOPIC, (uint8_t *)s_debug_info.c_str(), n, false);
    }
}

void mqtt_receive_debug_info(uint8_t *pIn, uint32_t len)
{
    int n;

    for( n=0; n<len; n++)
    {
        g_debug_rx_buffer.buffer[g_debug_rx_buffer.head] = pIn[n];
        g_debug_rx_buffer.head++;
        /* To avoid buffer overflow */
        if(g_debug_rx_buffer.head == CLOUD_DEBUG_BUFFER_SIZE)
        {
            g_debug_rx_buffer.head = 0;
        }
    }
}

/**
 * Determines if the system will attempt to connect or disconnect from the cloud.
 */
bool intorobot_cloud_flag_connected(void)
{
    if (g_intorobot_network_connected && g_intorobot_cloud_connected)
        return true;
    else
        return false;
}

void intorobot_cloud_disconnect(void)
{
    if(g_mqtt_client.connected())
    {
        g_mqtt_client.disconnect();
    }
}

int intorobot_cloud_connect(void)
{
    intorobot_cloud_disconnect();
    DEBUG_D("1111");
    //set intorobot server
    char sv_domain[36]={0};
    int sv_port;
    if(0x01 == HAL_PARAMS_Get_Server_Select_Flag()) {
        HAL_PARAMS_Get_Server_Domain(sv_domain, sizeof(sv_domain));
        sv_port=HAL_PARAMS_Get_Server_Port();
    }
    else {
        strcpy(sv_domain, INTOROBOT_SERVER_DOMAIN);
        sv_port=INTOROBOT_SERVER_PORT;
    }
    g_mqtt_client.setServer(sv_domain, sv_port);

    DEBUG_D("2222");
    char device_id[36]={0},access_token[36]={0};
    HAL_PARAMS_Get_DeviceID(device_id, sizeof(device_id));
    HAL_PARAMS_Get_AccessToken(access_token, sizeof(access_token));
    //mqtt连接平台
    DEBUG_D("3333");
    if(g_mqtt_client.connect("", access_token, device_id))
    {
        char fw_version[28]={0}, subsys_version[28]={0}, temp[64]={0};

        DEBUG_D("3333");
        HAL_PARAMS_Get_FirmwareLib_Version(fw_version, sizeof(fw_version));
        HAL_PARAMS_Get_Subsys_Version(subsys_version, sizeof(subsys_version));
        memset(temp, 0, sizeof(temp));
        sprintf(temp,"{\"fw_ver\":\"%s\",\"sys_ver\":\"%s\"}", fw_version, subsys_version);
        intorobot_publish(INTOROBOT_MQTT_VERSION_TOPIC, (uint8_t*)temp, strlen(temp), 0, true);

        //重新订阅
        resubscribe();
        return 0;
    }
    return -1;
}

int intorobot_cloud_handle(void)
{
    if(true == g_mqtt_client.loop())
    {
        //发送IntoRobot.printf打印到平台
        mqtt_send_debug_info();
        return 0;
    }
    return -1;
}

void intorobot_process(void)
{


}
