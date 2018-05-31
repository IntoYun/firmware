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
#include "hw_config.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct dac_state_t {
    uint8_t resolution;
    uint8_t buffer;
} dac_state_t;

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static DAC_HandleTypeDef DacHandle;

static uint8_t dacInitFirstTime = true;

static dac_state_t DAC_State[2] = {
    {
        .resolution = DAC_ALIGN_12B_R,
        .buffer = 1
    },
    {
        .resolution = DAC_ALIGN_12B_R,
        .buffer = 1
    }
};
/* Extern variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/*
 * @brief Initialize the DAC peripheral.
 */
static void HAL_DAC_Initial()
{
    DAC_ChannelConfTypeDef sConfig;

    __HAL_RCC_DAC_CLK_ENABLE();

    DacHandle.Instance = DAC;
    HAL_DAC_Init(&DacHandle);

    sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
    sConfig.DAC_OutputBuffer = DAC_State[0].buffer ? DAC_OUTPUTBUFFER_ENABLE : DAC_OUTPUTBUFFER_DISABLE;
    HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DAC_CHANNEL_1);

    sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
    sConfig.DAC_OutputBuffer = DAC_State[1].buffer ? DAC_OUTPUTBUFFER_ENABLE : DAC_OUTPUTBUFFER_DISABLE;
    HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DAC_CHANNEL_2);
}

/*
 * @brief Write the analog value to the pin.
 * DAC is 12-bit. Range of values: 0-4096
 */
void HAL_DAC_Write(pin_t pin, uint16_t value)
{
    STM32_Pin_Info* PIN_MAP = HAL_Pin_Map();

    DacHandle.Instance = DAC;

    if (dacInitFirstTime == true) {
        HAL_DAC_Initial();
        dacInitFirstTime = false;
    }

    if (HAL_Get_Pin_Mode(pin) != AN_OUTPUT) {
        HAL_GPIO_Save_Pin_Mode(pin);
        HAL_Pin_Mode(pin, AN_OUTPUT);
        HAL_DAC_Enable(pin, 1);
    }

    if (PIN_MAP[pin].dac_channel == DAC_CHANNEL_1) {
        HAL_DAC_SetValue(&DacHandle, DAC_CHANNEL_1, DAC_State[0].resolution, value);
    } else if (PIN_MAP[pin].dac_channel == DAC_CHANNEL_2) {
        HAL_DAC_SetValue(&DacHandle, DAC_CHANNEL_2, DAC_State[1].resolution, value);
    }
}

uint8_t HAL_DAC_Is_Enabled(pin_t pin)
{
    STM32_Pin_Info* PIN_MAP = HAL_Pin_Map();
    if (!dacInitFirstTime && HAL_Get_Pin_Mode(pin) == AN_OUTPUT) {
        if (DacHandle.Instance->CR & (DAC_CR_EN1 << PIN_MAP[pin].dac_channel)) {
            return 1;
        }
    }
    return 0;
}

uint8_t HAL_DAC_Enable(pin_t pin, uint8_t state)
{
    STM32_Pin_Info* PIN_MAP = HAL_Pin_Map();
    if (!dacInitFirstTime && HAL_Get_Pin_Mode(pin) == AN_OUTPUT) {
        if(state) {
            HAL_DAC_Start(&DacHandle, PIN_MAP[pin].dac_channel);
        } else {
            HAL_DAC_Stop(&DacHandle, PIN_MAP[pin].dac_channel);
        }
        return 0;
    }
    return 1;
}

uint8_t HAL_DAC_Get_Resolution(pin_t pin)
{
    STM32_Pin_Info* PIN_MAP = HAL_Pin_Map();

    if (PIN_MAP[pin].dac_channel == DAC_CHANNEL_1) {
        return DAC_State[0].resolution == DAC_ALIGN_12B_R ? 12 : 8;
    } else if (PIN_MAP[pin].dac_channel == DAC_CHANNEL_2) {
        return DAC_State[1].resolution == DAC_ALIGN_12B_R ? 12 : 8;
    }
    return 0;
}

void HAL_DAC_Set_Resolution(pin_t pin, uint8_t resolution)
{
    STM32_Pin_Info* PIN_MAP = HAL_Pin_Map();

    if (PIN_MAP[pin].dac_channel == DAC_CHANNEL_1) {
        DAC_State[0].resolution = resolution <= 8 ? DAC_ALIGN_8B_R : DAC_ALIGN_12B_R;
    } else if (PIN_MAP[pin].dac_channel == DAC_CHANNEL_2) {
        DAC_State[1].resolution = resolution <= 8 ? DAC_ALIGN_8B_R : DAC_ALIGN_12B_R;
    }
}

void HAL_DAC_Enable_Buffer(pin_t pin, uint8_t state)
{
    STM32_Pin_Info* PIN_MAP = HAL_Pin_Map();
    DAC_ChannelConfTypeDef sConfig;

    if (PIN_MAP[pin].dac_channel == DAC_CHANNEL_1) {
        DAC_State[0].buffer = state;
        if (!dacInitFirstTime) {
            sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
            sConfig.DAC_OutputBuffer = DAC_State[0].buffer ? DAC_OUTPUTBUFFER_ENABLE : DAC_OUTPUTBUFFER_DISABLE;
            HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DAC_CHANNEL_1);
        }
    } else if (PIN_MAP[pin].dac_channel == DAC_CHANNEL_2) {
        DAC_State[1].buffer = state;
        if (!dacInitFirstTime) {
            sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
            sConfig.DAC_OutputBuffer = DAC_State[1].buffer ? DAC_OUTPUTBUFFER_ENABLE : DAC_OUTPUTBUFFER_DISABLE;
            HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DAC_CHANNEL_2);
        }
    }
}

