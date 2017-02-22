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
#include "pinmap_hal.h"

#if 0
#include "soc/soc.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_struct.h"
#include "soc/gpio_reg.h"
#include "soc/rtc_io_reg.h"

/* #include "soc/soc.h" */
#include "soc/cpu.h"
#include "soc/dport_reg.h"
/* #include "soc/io_mux_reg.h" */
#include "soc/efuse_reg.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/timer_group_reg.h"
/* #include "soc/gpio_reg.h" */
#include "soc/gpio_sig_map.h"
#endif

/* #include "esp32-hal-gpio.h" */
/* #include "esp_attr.h" */
/* #include "esp_log.h" */

/* #include "rom/cache.h" */
/* #include "rom/ets_sys.h" */
/* #include "rom/spi_flash.h" */
/* #include "rom/crc.h" */
/* #include "rom/rtc.h" */
/* #include "rom/uart.h" */
#include "rom/gpio.h"
/* #include "rom/secure_boot.h" */

#include "soc/soc.h"
#include "soc/gpio_struct.h"
#include "soc/cpu.h"
/* #include "soc/dport_reg.h" */
#include "soc/io_mux_reg.h"
/* #include "soc/efuse_reg.h" */
/* #include "soc/rtc_cntl_reg.h" */
#include "soc/timer_group_reg.h"
#include "soc/gpio_reg.h"
#include "soc/gpio_sig_map.h"
/* #include "driver/gpio.h" */
/* #include "driver/rtc_io.h" */

#include "sdkconfig.h"
/* #include "esp_image_format.h" */
/* #include "esp_secure_boot.h" */
/* #include "esp_flash_encrypt.h" */
/* #include "esp_flash_partitions.h" */
/* #include "bootloader_flash.h" */
/* #include "bootloader_random.h" */
/* #include "bootloader_config.h" */
/* #include "rtc.h" */

gpio_dev_t GPIO;

#define RGB_R_GPIO_PIN         27
#define RGB_G_GPIO_PIN         21
#define RGB_B_GPIO_PIN         22

#define MODE_BOTTON_GPIO_PIN   0

#define RGB_GPIO_OUTPUT_PIN_SEL    ((1<<RGB_R_GPIO_PIN) | (1<<RGB_G_GPIO_PIN)|(1<<RGB_B_GPIO_PIN))
#define BUTTON_GPIO_INPUT_PIN_SEL  GPIO_SEL_0

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
    /* gpio_set_level(RGB_R_GPIO_PIN, 0); // low level effort */
      /* GPIO.out_w1tc = (1 << RGB_R_GPIO_PIN); */
      GPIO_OUTPUT_SET(RGB_R_GPIO_PIN,0);
  else
      /* GPIO.out_w1ts = (1 << RGB_R_GPIO_PIN);   /\* gpio_set_level(RGB_R_GPIO_PIN, 1); *\/ */
      GPIO_OUTPUT_SET(RGB_R_GPIO_PIN,1);

  if(green)
      /* GPIO.out_w1tc = (1 << RGB_G_GPIO_PIN); */
      GPIO_OUTPUT_SET(RGB_G_GPIO_PIN,0);
   /* gpio_set_level(RGB_G_GPIO_PIN, 0); // low level effort */
  else
      /* GPIO.out_w1ts = (1 << RGB_G_GPIO_PIN);  //gpio_set_level(RGB_G_GPIO_PIN, 1); */
      GPIO_OUTPUT_SET(RGB_G_GPIO_PIN,1);

  if(blue)
      /* GPIO.out_w1tc = (1 << RGB_B_GPIO_PIN); */
      GPIO_OUTPUT_SET(RGB_B_GPIO_PIN,0);
   /* gpio_set_level(RGB_B_GPIO_PIN, 0); // low level effort */
  else
      /* GPIO.out_w1ts = (1 << RGB_B_GPIO_PIN);  // gpio_set_level(RGB_B_GPIO_PIN, 1); */
      GPIO_OUTPUT_SET(RGB_B_GPIO_PIN,1);
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
    gpio_matrix_out(RGB_R_GPIO_PIN, SIG_GPIO_OUT_IDX, 0, 0);
    GPIO.enable_w1ts = (0x1 << RGB_R_GPIO_PIN); //output enable
    GPIO.pin[RGB_R_GPIO_PIN].int_ena = 0;       //disable GPIO intr
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO27_U, PIN_FUNC_GPIO);
    /* REG_CLR_BIT(PERIPHS_IO_MUX_GPIO27_U, FUN_PD); */
    /* REG_CLR_BIT(PERIPHS_IO_MUX_GPIO27_U, FUN_PU); */
    /* GPIO.pin[RGB_R_GPIO_PIN].int_ena = 1UL<<2;  //disable pro cpu intr */

    /* gpio_matrix_out(RGB_G_GPIO_PIN, SIG_GPIO_OUT_IDX, 0, 0); */
    GPIO.enable_w1ts = (0x1 << RGB_G_GPIO_PIN); //output enable
    GPIO.pin[RGB_G_GPIO_PIN].int_ena = 0;       //disable GPIO intr
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO21_U, PIN_FUNC_GPIO);//set register

    /* gpio_matrix_out(RGB_B_GPIO_PIN, SIG_GPIO_OUT_IDX, 0, 0); */
    GPIO.enable_w1ts = (0x1 << RGB_B_GPIO_PIN); //output enable
    GPIO.pin[RGB_B_GPIO_PIN].int_ena = 0;       //disable GPIO intr
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO22_U, PIN_FUNC_GPIO);

    PIN_PULLDWN_DIS(MODE_BOTTON_GPIO_PIN);
    /* PIN_PULLUP_EN(MODE_BOTTON_GPIO_PIN); */
    PIN_PULLUP_DIS(MODE_BOTTON_GPIO_PIN);
    PIN_INPUT_ENABLE(MODE_BOTTON_GPIO_PIN);

    /* GPIO.out_w1ts = (1 << RGB_R_GPIO_PIN); */
    /* GPIO.out_w1ts = (1 << RGB_G_GPIO_PIN); */
    /* GPIO.out_w1ts = (1 << RGB_B_GPIO_PIN); */

    /* GPIO.out_w1tc = (1 << RGB_R_GPIO_PIN); */
    /* GPIO.out_w1tc = (1 << RGB_G_GPIO_PIN); */
    /* GPIO.out_w1tc = (1 << RGB_B_GPIO_PIN); */

   GPIO_OUTPUT_SET(RGB_R_GPIO_PIN,1);
    while(1);


    #if 0
    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO27/21/22
    io_conf.pin_bit_mask = RGB_GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    //interrupt of rising edge
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //bit mask of the pins, use GPIO0 here
    io_conf.pin_bit_mask = BUTTON_GPIO_INPUT_PIN_SEL;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);
    #endif
}

uint8_t HAL_UI_Mode_BUTTON_GetState(Button_TypeDef Button)
{
  /* return gpio_get_level(MODE_BOTTON_GPIO_PIN); */
    /* return (GPIO.in >> MODE_BOTTON_GPIO_PIN) & 0x1; */
    return GPIO_INPUT_GET(MODE_BOTTON_GPIO_PIN);
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
