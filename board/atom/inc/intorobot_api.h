/**
 ******************************************************************************
 * @file     : intorobot.h
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
#ifndef   INTOROBOT_API_H_
#define   INTOROBOT_API_H_

#include "variant.h"
#include "lib_mqttclient.h"

#define INTOROBOT_FIRMWARE_LIB_VER  "1.3.0.160625"

#define INTOROBOT_BOARD_TYPE1    "888001" //sevice info
#define INTOROBOT_BOARD_TYPE2    "887001"


//publish
//last will
#define INTOROBOT_MQTT_WILLTOPIC   "platform/default/info/online"
#define INTOROBOT_MQTT_WILLQOS     0
#define INTOROBOT_MQTT_WILLRETAIN  true
#define INTOROBOT_MQTT_WILLMESSAGE "{\"key\": \"offline\"}"

//device info
#define INTOROBOT_MQTT_INFOTOPIC   "firmware/default/info/version"
#define INTOROBOT_MQTT_INFOQOS     0
#define INTOROBOT_MQTT_INFORETAIN  true

//device update response
#define INTOROBOT_MQTT_RESPONSE_TOPIC   "platform/default/info/notify"
#define INTOROBOT_MQTT_RESPONSE_QOS     0
#define INTOROBOT_MQTT_RESPONSE_RETAIN  false

#define INTOROBOT_MQTT_RESMES_ST_FWUPREADY    "10"  //stm32 ready firmware down
#define INTOROBOT_MQTT_RESMES_ST_FWDOWNFAIL   "11"  //stm32 firmware down failed
#define INTOROBOT_MQTT_RESMES_ST_FWDOWNSUCC   "12"  //stm32 firmware down success
#define INTOROBOT_MQTT_RESMES_ST_FWUPFAIL     "13"  //stm32 firmware update failed
#define INTOROBOT_MQTT_RESMES_ST_FWUPSUCC     "14"  //stm32 firmware update success
#define INTOROBOT_MQTT_RESMES_ST_FWUPSUCC1    "15"   //stm32 firmware down success exit
#define INTOROBOT_MQTT_RESMES_ST_FWUPTYPEEEOR "16"   //stm32 firmware update failed of board type error
#define INTOROBOT_MQTT_RESMES_TKUPREADY    "20"  //ready token update
#define INTOROBOT_MQTT_RESMES_TKUPFAIL     "21"  //token update failed
#define INTOROBOT_MQTT_RESMES_TKUPSUCC     "22"  //token update success
#define INTOROBOT_MQTT_RESMES_SRSETREADY   "30"  //ready system reset
#define INTOROBOT_MQTT_RESMES_SRSTTFAIL    "31"  //system reset failed
#define INTOROBOT_MQTT_RESMES_SRSETSUCC    "32"  //system reset success

#define INTOROBOT_MQTT_RESPONSE_JSON_TOPIC   "platform/default/info/json"
#define INTOROBOT_MQTT_RESPONSE_JSON_QOS     0
#define INTOROBOT_MQTT_RESPONSE_JSON_RETAIN  false

#define INTOROBOT_MQTT_RESMES_OW_FWUPREADY    "10"  //openwrt ready down
#define INTOROBOT_MQTT_RESMES_OW_FWDOWNFAIL   "11"  //openwrt down failed
#define INTOROBOT_MQTT_RESMES_OW_FWDOWNSUCC   "12"  //openwrt down success
#define INTOROBOT_MQTT_RESMES_OW_FWUPFAIL     "13"  //openwrt update failed
#define INTOROBOT_MQTT_RESMES_OW_FWUPSUCC     "14"  //openwrt update success
#define INTOROBOT_MQTT_RESMES_OW_FWUPROCESS   "15"  //openwrt update process
#define INTOROBOT_MQTT_RESMES_OW_FWUPTYPEEEOR "16"  //openwrt update type error

//device debug info
#define INTOROBOT_MQTT_SENDBUGTOPIC   "firmware/default/info/debug"
#define INTOROBOT_MQTT_SENDDEBUGQOS   0
//subscribe
//stm32 firmware update
#define INTOROBOT_MQTT_ST_UPDATETOPIC   "firmware/default/action/flash"
#define INTOROBOT_MQTT_ST_UPDATEQOS     0
//openwrt firmware update
#define INTOROBOT_MQTT_OW_UPDATETOPIC   "platform/default/action/json"
#define INTOROBOT_MQTT_OW_UPDATEQOS     0
//time update
#define INTOROBOT_MQTT_TIMETOPIC   "platform/default/info/time"
#define INTOROBOT_MQTT_TIMEQOS     0
//token update
#define INTOROBOT_MQTT_TOKENTOPIC   "token/default/action/flash"
#define INTOROBOT_MQTT_TOKENQOS     0
//reset update
#define INTOROBOT_MQTT_RESETTOPIC   "firmware/default/action/reset"
#define INTOROBOT_MQTT_RESETQOS     0
//receive debug
#define INTOROBOT_MQTT_RECDEBUGTOPIC   "firmware/default/action/debug"
#define INTOROBOT_MQTT_RECDEBUGQOS     0


typedef void (*CB)(uint8_t*, uint32_t);

typedef enum
{
    MODE_AUTOMATIC = 0, MODE_SEMI_AUTOMATIC = 1, MODE_MANUAL = 2
} System_Mode_TypeDef;


extern volatile uint8_t intorobot_cloud_connect_flag;
extern volatile uint8_t intorobot_cloud_socketed_flag;
extern volatile uint8_t intorobot_cloud_connected_flag;


#define INTOROBOT_WIFICHECK_FAIL_PERIOD_MILLIS	        1000
#define INTOROBOT_WIFICHECK_SUCC_PERIOD_MILLIS          30000      //*30sec

#define INTOROBOT_MQTTCONNECT_SHORT_PERIOD_MILLIS       1000
#define INTOROBOT_MQTTCONNECT_LONG_PERIOD_MILLIS        15000      //*15sec

#define INTOROBOT_SHORT_TO_LONG_MAC_COUNT			    20




#define SYSTEM_MODE(mode)  SystemClass SystemMode(mode);


#define MAX_CALLBACK_NUM    32

class WidgetBaseClass
{
    public:
        virtual void begin(void (*UserCallBack)(void));
        virtual void widgetBaseCallBack(uint8_t *payload, uint32_t len);
};

struct _callbacknode
{
    void (*callback)(uint8_t *payload, uint32_t len);
    uint8_t qos;
    const char *topic;
    const char *device_id;
};

struct _wcallbacknode
{
    WidgetBaseClass *pWidgetBase;
    uint8_t qos;
    const char *topic;
    const char *device_id;
};

struct _callbacklist
{
    struct _callbacknode callbacknode[MAX_CALLBACK_NUM];
    struct _wcallbacknode wcallbacknode[MAX_CALLBACK_NUM];
    int total_callbacks;
    int total_wcallbacks;
};


class SystemClass
{
    private:
        static System_Mode_TypeDef _mode;

    public:
        SystemClass();
        SystemClass(System_Mode_TypeDef mode);
        static System_Mode_TypeDef mode(void);
};


#define Cloud_DEBUG_BUFFER_SIZE 128

typedef struct
{
    unsigned char buffer[Cloud_DEBUG_BUFFER_SIZE];
    volatile unsigned int head;
    volatile unsigned int tail;
} Cloud_Debug_Buffer;


class IntorobotClass: public Print
{
    private:
        Cloud_Debug_Buffer  Debug_tx_buffer;
        Cloud_Debug_Buffer  Debug_rx_buffer;

        TcpClient mqtttcpclient;
        MqttClientClass ApiMqttClient;

        void sendDebug(void);
        void fill_mqtt_topic(String &fulltopic, const char *topic, const char *device_id);

        virtual size_t write(uint8_t byte);
        using Print::write; // pull in write(str) and write(buf, size) from Print

    public:
        IntorobotClass();
        bool connected(void);
        void connect(void);
        void disconnect(void);
        uint8_t publish(const char *topic, bool payload);
        uint8_t publish(const char *topic, char payload);
        uint8_t publish(const char *topic, uint8_t payload);
        uint8_t publish(const char *topic, short payload);
        uint8_t publish(const char *topic, unsigned short payload);
        uint8_t publish(const char *topic, int payload );
        uint8_t publish(const char *topic, unsigned int payload);
        uint8_t publish(const char *topic, long payload);
        uint8_t publish(const char *topic, unsigned long payload);
        uint8_t publish(const char *topic, float payload);
        uint8_t publish(const char *topic, double payload);
        uint8_t publish(const char *topic, String payload);
        uint8_t publish(const char *topic, char *payload);
        uint8_t publish(const char* topic, uint8_t* payload, unsigned int plength);
        uint8_t publish(const char* topic, uint8_t* payload, unsigned int plength, uint8_t retained);
        uint8_t publish(const char* topic, uint8_t* payload, unsigned int plength, uint8_t qos, uint8_t retained);
        uint8_t subscribe(const char* topic, const char *device_id, void (*callback)(uint8_t*, uint32_t));
        uint8_t subscribe(const char* topic, const char *device_id, void (*callback)(uint8_t*, uint32_t), uint8_t qos);
        uint8_t subscribe(const char* topic, const char *device_id, WidgetBaseClass *pWidgetBase);
        uint8_t subscribe(const char* topic, const char *device_id, WidgetBaseClass *pWidgetBase, uint8_t qos);
        uint8_t unsubscribe(const char *topic, const char *device_id);
        int deviceInfo(char *product_id, char *device_id, char *access_token, char *device_sn);
        void syncTime(void);
        void process(void);
        int read(void);
        int available(void);

        //not for user.
        void receiveDebug(uint8_t *pIn, uint32_t len);
};

extern IntorobotClass IntoRobot;

#ifdef __cplusplus
extern "C" {
#endif

String clientId(void);
String deviceSn(void);
//subscribe topic info
CB getsubcallback(char * fulltopic);
WidgetBaseClass *getsubwcallback(char * fulltopic);
void addsubcallback(char *topic, char *device_id, void (*callback)(uint8_t*, uint32_t), uint8_t qos);
void addsubwcallback(char *topic, char *device_id, WidgetBaseClass *pWidgetBase, uint8_t qos);
void delsubcallback(char * topic, char *device_id);
void resubscribe(void);

void apiMqttClientCallBack(char *topic, uint8_t *payload, uint32_t length);
void syncTimeCallback(unsigned char * payload, uint32_t len);
void stm32FwUpdateCallback(unsigned char * payload, uint32_t len);
void openwrtFwUpdateCallback(unsigned char * payload, uint32_t len);
void tokenUpdateCallback(uint8_t *payload, uint32_t len);
void systemResetCallback(uint8_t *payload, uint32_t len);
void systemDebugCallback(uint8_t *payload, uint32_t len);

bool start_imlink_config();
bool end_imlink_config();
bool system_rgb_color(uint8_t red, uint8_t green, uint8_t blue);
bool system_rgb_blink(uint8_t red, uint8_t green, uint8_t blue, uint16_t period);
bool system_rgb_breath(uint8_t red, uint8_t green, uint8_t blue, uint16_t period);


void intorobot_deviceconfig(void);
void intorobot_info_reset(void);
void intorobot_info_debug(void);
void intorobot_loop(void);
void intorobot_init(void);


void setup() __attribute__ ((weak));
void loop() __attribute__ ((weak));

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif /*API_H_*/

