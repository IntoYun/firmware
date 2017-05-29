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

#include "lora_gateway_config.h"
#include "lora_gateway_hal.h"
#include "lora_gateway_log.h"

void hal_init(void) {
    log_gateway_int();
}

uint32_t hal_ticks(void) {
    return micros();
}

bool hal_checkTimer (uint32_t time) {
    uint32_t current = micros();

    if(((current >= time) && ((current - time) < 60*1000000)) \
            || ((current <= time) && ((time - current) > (0xFFFFFFFF - 60*1000000))))
    {
        return true;
    }
    return false;
}

static uint8_t irqlevel = 0;

void hal_disableIRQs(void) {
    noInterrupts();
    irqlevel++;
}

void hal_enableIRQs(void) {
    if(--irqlevel == 0) {
        interrupts();
    }
}

void hal_sleep(void) {
}

