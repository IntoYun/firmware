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

#include "ota_flash_hal.h"
#include "esp8266_downfile.h"
#include "core_hal.h"
#include "flash_map.h"
#include "memory_hal.h"
#include "flash_utils.h"
#include "params_hal.h"
#include "ui_hal.h"
#include "service_debug.h"

extern "C"
{
    extern uint32_t HAL_PARAMS_Get_Boot_def_app_size(void);
    extern int HAL_PARAMS_Set_Boot_def_app_size(uint32_t size);
    extern uint32_t HAL_PARAMS_Get_Boot_boot_size(void);
    extern int HAL_PARAMS_Set_Boot_boot_size(uint32_t size);
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
        if (HAL_FLASH_Interminal_Erase(daddr/buffer_size)){
            return false;
        }
        if (HAL_FLASH_Interminal_Read(saddr, (uint32_t*)buffer, buffer_size)){
            return false;
        }
        if (HAL_FLASH_Interminal_Write(daddr, (uint32_t*)buffer, buffer_size)){
            return false;
        }
        saddr += buffer_size;
        daddr += buffer_size;
        left  -= buffer_size;
    }
    return true;
}

down_status_t HAL_OTA_Download_App(const char *host, const char *param, const char * md5)
{
    return esp8266_downOnlineApp(host, param, md5);
}

down_status_t HAL_OTA_Get_App_Download_Status(void)
{
    return esp8266_getDownOnlineAppStatus();
}

void HAL_OTA_Update_App(void)
{
    HAL_Core_Enter_Ota_Update_Mode();
}

down_status_t HAL_OTA_Download_Subsys(const char *host, const char *param)
{
    return esp8266_downDefaultApp(host, param);
}

down_status_t HAL_OTA_Get_Subsys_Download_Status(void)
{
    return esp8266_getDownDefaultAppStatus();
}

void HAL_OTA_Upadate_Subsys(void)
{
    int count=3;
    uint32_t def_app_size = HAL_PARAMS_Get_Boot_def_app_size();
    uint32_t boot_size = HAL_PARAMS_Get_Boot_boot_size();

    if(def_app_size&&boot_size)
    {
        while(count--) {
            if(copy_raw(CACHE_BOOT_ADDR, BOOT_ADDR, boot_size)) {
                if(copy_raw(CACHE_DEFAULT_APP_ADDR, DEFAULT_APP_ADDR, def_app_size)) {
                    break;
                }
            }
        }
    }
    HAL_PARAMS_Set_Boot_boot_size(0);
    HAL_PARAMS_Set_Boot_def_app_size(0);
    HAL_PARAMS_Save_Params();
}

uint8_t HAL_OTA_Get_Download_Progress()
{
    return esp8266_getDownloadProgress();
}

