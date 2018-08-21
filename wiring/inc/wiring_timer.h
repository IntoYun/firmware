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

#include "stddef.h"
#include "timers_hal.h"
#include <functional>

class Timer
{
    public:

        typedef std::function<void(void)> timer_callback_fn;

        Timer(unsigned period, timer_callback_fn callback_, bool one_shot=false) : running(false), handle(nullptr), callback(std::move(callback_)) {
            HAL_Timers_Create(&handle, period, invoke_timer, this, one_shot);
        }

        template <typename T>
            Timer(unsigned period, void (T::*handler)(), T& instance, bool one_shot=false) : Timer(period, std::bind(handler, &instance), one_shot)
        {
        }

        virtual ~Timer() {
            // when the timer is calling the std::function, we cannot dispose of it until the function completes.
            // the call has exited.
            dispose();
        }

        bool startFromISR() { return _start(0, true); }
        bool stopFromISR() { return _stop(0, true); }
        bool resetFromISR() { return _reset(0, true); }
        bool changePeriodFromISR(unsigned period) { return _changePeriod(period, 0, true); }

        static const unsigned default_wait = 0x7FFFFFFF;

        bool start(unsigned block=default_wait) { return _start(block, false); }
        bool stop(unsigned block=default_wait) { return _stop(block, false); }
        bool reset(unsigned block=default_wait) { return _reset(block, false); }
        bool changePeriod(unsigned period, unsigned block=default_wait) { return _changePeriod(period, block, false); }

        bool isValid() const { return handle!=nullptr; }
        bool isActive() const { return isValid() && HAL_Timers_Is_Active(handle); }

        void dispose()
        {
            if (handle)
            {
                stop();
                while (running) {
                    delay(1);
                }
                HAL_Timers_Dispose(handle);
                handle = nullptr;
            }
        }

    private:
        volatile bool running;
        hal_timer_t handle;
        timer_callback_fn callback;

        bool _start(unsigned block, bool fromISR=false)
        {
            stop(fromISR);
            return handle ? !HAL_Timers_Start(handle, fromISR, block) : false;
        }

        bool _stop(unsigned block, bool fromISR=false)
        {
            return handle ? !HAL_Timers_Stop(handle, fromISR, block) : false;
        }

        bool _reset(unsigned block, bool fromISR=false)
        {
            return handle ? !HAL_Timers_Reset(handle, fromISR, block) : false;
        }

        bool _changePeriod(unsigned period, unsigned block, bool fromISR=false)
        {
            return handle ? !HAL_Timers_Change_Period(handle, period, fromISR , block) : false;
        }

        /*
         * Subclasses can either provide a callback function, or override
         * this timeout method.
         */
        virtual void timeout()
        {
            running = true;
            if (callback)
            {
                callback();
            }
            running = false;
        }

        static void invoke_timer(hal_timer_t timer)
        {
            void *timer_id = NULL;
            if (!HAL_Timers_Get_Id(timer, &timer_id)) {
                if (timer_id)
                    ((Timer*)timer_id)->timeout();
            }
        }
};

#endif
