/* Copyright (c) 2015-2016 Ivan Grokhotkov. All rights reserved.
 * This file is part of eboot bootloader.
 *
 * Redistribution and use is permitted according to the conditions of the
 * 3-clause BSD license to be found in the LICENSE file.
 */

#include "hw_config.h"
#include "boot_mode.h"
#include "flash.h"
#include "esp8266/esp8266_config.h"
#include "params_hal.h"
#include "ui_hal.h"
#include "bkpreg_hal.h"
#include "boot_debug.h"


//#define BOOTLOADER_VERSION  1
//#define BOOTLOADER_VERSION  2       //修改开机三色灯高亮问题
#define BOOTLOADER_VERSION  2       //1. 简化工作模式  2. 简化灯的变化


uint8_t DEFAULT_FIRMWARE_MODE = 0;
uint8_t FACTORY_RESET_MODE = 0;
uint8_t START_APP_MODE = 0;
uint8_t OTA_FIRMWARE_MODE = 0;

uint32_t BUTTON_press_time = 0;

int main()
{
    BOOT_DEBUG("boot start...\r\n");
    Set_System();

    //有时上电时，RGB灯很亮，应该是GPIO5脚，上电过程中管脚状态不确定，需确定。
    //重复设置5次，避免RGB很亮。
    for(int i = 0; i < 5; i++)
    {
        HAL_UI_RGB_Color(RGB_COLOR_WHITE);
        delay(1);
    }

    HAL_PARAMS_Load_Boot_Params();
    HAL_PARAMS_Load_System_Params();

    if(0x7DEA != HAL_Core_Read_Backup_Register(BKP_DR_03))
    {
        //延时1.5s 等待用户进入配置模式
        delay(1500);
    }
    HAL_Core_Write_Backup_Register(BKP_DR_03, 0xFFFF);

    if(BOOTLOADER_VERSION != HAL_PARAMS_Get_Boot_boot_version())
    {
        BOOT_DEBUG("save boot version...\r\n");
        HAL_PARAMS_Set_Boot_boot_version(BOOTLOADER_VERSION);
        HAL_PARAMS_Save_Params();
    }

    if(!HAL_UI_Mode_BUTTON_GetState(BUTTON1))
    {
#define TIMING_DEFAULT_RESTORE_MODE  4000   //默认程序恢复判断时间
        while (!HAL_UI_Mode_BUTTON_GetState(BUTTON1))
        {
            BUTTON_press_time = HAL_UI_Mode_Button_Pressed();
            if(BUTTON_press_time > TIMING_DEFAULT_RESTORE_MODE)
            {
                DEFAULT_FIRMWARE_MODE = 1;
                HAL_UI_RGB_Color(RGB_COLOR_GREEN);
            }
        }
    }
    else
    {
        switch(HAL_PARAMS_Get_Boot_boot_flag())
        {
            case BOOT_FLAG_NORMAL:          //正常启动
                START_APP_MODE = 1;
                break;
            case BOOT_FLAG_DEFAULT_RESTORE: //默认程序下载
                DEFAULT_FIRMWARE_MODE = 1;
                break;
            case BOOT_FLAG_FACTORY_RESET:   //恢复出厂
                FACTORY_RESET_MODE = 1;
                break;
            case BOOT_FLAG_OTA_UPDATE:      //在线升级
                OTA_FIRMWARE_MODE = 1;
                break;
            default:
                break;
        }
    }

    if(FACTORY_RESET_MODE)
    {
        BOOT_DEBUG("factroy reset\r\n");
        Enter_Factory_RESTORE_Mode();
        FACTORY_RESET_MODE = 0;
    }
    else if(DEFAULT_FIRMWARE_MODE)
    {
        BOOT_DEBUG("default firmware mode\r\n");
        Enter_Default_RESTORE_Mode();
        DEFAULT_FIRMWARE_MODE = 0;
    }
    else if(OTA_FIRMWARE_MODE)
    {
        BOOT_DEBUG("ota firmware\r\n");
        Enter_OTA_Update_Mode();
        OTA_FIRMWARE_MODE = 0;
    }

    BOOT_DEBUG("start app\r\n");
    HAL_UI_RGB_Color(RGB_COLOR_BLACK); //防止进入应用程序初始化三色灯 导致闪灯
    start_app();
    return 0;
}

