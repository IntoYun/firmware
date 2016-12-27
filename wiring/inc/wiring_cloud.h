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
            return intorobot_publish(API_VERSION_V1, topic, payload, plength, 0, retained);
        }

        static uint8_t subscribe(const char *topic, const char *device_id, void (*callback)(uint8_t*, uint32_t))
        {
            return subscribe(topic, device_id, callback, 0);
        }

        static uint8_t subscribe(const char *topic, const char *device_id, void (*callback)(uint8_t*, uint32_t), uint8_t qos)
        {
            return intorobot_subscribe(API_VERSION_V1, topic, device_id, callback, 0);
        }

        static uint8_t subscribe(const char *topic, const char *device_id, WidgetBaseClass *pWidgetBase)
        {
            return subscribe(topic, device_id, pWidgetBase, 0);
        }

        static uint8_t subscribe(const char *topic, const char *device_id, WidgetBaseClass *pWidgetBase, uint8_t qos)
        {
            return intorobot_widget_subscribe(API_VERSION_V1, topic, device_id, pWidgetBase, 0);
        }

        static uint8_t unsubscribe(const char *topic, const char *device_id)
        {
            return intorobot_unsubscribe(API_VERSION_V1, topic, device_id);
        }

        //添加通用数据收发接口
        static void addDataPointBool(const uint16_t dpID, const char *permission)
        {
            addExternalDataPointBool(NULL, dpID, permission, false, "", 0);
        }

        static void addDataPointBool(const uint16_t dpID, const char *permission, const bool value)
        {
            addExternalDataPointBool(NULL, dpID, permission, value, "", 0);
        }

        static void addDataPointBool(const uint16_t dpID, const char *permission, const bool value, const char *policy)
        {
            addExternalDataPointBool(NULL, dpID, permission, value, policy, 0);
        }

        static void addDataPointBool(const uint16_t dpID, const char *permission, const bool value, const char *policy, int lapse)
        {
            addExternalDataPointBool(NULL, dpID, permission, value, policy, lapse);
        }

        static void addDataPointDouble(const uint16_t dpID, const char *permission, const double minValue, const double maxValue, const double Resolution)
        {
            addExternalDataPointDouble(NULL, dpID, permission, minValue, maxValue, Resolution, minValue, "", 0);
        }

        static void addDataPointDouble(const uint16_t dpID, const char *permission, const double minValue, const double maxValue, const double Resolution, const double value)
        {
            addExternalDataPointDouble(NULL, dpID, permission, minValue, maxValue, Resolution, value, "", 0);
        }

        static void addDataPointDouble(const uint16_t dpID, const char *permission, const double minValue, const double maxValue, const double Resolution, const double value, const char *policy)
        {
            addExternalDataPointDouble(NULL, dpID, permission, minValue, maxValue, Resolution, value, policy, 0);
        }

        static void addDataPointDouble(const uint16_t dpID, const char *permission, const double minValue, const double maxValue, const double Resolution, const double value, const char *policy, int lapse)
        {
            addExternalDataPointDouble(NULL, dpID, permission, minValue, maxValue, Resolution, value, policy, lapse);
        }

        static void addDataPointEnum(const uint16_t dpID, const char *permission)
        {
            addExternalDataPointEnum(NULL, dpID, permission, 0, "", 0);
        }

        static void addDataPointEnum(const uint16_t dpID, const char *permission, const int value)
        {
            addExternalDataPointEnum(NULL, dpID, permission, value, "", 0);
        }

        static void addDataPointEnum(const uint16_t dpID, const char *permission, const int value, const char *policy)
        {
            addExternalDataPointEnum(NULL, dpID, permission, value, policy, 0);
        }

        static void addDataPointEnum(const uint16_t dpID, const char *permission, const int value, const char *policy, int lapse)
        {
            addExternalDataPointEnum(NULL, dpID, permission, value, policy, lapse);
        }

        static void addDataPointString(const uint16_t dpID, const char *permission)
        {
            addExternalDataPointString(NULL, dpID, permission, "", "", 0);
        }

        static void addDataPointString(const uint16_t dpID, const char *permission, const char *value)
        {
            addExternalDataPointString(NULL, dpID, permission, value, "", 0);
        }

        static void addDataPointString(const uint16_t dpID, const char *permission, const char *value, const char *policy)
        {
            addExternalDataPointString(NULL, dpID, permission, value, policy, 0);
        }

        static void addDataPointString(const uint16_t dpID, const char *permission, const char *value, const char *policy, int lapse)
        {
            addExternalDataPointString(NULL, dpID, permission, value, policy, lapse);
        }

        static void addDataPointBinary(const uint16_t dpID, const char *permission)
        {
            addExternalDataPointBinary(NULL, dpID, permission, NULL, 0, "", 0);
        }

        static void addDataPointBinary(const uint16_t dpID, const char *permission, const uint8_t *value, const uint16_t len)
        {
            addExternalDataPointBinary(NULL, dpID, permission, value, len, "", 0);
        }

        static void addDataPointBinary(const uint16_t dpID, const char *permission, const uint8_t *value, const uint16_t len, const char *policy)
        {
            addExternalDataPointBinary(NULL, dpID, permission, value, len, policy, 0);
        }

        static void addDataPointBinary(const uint16_t dpID, const char *permission, const uint8_t *value, const uint16_t len, const char *policy, int lapse)
        {
            addExternalDataPointBinary(NULL, dpID, permission, value, len, policy, lapse);
        }

        static void addExternalDataPointBool(const char *device_id, const uint16_t dpID, const char *permission, const bool value, const char *policy, int lapse)
        {

        }

        static void addExternalDataPointDouble(const char *device_id, const uint16_t dpID, const char *permission, const double minValue, const double maxValue, const double Resolution, const double value, const char *policy, int lapse)
        {

        }

        static void addExternalDataPointEnum(const char *device_id, const uint16_t dpID, const char *permission, const int value, const char *policy, int lapse)
        {

        }

        static void addExternalDataPointString(const char *device_id, const uint16_t dpID, const char *permission, const char *value, const char *policy, int lapse)
        {

        }

        static void addExternalDataPointBinary(const char *device_id, const uint16_t dpID, const char *permission, const uint8_t *value, const uint16_t len, const char *policy, int lapse)
        {

        }

        bool readDataPoint(const uint16_t dpID, bool &value)
        {
            return readDataPoint(NULL, dpID, value);
        }

        bool readDataPoint(const uint16_t dpID, int &value)
        {
            return readDataPoint(NULL, dpID, value);
        }

        bool readDataPoint(const uint16_t dpID, double &value)
        {
            return readDataPoint(NULL, dpID, value);
        }

        bool readDataPoint(const uint16_t dpID, float &value)
        {
            return readDataPoint(NULL, dpID, value);
        }

        bool readDataPoint(const uint16_t dpID, char *value)
        {
            return readDataPoint(NULL, dpID, value);
        }

        bool readDataPoint(const uint16_t dpID, uint8_t *value, uint16_t &len)
        {
            return readDataPoint(NULL, dpID, value, len);
        }

        bool readDataPoint(const char *device_id, const uint16_t dpID, bool &value)
        {
            return readDataPoint(device_id, dpID, value);
        }

        bool readDataPoint(const char *device_id, const uint16_t dpID, int &value)
        {
            return readDataPoint(device_id, dpID, value);
        }

        bool readDataPoint(const char *device_id, const uint16_t dpID, double &value)
        {
            return readDataPoint(device_id, dpID, value);
        }

        bool readDataPoint(const char *device_id, const uint16_t dpID, float &value)
        {
            return readDataPoint(device_id, dpID, value);
        }

        bool readDataPoint(const char *device_id, const uint16_t dpID, char *value)
        {
            return readDataPoint(device_id, dpID, value);
        }

        bool readDataPoint(const char *device_id, const uint16_t dpID, uint8_t *value, uint16_t &len)
        {
            return readDataPoint(device_id, dpID, value, len);
        }

        void writeDataPoint(const uint16_t dpID, bool value)
        {
            writeDataPoint(NULL, dpID, value);
        }

        void writeDataPoint(const uint16_t dpID, int value)
        {
            writeDataPoint(NULL, dpID, value);
        }

        void writeDataPoint(const uint16_t dpID, double value)
        {
            writeDataPoint(NULL, dpID, value);
        }

        void writeDataPoint(const uint16_t dpID, float value)
        {
            writeDataPoint(NULL, dpID, value);
        }

        void writeDataPoint(const uint16_t dpID, const char *value)
        {
            writeDataPoint(NULL, dpID, value);
        }

        void writeDataPoint(const uint16_t dpID, const uint8_t *value, uint16_t &len)
        {
            writeDataPoint(NULL, dpID, value, len);
        }

        void writeDataPoint(const char *device_id, const uint16_t dpID, bool value)
        {

        }

        void writeDataPoint(const char *device_id, const uint16_t dpID, int value)
        {

        }

        void writeDataPoint(const char *device_id, const uint16_t dpID, double value)
        {

        }

        void writeDataPoint(const char *device_id, const uint16_t dpID, float value)
        {

        }

        void writeDataPoint(const char *device_id, const uint16_t dpID, const char *value)
        {

        }

        void writeDataPoint(const char *device_id, const uint16_t dpID, const uint8_t *value, uint16_t &len)
        {

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
