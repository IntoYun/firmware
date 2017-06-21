/**
******************************************************************************
Copyright (c) 2013-2014 IntoRobot Team.  All right reserved.

robin  2017-05-03

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
#include "yunslave_wifi_config.h"
#include "yunslave_wifi_at.h"
#include "yunslave_wifi_at_cmd.h"
#include "yunslave_wifi_at_parse.h"
#include "yunslave_wifi_log.h"

Serial1DebugOutput debugOutput(115200, ALL_LEVEL);

void init_before_setup(void)
{
    //关闭部分功能
    System.disableFeature(SYSTEM_FEATURE_SEND_INFO_ENABLED);            //发送设备信息
    System.disableFeature(SYSTEM_FEATURE_CONFIG_SAVE_ENABLED);          //配置模式保存
    System.disableFeature(SYSTEM_FEATURE_AUTO_CONFIG_PROCESS_ENABLED);  //自动配置处理
    System.disableFeature(SYSTEM_FEATURE_AUTO_TIME_SYN_ENABLED);        //自动时间同步
    System.disableFeature(SYSTEM_FEATURE_DATAPOINT_ENABLED);            //数据点处理
    System.disableFeature(SYSTEM_FEATURE_REGISTER_ENABLED);             //设备注册
    System.disableFeature(SYSTEM_FEATURE_ACTIVATE_ENABLED);             //设备激活
}

STARTUP( init_before_setup() );

#if PLATFORM_ID == PLATFORM_W67
#define INDICATOR_LED_PIN    GPIO0    //定义指示灯引脚
#define MODE_PIN             GPIO16    //定义按键引脚
#elif PLATFORM_ID == PLATFORM_W323
#define INDICATOR_LED_PIN    GPIO0    //定义指示灯引脚
#define MODE_PIN             GPIO4    //定义按键引脚
#endif

system_tick_t g_userProcess_time = 0;
bool g_userProcess_imme = false;    //条件满足立即执行

void at_event_net_response(event_net_type_t event)
{
    String netStatusString = "";

    if(SYSTEM_CONFIG_TYPE_NONE == System.configCurrentMode()) {
        switch(event)
        {
            case EVENT_NET_DISCON_ROUTER:    //YunSlave已断开连接路由器
                netStatusString = "\r\n+RECNET:";
                netStatusString += event;
                netStatusString += "\r\n";
                at_response(netStatusString.c_str());
                break;

            case EVENT_NET_CON_ROUTER:       //YunSlave连接路由器
            case EVENT_NET_CON_SERVER:       //YunSlave连接服务器
            case EVENT_NET_DISCON_SERVER:    //YunSlave断开服务器
                netStatusString = "\r\n+RECNET:";
                netStatusString += event;
                netStatusString += ",\"";
                netStatusString += WiFi.SSID();
                netStatusString += "\",\"";
                netStatusString += String(WiFi.localIP());
                netStatusString += "\",";
                netStatusString += WiFi.RSSI();
                netStatusString += "\r\n";
                at_response(netStatusString.c_str());
                break;
            default:
                break;
        }
    }
}

void system_event_callback(system_event_t event, int param, uint8_t *data, uint16_t datalen)
{
    String recNetString = "";

    switch(event) {
        case event_mode_changed:
            {
                //log_v("event_mode_changed\r\n");
                switch(param) {
                    case ep_mode_normal:
                        g_userProcess_imme = true;
                        modeType = MODE_NORMAL;
                        at_response("\r\n+RECMODE:1\r\n");
                        break;
                    case ep_mode_imlink_config:
                        modeType = MODE_IMLINK_CONFIG;
                        at_response("\r\n+RECMODE:2\r\n");
                        break;
                    case ep_mode_ap_config:
                        modeType = MODE_AP_CONFIG;
                        at_response("\r\n+RECMODE:3\r\n");
                        break;
                    case ep_mode_binding:
                        modeType = MODE_BINDING;
                        at_response("\r\n+RECMODE:4\r\n");
                        break;
                    case ep_mode_test:
                        modeType = MODE_TEST;
                        at_response("\r\n+RECMODE:5\r\n");
                        break;
                    default:
                        break;
                }
                break;
            }

        case event_network_status:
            {
                //log_v("event_network_status\r\n");
                switch(param) {
                    case ep_network_status_connected:
                        if(NETWORK_STATUS_ROUTER_CONNECTED != networkStatus) {
                            networkStatus = NETWORK_STATUS_ROUTER_CONNECTED;
                            at_event_net_response(EVENT_NET_CON_ROUTER);
                        }
                        break;
                    case ep_network_status_disconnected:
                        if(NETWORK_STATUS_ROUTER_DISCONNECT != networkStatus) {
                            networkStatus = NETWORK_STATUS_ROUTER_DISCONNECT;
                            at_event_net_response(EVENT_NET_DISCON_ROUTER);
                        }
                    default:
                        break;
                }
                break;
            }

        case event_cloud_status:
            {
                //log_v("event_cloud_status\r\n");
                switch(param) {
                    case ep_cloud_status_connected:
                        if(NETWORK_STATUS_CLOUD_CONNECTED != networkStatus) {
                            networkStatus = NETWORK_STATUS_CLOUD_CONNECTED;
                            at_event_net_response(EVENT_NET_CON_SERVER);
                        }
                        break;
                    case ep_cloud_status_disconnected:
                        if(NETWORK_STATUS_ROUTER_CONNECTED != networkStatus) {
                            networkStatus = NETWORK_STATUS_ROUTER_CONNECTED;
                            at_event_net_response(EVENT_NET_DISCON_SERVER);
                        }
                    default:
                        break;
                }
                break;
            }

        case event_app_status:
            break;

        default:
            break;
    }
}

void cloudDataCb(uint8_t *payload, uint32_t len)
{
    log_v("cloud data:");
    log_v_dump(payload, len);
    String eventString = "";

    eventString = "\r\n+RECDATA,";
    eventString += len;
    eventString += ":";
    at_response(eventString.c_str());
    at_response_data(payload, len);
}

bool sendProductInfo(void)
{
    log_v("sendProductInfo\r\n");

    String infoTopic = "";
    aJsonClass aJson;
    bool flag = false;
    char buffer[17];

    infoTopic = "v2/device/";
    infoTopic += System.deviceID();
    infoTopic += "/info";

    // start of JSON structure that will make payload
    aJsonObject* root = aJson.createObject();
    if(root == NULL) {
        return false;
    }

    aJson.addStringToObject(root, "productId", yunSlaveParams.product_id);
    aJson.addStringToObject(root, "productMode", "slave");
    System.get(SYSTEM_PARAMS_PRODUCT_BOARD_ID, buffer, sizeof(buffer));
    aJson.addStringToObject(root, "board", buffer);
    aJson.addStringToObject(root, "slaveVer", YUNSLAVE_WIFI_VERSION);
    aJson.addStringToObject(root, "productVer", yunSlaveParams.soft_version);
    aJson.addStringToObject(root, "productHwVer", yunSlaveParams.hardware_version);
    aJson.addBooleanToObject(root, "online", true);

    char* string = aJson.print(root);
    if(IntoRobot.publish(infoTopic.c_str(), string, strlen(string), 0, true, TOPIC_VERSION_CUSTOM)) {
        flag = true;
    } else {
        flag = false;
    }

    free(string);
    aJson.deleteItem(root);
    if( flag ) {
        return true;
    } else {
        return false;
    }
}

void systemConfigKeyDeal()
{
    if(SYSTEM_CONFIG_TYPE_IMLINK != System.configCurrentMode()) {
        System.configEnterMode(SYSTEM_CONFIG_TYPE_IMLINK);
        digitalWrite(INDICATOR_LED_PIN, LOW);
        log_v("enter config ...\r\n");
    } else {
        System.configExit();
        digitalWrite(INDICATOR_LED_PIN, HIGH);
        log_v("exit config ...\r\n");
    }
}

void userInit(void)
{
    pinMode(INDICATOR_LED_PIN, OUTPUT);
    if(SYSTEM_CONFIG_TYPE_IMLINK_SERIAL == System.configCurrentMode()) {
        digitalWrite(INDICATOR_LED_PIN, LOW);
    } else {
        digitalWrite(INDICATOR_LED_PIN, HIGH);
    }
    attachInterrupt(MODE_PIN, systemConfigKeyDeal, FALLING);

    String CloudDataRxTopic = "";
    CloudDataRxTopic = "v2/device/";
    CloudDataRxTopic += System.deviceID();
    CloudDataRxTopic += "/tx";

    IntoRobot.subscribe(CloudDataRxTopic.c_str(), NULL, cloudDataCb, 0, TOPIC_VERSION_CUSTOM);

    int mode;
    System.get(SYSTEM_PARAMS_SECURITY_MODE, mode);
    if(AT_MODE_FLAG_OTAA_INACTIVE == mode) {
        g_productActivateRequest = true;
        g_productActivateTimes = 0;
    }

    g_userProcess_imme = true;
    g_userProcess_time = millis();
}

void userProcess(void)
{
    if((SYSTEM_CONFIG_TYPE_NONE == System.configCurrentMode()) && WiFi.ready()) {
        if(IntoRobot.connected() && !g_productInfoSended) {
            if(sendProductInfo()) {
                g_productInfoSended = true;
            }
        }

        if(g_productRegisterRequest) {
            if(IntoRobot.deviceRegister(yunSlaveParams.product_id, g_signature)) {
                g_productActivateRequest = true;
                g_productActivateTimes = 0;
            }
            g_productRegisterRequest = false;
        }

        system_tick_t current_millis = millis();
        system_tick_t elapsed_millis = current_millis - g_userProcess_time;

        //Check for wrapping
        if (elapsed_millis < 0) {
            elapsed_millis =  UINT_MAX - g_userProcess_time + current_millis;
        }

        if ((elapsed_millis >= PRODUCT_ACTIVATE_INTERVAL) || g_userProcess_imme) {
            g_userProcess_imme = false;
            if(g_productActivateRequest && (g_productActivateTimes < PRODUCT_ACTIVATE_TIMES)) {
                IntoRobot.syncTime();
                if(IntoRobot.deviceActivate()) {
                    g_productActivateRequest = false;
                    g_productActivateTimes = 0;
                } else {
                    g_productActivateTimes++;
                }
            }
            g_userProcess_time = millis();
        }
    }
}

void setup()
{
    at_init();
    at_uart_init(115200, 115200); //第一个参数：通讯串口波特率  第二个参数：调试串口波特率
    System.on(event_all, &system_event_callback);
    userInit();
    at_ready();
    System.configExit();
}

void loop()
{
    userProcess();
    System.configProcess();
    at_loop();
}

