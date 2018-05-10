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

#include "updater_hal.h"
#include "core_hal.h"
#include "flash_mal.h"
#include "flash_map.h"
#include "flash_storage_impl.h"
#include "params_hal.h"
#include "ui_hal.h"
#include "esp8266_downfile.h"

static THandlerFunction_Progress _update_handle = NULL;

bool HAL_Update(const char *host, const char *uri, const char * md5, updater_mode_t mode)
{
    file_type_t type = OTA_APP_FILE;
    bool flag = false;
    down_status_t status;

    if(UPDATER_MODE_DOWNLOAD == mode) {
        type = DOWNLOAD_FILE;
    }

    if(NULL != _update_handle) {
        esp8266_set_downfile_handle((esp8266_downfile_handle_t)_update_handle);
    }

    down_status_t result = esp8266_downfile(host, uri, "", type);
    switch(result) {
        case DOWNSTATUS_SUCCESS:
            flag = true;
            break;
        case DOWNSTATUS_DOWNING:
            do {
                status = esp8266_getDownfileStatus();
                if(DOWNSTATUS_SUCCESS == status) {
                    flag = true;
                    break;
                } else if(DOWNSTATUS_FAIL == status) {
                    break;
                }
            } while(1);
            break;
        default:
            break;
    }

    return flag;
}

void HAL_Set_Update_Handle(THandlerFunction_Progress fn)
{
    _update_handle = fn;
}

void HAL_Set_Update_Flag(uint32_t size)
{
    HAL_PARAMS_Set_Boot_ota_app_size(size);
    HAL_PARAMS_Set_Boot_boot_flag(BOOT_FLAG_OTA_UPDATE);
    HAL_PARAMS_Save_Params();
}

uint32_t HAL_Update_StartAddress()
{
    return 0;
}

uint32_t HAL_Update_FlashLength()
{
    return 0;
}

int HAL_Update_Flash(const uint8_t *pBuffer, uint32_t address, uint32_t length, void* reserved)
{
    return -1;
}
