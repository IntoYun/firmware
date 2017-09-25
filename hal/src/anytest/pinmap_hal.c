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

/* Includes ------------------------------------------------------------------*/
#include "pinmap_hal.h"
#include "pinmap_impl.h"
#include <stddef.h>

#define NONE CHANNEL_NONE

#define GPIO_PINSource0            ((uint8_t)0x00)
#define GPIO_PINSource1            ((uint8_t)0x01)
#define GPIO_PINSource2            ((uint8_t)0x02)
#define GPIO_PINSource3            ((uint8_t)0x03)
#define GPIO_PINSource4            ((uint8_t)0x04)
#define GPIO_PINSource5            ((uint8_t)0x05)
#define GPIO_PINSource6            ((uint8_t)0x06)
#define GPIO_PINSource7            ((uint8_t)0x07)
#define GPIO_PINSource8            ((uint8_t)0x08)
#define GPIO_PINSource9            ((uint8_t)0x09)
#define GPIO_PINSource10           ((uint8_t)0x0A)
#define GPIO_PINSource11           ((uint8_t)0x0B)
#define GPIO_PINSource12           ((uint8_t)0x0C)
#define GPIO_PINSource13           ((uint8_t)0x0D)
#define GPIO_PINSource14           ((uint8_t)0x0E)
#define GPIO_PINSource15           ((uint8_t)0x0F)


/* Private typedef -----------------------------------------------------------*/

const STM32_Pin_Info __PIN_MAP[TOTAL_PINS] =
{
    /*
    * gpio_peripheral (GPIOA or GPIOB or GPIOC or GPIOD)
    * gpio_pin (0-15)
    * adc_channel (0-15 or NONE. Note we don't define the peripheral because our chip only has one)
    * timer_peripheral (TIM1 - TIM4, or NONE)
    * timer_ch (1-4, or NONE)
    * pin_mode (NONE by default, can be set to OUTPUT, INPUT, or other types)
    * timer_ccr (0 by default, store the CCR value for TIM interrupt use)
    * user_property (0 by default, user variable storage)
    */

/* D0       - 00 */ { GPIOB, GPIO_PIN_9,  GPIO_PINSource9,  NONE, NONE, TIM4, TIM_CHANNEL_4, PIN_MODE_NONE, 0, 0 },
/* D1       - 01 */ { GPIOB, GPIO_PIN_8,  GPIO_PINSource8,  NONE, NONE, TIM4, TIM_CHANNEL_3, PIN_MODE_NONE, 0, 0 },
/* D2       - 02 */ { GPIOB, GPIO_PIN_7,  GPIO_PINSource7,  NONE, NONE, TIM4, TIM_CHANNEL_2, PIN_MODE_NONE, 0, 0 },
/* D3       - 03 */ { GPIOB, GPIO_PIN_6,  GPIO_PINSource6,  NONE, NONE, TIM4, TIM_CHANNEL_1, PIN_MODE_NONE, 0, 0 },
/* D4       - 04 */ { GPIOB, GPIO_PIN_5,  GPIO_PINSource5,  NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* D5       - 05 */ { GPIOB, GPIO_PIN_4,  GPIO_PINSource4,  NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* D6       - 06 */ { GPIOB, GPIO_PIN_3,  GPIO_PINSource3,  NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* D7       - 07 */ { GPIOD, GPIO_PIN_2,  GPIO_PINSource2,  NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* D8       - 08 */ { GPIOC, GPIO_PIN_12, GPIO_PINSource12, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* D9       - 09 */ { GPIOC, GPIO_PIN_11, GPIO_PINSource11, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* D10      - 10 */ { GPIOC, GPIO_PIN_10, GPIO_PINSource10, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* D11      - 11 */ { GPIOA, GPIO_PIN_15, GPIO_PINSource15, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* D12      - 12 */ { GPIOA, GPIO_PIN_10, GPIO_PINSource10, NONE, NONE, TIM1, TIM_CHANNEL_3, PIN_MODE_NONE, 0, 0 },
/* D13      - 13 */ { GPIOA, GPIO_PIN_9,  GPIO_PINSource9,  NONE, NONE, TIM1, TIM_CHANNEL_2, PIN_MODE_NONE, 0, 0 },
/* D14      - 14 */ { GPIOA, GPIO_PIN_8,  GPIO_PINSource8,  NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* D15      - 15 */ { GPIOC, GPIO_PIN_9,  GPIO_PINSource9,  NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* D16      - 16 */ { GPIOC, GPIO_PIN_7,  GPIO_PINSource7,  NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* D17      - 17 */ { GPIOC, GPIO_PIN_6,  GPIO_PINSource6,  NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* D18      - 18 */ { GPIOB, GPIO_PIN_15, GPIO_PINSource15, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* D19      - 19 */ { GPIOB, GPIO_PIN_14, GPIO_PINSource14, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* D20      - 20 */ { GPIOB, GPIO_PIN_13, GPIO_PINSource13, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* D21      - 21 */ { GPIOB, GPIO_PIN_12, GPIO_PINSource12, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* D22      - 22 */ { GPIOB, GPIO_PIN_11, GPIO_PINSource11, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* D23      - 23 */ { GPIOB, GPIO_PIN_10, GPIO_PINSource10, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* D24      - 24 */ { GPIOB, GPIO_PIN_1,  GPIO_PINSource1, ADC_CHANNEL_9, NONE, TIM3, TIM_CHANNEL_4,  PIN_MODE_NONE, 0, 0 },
/* D25      - 25 */ { GPIOC, GPIO_PIN_8,  GPIO_PINSource8, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },

/* NOT USED - 26 */ { NULL, GPIO_PIN_0, NONE, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* NOT USED - 27 */ { NULL, GPIO_PIN_0, NONE, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* NOT USED - 28 */ { NULL, GPIO_PIN_0, NONE, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* NOT USED - 29 */ { NULL, GPIO_PIN_0, NONE, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },

/* A0       - 30 */ { GPIOC, GPIO_PIN_13, GPIO_PINSource13, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* A1       - 31 */ { GPIOC, GPIO_PIN_0, GPIO_PINSource0, ADC_CHANNEL_10, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* A2       - 32 */ { GPIOC, GPIO_PIN_1, GPIO_PINSource1, ADC_CHANNEL_11, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* A3       - 33 */ { GPIOC, GPIO_PIN_2, GPIO_PINSource2, ADC_CHANNEL_12, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* A4       - 34 */ { GPIOC, GPIO_PIN_3, GPIO_PINSource3, ADC_CHANNEL_13, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* A5       - 35 */ { GPIOA, GPIO_PIN_0, GPIO_PINSource0, ADC_CHANNEL_0, NONE, TIM2, TIM_CHANNEL_1,  PIN_MODE_NONE, 0, 0 },
/* A6       - 36 */ { GPIOA, GPIO_PIN_1, GPIO_PINSource1, ADC_CHANNEL_1, NONE, TIM2, TIM_CHANNEL_2,  PIN_MODE_NONE, 0, 0 },
/* A7       - 37 */ { GPIOA, GPIO_PIN_2, GPIO_PINSource2, ADC_CHANNEL_2, NONE, TIM2, TIM_CHANNEL_3,  PIN_MODE_NONE, 0, 0 },
/* A8       - 38 */ { GPIOA, GPIO_PIN_3, GPIO_PINSource3, ADC_CHANNEL_3, NONE, TIM2, TIM_CHANNEL_4,  PIN_MODE_NONE, 0, 0 },
/* A9       - 39 */ { GPIOA, GPIO_PIN_4, GPIO_PINSource4, ADC_CHANNEL_4, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* A10      - 40 */ { GPIOA, GPIO_PIN_5, GPIO_PINSource5, ADC_CHANNEL_5, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* A11      - 41 */ { GPIOA, GPIO_PIN_6, GPIO_PINSource6, ADC_CHANNEL_6, NONE, TIM3, TIM_CHANNEL_1,  PIN_MODE_NONE, 0, 0 },
/* A12      - 42 */ { GPIOA, GPIO_PIN_7, GPIO_PINSource7, ADC_CHANNEL_7, NONE, TIM3, TIM_CHANNEL_2,  PIN_MODE_NONE, 0, 0 },
/* A13      - 43 */ { GPIOC, GPIO_PIN_4, GPIO_PINSource4, ADC_CHANNEL_14, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* A14      - 44 */ { GPIOC, GPIO_PIN_5, GPIO_PINSource5, ADC_CHANNEL_15, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* A15      - 45 */ { GPIOB, GPIO_PIN_0, GPIO_PINSource0, ADC_CHANNEL_8, NONE, TIM3, TIM_CHANNEL_3,  PIN_MODE_NONE, 0, 0 },
};

STM32_Pin_Info* HAL_Pin_Map() {
    return __PIN_MAP;
}
