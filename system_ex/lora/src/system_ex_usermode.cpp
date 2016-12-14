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
#include "service_debug.h"
#include "core_hal.h"
#include "params_hal.h"
#include "system_rgbled.h"
#include "system_ex_setup.h"
#include "system_ex_usermode.h"

volatile button_mode_t BUTTON_Mode = BUTTON_MODE_NONE;
volatile uint32_t BUTTON_press_time;


#define TIMING_CONFIG_MODE           3000   //进入配置模式判断时间
#define TIMING_DEFAULT_RESTORE_MODE  7000   //进入默认固件灯程序升级判断时间
#define TIMING_SERIAL_COM_MODE       10000  //进入串口转接程序判断时间
#define TIMING_FACTORY_RESET_MODE    13000  //恢复出厂程序判断时间 不清空密钥
#define TIMING_NC                    20000  //无操作判断时间
#define TIMING_ALL_RESET_MODE        30000  //完全恢复出厂判断时间 清空密钥

extern "C" void HAL_SysTick_Handler(void)
{
#ifndef configNO_SETUPBUTTON_UI
    BUTTON_press_time = HAL_UI_Mode_Button_Pressed();
    if(BUTTON_press_time)
    {
        if( BUTTON_press_time > TIMING_ALL_RESET_MODE ) {
            if(BUTTON_Mode!=BUTTON_MODE_RESET) {
                BUTTON_Mode=BUTTON_MODE_RESET; //恢复出厂设置  清除密钥
                HAL_UI_RGB_Color(RGB_COLOR_YELLOW);//黄灯打开
            }
        }
        else if( BUTTON_press_time > TIMING_NC ) {
            if(BUTTON_Mode!=BUTTON_MODE_NC) {
                BUTTON_Mode=BUTTON_MODE_NC; //退出
                HAL_UI_RGB_Color(RGB_COLOR_BLACK);//关灯
            }
        }
        else if( BUTTON_press_time > TIMING_FACTORY_RESET_MODE ) {
            if(BUTTON_Mode!=BUTTON_MODE_FAC) {
                BUTTON_Mode=BUTTON_MODE_FAC;//恢复出厂设置  不清除密钥
                HAL_UI_RGB_Color(RGB_COLOR_CYAN);//浅蓝灯打开
            }
        }
        else if( BUTTON_press_time > TIMING_SERIAL_COM_MODE ) {
            if(BUTTON_Mode!=BUTTON_MODE_COM) {
                BUTTON_Mode=BUTTON_MODE_COM;//进入串口转发程序
                HAL_UI_RGB_Color(RGB_COLOR_BLUE);//蓝灯打开
            }
        }
        else if( BUTTON_press_time > TIMING_DEFAULT_RESTORE_MODE ) {
            if(BUTTON_Mode!=BUTTON_MODE_DEFFW) {
                BUTTON_Mode=BUTTON_MODE_DEFFW; //恢复默认出厂程序
                HAL_UI_RGB_Color(RGB_COLOR_GREEN);//绿灯打开
            }
        }
        else if( BUTTON_press_time > TIMING_CONFIG_MODE ) {
            if(BUTTON_Mode!=BUTTON_MODE_CONFIG) {
                BUTTON_Mode=BUTTON_MODE_CONFIG; //wifi配置模式
                HAL_UI_RGB_Color(RGB_COLOR_RED);//红灯打开
            }
        }
    }
    else
    {
        switch(BUTTON_Mode)
        {
            case BUTTON_MODE_CONFIG:
                HAL_Core_Enter_Config_Mode();
                break;

            case BUTTON_MODE_DEFFW:
                HAL_Core_Enter_Firmware_Recovery_Mode();
                break;

            case BUTTON_MODE_COM:
                HAL_Core_Enter_Com_Mode();
                break;

            case BUTTON_MODE_FAC:
                HAL_Core_Enter_Factory_Reset_Mode();
                break;

            case BUTTON_MODE_NC:
                HAL_Core_System_Reset();
                break;

            case BUTTON_MODE_RESET:
                HAL_Core_Enter_Factory_All_Reset_Mode();
                break;

            default:
                break;
        }
        if(BUTTON_MODE_NONE != BUTTON_Mode) {
            BUTTON_Mode = BUTTON_MODE_NONE;
        }
    }
#endif
}

// These are internal methods
void manage_setup_config(void)
{
    if(HAL_PARAMS_Get_System_config_flag())
    {
        DEBUG_D(("enter device config\r\n"));
        system_rgb_blink(RGB_COLOR_RED, 1000);
        DeviceSetupUsb.init();
        DeviceSetupUsart.init();
        while(1)
        {
            if( DeviceSetupUsb.process() )
            {
                break;
            }

            if( DeviceSetupUsart.process() )
            {
                break;
            }
        }
        DEBUG_D(("exit  device config\r\n"));
        HAL_PARAMS_Set_System_config_flag(0);
        HAL_PARAMS_Save_Params();
    }
}

