/**
  Copyright (c) 2013-2015 IntoRobot Industries, Inc.  All rights reserved.

  Copyright 2012 STMicroelectronics
  http://www.st.com/software_license_agreement_liberty_v2

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


//#include <stdint.h>
#include <limits.h>
#include "os_type.h"
#include "osapi.h"
#include "hw_ticks.h"


#define ONCE 0
#define REPEAT 1


static os_timer_t micros_overflow_timer;
static uint32_t micros_at_last_overflow_tick = 0;
static uint32_t micros_overflow_count = 0;

void micros_overflow_tick(void* arg) {
    uint32_t m = system_get_time();
    if(m < micros_at_last_overflow_tick)
        ++micros_overflow_count;
    micros_at_last_overflow_tick = m;
}

void SysTick_Enable() {
    os_timer_setfn(&micros_overflow_timer, (os_timer_func_t*) &micros_overflow_tick, 0);
    os_timer_arm(&micros_overflow_timer, 60000, REPEAT);
}
/**
 * Fetches the current millisecond tick counter.
 * @return
 */
system_tick_t ICACHE_RAM_ATTR GetSystem1MsTick()
{
    uint32_t m = system_get_time();
    uint32_t c = micros_overflow_count + ((m < micros_at_last_overflow_tick) ? 1 : 0);
    return c * 4294967 + m / 1000;
}

/**
 * Fetches the current microsecond tick counter.
 * @return
 */
system_tick_t ICACHE_RAM_ATTR GetSystem1UsTick()
{
    return system_get_time();
}

