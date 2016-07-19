/**
 ******************************************************************************
 * @file        : lib_servo.h
 * @author   : robin
 * @version  : V1.0.0
 * @date      : 2015-02-01
 * @brief      :     
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
#ifndef   LIB_SERVO_H_
#define   LIB_SERVO_H_

#include "wiring.h"

// Pin number of unattached pins
#define NOT_ATTACHED                    (-1)

// Default min/max pulse widths (in microseconds) and angles (in
// degrees).  Values chosen for Arduino compatibility.  These values
// are part of the public API; DO NOT CHANGE THEM.
#define SERVO_DEFAULT_MIN_PW            544
#define SERVO_DEFAULT_MAX_PW            2400
#define SERVO_DEFAULT_MIN_ANGLE         0
#define SERVO_DEFAULT_MAX_ANGLE         180

/** Class for interfacing with RC servomotors. */
class Servo 
{
    public:
        Servo(void);

        bool attach(uint16_t pin,
        uint16_t minPulseWidth=SERVO_DEFAULT_MIN_PW,
        uint16_t maxPulseWidth=SERVO_DEFAULT_MAX_PW,
        int16_t minAngle=SERVO_DEFAULT_MIN_ANGLE,
        int16_t maxAngle=SERVO_DEFAULT_MAX_ANGLE);

        bool attached(void) const;
        int attachedPin(void) const;
        bool detach(void);
        void write(int angle);
        int read(void) const;
        void writeMicroseconds(uint16_t pulseWidth);
        uint16_t readMicroseconds(void) const;

    private:
        int16_t pin;
        uint16_t minPW;
        uint16_t maxPW;
        int16_t minAngle;
        int16_t maxAngle;

        void resetFields(void);
};

#endif  /* LIB_SERVO_H_ */

