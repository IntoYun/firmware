/**
 *******************************************************************************
 * @file    : variant.cpp
 * @author  : lz
 * @version : V1.0.0
 * @date    : 6-December-2014
 * @brief   :
 *******************************************************************************
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

#include "variant.h"
#include "wiring.h"
#include "wiring_usartserial.h"

STM32_Pin_Info PIN_MAP[TOTAL_PINS] =
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
    /*0-7*/
    { GPIOB, GPIO_PIN_5, NONE, TIM3, TIM_CHANNEL_2, (PinMode)NONE, 0, 0 },  // D0
    // { GPIOA, GPIO_PIN_8, NONE, TIM1, TIM_CHANNEL_1, (PinMode)NONE, 0, 0 },  // D1
    { GPIOA, GPIO_PIN_8, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },  // D1
    { GPIOB, GPIO_PIN_4, NONE, TIM3, TIM_CHANNEL_1, (PinMode)NONE, 0, 0 },  // D2
    { GPIOB, GPIO_PIN_3, NONE, TIM2, TIM_CHANNEL_2, (PinMode)NONE, 0, 0 },  // D3
    // { GPIOA, GPIO_PIN_15, NONE, TIM2, TIM_CHANNEL_1,(PinMode)NONE, 0, 0 },  // D4
    { GPIOA, GPIO_PIN_15, NONE, NULL, NONE,(PinMode)NONE, 0, 0 },          // D4
    { GPIOA, GPIO_PIN_14, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },          // D5
    { GPIOA, GPIO_PIN_13, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },          // D6
    { GPIOB, GPIO_PIN_6, NONE, TIM4, TIM_CHANNEL_1, (PinMode)NONE, 0, 0 },  // D7
    /*0-7*/
    //{ GPIOB, GPIO_PIN_7, NONE, TIM4, TIM_CHANNEL_2, (PinMode)NONE, 0, 0 },  // D0
    //{ GPIOB, GPIO_PIN_6, NONE, TIM4, TIM_CHANNEL_1, (PinMode)NONE, 0, 0 },  // D1
    //{ GPIOB, GPIO_PIN_5, NONE, TIM3, TIM_CHANNEL_2, (PinMode)NONE, 0, 0 },  // D2
    // { GPIOB, GPIO_PIN_4, NONE, TIM3, TIM_CHANNEL_1, (PinMode)NONE, 0, 0 },  // D3
    // { GPIOB, GPIO_PIN_3, NONE, TIM2, TIM_CHANNEL_2, (PinMode)NONE, 0, 0 },  // D4
    // { GPIOA, GPIO_PIN_15, NONE, TIM2, TIM_CHANNEL_1,(PinMode)NONE, 0, 0 },  // D5
    // { GPIOA, GPIO_PIN_14, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },          // D6
    // { GPIOA, GPIO_PIN_13, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },          // D7
    /*8-9 ESP_BOOT  ESP_RST */
    // { GPIOB, GPIO_PIN_12, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { GPIOB, GPIO_PIN_7, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { GPIOC, GPIO_PIN_13, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    /*10 BT button*/
    { GPIOB, GPIO_PIN_2, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    /*11 12 13 LR LG LB reserve*/
    { GPIOB, GPIO_PIN_1, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { GPIOB, GPIO_PIN_13, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { GPIOB, GPIO_PIN_14, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    /*14 PO188 */
    { GPIOB, GPIO_PIN_0, ADC_CHANNEL_8, NULL, NONE, (PinMode)NONE, 0, 0 },
    /*15-19 reserve*/
    { NULL, GPIO_PIN_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { NULL, GPIO_PIN_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { NULL, GPIO_PIN_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { NULL, GPIO_PIN_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { NULL, GPIO_PIN_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    /*20-29  reserve*/
    { NULL, GPIO_PIN_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { NULL, GPIO_PIN_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { NULL, GPIO_PIN_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { NULL, GPIO_PIN_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { NULL, GPIO_PIN_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { NULL, GPIO_PIN_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { NULL, GPIO_PIN_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { NULL, GPIO_PIN_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { NULL, GPIO_PIN_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { NULL, GPIO_PIN_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    /*30-37*/
    { GPIOA, GPIO_PIN_0, ADC_CHANNEL_0, TIM5, TIM_CHANNEL_1, (PinMode)NONE, 0, 0 },  // A0
    { GPIOA, GPIO_PIN_1, ADC_CHANNEL_1, TIM5, TIM_CHANNEL_2, (PinMode)NONE, 0, 0 },  // A1
    { GPIOA, GPIO_PIN_2, ADC_CHANNEL_2, TIM5, TIM_CHANNEL_3, (PinMode)NONE, 0, 0 },  // A2
    { GPIOA, GPIO_PIN_3, ADC_CHANNEL_3, TIM5, TIM_CHANNEL_4, (PinMode)NONE, 0, 0 },  // A3
    { GPIOA, GPIO_PIN_4, ADC_CHANNEL_4, NULL, NONE, (PinMode)NONE, 0, 0 },           // A4
    { GPIOA, GPIO_PIN_5, ADC_CHANNEL_5, TIM2, TIM_CHANNEL_1, (PinMode)NONE, 0, 0 },  // A5
    { GPIOA, GPIO_PIN_6, ADC_CHANNEL_6, TIM3, TIM_CHANNEL_1, (PinMode)NONE, 0, 0 },  // A6
    { GPIOA, GPIO_PIN_7, ADC_CHANNEL_7, TIM3, TIM_CHANNEL_2, (PinMode)NONE, 0, 0 },  // A7
    /*38-39  usart1*/
    { GPIOA, GPIO_PIN_9, NONE, TIM1, TIM_CHANNEL_2, (PinMode)NONE, 0, 0 },
    { GPIOA, GPIO_PIN_10, NONE, TIM1, TIM_CHANNEL_3, (PinMode)NONE, 0, 0 },
};

// ----------------------------------------------------------------------------
void __libc_init_array(void);

void serialEvent() __attribute__((weak));
void serialEvent() { }

/*********************************************************************************
 *Function       : void serialEventRun(void)
 *Description   : serial event run
 *Input           : none
 *Output         : none
 *Return         : none
 *author         : lz
 *date            : 6-December-2014
 *Others         : none
 **********************************************************************************/
void serialEventRun(void)
{
    if (Serial.available()) serialEvent();
}

