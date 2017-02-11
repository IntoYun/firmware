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
#include "wiring_ex_process.h"
#include "ui_hal.h"

#define USER_LED_GPIO_PIN       GPIO_PIN_9
#define USER_LED_GPIO_PORT      GPIOB

rgb_info_t rgb_info;

void HAL_UI_Initial(void)
{
    //例子灯配置
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.Pin = USER_LED_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(USER_LED_GPIO_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(USER_LED_GPIO_PORT, USER_LED_GPIO_PIN, GPIO_PIN_RESET);
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
    memcpy(pinfo, (void *)&rgb_info, sizeof(rgb_info_t));
    return 0;
}

int HAL_UI_RGB_Set_Info(rgb_info_t info)
{
    memcpy((void *)&rgb_info, &info, sizeof(rgb_info_t));
    return 0;
}

void HAL_UI_RGB_Color(uint32_t color)
{
    uint8_t red, green, blue;
    Process Proc;

    memset((void *)&rgb_info, 0, sizeof(rgb_info));
    rgb_info.rgb_mode = RGB_MODE_COLOR;
    rgb_info.rgb_color = color;

    red = (uint8_t)(color >> 16);
    green = (uint8_t)(color >> 8);
    blue = (uint8_t)color;

    if(color)
    {
        Proc.begin("rgb_control");
        Proc.addParameter("color");
        Proc.addParameter(String(red));
        Proc.addParameter(String(green));
        Proc.addParameter(String(blue));
    }
    else
    {
        Proc.begin("rgb_control");
        Proc.addParameter("kill");
    }
    Proc.run();
}

void HAL_UI_RGB_Blink(uint32_t color, uint16_t period)
{
    uint8_t red, green, blue;
    Process Proc;

    memset((void *)&rgb_info, 0, sizeof(rgb_info));
    rgb_info.rgb_mode = RGB_MODE_BLINK;
    rgb_info.rgb_color = color;
    rgb_info.rgb_period = period>100 ? period >> 1 : 50;

    red = (uint8_t)(color >> 16);
    green = (uint8_t)(color >> 8);
    blue = (uint8_t)color;

    Proc.begin("rgb_control");
    Proc.addParameter("blink");
    Proc.addParameter(String(red));
    Proc.addParameter(String(green));
    Proc.addParameter(String(blue));
    Proc.addParameter(String(period));
    Proc.run();
}

void HAL_UI_RGB_Breath(uint32_t color, uint16_t period)
{
    HAL_UI_RGB_Blink(color, period);
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

void HAL_UI_SysTick_Handler(void)
{
}

