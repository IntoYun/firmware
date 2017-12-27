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

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
  HAL_FLASH_STATUS_OK       = 0x00U,
  HAL_FLASH_STATUS_ERROR    = 0x01U,
  HAL_FLASH_STATUS_BUSY     = 0x02U,
  HAL_FLASH_STATUS_TIMEOUT  = 0x03U
} HAL_Flash_StatusTypeDef;

uint32_t HAL_FLASH_Interminal_Get_Sector(uint32_t address);
HAL_Flash_StatusTypeDef HAL_FLASH_Interminal_Erase(uint32_t sector);
HAL_Flash_StatusTypeDef HAL_FLASH_Interminal_Read(uint32_t address, uint32_t *pdata, uint32_t datalen);
HAL_Flash_StatusTypeDef HAL_FLASH_Interminal_Write(uint32_t address, uint32_t *pdata, uint32_t datalen);

#ifdef __cplusplus
}
#endif


