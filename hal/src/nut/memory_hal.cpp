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
#include "flash_utils.h"
#include "binary.h"

static const int FLASH_INT_MASK = ((B10 << 8) | B00111010);

/**
 * @brief  Gets the sector of a given address
 * @param  None
 * @retval The sector of a given address
 */
uint32_t HAL_FLASH_Interminal_Get_Sector(uint32_t address)
{
    return address/FLASH_SECTOR_SIZE;
}

/**
 * @brief  Gets the sector of a given address
 * @param  None
 * @retval The sector of a given address
 */
void HAL_FLASH_Interminal_Erase(uint32_t sector)
{
    ets_isr_mask(FLASH_INT_MASK);
    spi_flash_erase_sector(sector);
    ets_isr_unmask(FLASH_INT_MASK);
}

void HAL_FLASH_Interminal_Read(uint32_t address, uint32_t *pdata, uint32_t datalen)
{
    ets_isr_mask(FLASH_INT_MASK);
    spi_flash_read(address, pdata, datalen);
    ets_isr_unmask(FLASH_INT_MASK);
}

int HAL_FLASH_Interminal_Write(uint32_t address, uint32_t *pdata, uint32_t datalen)
{
    ets_isr_mask(FLASH_INT_MASK);
    spi_flash_write(address, pdata, datalen);
    ets_isr_unmask(FLASH_INT_MASK);
}

