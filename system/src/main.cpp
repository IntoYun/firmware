/**
 ******************************************************************************
 * @file    main.cpp

 * @version V1.0.0
 * @date    13-March-2013
 *
 * Updated: 14-Feb-2014 David Sidrane <david_s5@usa.net>
 *
 * @brief   Main program body.
 ******************************************************************************
 Copyright (c) 2013-2015 Particle Industries, Inc.  All rights reserved.

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation, either
 version 3 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this program; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "service_debug.h"
#include "system_event.h"
#include "system_mode.h"
#include "system_task.h"
#include "system_network.h"
#include "system_network_internal.h"
#include "system_sleep.h"
#include "system_threading.h"
#include "system_user.h"
#include "system_update.h"
#include "core_hal.h"
#include "delay_hal.h"
#include "syshealth_hal.h"
#include "watchdog_hal.h"
#include "usb_hal.h"
#include "system_mode.h"
#include "system_rgbled.h"
#include "params_hal.h"
#include "wiring_interrupts.h"
#include "wiring_cellular.h"
#include "wiring_cellular_printable.h"
#include "system_rgbled.h"
#include "system_version.h"

using namespace intorobot;

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
volatile button_mode_t BUTTON_Mode = BUTTON_MODE_NONE;
volatile uint32_t BUTTON_press_time;
volatile uint8_t INTOROBOT_IMLINK_CONFIG_IS_NEEDED=true; //进入配置模式
volatile uint8_t INTOROBOT_IMLINK_CONFIG_START=false; //配置模式开始


/*******************************************************************************
 * Function Name  : HAL_SysTick_Handler
 * Description    : Decrements the various Timing variables related to SysTick.
 * Input          : None
 * Output         : None.
 * Return         : None.
 ************************************************
 *******************************/
#define TIMING_DFU_DOWNLOAD_MODE     1000   //dfu 下载模式
#define TIMING_ESP8266_UPDATE_MODE   3000   //esp8266 升级判断时间
#define TIMING_DEFAULT_RESTORE_MODE  7000   //默认固件灯程序升级判断时间
#define TIMING_ESP8266_COM_MODE      10000  //esp8266串口转接判断时间
#define TIMING_FACTORY_RESET_MODE    13000  //恢复出厂程序判断时间 不清空密钥
#define TIMING_NC                    20000  //无操作判断时间
#define TIMING_ALL_RESET_MODE        30000  //完全恢复出厂判断时间 清空密钥

extern "C" void HAL_SysTick_Handler(void)
{
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
        else if( BUTTON_press_time > TIMING_ESP8266_COM_MODE ) {
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
        else if( BUTTON_press_time > TIMING_DFU_DOWNLOAD_MODE ) {
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
                INTOROBOT_IMLINK_CONFIG_IS_NEEDED = true;
                INTOROBOT_IMLINK_CONFIG_START=false;
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
}

void app_loop(void)
{
    if(!INTOROBOT_IMLINK_CONFIG_IS_NEEDED)
    {
        DECLARE_SYS_HEALTH(ENTERED_WLAN_Loop);

        static uint8_t INTOROBOT_WIRING_APPLICATION = 0;
        if ((INTOROBOT_WIRING_APPLICATION != 1))
        {
            //Execute user application setup only once
            DECLARE_SYS_HEALTH(ENTERED_Setup);
            if (system_mode()!=SAFE_MODE)
                setup();
            INTOROBOT_WIRING_APPLICATION = 1;
            _post_loop();
        }

        //Execute user application loop
        DECLARE_SYS_HEALTH(ENTERED_Loop);
        if (system_mode()!=SAFE_MODE) {
            loop();
            DECLARE_SYS_HEALTH(RAN_Loop);
            _post_loop();
        }
    }
}

static void app_load_params(void)
{
    // load params
    HAL_PARAMS_Load_System_Params();
    HAL_PARAMS_Load_Boot_Params();
    // check if need init params
    if(INITPARAM_FLAG_FACTORY_RESET == HAL_PARAMS_Get_Boot_initparam_flag()) //初始化参数 保留密钥
    {
        DEBUG_D("init params fac\r\n");
        HAL_PARAMS_Init_Fac_System_Params();
    }
    else if(INITPARAM_FLAG_ALL_RESET == HAL_PARAMS_Get_Boot_initparam_flag()) //初始化所有参数
    {
        DEBUG_D("init params all\r\n");
        HAL_PARAMS_Init_All_System_Params();
    }
    if(INITPARAM_FLAG_NORMAL != HAL_PARAMS_Get_Boot_initparam_flag()) //初始化参数 保留密钥
    {
        HAL_PARAMS_Set_Boot_initparam_flag(INITPARAM_FLAG_NORMAL);
    }

    //保存子系统程序版本号
    char subsys_ver1[32] = {0}, subsys_ver2[32] = {0};
    HAL_Core_Get_Subsys_Version(subsys_ver1, sizeof(subsys_ver1));
    HAL_PARAMS_Get_System_subsys_ver(subsys_ver2, sizeof(subsys_ver2));
    if(strcmp(subsys_ver1, subsys_ver2))
    {
        HAL_PARAMS_Set_System_subsys_ver(subsys_ver1);
    }

    //保存固件库版本号
    char fw_ver1[32] = {0}, fw_ver2[32] = {0};
    system_version(fw_ver1);
    HAL_PARAMS_Get_System_fwlib_ver(fw_ver2, sizeof(fw_ver2));
    if(strcmp(fw_ver1, fw_ver2))
    {
        HAL_PARAMS_Set_System_fwlib_ver(fw_ver1);
    }
    HAL_PARAMS_Save_Params();
}
/*******************************************************************************
 * Function Name  : main.
 * Description    : main routine.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void app_setup_and_loop_initial(void)
{
    HAL_Core_Init();
    // We have running firmware, otherwise we wouldn't have gotten here
    DECLARE_SYS_HEALTH(ENTERED_Main);
    // load params
    app_load_params();

    set_system_mode(DEFAULT);

    DEBUG_D("welcome from IntoRobot!\r\n");
    String s = intorobot_deviceID();
    DEBUG_D("Device %s started\r\n", s.c_str());

#if defined (START_DFU_FLASHER_SERIAL_SPEED) || defined (START_YMODEM_FLASHER_SERIAL_SPEED)
    USB_USART_LineCoding_BitRate_Handler(system_lineCodingBitRateHandler);
#endif

#if PLATFORM_THREADING
    create_system_task();
#else
    HAL_Core_Set_System_Loop_Handler(&system_process_loop);
#endif
}

/*******************************************************************************
 * Function Name  : main.
 * Description    : main routine.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void app_setup_and_loop(void)
{
    app_setup_and_loop_initial();
    /* Main loop */
    while (1) {
        app_loop();
    }
}

#ifdef USE_FULL_ASSERT
/*******************************************************************************
 * Function Name  : assert_failed
 * Description    : Reports the name of the source file and the source line number
 *                  where the assert_param error has occurred.
 * Input          : - file: pointer to the source file name
 *                  - line: assert_param error line source number
 * Output         : None
 * Return         : None
 *******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif
