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


#ifndef FAST_PIN_H_
#define FAST_PIN_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include "gpio_hal.h"

inline void pinSetFast(pin_t _pin) __attribute__((always_inline));
inline void pinResetFast(pin_t _pin) __attribute__((always_inline));
inline int32_t pinReadFast(pin_t _pin) __attribute__((always_inline));

inline void pinModeFast(pin_t pin, PinMode mode)
{
    HAL_pinModeFast(pin, mode);
}

inline void pinSetFast(pin_t _pin)
{
    HAL_pinSetFast(_pin);
}

inline void pinResetFast(pin_t _pin)
{
    HAL_pinResetFast(_pin);
}

inline int32_t pinReadFast(pin_t _pin)
{
    return HAL_pinReadFast(_pin);
}

inline void digitalWriteFast(pin_t pin, uint8_t value)
{
    if (value) {
        HAL_pinSetFast(pin);
    } else {
        HAL_pinResetFast(pin);
    }
}

inline int32_t digitalReadFast(pin_t pin)
{
    return HAL_pinReadFast(pin);
}

#ifdef	__cplusplus
}
#endif

#endif	/* FAST_PIN_H */
