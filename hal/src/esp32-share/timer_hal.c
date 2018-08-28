/**
 ******************************************************************************
 * @file    timers_hal.c
 * @authors robin
 * @version V1.0.0
 * @date    19-06-2017
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

#include "concurrent_hal.h"
#include "timer_hal.h"

#if 0
int HAL_Timers_Create(hal_timer_t *timer, unsigned period, void (*callback)(hal_timer_t timer), void* const timer_id, bool one_shot)
{
    return os_timer_create((os_timer_t *)timer, period, callback, timer_id, one_shot, nullptr);
}

int HAL_Timers_Start(hal_timer_t timer, bool fromISR, unsigned block)
{
    return os_timer_change((os_timer_t)timer, OS_TIMER_CHANGE_START, fromISR, 0, block, nullptr);
}

int HAL_Timers_Stop(hal_timer_t timer, bool fromISR, unsigned block)
{
    return os_timer_change((os_timer_t)timer, OS_TIMER_CHANGE_STOP, fromISR, 0, block, nullptr);
}

int HAL_Timers_Reset(hal_timer_t timer, bool fromISR, unsigned block)
{
    return os_timer_change((os_timer_t)timer, OS_TIMER_CHANGE_RESET, fromISR, 0, block, nullptr);
}

int HAL_Timers_Change_Period(hal_timer_t timer, unsigned period, bool fromISR, unsigned block)
{
    return os_timer_change((os_timer_t)timer, OS_TIMER_CHANGE_PERIOD, fromISR, period, block, nullptr);
}

int HAL_Timers_Dispose(hal_timer_t timer)
{
    return os_timer_destroy((os_timer_t)timer, nullptr);
}

int HAL_Timers_Is_Active(hal_timer_t timer)
{
    return os_timer_is_active((os_timer_t)timer, nullptr);
}

int HAL_Timers_Get_Id(hal_timer_t timer, void** timer_id)
{
    return os_timer_get_id((os_timer_t)timer, timer_id);
}
#endif

void HAL_Timer_Start(uint32_t timeout)
{
}

void HAL_Timer_Stop(void)
{
}

uint32_t HAL_Timer_Get_ElapsedTime(void)
{
    return 0;
}

void HAL_Timer_Set_Callback(TimerCallback_t callback)
{
}

