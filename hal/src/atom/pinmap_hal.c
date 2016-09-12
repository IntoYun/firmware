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

STM32_Pin_Info __PIN_MAP[TOTAL_PINS] =
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
    /*0-9*/
    { GPIOB, GPIO_PIN_11, GPIO_PINSource11, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    { GPIOB, GPIO_PIN_10, GPIO_PINSource10, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    { GPIOB, GPIO_PIN_12, GPIO_PINSource12, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    { GPIOB, GPIO_PIN_13, GPIO_PINSource13, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    { GPIOB, GPIO_PIN_14, GPIO_PINSource14, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    { GPIOB, GPIO_PIN_15, GPIO_PINSource15, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    { GPIOB, GPIO_PIN_8,  GPIO_PINSource8,  NONE, NONE, TIM4, TIM_CHANNEL_3, PIN_MODE_NONE, 0, 0 },
    { GPIOB, GPIO_PIN_9,  GPIO_PINSource9,  NONE, NONE, TIM4, TIM_CHANNEL_4, PIN_MODE_NONE, 0, 0 },
    { GPIOB, GPIO_PIN_7,  GPIO_PINSource7,  NONE, NONE, TIM4, TIM_CHANNEL_2, PIN_MODE_NONE, 0, 0 },
    { GPIOB, GPIO_PIN_6,  GPIO_PINSource6,  NONE, NONE, TIM4, TIM_CHANNEL_1, PIN_MODE_NONE, 0, 0 },
    /*10-19*/
    { GPIOB, GPIO_PIN_5,  GPIO_PINSource5,  NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    { GPIOA, GPIO_PIN_8,  GPIO_PINSource8,  NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    { GPIOD, GPIO_PIN_2,  GPIO_PINSource2,  NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    { GPIOC, GPIO_PIN_6,  GPIO_PINSource6,  NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    { GPIOC, GPIO_PIN_7,  GPIO_PINSource7,  NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    { GPIOC, GPIO_PIN_8,  GPIO_PINSource8,  NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    { GPIOC, GPIO_PIN_9,  GPIO_PINSource9,  NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    { GPIOC, GPIO_PIN_10, GPIO_PINSource10, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    { GPIOC, GPIO_PIN_11, GPIO_PINSource11, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    { GPIOC, GPIO_PIN_13, GPIO_PINSource13, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    /*20-24*/
    { GPIOA, GPIO_PIN_13, GPIO_PINSource13, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    { GPIOA, GPIO_PIN_14, GPIO_PINSource14, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    { GPIOA, GPIO_PIN_15, GPIO_PINSource15, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    { GPIOB, GPIO_PIN_3,  GPIO_PINSource3,  NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    { GPIOB, GPIO_PIN_4,  GPIO_PINSource4,  NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    /*25-29  reserve*/
    { NULL, GPIO_PIN_0, NONE,NONE,NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    { NULL, GPIO_PIN_0, NONE,NONE,NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    { NULL, GPIO_PIN_0, NONE,NONE,NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    { NULL, GPIO_PIN_0, NONE,NONE,NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    { NULL, GPIO_PIN_0, NONE,NONE,NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
    /*30-39*/
    { GPIOA, GPIO_PIN_0, GPIO_PINSource0,ADC_CHANNEL_0, TIM2, TIM_CHANNEL_1,  PIN_MODE_NONE, 0, 0 },
    { GPIOA, GPIO_PIN_1, GPIO_PINSource1,ADC_CHANNEL_1, TIM2, TIM_CHANNEL_2,  PIN_MODE_NONE, 0, 0 },
    { GPIOA, GPIO_PIN_3, GPIO_PINSource3,ADC_CHANNEL_3, TIM2, TIM_CHANNEL_4,  PIN_MODE_NONE, 0, 0 },
    { GPIOA, GPIO_PIN_2, GPIO_PINSource2,ADC_CHANNEL_2, TIM2, TIM_CHANNEL_3,  PIN_MODE_NONE, 0, 0 },
    { GPIOA, GPIO_PIN_4, GPIO_PINSource4,ADC_CHANNEL_4, NULL, (uint16_t)NULL, PIN_MODE_NONE, 0, 0 },
    { GPIOA, GPIO_PIN_5, GPIO_PINSource5,ADC_CHANNEL_5, NULL, (uint16_t)NULL, PIN_MODE_NONE, 0, 0 },
    { GPIOA, GPIO_PIN_6, GPIO_PINSource6,ADC_CHANNEL_6, TIM3, TIM_CHANNEL_1,  PIN_MODE_NONE, 0, 0 },
    { GPIOA, GPIO_PIN_7, GPIO_PINSource7,ADC_CHANNEL_7, TIM3, TIM_CHANNEL_2,  PIN_MODE_NONE, 0, 0 },
    { GPIOB, GPIO_PIN_0, GPIO_PINSource0,ADC_CHANNEL_8, TIM3, TIM_CHANNEL_3,  PIN_MODE_NONE, 0, 0 },
    { GPIOB, GPIO_PIN_1, GPIO_PINSource1,ADC_CHANNEL_9, TIM3, TIM_CHANNEL_4,  PIN_MODE_NONE, 0, 0 },
    /*40-45*/
    { GPIOC, GPIO_PIN_0, GPIO_PINSource0,ADC_CHANNEL_10, NULL, (uint16_t)NULL, PIN_MODE_NONE, 0, 0 },
    { GPIOC, GPIO_PIN_1, GPIO_PINSource1,ADC_CHANNEL_11, NULL, (uint16_t)NULL, PIN_MODE_NONE, 0, 0 },
    { GPIOC, GPIO_PIN_2, GPIO_PINSource2,ADC_CHANNEL_12, NULL, (uint16_t)NULL, PIN_MODE_NONE, 0, 0 },
    { GPIOC, GPIO_PIN_3, GPIO_PINSource3,ADC_CHANNEL_13, NULL, (uint16_t)NULL, PIN_MODE_NONE, 0, 0 },
    { GPIOC, GPIO_PIN_4, GPIO_PINSource4,ADC_CHANNEL_14, NULL, (uint16_t)NULL, PIN_MODE_NONE, 0, 0 },
    { GPIOC, GPIO_PIN_5, GPIO_PINSource5,ADC_CHANNEL_15, NULL, (uint16_t)NULL, PIN_MODE_NONE, 0, 0 },
    /* /\*46-47  usart1*\/  */
    /* { GPIOA, GPIO_PIN_10, GPIO_PINSource10,NONE, TIM1, TIM_CHANNEL_3, PIN_MODE_NONE, 0, 0 }, */
    /* { GPIOA, GPIO_PIN_9, GPIO_PINSource10,NONE, TIM1, TIM_CHANNEL_2, PIN_MODE_NONE, 0, 0 }, */
    /* /\*48-49 reserve*\/ */
    /* { NULL, GPIO_PIN_0, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 }, */
    /* { NULL, GPIO_PIN_0, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 }, */
    /* /\*50-55 reserve*\/ */
    /* { NULL, GPIO_PIN_0, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 }, */
    /* { NULL, GPIO_PIN_0, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 }, */
    /* { NULL, GPIO_PIN_0, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 }, */
    /* { NULL, GPIO_PIN_0, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 }, */
    /* { NULL, GPIO_PIN_0, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 }, */
    /* { NULL, GPIO_PIN_0, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 }, */
    /* /\*56 reserve*\/ */
    /* { NULL, GPIO_PIN_0, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 } */

};


STM32_Pin_Info* HAL_Pin_Map() {
    return __PIN_MAP;
}
