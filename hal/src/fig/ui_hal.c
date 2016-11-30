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
#include "service_debug.h"
#include "gpio_hal.h"

#define RGB_R_GPIO_PIN       27
#define RGB_G_GPIO_PIN       21
#define RGB_B_GPIO_PIN       22

#define MODE_BOTTON_GPIO_PIN       0

volatile uint32_t BUTTON_last_state = 0;
volatile uint32_t TimingBUTTON=0;
volatile uint32_t TimingLED;
volatile rgb_info_t rgb_info;

void Set_RGB_Color(uint32_t color) {
  uint8_t red,green,blue;

  red = color>>16 & 0xFF;
  green = color>>8 & 0xFF;
  blue = color & 0xFF;
  if(red)
    HAL_GPIO_Write(RGB_R_GPIO_PIN, 0); // low level effort
  else
    HAL_GPIO_Write(RGB_R_GPIO_PIN, 1);

  if(green)
    HAL_GPIO_Write(RGB_G_GPIO_PIN, 0); // low level effort
  else
    HAL_GPIO_Write(RGB_G_GPIO_PIN, 1);

  if(blue)
    HAL_GPIO_Write(RGB_B_GPIO_PIN, 0); // low level effort
  else
    HAL_GPIO_Write(RGB_B_GPIO_PIN, 1);
}

void RGB_Color_Toggle(void) {
  if(rgb_info.rgb_last_color) {
    Set_RGB_Color(0);
    rgb_info.rgb_last_color = 0;
  }
  else {
    Set_RGB_Color(rgb_info.rgb_color);
    rgb_info.rgb_last_color = rgb_info.rgb_color;
  }
}

void HAL_UI_Initial(void)
{
  HAL_Pin_Mode(RGB_R_GPIO_PIN, OUTPUT);
  HAL_Pin_Mode(RGB_G_GPIO_PIN, OUTPUT);
  HAL_Pin_Mode(RGB_B_GPIO_PIN, OUTPUT);

  HAL_Pin_Mode(MODE_BOTTON_GPIO_PIN, INPUT);
}

uint8_t HAL_UI_Mode_BUTTON_GetState(Button_TypeDef Button)
{
  return HAL_GPIO_Read(MODE_BOTTON_GPIO_PIN);
}

uint32_t HAL_UI_Mode_Button_Pressed(void)
{
  return TimingBUTTON;
}

int HAL_UI_RGB_Get_Info(rgb_info_t *pinfo)
{
  memcpy(pinfo, &rgb_info, sizeof(rgb_info_t));
  return 0;
}

int HAL_UI_RGB_Set_Info(rgb_info_t info)
{
  memcpy(&rgb_info, &info, sizeof(rgb_info_t));
  return 0;
}

void HAL_UI_RGB_Color(uint32_t color)
{
  memset(&rgb_info, 0, sizeof(rgb_info));
  rgb_info.rgb_mode = RGB_MODE_COLOR;
  rgb_info.rgb_color = color;
  Set_RGB_Color(color);
}

void HAL_UI_RGB_Blink(uint32_t color, uint16_t period)
{
  memset(&rgb_info, 0, sizeof(rgb_info));
  rgb_info.rgb_mode = RGB_MODE_BLINK;
  rgb_info.rgb_color = color;
  rgb_info.rgb_period = period>100 ? period >> 1 : 50;
}

void HAL_UI_RGB_Breath(uint32_t color, uint16_t period)
{
  HAL_UI_RGB_Blink(color, period);
}

void HAL_UI_UserLED_Control(uint8_t value)
{
}

void HAL_UI_SysTick_Handler(void)
{
  //三色灯处理
  if(RGB_MODE_BLINK == rgb_info.rgb_mode)
    {
      if (TimingLED != 0x00)
        {
          TimingLED--;
        }
      else
        {
          RGB_Color_Toggle();
          TimingLED = rgb_info.rgb_period;
        }
    }
  //侧边按键处理
  if(!HAL_UI_Mode_BUTTON_GetState(BUTTON1))
    {
      if(!BUTTON_last_state) {
        TimingBUTTON = 0;
        BUTTON_last_state = 1;
      }
      else {
        TimingBUTTON++;
      }
    }
  else {
    if(BUTTON_last_state)
      {
        TimingBUTTON = 0;
        BUTTON_last_state = 1;
      }
  }
}
