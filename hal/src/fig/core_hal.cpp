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
#include "core_hal.h"
#include "intorobot_macros.h"
#include "flash_map.h"
#include "flash_storage_impl.h"

uint16_t HAL_Core_Get_Subsys_Version(char* buffer, uint16_t len)
{
    char data[32];
    uint16_t templen;
    InternalFlashStore flashStore;

    if (buffer!=NULL && len>0) {
        flashStore.read(SUBSYS_VERSION_ADDR, (uint32_t *)data, sizeof(data));
        DEBUG("subver = %s\r\n", data);
        if(!memcmp(data, "VERSION:", 8))
        {
            templen = MIN(strlen(&data[8]), len-1);
            memset(buffer, 0, len);
            memcpy(buffer, &data[8], templen);
            return templen;
        }
    }
    return 0;
}

