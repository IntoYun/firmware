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

#include "memory_hal.h"
#include "hw_config.h"

/**
 * @brief  Gets the sector of a given address
 * @param  None
 * @retval The sector of a given address
 */
uint32_t HAL_FLASH_Interminal_Get_Sector(uint32_t address)
{
    return (address-INTERNAL_FLASH_START)/INTERNAL_FLASH_PAGE_SIZE;
}

HAL_Flash_StatusTypeDef HAL_FLASH_Interminal_Erase(uint32_t sector)
{
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PAGEError = 0;

    HAL_FLASH_Unlock();

    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = INTERNAL_FLASH_PAGE_SIZE*sector + INTERNAL_FLASH_START;
    EraseInitStruct.NbPages     = 1;
    HAL_StatusTypeDef rslt = HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError);
    HAL_FLASH_Lock();
    return (HAL_Flash_StatusTypeDef)rslt;
}

HAL_Flash_StatusTypeDef HAL_FLASH_Interminal_Read(uint32_t address, uint32_t *pdata, uint32_t datalen)
{
    uint32_t endAddress = address + datalen*4;
    uint16_t i = 0;

    while(address < endAddress)
    {
        pdata[i++] = (*(uint32_t*)address);
        address = address + 4;
    }
    return HAL_FLASH_STATUS_OK;
}

HAL_Flash_StatusTypeDef HAL_FLASH_Interminal_Write(uint32_t address, uint32_t *pdata, uint32_t datalen)
{
    uint32_t endAddress = address + datalen * 4;
    uint16_t i = 0;

    HAL_StatusTypeDef flashStatus = HAL_OK;
    HAL_FLASH_Unlock();

    while((address < endAddress) && (flashStatus == HAL_OK))
    {
        flashStatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, pdata[i++]);
        address = address + 4;
    }

    HAL_FLASH_Lock();
    return (HAL_Flash_StatusTypeDef)flashStatus;
}

