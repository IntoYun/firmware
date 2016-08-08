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
void HAL_FLASH_Interminal_Erase(uint32_t address)
{
}

void HAL_FLASH_Interminal_Read(uint32_t address, uint16_t *pdata, uint32_t datalen)
{
}

int HAL_FLASH_Interminal_Write(uint32_t address, uint16_t *pdata, uint32_t datalen)
{
    return 0;
}

