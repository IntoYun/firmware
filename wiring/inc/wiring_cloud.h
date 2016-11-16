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

#ifndef WIRING_CLOUD_H_
#define WIRING_CLOUD_H_

#include "intorobot_config.h"
#ifndef configNO_CLOUD

#include <stdio.h>
#include "wiring_string.h"
//#include "events.h"
#include "system_cloud.h"
#include "system_sleep.h"
//#include "wiring_system.h"
#include "wiring_watchdog.h"
#include "interrupts_hal.h"
#include "string_convert.h"
#include <functional>
#include "service_debug.h"

#ifdef INTOROBOT_NO_CLOUD
#define CLOUD_FN(x,y) (y)
#else
#define CLOUD_FN(x,y) (x)
#endif

class CloudClass: public Print{

    public:

        static uint8_t publish(const char *topic, char payload)
        {
            return publish(topic, (uint8_t)payload);
        }
        static uint8_t publish(const char *topic, uint8_t payload)
        {
            String tmp;
            tmp += payload;
            return publish(topic, (uint8_t *)tmp.c_str(), strlen(tmp.c_str()), 0, true);
        }
        static uint8_t publish(const char *topic, short payload)
        {
            String tmp;
            tmp += payload;
            return publish(topic, (uint8_t *)tmp.c_str(), strlen(tmp.c_str()), 0, true);
        }
        static uint8_t publish(const char *topic, unsigned short payload)
        {
            String tmp;
            tmp += payload;
            return publish(topic, (uint8_t *)tmp.c_str(), strlen(tmp.c_str()), 0, true);
        }
        static uint8_t publish(const char *topic, int payload )
        {
            String tmp;
            tmp += payload;
            return publish(topic, (uint8_t *)tmp.c_str(), strlen(tmp.c_str()), 0, true);
        }
        static uint8_t publish(const char *topic, unsigned int payload)
        {
            String tmp;
            tmp += payload;
            return publish(topic, (uint8_t *)tmp.c_str(), strlen(tmp.c_str()), 0, true);
        }
        static uint8_t publish(const char *topic, long payload)
        {
            String tmp;
            tmp += payload;
            return publish(topic, (uint8_t *)tmp.c_str(), strlen(tmp.c_str()), 0, true);
        }

        static uint8_t publish(const char *topic, unsigned long payload)
        {
            String tmp;
            tmp += payload;
            return publish(topic, (uint8_t *)tmp.c_str(), strlen(tmp.c_str()), 0, true);
        }

        static uint8_t publish(const char *topic, float payload)
        {
            char buf[32];
            char *string = dtostrf(payload, 4, 2, buf);
            DEBUG_D("topic :%s", string);
            return publish(topic, (uint8_t *)string, strlen(string), 0, true);
        }

        static uint8_t publish(const char *topic, double payload)
        {
            char buf[32];
            char *string = dtostrf(payload, 4, 2, buf);
            DEBUG_D("topic :%s", string);
            return publish(topic, (uint8_t *)string, strlen(string), 0, true);
        }

        static uint8_t publish(const char *topic, String payload)
        {
            return publish(topic, (uint8_t *)payload.c_str(), strlen(payload.c_str()), 0, true);
        }

        static uint8_t publish(const char *topic, char *payload)
        {
            return publish(topic, (uint8_t *)payload, strlen(payload), 0, true);
        }

        static uint8_t publish(const char *topic, uint8_t *payload, unsigned int plength)
        {
            return publish(topic, payload, plength, 0, true);
        }

        static uint8_t publish(const char *topic, uint8_t *payload, unsigned int plength, uint8_t retained)
        {
            return publish(topic, payload, plength, 0, retained);
        }

        static uint8_t publish(const char *topic, uint8_t *payload, unsigned int plength, uint8_t qos, uint8_t retained)
        {
            return intorobot_publish(topic, payload, plength, 0, retained);
        }

        static uint8_t subscribe(const char *topic, const char *device_id, void (*callback)(uint8_t*, uint32_t))
        {
            return subscribe(topic, device_id, callback, 0);
        }

        static uint8_t subscribe(const char *topic, const char *device_id, void (*callback)(uint8_t*, uint32_t), uint8_t qos)
        {
            return intorobot_subscribe(topic, device_id, callback, 0);
        }

        static uint8_t subscribe(const char *topic, const char *device_id, WidgetBaseClass *pWidgetBase)
        {
            return subscribe(topic, device_id, pWidgetBase, 0);
        }

        static uint8_t subscribe(const char *topic, const char *device_id, WidgetBaseClass *pWidgetBase, uint8_t qos)
        {
            return intorobot_widget_subscribe(topic, device_id, pWidgetBase, 0);
        }

        static uint8_t unsubscribe(const char *topic, const char *device_id)
        {
            return intorobot_unsubscribe(topic, device_id);
        }

        static void syncTime(void)
        {
            return intorobot_sync_time();
        }

        static bool connected(void) { return intorobot_cloud_flag_connected(); }
        static bool disconnected(void) { return !connected(); }
        static void connect(void) { intorobot_cloud_flag_connect(); }
        static void disconnect(void) { intorobot_cloud_flag_disconnect(); }
        static void process(void) {
            //application_checkin();
            intorobot_process();
        }
        static String deviceID(void) { return intorobot_deviceID(); }

        static int read(void)
        {
            return intorobot_debug_info_read();
        }
        static int available(void)
        {
            return intorobot_debug_info_available();
        }

    private:
        virtual size_t write(uint8_t byte);
        using Print::write; // pull in write(str) and write(buf, size) from Print
};

extern CloudClass IntoRobot;

#endif

#endif
