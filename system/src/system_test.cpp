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

#if defined (TEST_ATOM)

void SetPinLevel(uint8_t level)
{
}

void ReadAnalogVal(void)
{
}


#elif defined (TEST_NEUTRON)
void SetPinLevel(uint8_t level)
{
}

void ReadAnalogVal(void)
{
}

#elif defined (TEST_NUT)
void SetPinLevel(uint8_t level)
{
}

void ReadAnalogVal(void)
{
}

#elif defined (TEST_W67)
void SetPinLevel(uint8_t level)
{
    uint8_t pin;
    for(pin = 1; pin <= 10; pin++)
    {
        pinMode(pin,OUTPUT);
    }

    for(pin = 1; pin <= 10; pin++)
    {
        digitalWrite(pin,level);
    }

}

uint16_t ReadAnalogVal(void)
{
    return analogRead(ADC);
}

#elif defined (TEST_FIG)
void SetPinLevel(uint8_t level)
{
}

void ReadAnalogVal(void)
{
}

#elif defined (TEST_W323)
void SetPinLevel(uint8_t level)
{
}

void ReadAnalogVal(void)
{
}

#elif defined (TEST_LORA)
void SetPinLevel(uint8_t level)
{
}

void ReadAnalogVal(void)
{
}

#elif defined (TEST_L6)
void SetPinLevel(uint8_t level)
{
}

void ReadAnalogVal(void)
{
}

#elif defined (TEST_GPRS)

void SetPinLevel(uint8_t level)
{
}

void ReadAnalogVal(void)
{
}
#endif
