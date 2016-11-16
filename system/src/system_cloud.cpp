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
#ifndef configNO_CLOUD

#include <stdlib.h>
#include <stdio.h>

#include "wiring_string.h"
#include "wiring_tcpclient.h"
#include "system_mqttclient.h"
#include "system_cloud_def.h"
#include "system_cloud.h"
#include "system_mode.h"
#include "system_task.h"
#include "system_threading.h"
#include "system_update.h"
#include "system_rgbled.h"
#include "wiring_time.h"
#include "params_hal.h"
#include "ota_flash_hal.h"
#include "core_hal.h"
#include "ajson.h"

volatile uint8_t g_intorobot_network_connected = 0;    //网络连接状态 1连接 0断开
volatile uint8_t g_intorobot_cloud_connected = 0;      //平台连接状态 1连接上了

struct CallBackList g_callback_list;  //回调结构体
struct CloudDebugBuffer  g_debug_tx_buffer;
struct CloudDebugBuffer  g_debug_rx_buffer;

TCPClient g_mqtt_tcp_client;
MqttClientClass g_mqtt_client;
RGBLEDState led_state;


pCallBack get_subscribe_callback(char * fulltopic);
WidgetBaseClass *get_widget_subscribe_callback(char * fulltopic);
void add_subscribe_callback(char *topic, char *device_id, void (*callback)(uint8_t*, uint32_t), uint8_t qos);
void add_widget_subscibe_callback(char *topic, char *device_id, WidgetBaseClass *pWidgetBase, uint8_t qos);
void del_subscribe_callback(char * topic, char *device_id);
void mqtt_receive_debug_info(uint8_t *pIn, uint32_t len);

void mqtt_client_callback(char *topic, uint8_t *payload, uint32_t length)
{
    //DEBUG_D("default:mqtt_client_callback\r\n");
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
    uint32_t n;
    String s_time="";

    //DEBUG_D("default:sync_time_callback\r\n");
    for(n=0; n<len; n++)
    {s_time+=(char)payload[n];}
    Time.setTime(atol(s_time.c_str()));
    //unsubscribe the system time topic
    intorobot_unsubscribe(INTOROBOT_MQTT_SUB_SYNC_TIME_TOPIC, "service");
}

void ota_update_callback(uint8_t *payload, uint32_t len)
{
    uint32_t n;
    char flag=0;
    String s_payload="", domain="", param="";
    aJsonClass aJson;

    for(n=0; n<len; n++)
    {s_payload+=(char)payload[n];}

    //DEBUG_D("default:ota_update_callback\r\n");
    led_state.save();
    system_rgb_color(RGB_COLOR_YELLOW);
    intorobot_publish(INTOROBOT_MQTT_RESPONSE_TOPIC, (uint8_t *)INTOROBOT_MQTT_RESPONSE_OTA_READY, strlen(INTOROBOT_MQTT_RESPONSE_OTA_READY), 0, false);

    aJsonObject *root = aJson.parse((char *)s_payload.c_str());
    if(root == NULL)
    {flag=1;}

    aJsonObject* type_Object = aJson.getObjectItem(root, "board");
    if(type_Object == NULL)
    {flag=1;}
    else
    {
        char board[8]={0}, board1[8]={0};
        HAL_Board_Type(board, sizeof(board), 0);
        HAL_Board_Type(board1, sizeof(board1), 1);
        if( strcmp(type_Object->valuestring, board) && strcmp(type_Object->valuestring, board1) )
        {flag=2;}
    }

    aJsonObject* md5_Object = aJson.getObjectItem(root, "md5");
    if(md5_Object == NULL)
    {flag=1;}

    aJsonObject* dtoken_Object = aJson.getObjectItem(root, "dwn_token");
    if(dtoken_Object == NULL)
    {flag=1;}


    if(0==flag)
    {
        if (0x01 == HAL_PARAMS_Get_System_sv_select()) {
            char down_domain[36]={0};
            HAL_PARAMS_Get_System_dw_domain(down_domain, sizeof(down_domain));
            domain+=down_domain;
        }
        else {
            domain+=INTOROBOT_UPDATE_DOMAIN;
        }

        param+=INTOROBOT_OTA_UPDATE_URL;
        param+="?dwn_token=";
        param+=dtoken_Object->valuestring;

        uint8_t down_status = 0, progress = 0;
        down_status_t status;
        down_status_t result = HAL_OTA_Download_App(domain.c_str(), param.c_str(),  md5_Object->valuestring);
        switch(result)
        {
            case DOWNSTATUS_SUCCESS:
                break;
            case DOWNSTATUS_DOWNING:
                do {
                    status = HAL_OTA_Get_App_Download_Status();
                    if(DOWNSTATUS_SUCCESS == status) {
                        break;
                    }
                    else if(DOWNSTATUS_DOWNING == status) {
                        progress = HAL_OTA_Get_Download_Progress();
                        //intorobot_publish(INTOROBOT_MQTT_RESPONSE_TOPIC, (uint8_t *)INTOROBOT_MQTT_RESPONSE_OTA_DOWN_SUCC_EXIT, strlen(INTOROBOT_MQTT_RESPONSE_OTA_DOWN_SUCC_EXIT),false);
                        delay(1000);
                    }
                    else if(DOWNSTATUS_FAIL == status) {
                        down_status = 1;
                        break;
                    }
                }while(1);
                break;
            default:
                down_status = 1;
                break;
        }
        if(!down_status) {
            intorobot_publish(INTOROBOT_MQTT_RESPONSE_TOPIC, (uint8_t *)INTOROBOT_MQTT_RESPONSE_OTA_DOWN_SUCC_EXIT, strlen(INTOROBOT_MQTT_RESPONSE_OTA_DOWN_SUCC_EXIT), 0, false);
            HAL_OTA_Update_App();
            HAL_Core_System_Reset();
            while(1); //不会运行到地方
        }
        else
        {flag=1;} //下载失败
    }
    if (root != NULL)
    {aJson.deleteItem(root);}
    if (2==flag) {  // board type error
        intorobot_publish(INTOROBOT_MQTT_RESPONSE_TOPIC, (uint8_t *)INTOROBOT_MQTT_RESPONSE_OTA_TYPEEEOR, strlen(INTOROBOT_MQTT_RESPONSE_OTA_TYPEEEOR), 0, false);
    }
    else {//download fall
        intorobot_publish(INTOROBOT_MQTT_RESPONSE_TOPIC, (uint8_t *)INTOROBOT_MQTT_RESPONSE_OTA_DOWN_FAIL, strlen(INTOROBOT_MQTT_RESPONSE_OTA_DOWN_FAIL), 0, false);
    }
    led_state.restore();
}

void subsys_update_callback(uint8_t *payload, uint32_t len)
{
    uint32_t n;
    String s_payload="", domain="", param="";
    aJsonClass aJson;
    char temp[64],flag=0,result=1;
    bool board_type_error = false;

    for(n=0; n<len; n++)
    {s_payload+=(char)payload[n];}

    //DEBUG_D("default:subsys_update_callback\r\n");
    led_state.save();
    system_rgb_color(RGB_COLOR_YELLOW);
    memset(temp,0,sizeof(temp));
    sprintf(temp,"{\"status\":\"%s\"}", INTOROBOT_MQTT_RESPONSE_SUBSYS_READY_PROGRESS);
    intorobot_publish(INTOROBOT_MQTT_RESPONSE_JSON_TOPIC, (uint8_t*)temp, strlen(temp), 0, false);

    aJsonObject *root = aJson.parse((char *)s_payload.c_str());
    if(root == NULL)
    {flag=1;}
/*
    aJsonObject* type_Object = aJson.getObjectItem(root, "board");
    if(type_Object == NULL)
    {flag=1;}
    else
    {
        char board[8]={0}, board1[8]={0};
        HAL_Board_Type(board, sizeof(board), 0);
        HAL_Board_Type(board1, sizeof(board1), 1);
        if( strcmp(type_Object->valuestring, board) && strcmp(type_Object->valuestring, board1) )
        {flag=2;}
    }
*/
    aJsonObject* sys_Ver_Object = aJson.getObjectItem(root, "sys_ver");
    if(sys_Ver_Object == NULL)
    {flag=1;}
    else
    {
        char sys_ver[24]={0};
        HAL_PARAMS_Get_System_subsys_ver(sys_ver, sizeof(sys_ver));
        if(strcmp(sys_ver, sys_Ver_Object->valuestring))
        {flag=2;}
    }

    if(0==flag)
    {
        if (0x01 == HAL_PARAMS_Get_System_sv_select()) {
            char down_domain[36]={0};
            HAL_PARAMS_Get_System_dw_domain(down_domain, sizeof(down_domain));
            domain+=down_domain;
        }
        else {
            domain+=INTOROBOT_UPDATE_DOMAIN;
        }
        char name[24]={0};
        HAL_Platform_Name(name, sizeof(name));
        param+="/downloads/";
        param+=name;
        param+="/";
        param+=sys_Ver_Object->valuestring;

        uint8_t down_status = 0, progress = 0;
        down_status_t status;
        down_status_t result = HAL_OTA_Download_Subsys(domain.c_str(), param.c_str());
        switch(result) {
            case DOWNSTATUS_SUCCESS:
                break;
            case DOWNSTATUS_DOWNING:
                do {
                    status = HAL_OTA_Get_Subsys_Download_Status();
                    if(DOWNSTATUS_SUCCESS == status) {
                        break;
                    }
                    else if(DOWNSTATUS_DOWNING == status) {
                        progress = HAL_OTA_Get_Download_Progress();
                        memset(temp,0,sizeof(temp));
                        sprintf(temp,"{\"status\":\"%s\",\"progress\": %d}", INTOROBOT_MQTT_RESPONSE_SUBSYS_READY_PROGRESS, progress);
                        intorobot_publish(INTOROBOT_MQTT_RESPONSE_JSON_TOPIC, (uint8_t*)temp, strlen(temp), 0, false);
                        delay(1000);
                    }
                    else if(DOWNSTATUS_FAIL == status) {
                        down_status = 1;
                        break;
                    }
                }while(1);
                break;
            default:
                down_status = 1;
                break;
        }
        if(!down_status) {
            memset(temp,0,sizeof(temp));
            sprintf(temp,"{\"status\":\"%s\"}", INTOROBOT_MQTT_RESPONSE_SUBSYS_DOWN_SUCC_EXIT, progress);
            intorobot_publish(INTOROBOT_MQTT_RESPONSE_JSON_TOPIC, (uint8_t*)temp, strlen(temp), 0, false);
            HAL_OTA_Upadate_Subsys();
            HAL_Core_System_Reset();
            while(1); //不会运行到地方
        }
        else
        {flag=1;} //下载失败
    }

    if (root != NULL)
    {aJson.deleteItem(root);}
    memset(temp,0,sizeof(temp));
    if (2==flag) {  // board type error
        sprintf(temp,"{\"status\":\"%s\"}", INTOROBOT_MQTT_RESPONSE_SUBSYS_TYPEEEOR);
    }
    else {//download fall
        sprintf(temp,"{\"status\":\"%s\"}", INTOROBOT_MQTT_RESPONSE_SUBSYS_DOWN_FAIL);
    }
    intorobot_publish(INTOROBOT_MQTT_RESPONSE_JSON_TOPIC, (uint8_t*)temp, strlen(temp), 0, false);
    led_state.restore();
}

void accesstoken_update_callback(uint8_t *payload, uint32_t len)
{
    uint32_t n;
    String s_access_token;

    //DEBUG_D("default:accesstoken_update_callback");
    for(n=0; n<len; n++)
    {s_access_token+=(char)payload[n];}
    //send response topic
    intorobot_publish(INTOROBOT_MQTT_RESPONSE_TOPIC, (uint8_t *)INTOROBOT_MQTT_RESPONSE_TOKEN_UPDATE_SUCC, strlen(INTOROBOT_MQTT_RESPONSE_TOKEN_UPDATE_SUCC), 0, false);
    //save access token
    char access_token[48]={0};
    HAL_PARAMS_Get_System_access_token(access_token, sizeof(access_token));
    DEBUG("before access_token = %s\r\n",access_token);
    HAL_PARAMS_Set_System_access_token(s_access_token.c_str());
    DEBUG("after access_token = %s\r\n",s_access_token.c_str());
    HAL_PARAMS_Save_Params();
    HAL_Core_System_Reset();
    while(1); //不会运行到地方
}

void system_reboot_callback(uint8_t *payload, uint32_t len)
{
    //DEBUG_D("default:system_reboot_callback\r\n");
    //system reset ready
    intorobot_publish(INTOROBOT_MQTT_RESPONSE_TOPIC, (uint8_t *)INTOROBOT_MQTT_RESPONSE_REBOOT_SUCC, strlen(INTOROBOT_MQTT_RESPONSE_REBOOT_SUCC), 0, false);
    HAL_Core_System_Reset();
}

void system_debug_callback(uint8_t *payload, uint32_t len)
{
    //DEBUG_D("default:system_debug_callback\r\n");
    mqtt_receive_debug_info(payload, len);
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
    char device_id[32]={0};

    HAL_PARAMS_Get_System_device_id(device_id, sizeof(device_id));
    return device_id;
}

bool intorobot_cloud_init(void)
{
    memset(&g_debug_tx_buffer,0,sizeof(g_debug_tx_buffer));
    memset(&g_debug_rx_buffer,0,sizeof(g_debug_rx_buffer));

    g_mqtt_client = MqttClientClass((char *)INTOROBOT_SERVER_DOMAIN, INTOROBOT_SERVER_PORT, mqtt_client_callback, g_mqtt_tcp_client);

    intorobot_subscribe(INTOROBOT_MQTT_SUB_UPDATE_TOPIC, NULL, ota_update_callback, 0);                 //固件升级
    intorobot_subscribe(INTOROBOT_MQTT_SUB_JSON_UPDATE_TOPIC, NULL, subsys_update_callback, 0);         //子系统升级
    intorobot_subscribe(INTOROBOT_MQTT_SUB_SYNC_TIME_TOPIC, "service", sync_time_callback, 0);          //同步时间
    intorobot_subscribe(INTOROBOT_MQTT_SUB_CHANGE_TOKEN_TOPIC, NULL, accesstoken_update_callback, 0);   //更新access token
    intorobot_subscribe(INTOROBOT_MQTT_SUB_REBOOT_TOPIC, NULL, system_reboot_callback, 0);              //stm32重启
    intorobot_subscribe(INTOROBOT_MQTT_SUB_RECEIVE_DEBUG_TOPIC, NULL, system_debug_callback, 0);        //从平台获取调试信息
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
    intorobot_subscribe(INTOROBOT_MQTT_SUB_SYNC_TIME_TOPIC, "service", sync_time_callback, 0);          //同步时间
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
    char device_id[32]={0};

    for (int i = 0 ; i < g_callback_list.total_callbacks; i++)
    {
        memset(topictmp, 0, sizeof(topictmp));
        if(g_callback_list.callback_node[i].device_id == NULL) {
            HAL_PARAMS_Get_System_device_id(device_id, sizeof(device_id));
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
    char device_id[32]={0};

    for (int i = 0 ; i < g_callback_list.total_wcallbacks; i++)
    {
        memset(topictmp, 0, sizeof(topictmp));
        if(g_callback_list.widget_callback_node[i].device_id == NULL) {
            HAL_PARAMS_Get_System_device_id(device_id, sizeof(device_id));
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
    //set intorobot server
    char sv_domain[32]={0};
    int sv_port;
    if(0x01 == HAL_PARAMS_Get_System_sv_select()) {
        HAL_PARAMS_Get_System_sv_domain(sv_domain, sizeof(sv_domain));
        sv_port=HAL_PARAMS_Get_System_sv_port();
    }
    else {
        strcpy(sv_domain, INTOROBOT_SERVER_DOMAIN);
        sv_port=INTOROBOT_SERVER_PORT;
    }
    g_mqtt_client.setServer(sv_domain, sv_port);

    char device_id[38]={0},access_token[48]={0};
    HAL_PARAMS_Get_System_device_id(device_id, sizeof(device_id));
    HAL_PARAMS_Get_System_access_token(access_token, sizeof(access_token));
    //mqtt连接平台
    // DEBUG_D("sv_domain:%s\r\n", sv_domain);
    // DEBUG_D("sv_port:%d\r\n", sv_port);
    // DEBUG_D("device_id=%s\r\n", device_id);
    // DEBUG_D("access_token=%s\r\n", access_token);
    if(g_mqtt_client.connect("", access_token, device_id))
    {
        char fw_version[28]={0}, subsys_version[28]={0}, temp[64]={0};

        HAL_PARAMS_Get_System_fwlib_ver(fw_version, sizeof(fw_version));
        HAL_PARAMS_Get_System_subsys_ver(subsys_version, sizeof(subsys_version));
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
    HAL_Core_System_Loop();
}

#endif
