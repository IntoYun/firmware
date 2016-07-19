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

#ifndef SYSTEM_CLOUD_H_
#define SYSTEM_CLOUD_H_

#include "static_assert.h"
#include "wiring_string.h"
#include <string.h>
#include <time.h>
#include <stdint.h>


#define MAX_CALLBACK_NUM    32

typedef void (*pCallBack)(uint8_t*, uint32_t);

class WidgetBaseClass
{
public:
    virtual void begin(void (*UserCallBack)(void));
    virtual void widgetBaseCallBack(uint8_t *payload, uint32_t len);
};

struct CallBackNode
{
    void (*callback)(uint8_t*, uint32_t);
    uint8_t qos;
    const char *topic;
    const char *device_id;
};

struct WidgetCallBackNode
{
    WidgetBaseClass *pWidgetBase;
    uint8_t qos;
    const char *topic;
    const char *device_id;
};

struct CallBackList
{
    struct CallBackNode callback_node[MAX_CALLBACK_NUM];
    struct WidgetCallBackNode widget_callback_node[MAX_CALLBACK_NUM];
    int total_callbacks;
    int total_wcallbacks;
};

#define CLOUD_DEBUG_BUFFER_SIZE 128

struct CloudDebugBuffer
{
    unsigned char buffer[CLOUD_DEBUG_BUFFER_SIZE];
    volatile unsigned int head;
    volatile unsigned int tail;
};


#ifdef __cplusplus
extern "C" {
#endif

void intorobot_cloud_flag_connect(void);
void intorobot_cloud_flag_disconnect(void);
bool intorobot_cloud_flag_auto_connect();
bool intorobot_cloud_flag_connected(void);
void intorobot_process(void);
String intorobot_deviceID(void);
uint8_t intorobot_publish(const char* topic, uint8_t* payload, unsigned int plength, uint8_t qos, uint8_t retained);
uint8_t intorobot_subscribe(const char* topic, const char *device_id, void (*callback)(uint8_t*, uint32_t), uint8_t qos);
uint8_t intorobot_widget_subscribe(const char* topic, const char *device_id, WidgetBaseClass *pWidgetBase, uint8_t qos);
uint8_t intorobot_unsubscribe(const char *topic, const char *device_id);
void intorobot_sync_time(void);
size_t intorobot_debug_info_write(uint8_t byte);
int intorobot_debug_info_read(void);
int intorobot_debug_info_available(void);

void cloud_disconnect(bool closeSocket=true);



#ifdef __cplusplus
}
#endif

#endif	/* SYSTEM_CLOUD_H_ */
