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

#ifndef SYSTEM_CLOUDDATA_H_
#define SYSTEM_CLOUDDATA_H_

#include "intorobot_config.h"

#include <string.h>
#include <stdint.h>
#include "static_assert.h"
#include "wiring_string.h"

#ifndef configNO_CLOUD

// Permission
#define JSON_DATA_FORMAT    0
#define BINARY_DATA_FORMAT  1

// Permission
#define UP_ONLY    "up_only"
#define DOWN_ONLY  "down_only"
#define UP_DOWN    "up_down"

// Polices
#define TIMED      "timed"
#define ON_CHANGE  "on_change"

#define PROPERTIES_MAX 50

typedef enum{
    DATA_TYPE_BOOL = 0,   //bool型
    DATA_TYPE_INT,        //数值型 int型
    DATA_TYPE_FLOAT,      //数值型 float型
    DATA_TYPE_ENUM,       //枚举型
    DATA_TYPE_STRING,     //字符串型
    DATA_TYPE_BINARY      //透传型
}data_type_t;

typedef enum{
    RESULT_DATAPOINT_OLD  = 0,   // 旧数据
    RESULT_DATAPOINT_NEW  = 1,   // 新收取数据
    RESULT_DATAPOINT_NONE = 2,   // 没有该数据点
}read_datapoint_result_t;

//int型属性
struct int_property_t{
    int minValue;
    int maxValue;
    int resolution;
};

//float型属性
struct float_property_t{
    double minValue;
    double maxValue;
    double resolution;
};

//透传型属性
struct binary_property_t{
    const uint8_t *value;
    uint16_t len;
};

// Property configuration
struct property_conf {
    const uint16_t dpID;
    const data_type_t dataType;
    const char *permission;
    const char *policy;
    long lapse;
    long runtime;
    read_datapoint_result_t readFlag;
    union
    {
        int_property_t intProperty;
        float_property_t floatProperty;
    };
    String value;
    uint8_t *binaryValue;
    uint16_t binaryLen;
};

#ifdef __cplusplus
extern "C" {
#endif

void intorobotAddDataPointBool(const uint16_t dpID, const char *permission, bool value, const char *policy, int lapse);
void intorobotAddDataPointNumber(const uint16_t dpID, const char *permission, const double minValue, const double maxValue, const double resolution, double value, const char *policy, int lapse);
void intorobotAddDataPointEnum(const uint16_t dpID, const char *permission, int value, const char *policy, int lapse);
void intorobotAddDataPointString(const uint16_t dpID, const char *permission, char *value, const char *policy, int lapse);
void intorobotAddDataPointBinary(const uint16_t dpID, const char *permission, uint8_t *value, uint16_t len, const char *policy, int lapse);

void intorobotReceiveDataProcessJson(uint8_t *payload, uint32_t len);
read_datapoint_result_t intorobotReadDataPointBool(const uint16_t dpID, bool &value);
read_datapoint_result_t intorobotReadDataPointInt(const uint16_t dpID, int &value);
read_datapoint_result_t intorobotReadDataPointInt32(const uint16_t dpID, int32_t &value);
read_datapoint_result_t intorobotReadDataPointUint32(const uint16_t dpID, uint32_t &value);
read_datapoint_result_t intorobotReadDataPointDouble(const uint16_t dpID, double &value);
read_datapoint_result_t intorobotReadDataPointFloat(const uint16_t dpID, float &value);
read_datapoint_result_t intorobotReadDataPointString(const uint16_t dpID, String &value);
read_datapoint_result_t intorobotReadDataPointStringChar(const uint16_t dpID, char *value);
read_datapoint_result_t intorobotReadDataPointBinary(const uint16_t dpID, uint8_t *value, uint16_t &len);

void intorobotWriteDataPointString(const uint16_t dpID, char* value);
void intorobotWriteDataPointBinary(const uint16_t dpID, uint8_t *value, uint16_t len);
void intorobotWriteDataPointAll(void);

#ifdef __cplusplus
}
#endif

#endif

#endif	/* SYSTEM_CLOUDDATA_H_ */
