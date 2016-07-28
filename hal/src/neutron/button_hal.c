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

#include "button_hal.h"
#include "stm32f4xx.h"

__IO uint16_t BUTTON_DEBOUNCED_TIME[] = {0};



void HAL_BUTTON_Initial(void)
{

}

uint8_t HAL_BUTTON_GetState(Button_TypeDef Button)
{
return 0;
}


void BUTTON_EXTI_Config(Button_TypeDef Button, FunctionalState NewState)
{
#if 0
    EXTI_InitTypeDef EXTI_InitStructure;

    /* Connect Button EXTI Line to Button GPIO Pin */
    GPIO_EXTILineConfig(BUTTON_GPIO_PORT_SOURCE[Button], BUTTON_GPIO_PIN_SOURCE[Button]);

    /* Clear the EXTI line pending flag */
    EXTI_ClearFlag(BUTTON_EXTI_LINE[Button]);

    /* Configure Button EXTI line */
    EXTI_InitStructure.EXTI_Line = BUTTON_EXTI_LINE[Button];
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = BUTTON_EXTI_TRIGGER[Button];
    EXTI_InitStructure.EXTI_LineCmd = NewState;
    EXTI_Init(&EXTI_InitStructure);
#endif
}

/**
 * @brief  Returns the selected Button Debounced Time.
 * @param  Button: Specifies the Button to be checked.
 *   This parameter can be one of following parameters:
 *     @arg BUTTON1: Button1
 *     @arg BUTTON2: Button2
 * @retval Button Debounced time in millisec.
 */
uint16_t BUTTON_GetDebouncedTime(Button_TypeDef Button)
{
    return BUTTON_DEBOUNCED_TIME[Button];
}

void BUTTON_ResetDebouncedState(Button_TypeDef Button)
{
    BUTTON_DEBOUNCED_TIME[Button] = 0;
}

#if 0

uint16_t HAL_Core_Mode_Button_Pressed_Time()
{
    return 0;
}

bool HAL_Core_Mode_Button_Pressed(uint16_t pressedMillisDuration)
{
    return false;
}

void HAL_Core_Mode_Button_Reset(void)
{
}

#endif
