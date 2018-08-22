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

#include "hw_config.h"
#include "timer_hal.h"

#if 0
#define MAX_SOFTWARE_TIMERS   5

typedef struct {
    ETSTimer Timer;
    uint32_t period;  //定时周期
    bool one_shot;    //重复或者单次
    os_timer_func_t *callback;
    void *timer_id;
    bool is_Active;
} software_timers_t;

static software_timers_t *s_software_timers[MAX_SOFTWARE_TIMERS] = {0};

int HAL_Timers_Create(hal_timer_t *timer, unsigned period, void (*callback)(hal_timer_t timer), void* const timer_id, bool one_shot)
{
    for(int n = 0; n < MAX_SOFTWARE_TIMERS; n++) {
        if(NULL == s_software_timers[n]) {
            s_software_timers[n] = new software_timers_t;
            memset(s_software_timers[n], 0, sizeof(software_timers_t));
            s_software_timers[n]->period = period;
            s_software_timers[n]->one_shot = one_shot;
            s_software_timers[n]->callback = (os_timer_func_t *)callback;
            s_software_timers[n]->timer_id = (void *)timer_id;
            s_software_timers[n]->is_Active = false;
            *timer = s_software_timers[n];
            return 0;
        }
    }
    return -1;
}

int HAL_Timers_Start(hal_timer_t timer, bool fromISR, unsigned block)
{
    if(NULL != timer) {
        software_timers_t *_timer = (software_timers_t *)timer;

        _timer->is_Active = true;
        os_timer_disarm(&(_timer->Timer));
        os_timer_setfn(&(_timer->Timer), _timer->callback, (void *)(timer));
        os_timer_arm(&(_timer->Timer), _timer->period, (_timer->one_shot) ? 0 : 1);
        return 0;
    }
    return -1;
}

int HAL_Timers_Stop(hal_timer_t timer, bool fromISR, unsigned block)
{
    if(NULL != timer) {
        software_timers_t *_timer = (software_timers_t *)timer;

        _timer->is_Active = false;
        os_timer_disarm(&(_timer->Timer));
        return 0;
    }
    return -1;
}

int HAL_Timers_Reset(hal_timer_t timer, bool fromISR, unsigned block)
{
    return HAL_Timers_Start(timer, fromISR, block);
}

int HAL_Timers_Change_Period(hal_timer_t timer, unsigned period, bool fromISR, unsigned block)
{
    if(NULL != timer) {
        software_timers_t *_timer = (software_timers_t *)timer;
        _timer->period = period;
        return HAL_Timers_Start(timer, fromISR, block);
    }
    return -1;
}

int HAL_Timers_Dispose(hal_timer_t timer)
{
    if(NULL != timer) {
        software_timers_t *_timer = (software_timers_t *)timer;

        _timer->is_Active = false;
        os_timer_disarm(&(_timer->Timer));
	    delete _timer;
        _timer = NULL;
        return 0;
    }
    return -1;
}

int HAL_Timers_Is_Active(hal_timer_t timer)
{
    if(NULL != timer) {
        software_timers_t *_timer = (software_timers_t *)timer;

        if(true == _timer->is_Active) {
            return 0;
        }
    }
    return -1;
}

int HAL_Timers_Get_Id(hal_timer_t timer, void **timer_id)
{
    if(NULL != timer) {
        software_timers_t *_timer = (software_timers_t *)timer;

        *timer_id = _timer->timer_id;
        return 0;
    }
    return -1;
}
#endif

int HAL_Timer_Create(timer_handle_t *handle, uint32_t period, timer_callback_fn_t callback_fn, bool one_shot, timer_precision_t precision)
{
    return -1;
}

int HAL_Timer_Start(timer_handle_t handle)
{
    return -1;
}

int HAL_Timer_Stop(timer_handle_t handle)
{
    return -1;
}

int HAL_Timer_Reset(timer_handle_t handle)
{
    return -1;
}

int HAL_Timer_Attach_Interrupt(timer_handle_t handle, uint32_t period)
{
    return -1;
}

int HAL_Timer_Change_Period(timer_handle_t handle, uint32_t period)
{
    return -1;
}

int HAL_Timer_Is_Active(timer_handle_t handle)
{
    return -1;
}

uint32_t HAL_Timer_Get_Remain_Time(timer_handle_t handle)
{
    return 0;
}


