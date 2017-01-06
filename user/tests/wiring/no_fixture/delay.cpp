/**
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


#include "application.h"
#include "unit-test/unit-test.h"

#if PLATFORM_THREADING
#define scheduler(b) os_thread_scheduling(b, NULL)
#else
#define scheduler(b)
#endif

class DelayTest
{
    public:

    DelayTest(uint16_t duration)
    {
        HAL_Delay_Milliseconds(duration);
    }
};

DelayTest dt(10);

test(delay_1_is_within_5_percent)
{
    // on RTOS have to stop task scheduling for the delays or we may not
    // be executed often enough
    for (int i=0; i<100; i++) {
        scheduler(false);
        uint32_t start = micros();
        // system_delay_ms(1, true);
        delay(1);
        uint32_t end = micros();
        scheduler(true);

        assertMoreOrEqual(end-start, 1000);
        assertLessOrEqual(end-start, 1050);
    }
}
