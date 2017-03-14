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
#include "core_hal.h"

#define OTA_CHUNK_SIZE          512

static bool bootloader_requires_update(void)
{
    return false;
}

static bool bootloader_update(void)
{
    return true;
}

bool HAL_Bootloader_Update_If_Needed(void)
{
    bool updated = false;

    if (bootloader_requires_update()) {
        updated = bootloader_update();
    }
    return updated;
}

down_status_t HAL_OTA_Download_App(const char *host, const char *param, const char * md5)
{
    return DOWNSTATUS_SUCCESS;
}

down_status_t HAL_OTA_Get_App_Download_Status(void)
{
    return DOWNSTATUS_SUCCESS;
}

void HAL_OTA_Update_App(void)
{
}

down_status_t HAL_OTA_Download_Subsys(const char *host, const char *param)
{
    return DOWNSTATUS_SUCCESS;
}

down_status_t HAL_OTA_Get_Subsys_Download_Status(void)
{
    return DOWNSTATUS_SUCCESS;
}

void HAL_OTA_Upadate_Subsys(void)
{
}

uint8_t HAL_OTA_Get_Download_Progress()
{
    return 0;
}

bool HAL_OTA_CheckValidAddressRange(uint32_t startAddress, uint32_t length)
{
    uint32_t endAddress = startAddress + length;

#ifdef USE_SERIAL_FLASH
    if (startAddress == EXTERNAL_FLASH_OTA_ADDRESS && endAddress <= 0x100000)
    {
        return true;
    }
#else
    if (startAddress == INTERNAL_FLASH_OTA_ADDRESS && endAddress < 0x8100000)
    {
        return true;
    }
#endif

    return false;
}

uint32_t HAL_OTA_FlashAddress()
{
#ifdef USE_SERIAL_FLASH
    return EXTERNAL_FLASH_OTA_ADDRESS;
#else
    return 0;
#endif
}

uint32_t HAL_OTA_FlashLength()
{
    return 0;
}

uint16_t HAL_OTA_ChunkSize()
{
    return OTA_CHUNK_SIZE;
}

bool HAL_FLASH_Begin(uint32_t address, uint32_t length, void* reserved)
{
    FLASH_Begin(address, length);
    return true;
}

int HAL_FLASH_Update(const uint8_t *pBuffer, uint32_t address, uint32_t length, void* reserved)
{
    return FLASH_Update(pBuffer, address, length);
}

hal_update_complete_t HAL_FLASH_End(hal_module_t* mod)
{
    return 0;
}

