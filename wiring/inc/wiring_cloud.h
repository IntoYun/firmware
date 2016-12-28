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
#include "system_clouddata.h"
#include "system_sleep.h"
//#include "wiring_system.h"
#include "wiring_watchdog.h"
#include "interrupts_hal.h"
#include "string_convert.h"
#include <functional>
#include "service_debug.h"

class CloudClass: public Print{

    public:

        uint8_t publish(const char *topic, char payload)
        {
            return publish(topic, (uint8_t)payload);
        }

        uint8_t publish(const char *topic, uint8_t payload)
        {
            String tmp;
            tmp += payload;
            return publish(topic, (uint8_t *)tmp.c_str(), strlen(tmp.c_str()), 0, true);
        }

        uint8_t publish(const char *topic, short payload)
        {
            String tmp;
            tmp += payload;
            return publish(topic, (uint8_t *)tmp.c_str(), strlen(tmp.c_str()), 0, true);
        }

        uint8_t publish(const char *topic, unsigned short payload)
        {
            String tmp;
            tmp += payload;
            return publish(topic, (uint8_t *)tmp.c_str(), strlen(tmp.c_str()), 0, true);
        }

        uint8_t publish(const char *topic, int payload )
        {
            String tmp;
            tmp += payload;
            return publish(topic, (uint8_t *)tmp.c_str(), strlen(tmp.c_str()), 0, true);
        }

        uint8_t publish(const char *topic, unsigned int payload)
        {
            String tmp;
            tmp += payload;
            return publish(topic, (uint8_t *)tmp.c_str(), strlen(tmp.c_str()), 0, true);
        }

        uint8_t publish(const char *topic, long payload)
        {
            String tmp;
            tmp += payload;
            return publish(topic, (uint8_t *)tmp.c_str(), strlen(tmp.c_str()), 0, true);
        }

        uint8_t publish(const char *topic, unsigned long payload)
        {
            String tmp;
            tmp += payload;
            return publish(topic, (uint8_t *)tmp.c_str(), strlen(tmp.c_str()), 0, true);
        }

        uint8_t publish(const char *topic, float payload)
        {
            char buf[32];
            char *string = dtostrf(payload, 4, 2, buf);
            return publish(topic, (uint8_t *)string, strlen(string), 0, true);
        }

        uint8_t publish(const char *topic, double payload)
        {
            char buf[32];
            char *string = dtostrf(payload, 4, 2, buf);
            return publish(topic, (uint8_t *)string, strlen(string), 0, true);
        }

        uint8_t publish(const char *topic, String payload)
        {
            return publish(topic, (uint8_t *)payload.c_str(), strlen(payload.c_str()), 0, true);
        }

        uint8_t publish(const char *topic, char *payload)
        {
            return publish(topic, (uint8_t *)payload, strlen(payload), 0, true);
        }

        uint8_t publish(const char *topic, uint8_t *payload, unsigned int plength)
        {
            return publish(topic, payload, plength, 0, true);
        }

        uint8_t publish(const char *topic, uint8_t *payload, unsigned int plength, uint8_t retained)
        {
            return publish(topic, payload, plength, 0, retained);
        }

        uint8_t publish(const char *topic, uint8_t *payload, unsigned int plength, uint8_t qos, uint8_t retained)
        {
            return intorobot_publish(API_VERSION_V1, topic, payload, plength, 0, retained);
        }

        uint8_t subscribe(const char *topic, const char *deviceID, void (*callback)(uint8_t*, uint32_t))
        {
            return subscribe(topic, deviceID, callback, 0);
        }

        uint8_t subscribe(const char *topic, const char *deviceID, void (*callback)(uint8_t*, uint32_t), uint8_t qos)
        {
            return intorobot_subscribe(API_VERSION_V1, topic, deviceID, callback, 0);
        }

        uint8_t subscribe(const char *topic, const char *deviceID, WidgetBaseClass *pWidgetBase)
        {
            return subscribe(topic, deviceID, pWidgetBase, 0);
        }

        uint8_t subscribe(const char *topic, const char *deviceID, WidgetBaseClass *pWidgetBase, uint8_t qos)
        {
            return intorobot_widget_subscribe(API_VERSION_V1, topic, deviceID, pWidgetBase, 0);
        }

        uint8_t unsubscribe(const char *topic, const char *deviceID)
        {
            return intorobot_unsubscribe(API_VERSION_V1, topic, deviceID);
        }

        //添加通用数据收发接口
        void addDataPointBool(const uint16_t dpID, const char *permission)
        {
            addExternalDataPointBool(NULL, dpID, permission, false, "", 0);
        }

        void addDataPointBool(const uint16_t dpID, const char *permission, bool value)
        {
            addExternalDataPointBool(NULL, dpID, permission, value, "", 0);
        }

        void addDataPointBool(const uint16_t dpID, const char *permission, bool value, const char *policy)
        {
            addExternalDataPointBool(NULL, dpID, permission, value, policy, 0);
        }

        void addDataPointBool(const uint16_t dpID, const char *permission, bool value, const char *policy, int lapse)
        {
            addExternalDataPointBool(NULL, dpID, permission, value, policy, lapse);
        }

        void addDataPointNumber(const uint16_t dpID, const char *permission, const double minValue, const double maxValue, const double Resolution)
        {
            addExternalDataPointNumber(NULL, dpID, permission, minValue, maxValue, Resolution, minValue, "", 0);
        }

        void addDataPointNumber(const uint16_t dpID, const char *permission, const double minValue, const double maxValue, const double Resolution, double value)
        {
            addExternalDataPointNumber(NULL, dpID, permission, minValue, maxValue, Resolution, value, "", 0);
        }

        void addDataPointNumber(const uint16_t dpID, const char *permission, const double minValue, const double maxValue, const double Resolution, double value, const char *policy)
        {
            addExternalDataPointNumber(NULL, dpID, permission, minValue, maxValue, Resolution, value, policy, 0);
        }

        void addDataPointNumber(const uint16_t dpID, const char *permission, const double minValue, const double maxValue, const double Resolution, double value, const char *policy, int lapse)
        {
            addExternalDataPointNumber(NULL, dpID, permission, minValue, maxValue, Resolution, value, policy, lapse);
        }

        void addDataPointEnum(const uint16_t dpID, const char *permission)
        {
            addExternalDataPointEnum(NULL, dpID, permission, 0, "", 0);
        }

        void addDataPointEnum(const uint16_t dpID, const char *permission, int value)
        {
            addExternalDataPointEnum(NULL, dpID, permission, value, "", 0);
        }

        void addDataPointEnum(const uint16_t dpID, const char *permission, int value, const char *policy)
        {
            addExternalDataPointEnum(NULL, dpID, permission, value, policy, 0);
        }

        void addDataPointEnum(const uint16_t dpID, const char *permission, int value, const char *policy, int lapse)
        {
            addExternalDataPointEnum(NULL, dpID, permission, value, policy, lapse);
        }

        void addDataPointString(const uint16_t dpID, const char *permission)
        {
            addExternalDataPointString(NULL, dpID, permission, "", "", 0);
        }

        void addDataPointString(const uint16_t dpID, const char *permission, char *value)
        {
            addExternalDataPointString(NULL, dpID, permission, value, "", 0);
        }

        void addDataPointString(const uint16_t dpID, const char *permission, char *value, const char *policy)
        {
            addExternalDataPointString(NULL, dpID, permission, value, policy, 0);
        }

        void addDataPointString(const uint16_t dpID, const char *permission, char *value, const char *policy, int lapse)
        {
            addExternalDataPointString(NULL, dpID, permission, value, policy, lapse);
        }

        void addDataPointBinary(const uint16_t dpID, const char *permission)
        {
            addExternalDataPointBinary(NULL, dpID, permission, NULL, 0, "", 0);
        }

        void addDataPointBinary(const uint16_t dpID, const char *permission, uint8_t *value, const uint16_t len)
        {
            addExternalDataPointBinary(NULL, dpID, permission, value, len, "", 0);
        }

        void addDataPointBinary(const uint16_t dpID, const char *permission, uint8_t *value, uint16_t len, const char *policy)
        {
            addExternalDataPointBinary(NULL, dpID, permission, value, len, policy, 0);
        }

        void addDataPointBinary(const uint16_t dpID, const char *permission, uint8_t *value, uint16_t len, const char *policy, int lapse)
        {
            addExternalDataPointBinary(NULL, dpID, permission, value, len, policy, lapse);
        }

        void addExternalDataPointBool(const char *other_device_id, const uint16_t dpID, const char *permission, bool value, const char *policy, int lapse)
        {
            intorobotAddExternalDataPointBool(other_device_id, dpID, permission, value, policy, lapse);
        }

        void addExternalDataPointNumber(const char *other_device_id, const uint16_t dpID, const char *permission, const double minValue, const double maxValue, const double resolution, const double value, const char *policy, int lapse)
        {
            intorobotAddExternalDataPointNumber(other_device_id, dpID, permission, minValue, maxValue, resolution, value, policy, lapse);
        }

        void addExternalDataPointEnum(const char *other_device_id, const uint16_t dpID, const char *permission, int value, const char *policy, int lapse)
        {
            intorobotAddExternalDataPointEnum(other_device_id, dpID, permission, value, policy, lapse);
        }

        void addExternalDataPointString(const char *other_device_id, const uint16_t dpID, const char *permission, char *value, const char *policy, int lapse)
        {
            intorobotAddExternalDataPointString(other_device_id, dpID, permission, value, policy, lapse);
        }

        void addExternalDataPointBinary(const char *other_device_id, const uint16_t dpID, const char *permission, uint8_t *value, uint16_t len, const char *policy, int lapse)
        {
            intorobotAddExternalDataPointBinary(other_device_id, dpID, permission, value, len, policy, lapse);
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

        bool readDataPoint(const char *other_device_id, const uint16_t dpID, bool &value)
        {
            return intorobotReadDataPointBool(other_device_id, dpID, value);
        }

        bool readDataPoint(const char *other_device_id, const uint16_t dpID, int &value)
        {
            return intorobotReadDataPointInt(other_device_id, dpID, value);
        }

        bool readDataPoint(const char *other_device_id, const uint16_t dpID, double &value)
        {
            return intorobotReadDataPointDouble(other_device_id, dpID, value);
        }

        bool readDataPoint(const char *other_device_id, const uint16_t dpID, float &value)
        {
            return intorobotReadDataPointFloat(other_device_id, dpID, value);
        }

        bool readDataPoint(const char *other_device_id, const uint16_t dpID, char *value)
        {
            return intorobotReadDataPointString(other_device_id, dpID, value);
        }

        bool readDataPoint(const char *other_device_id, const uint16_t dpID, uint8_t *value, uint16_t &len)
        {
            return intorobotReadDataPointBinary(other_device_id, dpID, value, len);
        }

        void writeDataPoint(const uint16_t dpID, bool value)
        {
            writeDataPoint(dpID, String(value).c_str());
        }

        void writeDataPoint(const uint16_t dpID, int value)
        {
            writeDataPoint(dpID, String(value).c_str());
        }

        void writeDataPoint(const uint16_t dpID, double value)
        {
            writeDataPoint(dpID, String(value).c_str());
        }

        void writeDataPoint(const uint16_t dpID, float value)
        {
            writeDataPoint(dpID, String(value).c_str());
        }

        void writeDataPoint(const uint16_t dpID, String value)
        {
            writeDataPoint(dpID, value.c_str());
        }

        void writeDataPoint(const uint16_t dpID, char *value)
        {
            intorobotWriteDataPointString(dpID, value);
        }

        void writeDataPoint(const uint16_t dpID, uint8_t *value, uint16_t len)
        {
            intorobotWriteDataPointBinary(dpID, value, len);
        }

        void writeDataPoints(void)
        {
            intorobotWriteDataPoints();
        }

        void syncTime(void)
        {
            return intorobot_sync_time();
        }

        bool connected(void) { return intorobot_cloud_flag_connected(); }
        bool disconnected(void) { return !connected(); }
        void connect(void) { intorobot_cloud_flag_connect(); }
        void disconnect(void) { intorobot_cloud_flag_disconnect(); }
        void process(void) {
            //application_checkin();
            intorobot_process();
        }
        String deviceID(void) { return intorobot_deviceID(); }

        int read(void)
        {
            return intorobot_debug_info_read();
        }
        int available(void)
        {
            return intorobot_debug_info_available();
        }

    private:
        virtual size_t write(uint8_t byte)
        {return intorobot_debug_info_write(byte);}

        using Print::write; // pull in write(str) and write(buf, size) from Print
};

extern CloudClass IntoRobot;

#endif

#endif
