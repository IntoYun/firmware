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
#include "hw_config.h"
#include "ui_hal.h"

#define USER_LED_GPIO_PIN       GPIO_PIN_9
#define USER_LED_GPIO_PORT      GPIOB

void HAL_UI_Initial(void)
{
    //三色灯管脚初始化
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.Pin = USER_LED_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(USER_LED_GPIO_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(USER_LED_GPIO_PORT, USER_LED_GPIO_PIN, GPIO_PIN_SET);
}

uint8_t HAL_UI_Mode_BUTTON_GetState(Button_TypeDef Button)
{
    return 0;
}

uint32_t HAL_UI_Mode_Button_Pressed(void)
{
    return 0;
}

int HAL_UI_RGB_Get_Info(rgb_info_t *pinfo)
{
    return 0;
}

int HAL_UI_RGB_Set_Info(rgb_info_t info)
{
    return 0;
}

void HAL_UI_RGB_Color(uint32_t color)
{
}

void HAL_UI_RGB_Blink(uint32_t color, uint16_t period)
{
}

void HAL_UI_RGB_Breath(uint32_t color, uint16_t period)
{
}

void HAL_UI_SysTick_Handler(void)
{
}

void HAL_UI_UserLED_Control(uint8_t value)
{
    if(value == 0)
    {
        HAL_GPIO_WritePin(USER_LED_GPIO_PORT, USER_LED_GPIO_PIN, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(USER_LED_GPIO_PORT, USER_LED_GPIO_PIN, GPIO_PIN_SET);
    }
}

