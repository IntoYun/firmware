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

#include "timer_hal.h"
#include "hw_config.h"
#include "esp_timer.h"
#include "esp_attr.h"
#include "molmc_log.h"

const static char *TAG = "hal-core";

static esp_timer_handle_t _timer = NULL;
static TimerCallback_t _timerCallback = NULL;
static uint64_t _elapsedTickStart = 0;

static void _system_timer_handler(void* arg)
{
    if(_timerCallback) {
        _timerCallback();
    }
}

void HAL_Timer_Start(uint32_t timeout)
{
    _elapsedTickStart = esp_timer_get_time();

    if(timeout < 1) {
        timeout = 1;
    }

    esp_timer_create_args_t _timerConfig;
    _timerConfig.arg = (void*)0;
    _timerConfig.callback = _system_timer_handler;
    _timerConfig.dispatch_method = ESP_TIMER_TASK;
    _timerConfig.name = "Ticker";

    HAL_Timer_Stop();

    esp_timer_create(&_timerConfig, &_timer);
    esp_timer_start_once(_timer, timeout * 1000);
}

void HAL_Timer_Stop(void)
{
    if (_timer) {
        esp_timer_stop(_timer);
        esp_timer_delete(_timer);
    }
}

uint32_t HAL_Timer_Get_ElapsedTime(void)
{
    uint64_t elapsedTick = 0;
    uint64_t _elapsedTickCurrent = esp_timer_get_time();

    if (_elapsedTickCurrent < _elapsedTickStart) {
        elapsedTick =  UINT64_MAX - _elapsedTickStart + _elapsedTickCurrent;
    } else {
        elapsedTick = _elapsedTickCurrent - _elapsedTickStart;
    }

    return elapsedTick / 1000;
}

void HAL_Timer_Set_Callback(TimerCallback_t callback)
{
    _timerCallback = callback;
}

