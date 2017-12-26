#include <string.h>
#include <stdint.h>
#include <limits.h>

#include "boot_mode.h"
#include "memory_hal.h"
#include "system_config.h"
#include "params_hal.h"

#include "flash_map.h"
#include "boot_debug.h"

extern "C"
{
#include "bootloader_flash.h"
}

#define UPDATE_BLINK_PERIOD 100

void start_app(void)
{
}

/*从flash起始地址复制至目标地址*/
bool copy_raw(const uint32_t src_addr, const uint32_t dst_addr, const uint32_t size)
{
    // require regions to be aligned
    if (src_addr & 0xfff != 0 ||
        dst_addr & 0xfff != 0){
        return false;
    }

    const uint32_t buffer_size = FLASH_SECTOR_SIZE;
    uint8_t buffer[buffer_size];
    uint32_t left = ((size+buffer_size-1) & ~(buffer_size-1));
    uint32_t saddr = src_addr;
    uint32_t daddr = dst_addr;

    while (left){
        if (ESP_OK != bootloader_flash_erase_sector(daddr/buffer_size)){
            return false;
        }
        if (ESP_OK != bootloader_flash_read(saddr, (uint32_t*)buffer, buffer_size, false)){
            return false;
        }
        if (ESP_OK != bootloader_flash_write(daddr, (uint32_t*)buffer, buffer_size, false)){
            return false;
        }
        saddr += buffer_size;
        daddr += buffer_size;
        left  -= buffer_size;
    }

    return true;
}
extern "C"
{
    extern uint32_t HAL_PARAMS_Get_Boot_ota_app_size(void);
    extern uint32_t HAL_PARAMS_Get_Boot_def_app_size(void);
}

bool FLASH_Restore(Firmware_TypeDef FmType)
{
    bool result;
    uint32_t size;

    if(DEFAULT_FIRWARE == FmType) {
        result = true;
    }
    else
    {
        size = HAL_PARAMS_Get_Boot_ota_app_size();
        if(0 == size) {
            return true;
        }
        result = copy_raw(CACHE_ONLINE_APP_START_ADDR, APP_START_ADDR, size);
    }
    return result;
}

bool OTA_Flash_Reset(void)
{
    return FLASH_Restore(OTA_FIRWARE);
}

void Enter_Factory_RESTORE_Mode(void)
{
    HAL_PARAMS_Set_Boot_initparam_flag(INITPARAM_FLAG_FACTORY_RESET);
    HAL_PARAMS_Set_Boot_boot_flag(BOOT_FLAG_NORMAL);
    HAL_PARAMS_Save_Params();
}

void Enter_OTA_Update_Mode(void)
{
    if(OTA_Flash_Reset()){
        HAL_PARAMS_Set_Boot_boot_flag(BOOT_FLAG_NORMAL);
        HAL_PARAMS_Save_Params();
    } else {
        System_Reset();
    }
}

