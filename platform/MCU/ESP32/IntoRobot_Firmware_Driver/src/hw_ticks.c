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

#include "esp32-hal.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_attr.h"
#include "hw_ticks.h"

/**
 * Fetches the current millisecond tick counter.
 * @return
 */
system_tick_t IRAM_ATTR GetSystem1MsTick()
{
    return (system_tick_t)(esp_timer_get_time() / 1000);
}

/**
 * Fetches the current microsecond tick counter.
 * @return
 */
system_tick_t IRAM_ATTR GetSystem1UsTick()
{
    return (system_tick_t)esp_timer_get_time();
}

