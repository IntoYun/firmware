/**
******************************************************************************
* @file    servo_hal.c
* @author  Matthew McGowan
* @version V1.0.0
* @date    27-Sept-2014
* @brief
******************************************************************************
Copyright (c) 2013-2015 IntoRobot Industries, Inc.  All rights reserved.

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

/* Includes ------------------------------------------------------------------*/
#include "servo_hal.h"
#include "pinmap_impl.h"
#include "Arduino_Nut.h"
#include "Servo.h"

#define MAX_SERVOS_NUM 11

static ServoHal* myServo[MAX_SERVOS_NUM];

typedef struct pinInfo {
    uint16_t pin;
    bool isActive;
}pinInfo;

pinInfo PinServoIndex[MAX_SERVOS_NUM] = {
    {GPIO0, false}
    ,{GPIO1, false}
    ,{GPIO2, false}
    ,{GPIO3, false}
    ,{GPIO4, false}
    ,{GPIO5, false}
    ,{GPIO12, false}
    ,{GPIO13, false}
    ,{GPIO14, false}
    ,{GPIO15, false}
    ,{GPIO16, false}
};

int8_t findServoIndex(uint16_t pin)
{
    for (int i = 0; i< MAX_SERVOS_NUM; i++) {
        if (PinServoIndex[i].pin == pin) {
            return i;
        }
    }
    return -1;
}


void HAL_Servo_Attach(uint16_t pin)
{
    int8_t pin_severindex = findServoIndex(pin);
    if (pin_severindex != -1) {
        EESP82666_Pin_Info* PIN_MAP = HAL_Pin_Map();
        pin_t gpio_pin = PIN_MAP[pin].gpio_pin;
        if (PinServoIndex[pin_severindex].isActive == false) {
            myServo[pin_severindex] = new ServoHal();
            PinServoIndex[pin_severindex].isActive = true;
        }
        myServo[pin_severindex]->attach(gpio_pin);
    }
}

void HAL_Servo_Detach(uint16_t pin)
{
    int8_t pin_severindex = findServoIndex(pin);
    myServo[pin_severindex]->detach();
}

void HAL_Servo_Write_Pulse_Width(uint16_t pin, uint16_t pulseWidth)
{
    int8_t pin_severindex = findServoIndex(pin);
    myServo[pin_severindex]->write(pulseWidth);
}

uint16_t HAL_Servo_Read_Pulse_Width(uint16_t pin)
{
    int8_t pin_severindex = findServoIndex(pin);
    return myServo[pin_severindex]->read();
}

uint16_t HAL_Servo_Read_Frequency(uint16_t pin)
{
    return 0;
}
