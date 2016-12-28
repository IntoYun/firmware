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

#include "wiring_cloud.h"

void CloudClass::addExternalDataPointBool(const char *other_device_id, const uint16_t dpID, const char *permission, const bool value, const char *policy, int lapse)
{
    // Create property structure
    property_conf* prop = new property_conf {dpID, DATA_TYPE_BOOL, permission, other_device_id, policy, (long)lapse*1000, 0};
    prop->boolValue = value;
    properties[properties_count] = prop; // Save pointer to scructure
    properties_count++; // count the number of properties
}

void CloudClass::addExternalDataPointNumber(const char *other_device_id, const uint16_t dpID, const char *permission, const double minValue, const double maxValue, const double Resolution, const double value, const char *policy, int lapse)
{
    property_conf* prop;
    // Create property structure
    if(Resolution == int(Resolution))
    {
        prop = new property_conf {dpID, DATA_TYPE_INT, permission, other_device_id, policy, (long)lapse*1000, 0};
        prop->intValue.minValue = minValue;
        prop->intValue.maxValue = maxValue;
        prop->intValue.resolution = Resolution;
        prop->intValue.value = value;
    }
    else
    {
        prop = new property_conf {dpID, DATA_TYPE_FLOAT, permission, other_device_id, policy, (long)lapse*1000, 0};
        prop->floatValue.minValue = minValue;
        prop->floatValue.maxValue = maxValue;
        prop->floatValue.resolution = Resolution;
        prop->floatValue.value = value;
    }
    properties[properties_count] = prop; // Save pointer to scructure
    properties_count++; // count the number of properties
}

void CloudClass::addExternalDataPointEnum(const char *other_device_id, const uint16_t dpID, const char *permission, const int value, const char *policy, int lapse)
{
    // Create property structure
    property_conf* prop = new property_conf {dpID, DATA_TYPE_ENUM, permission, other_device_id, policy, (long)lapse*1000, 0};
    prop->enumValue = value;
    properties[properties_count] = prop; // Save pointer to scructure
    properties_count++; // count the number of properties
}

void CloudClass::addExternalDataPointString(const char *other_device_id, const uint16_t dpID, const char *permission, const char *value, const char *policy, int lapse)
{
    // Create property structure
    property_conf* prop = new property_conf {dpID, DATA_TYPE_STRING, permission, other_device_id, policy, (long)lapse*1000, 0};
    prop->stringValue = value;
    properties[properties_count] = prop; // Save pointer to scructure
    properties_count++; // count the number of properties
}

void CloudClass::addExternalDataPointBinary(const char *other_device_id, const uint16_t dpID, const char *permission, const uint8_t *value, const uint16_t len, const char *policy, int lapse)
{
    // Create property structure
    property_conf* prop = new property_conf {dpID, DATA_TYPE_BINARY, permission, other_device_id, policy, (long)lapse*1000, 0};
    prop->binaryValue.value = value;
    prop->binaryValue.len = len;
    properties[properties_count] = prop; // Save pointer to scructure
    properties_count++; // count the number of properties
}

void CloudClass::writeDataPoint(const char *other_device_id, const uint16_t dpID, bool value)
{
    int i = discoverProperty(other_device_id, dpID);

    if (i == -1) {
        // not found, nothing to do
        return;
    }

    if (properties[i]->value.equals(value) && strcmp(properties[i]->policy, ON_CHANGE) == 0) {
        return;
    }
    if ((long)(millis() - properties[i]->runtime) >= properties[i]->lapse) {
        properties[i]->value = value;
        String topic = buildTopicProperty(i);
        publish(topic.c_str(), value);
        properties[i]->runtime = millis();
    }
}

void CloudClass::writeDataPoint(const char *other_device_id, const uint16_t dpID, int value)
{

}

void CloudClass::writeDataPoint(const char *other_device_id, const uint16_t dpID, double value)
{

}

void CloudClass::writeDataPoint(const char *other_device_id, const uint16_t dpID, float value)
{

}

void CloudClass::writeDataPoint(const char *other_device_id, const uint16_t dpID, const char *value)
{

}

void CloudClass::writeDataPoint(const char *other_device_id, const uint16_t dpID, const uint8_t *value, uint16_t &len)
{

}

int CloudClass::discoverProperty(const char *device_id, const uint16_t dpID)
{
    for (int i = 0; i <= properties_count; i++)
    {
        if (properties[i]->device_id == device_id && properties[i]->dpID == dpID)
        {
            return i;
        }
    }
    return -1;
}

CloudClass IntoRobot;

#endif
