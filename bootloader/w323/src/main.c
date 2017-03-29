/* Copyright (c) 2015-2016 Ivan Grokhotkov. All rights reserved.
 * This file is part of eboot bootloader.
 *
 * Redistribution and use is permitted according to the conditions of the
 * 3-clause BSD license to be found in the LICENSE file.
 */

#include "boot_mode.h"
#include "system_config.h"
#include "params_hal.h"
#include "bkpreg_hal.h"
#include "boot_debug.h"

#include "esp_attr.h"
#include "soc/dport_reg.h"
#include "soc/cpu.h"


#define BOOTLOADER_VERSION  1
#define LIGHTTIME           400

uint8_t FACTORY_RESET_MODE=0;
uint8_t START_APP_MODE=0;
uint8_t OTA_FIRMWARE_MODE=0;

uint32_t BUTTON_press_time=0;


extern int _bss_start;
extern int _bss_end;


void IRAM_ATTR call_start_cpu0()
{
    cpu_configure_region_protection();
    //Clear bss
    memset(&_bss_start, 0, (&_bss_end - &_bss_start) * sizeof(_bss_start));
    /* completely reset MMU for both CPUs
       (in case serial bootloader was running) */
    Cache_Read_Disable(0);
    Cache_Read_Disable(1);
    Cache_Flush(0);
    Cache_Flush(1);
    mmu_init(0);
    REG_SET_BIT(DPORT_APP_CACHE_CTRL1_REG, DPORT_APP_CACHE_MMU_IA_CLR);
    mmu_init(1);
    REG_CLR_BIT(DPORT_APP_CACHE_CTRL1_REG, DPORT_APP_CACHE_MMU_IA_CLR);
    /* (above steps probably unnecessary for most serial bootloader
       usage, all that's absolutely needed is that we unmask DROM0
       cache on the following two lines - normal ROM boot exits with
       DROM0 cache unmasked, but serial bootloader exits with it
       masked. However can't hurt to be thorough and reset
       everything.)

       The lines which manipulate DPORT_APP_CACHE_MMU_IA_CLR bit are
       necessary to work around a hardware bug.
    */
    REG_CLR_BIT(DPORT_PRO_CACHE_CTRL1_REG, DPORT_PRO_CACHE_MASK_DROM0);
    REG_CLR_BIT(DPORT_APP_CACHE_CTRL1_REG, DPORT_APP_CACHE_MASK_DROM0);

    main();
}

int main()
{
    BOOT_DEBUG("boot start...\r\n");
    Set_System();

    HAL_PARAMS_Load_Boot_Params();
    HAL_PARAMS_Load_System_Params();

    if(BOOTLOADER_VERSION != HAL_PARAMS_Get_Boot_boot_version())
    {
        BOOT_DEBUG("save boot version...\r\n");
        HAL_PARAMS_Set_Boot_boot_version(BOOTLOADER_VERSION);
        HAL_PARAMS_Save_Params();
    }

    switch(HAL_PARAMS_Get_Boot_boot_flag())
    {
        case BOOT_FLAG_NORMAL:          //正常启动
            START_APP_MODE = 1;
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

    if(FACTORY_RESET_MODE)
    {
        BOOT_DEBUG("factroy reset\r\n");
        Enter_Factory_RESTORE_Mode();
        FACTORY_RESET_MODE = 0;
    }
    else if(OTA_FIRMWARE_MODE)
    {
        BOOT_DEBUG("ota firmware\r\n");
        Enter_OTA_Update_Mode();
        OTA_FIRMWARE_MODE = 0;
    }

    BOOT_DEBUG("start app\r\n");
    start_app();
    return 0;
}
