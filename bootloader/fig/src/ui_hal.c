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
#include "ui_hal.h"
#include "soc/gpio_struct.h"
#include "soc/gpio_sig_map.h"
#include "soc/io_mux_reg.h"
#include "soc/rtc_io_reg.h"


#define USER_GPIO_PIN          4

#define RGB_R_GPIO_PIN         27
#define RGB_G_GPIO_PIN         21
#define RGB_B_GPIO_PIN         22

#define MODE_BOTTON_GPIO_PIN   0

volatile uint32_t BUTTON_last_state = 0;
volatile uint32_t TimingBUTTON=0;
volatile uint32_t TimingLED;
rgb_info_t rgb_info;

void Set_RGB_Color(uint32_t color) {
    uint8_t red,green,blue;

    red = color>>16 & 0xFF;
    green = color>>8 & 0xFF;
    blue = color & 0xFF;
    if(red)
        GPIO.out_w1tc = (1 << RGB_R_GPIO_PIN);
    else
        GPIO.out_w1ts = (1 << RGB_R_GPIO_PIN);

    if(green)
        GPIO.out_w1tc = (1 << RGB_G_GPIO_PIN);
    else
        GPIO.out_w1ts = (1 << RGB_G_GPIO_PIN);

    if(blue)
        GPIO.out_w1tc = (1 << RGB_B_GPIO_PIN);
    else
        GPIO.out_w1ts = (1 << RGB_B_GPIO_PIN);
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
    // user pin
    gpio_matrix_out(USER_GPIO_PIN, SIG_GPIO_OUT_IDX, 0, 0);
    GPIO.enable_w1ts = (0x1 << USER_GPIO_PIN); //output enable
    GPIO.pin[USER_GPIO_PIN].int_ena = 0;       //disable GPIO intr
    CLEAR_PERI_REG_MASK(RTC_IO_TOUCH_PAD0_REG, RTC_IO_TOUCH_PAD0_RDE_M); //disable pull down
    CLEAR_PERI_REG_MASK(RTC_IO_TOUCH_PAD0_REG, RTC_IO_TOUCH_PAD0_RUE_M); //disable pull up
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, PIN_FUNC_GPIO); //set register
    GPIO.out_w1ts = (1 << USER_GPIO_PIN); //默认高电平 灯灭

    // rgb pin
    GPIO.out_w1ts = (1 << RGB_R_GPIO_PIN); //默认高电平 灯灭
    gpio_matrix_out(RGB_R_GPIO_PIN, SIG_GPIO_OUT_IDX, 0, 0);
    GPIO.enable_w1ts = (0x1 << RGB_R_GPIO_PIN); //output enable
    GPIO.pin[RGB_R_GPIO_PIN].int_ena = 0;       //disable GPIO intr
    CLEAR_PERI_REG_MASK(RTC_IO_TOUCH_PAD7_REG, RTC_IO_TOUCH_PAD7_RDE_M); //disable pull down
    CLEAR_PERI_REG_MASK(RTC_IO_TOUCH_PAD7_REG, RTC_IO_TOUCH_PAD7_RUE_M); //disable pull up
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO27_U, PIN_FUNC_GPIO); //set register

    GPIO.out_w1ts = (1 << RGB_G_GPIO_PIN); //默认高电平 灯灭
    gpio_matrix_out(RGB_G_GPIO_PIN, SIG_GPIO_OUT_IDX, 0, 0);
    GPIO.enable_w1ts = (0x1 << RGB_G_GPIO_PIN); //output enable
    GPIO.pin[RGB_G_GPIO_PIN].int_ena = 0;       //disable GPIO intr
    REG_CLR_BIT(PERIPHS_IO_MUX_GPIO21_U, FUN_PD); //disable pull down
    REG_CLR_BIT(PERIPHS_IO_MUX_GPIO21_U, FUN_PU); //disable pull up
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO21_U, PIN_FUNC_GPIO);//set register

    GPIO.out_w1ts = (1 << RGB_B_GPIO_PIN); //默认高电平 灯灭
    gpio_matrix_out(RGB_B_GPIO_PIN, SIG_GPIO_OUT_IDX, 0, 0);
    GPIO.enable_w1ts = (0x1 << RGB_B_GPIO_PIN); //output enable
    GPIO.pin[RGB_B_GPIO_PIN].int_ena = 0;       //disable GPIO intr
    REG_CLR_BIT(PERIPHS_IO_MUX_GPIO22_U, FUN_PD); //disable pull down
    REG_CLR_BIT(PERIPHS_IO_MUX_GPIO22_U, FUN_PU); //disable pull up
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO22_U, PIN_FUNC_GPIO); //set register

    //button
    PIN_INPUT_ENABLE(PERIPHS_IO_MUX_GPIO0_U);
    GPIO.pin[MODE_BOTTON_GPIO_PIN].int_ena = 0;       //disable GPIO intr
    SET_PERI_REG_MASK(RTC_IO_TOUCH_PAD1_REG, RTC_IO_TOUCH_PAD1_RUE_M); //enable pull up
    CLEAR_PERI_REG_MASK(RTC_IO_TOUCH_PAD1_REG, RTC_IO_TOUCH_PAD1_RDE_M); //disable pull down
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, PIN_FUNC_GPIO); //set register
}

uint8_t HAL_UI_Mode_BUTTON_GetState(Button_TypeDef Button)
{
    return (GPIO.in >> MODE_BOTTON_GPIO_PIN) & 0x1;
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
    if(value)
        GPIO.out_w1tc = (1 << USER_GPIO_PIN);
    else
        GPIO.out_w1ts = (1 << USER_GPIO_PIN);
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
            BUTTON_last_state = 0;
        }
    }
}
