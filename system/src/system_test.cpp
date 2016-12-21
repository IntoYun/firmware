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

#include "ajson.h"
#include "system_test.h"
#include <stdio.h>
#include <stdlib.h>
#include "system_config.h"
// #include "system_rgbled.h"
// #include "system_cloud_def.h"
// #include "system_network.h"
// #include "params_hal.h"
// #include "core_hal.h"
// #include "inet_hal.h"
// #include "wlan_hal.h"
// #include "wiring_wifi.h"
// #include "string_convert.h"
// #include "system_mode.h"
// #include "system_task.h"
#ifndef configNO_NETWORK
using namespace intorobot;
#endif

#if (PLATFORM_ID == 888004)
#define TEST_ATOM

#elif (PLATFORM_ID == 888002)
#define TEST_NEUTRON

#elif (PLATFORM_ID == 888003)
#define TEST_NUT

#elif (PLATFORM_ID == 888101)
#define TEST_W67

#elif (PLATFORM_ID == 888005)
#define TEST_FIG

#elif (PLATFORM_ID == 888102)
#define TEST_W323

#elif (PLATFORM_ID == 888006)
#define TEST_LORA

#elif (PLATFORM_ID == 888103)
#define TEST_L6

#elif (PLATFORM_ID == 888007)
#define TEST_GPRS
#endif

void SetPinLevel(uint8_t level)
{
#if defined TEST_ATOM

#elif defined TEST_NEUTRON

#elif defined TEST_NUT

#elif defined TEST_W67
    uint8_t pin;
    for(pin = 1; pin <= 10; pin++)
    {
        pinMode(pin,OUTPUT);
    }

    for(pin = 1; pin <= 10; pin++)
    {
        digitalWrite(pin,level);
    }

#elif defined TEST_FIG

#elif defined TEST_W323

#elif defined TEST_LORA

#elif defined TEST_L6

#elif defined TEST_GPRS

#endif
}

uint16_t ReadAnalogVal(void)
{
#if defined TEST_ATOM
 return analogRead(A0);

#elif defined TEST_NEUTRON
 return analogRead(A0);

#elif defined TEST_NUT
 return analogRead(A0);

#elif defined TEST_W67
 return analogRead(ADC);

#elif defined TEST_FIG
 return analogRead(A0);

#elif defined TEST_W323
 return analogRead(A0);

#elif defined TEST_LORA
 return analogRead(A0);

#elif defined TEST_L6
 return analogRead(A0);

#elif defined TEST_GPRS
 return analogRead(A0);
#endif

}

void TestWiFi(void)
{
#if defined(TEST_ATOM) || defined(TEST_NEUTRON) || defined(TEST_NUT) || defined(TEST_W67) || defined(TEST_FIG) || defined(TEST_W323)

    aJsonClass aJson;
    aJsonObject* root = aJson.createObject();
    char* strPtr = nullptr;
    wlan_Imlink_stop();
    WiFiAccessPoint ap[10];
    int found = WiFi.scan(ap, 10);
    if(found > 0 )
    {
        aJson.addNumberToObject(root, "status", 200);
        aJson.addNumberToObject(root, "listnum",found);
        aJsonObject* ssidlistarray = aJson.createArray();
        if (ssidlistarray == NULL)
        {
            aJson.deleteItem(root);
            return;
        }
        aJson.addItemToObject(root, "ssidlist", ssidlistarray);

        for(int n = 0; n < found; n++)
        {
            aJsonObject* ssid_object = aJson.createObject();
            if (ssid_object == NULL)
            {
                aJson.deleteItem(root);
                return;
            }
            aJson.addItemToArray(ssidlistarray, ssid_object);
            aJson.addStringToObject(ssid_object, "ssid", ap[n].ssid);
            aJson.addNumberToObject(ssid_object, "entype", ap[n].security);
            aJson.addNumberToObject(ssid_object, "signal", ap[n].rssi);
        }
    }
    else
    {
        aJson.addNumberToObject(root, "status", 201);
    }
    strPtr = aJson.print(root);
    DeviceSetupSerial.write((unsigned char *)strPtr, strlen(strPtr));
    free(strPtr);

#endif
}
