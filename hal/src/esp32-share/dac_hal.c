/**
 ******************************************************************************
 * @file    dac_hal.c
 * @authors Satish Nair
 * @version V1.0.0
 * @date    7-Jan-2015
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

/* Includes ------------------------------------------------------------------*/
#include "dac_hal.h"
#include "gpio_hal.h"
#include "pinmap_impl.h"
#include "esp32-hal-dac.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Extern variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

void HAL_DAC_Write(pin_t pin, uint16_t value)
{
    EESP32_Pin_Info* PIN_MAP = HAL_Pin_Map();

    if (HAL_Get_Pin_Mode(pin) != AN_OUTPUT) {
        HAL_Pin_Mode(pin, AN_OUTPUT);
        HAL_DAC_Enable(pin, 1);
    }
    if(value > 255) {
        value = 255;
    }
    __dacWrite(PIN_MAP[pin].gpio_pin, value);
}

uint8_t HAL_DAC_Is_Enabled(pin_t pin)
{
    EESP32_Pin_Info* PIN_MAP = HAL_Pin_Map();

    if (HAL_Get_Pin_Mode(pin) == AN_OUTPUT) {
        return 1;
    }
    return 0;
}

uint8_t HAL_DAC_Enable(pin_t pin, uint8_t state)
{
    return 0;
}

uint8_t HAL_DAC_Get_Resolution(pin_t pin)
{
    return 8;
}

void HAL_DAC_Set_Resolution(pin_t pin, uint8_t resolution)
{
}

void HAL_DAC_Enable_Buffer(pin_t pin, uint8_t state)
{
}
