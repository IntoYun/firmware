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

#include <stddef.h>
#include <string.h>
#include "system_user.h"
#include "wiring_usbserial.h"
#include "wiring_usartserial.h"
#include "wiring_watchdog.h"


/**
 * Declare the following function bodies as weak. They will only be used if no
 * other strong function body is found when linking.
 */
void setup() __attribute((weak));
void loop() __attribute((weak));

/**
 * Declare weak setup/loop implementations so that they are always defined.
 */
void setup()
{
}

void loop()
{
}

/**
 * Allow the application to override this to avoid processing
 * overhead when serial events are not required.
 */
void serialEventRun() __attribute__((weak));

void _post_loop()
{
    serialEventRun();
    //application_checkin();
}

/**
 * Provides background processing of serial data.
 */
void serialEventRun()
{
#if FIRMWARE_CONFIG_WIRING_USART > 1
    if (serialEvent && Serial.available()>0) { serialEvent(); }
#endif

#if FIRMWARE_CONFIG_WIRING_USART > 1
    if (serialEvent1 && Serial1.available()>0) { serialEvent1(); }
#endif

#if FIRMWARE_CONFIG_WIRING_USART > 2
    if (serialEvent2 && Serial2.available()>0) { serialEvent2(); }
#endif

#if FIRMWARE_CONFIG_WIRING_USART > 3
    if (serialEvent3 && Serial3.available()>0) { serialEvent3(); }
#endif

#if FIRMWARE_CONFIG_WIRING_USART > 4
    if (serialEvent4 && Serial4.available()>0) { serialEvent4(); }
#endif

#if FIRMWARE_CONFIG_WIRING_USART > 5
    if (serialEvent5 && Serial5.available()>0) { serialEvent5(); }
#endif

#if FIRMWARE_CONFIG_WIRING_USB_USART > 0
    if (usbSerialEvent && SerialUSB.available() > 0) {
        usbSerialEvent();
    }
#endif
}

