/**
 ******************************************************************************
 * @file    tone_hal.c
 * @author  Satish Nair, Brett Walach
 * @version V1.0.0
 * @date    12-Sept-2014
 * @brief
 ******************************************************************************
 Copyright (c) 2013-2015 Particle Industries, Inc.  All rights reserved.

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

/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"
#include "tone_hal.h"
#include "pinmap_impl.h"

#define AVAILABLE_TONE_PINS 1
#define T1INDEX 0

const uint8_t tone_timers[] = { 1 };
static uint8_t tone_pins[AVAILABLE_TONE_PINS] = { 255, };
static long toggle_counts[AVAILABLE_TONE_PINS] = { 0, };
uint32_t tone_freq = TIM_PWM_FREQ;

void t1IntHandler();

static int8_t toneBegin(uint8_t _pin) {
    int8_t _index = -1;

    // if we're already using the pin, reuse it.
    for (int i = 0; i < AVAILABLE_TONE_PINS; i++) {
        if (tone_pins[i] == _pin) {
            return i;
        }
    }

    // search for an unused timer.
    for (int i = 0; i < AVAILABLE_TONE_PINS; i++) {
        if (tone_pins[i] == 255) {
            tone_pins[i] = _pin;
            _index = i;
            break;
        }
    }
    return _index;
}

void disableTimer(uint8_t _index) {
    tone_pins[_index] = 255;

    switch (tone_timers[_index]) {
        case 0:
            // Not currently supported
            break;

        case 1:
            timer1_disable();
            break;
    }
}

ICACHE_RAM_ATTR void t1IntHandler() {
    if (toggle_counts[T1INDEX] != 0){
        // toggle the pin
        HAL_GPIO_Write(tone_pins[T1INDEX], toggle_counts[T1INDEX] % 2);
        toggle_counts[T1INDEX]--;
        // handle the case of indefinite duration
        if (toggle_counts[T1INDEX] < -2){
            toggle_counts[T1INDEX] = -1;
        }
    }else{
        disableTimer(T1INDEX);
        HAL_GPIO_Write(tone_pins[T1INDEX], 1);
    }
}

void HAL_Tone_Start(uint8_t pin, uint32_t frequency, uint32_t duration)
{
    int8_t _index;

    _index = toneBegin(pin);

    if (_index >= 0) {
        tone_freq = frequency;
        // Set the pinMode as OUTPUT
        HAL_Pin_Mode(pin, OUTPUT);

        // Calculate the toggle count
        if (duration > 0) {
            toggle_counts[_index] = 2 * frequency * duration / 1000;
        } else {
            toggle_counts[_index] = -1;
        }

        // set up the interrupt frequency
        switch (tone_timers[_index]) {
            case 0:
                // Not currently supported
                break;

            case 1:
                timer1_disable();
                timer1_isr_init();
                timer1_attachInterrupt(t1IntHandler);
                timer1_enable(TIM_DIV1, TIM_EDGE, TIM_LOOP);
                timer1_write((clockCyclesPerMicrosecond() * 500000) / frequency);
                break;
        }
    }
}

void HAL_Tone_Stop(uint8_t pin)
{
    for (int i = 0; i < AVAILABLE_TONE_PINS; i++) {
        if (tone_pins[i] == pin) {
            tone_pins[i] = 255;
            disableTimer(i);
            break;
        }
    }
    HAL_GPIO_Write(pin, 0);
}

uint32_t HAL_Tone_Get_Frequency(uint8_t pin)
{
    return tone_freq;
}

bool HAL_Tone_Is_Stopped(uint8_t pin)
{
    for (int i = 0; i < AVAILABLE_TONE_PINS; i++) {
        if (tone_pins[i] == pin) {
            return false;
        }
    }
    return true;
}
