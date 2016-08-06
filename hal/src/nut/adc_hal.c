/**
 ******************************************************************************
 * @file    adc_hal.c
 * @authors Matthew McGowan
 * @version V1.0.0
 * @date    27-Sept-2014
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

#include "adc_hal.h"
#include "pinmap_impl.h"
#include "hw_config.h"
//#include "gpio_hal_h"

void HAL_ADC_Set_Sample_Time(uint8_t ADC_SampleTime)
{
}

/*
 * @brief Read the analog value of a pin.
 * Should return a 16-bit value, 0-65536 (0 = LOW, 65536 = HIGH)
 * Note: ADC input voltage range is 0 - 1.0V
 */
int32_t HAL_ADC_Read(uint16_t pin)
{
    EESP82666_Pin_Info* PIN_MAP = HAL_Pin_Map();
    pin_t gpio_pin = PIN_MAP[pin].gpio_pin;
    // accept both A0 constant and ADC channel number
    if(gpio_pin == 17 || gpio_pin == 0) {
        return 2*system_adc_read();
    }
    return digitalRead(gpio_pin) * 1023;
}

/*
 * @brief Initialize the ADC peripheral.
 */
void HAL_ADC_DMA_Init()
{

}
