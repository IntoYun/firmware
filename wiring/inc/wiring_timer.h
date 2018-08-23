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

#ifndef WIRING_TIMER_H_
#define WIRING_TIMER_H_

#include "timer_hal.h"

class Timer
{
    public:
        Timer(unsigned period, timer_callback_fn_t callback_fn, bool one_shot = false, timer_precision_t precision = TIMER_PRECISION_MS) {
            HAL_Timer_Create(&handle, period, callback_fn, one_shot, precision);
        }

        virtual ~Timer() {
        }

        bool start(void) {
            stop();
            return !HAL_Timer_Start(handle);
        }

        bool stop(void) {
            return !HAL_Timer_Stop(handle);
        }

        bool reset(void) {
            return !HAL_Timer_Reset(handle);
        }

        bool attachInterrupt(timer_callback_fn_t callback_fn) {
            return !HAL_Timer_Attach_Interrupt(handle, callback_fn);
        }

        bool changePeriod(uint32_t period) {
            return !HAL_Timer_Change_Period(handle, period);
        }

        bool isActive(void) {
            return !HAL_Timer_Is_Active(handle);
        }

        uint32_t getRemainTime(void) {
            return HAL_Timer_Get_Remain_Time(handle);
        }

    private:
        timer_handle_t handle;
};

#endif
