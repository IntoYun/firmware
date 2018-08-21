/**
 ******************************************************************************
 * @file    software_timers_hal.c
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

#include "software_timers_hal.h"


int HAL_Software_Timers_Create(hal_timer_t *timer, unsigned period, void (*callback)(hal_timer_t timer), void* const timer_id, bool one_shot)
{
    return -1;
}

int HAL_Software_Timers_Start(hal_timer_t timer, bool fromISR, unsigned block)
{
    return -1;
}

int HAL_Software_Timers_Stop(hal_timer_t timer, bool fromISR, unsigned block)
{
    return -1;
}

int HAL_Software_Timers_Reset(hal_timer_t timer, bool fromISR, unsigned block)
{
    return -1;
}

int HAL_Software_Timers_Change_Period(hal_timer_t timer, unsigned period, bool fromISR, unsigned block)
{
    return -1;
}

int HAL_Software_Timers_Dispose(hal_timer_t timer)
{
    return -1;
}

int HAL_Software_Timers_Is_Active(hal_timer_t timer)
{
    return -1;
}

int HAL_Software_Timers_Get_Id(hal_timer_t timer, void** timer_id)
{
    return -1;
}
