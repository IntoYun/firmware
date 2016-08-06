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

MemoryDevice::~MemoryDevice()
{
}

class ExternalFlashDevice : public MemoryDevice {

    /**
     * @return The size of each page in this flash device.
     */
    virtual mem_page_size_t pageSize() const {
        return sFLASH_PAGESIZE;
    }

    /**
     * @return The number of pages in this flash device.
     */
    virtual mem_page_count_t pageCount() const {
        return 512;
    }

    virtual bool erasePage(mem_addr_t address) {
        bool success = false;
        if (address < pageAddress(pageCount()) && (address % pageSize()) == 0) {
            sFLASH_EraseSector(address);
            success = true;
        }
        return success;
    }

    /**
     * Writes directly to the flash. Depending upon the state of the flash, the
     * write may provide the data required or it may not.
     * @param data
     * @param address
     * @param length
     * @return
     */
    virtual bool write(const void* data, mem_addr_t address, mem_page_size_t length) {
        // TODO: SPI interface shouldn't need mutable data buffer to write?
        sFLASH_WriteBuffer(const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(data)), address, length);
        return true;
    }

    virtual bool read(void* data, mem_addr_t address, mem_page_size_t length) const {
        sFLASH_ReadBuffer((uint8_t*) data, address, length);
        return true;
    }

};

/**
 * @brief  Gets the sector of a given address
 * @param  None
 * @retval The sector of a given address
 */
static uint32_t FLASH_Interminal_Get_Sector(uint32_t address)
{
    uint32_t sector = 0;

    if((address < ADDR_FLASH_SECTOR_1) && (address >= ADDR_FLASH_SECTOR_0))
    {
        sector = FLASH_SECTOR_0;
    }
    else if((address < ADDR_FLASH_SECTOR_2) && (address >= ADDR_FLASH_SECTOR_1))
    {
        sector = FLASH_SECTOR_1;
    }
    else if((address < ADDR_FLASH_SECTOR_3) && (address >= ADDR_FLASH_SECTOR_2))
    {
        sector = FLASH_SECTOR_2;
    }
    else if((address < ADDR_FLASH_SECTOR_4) && (address >= ADDR_FLASH_SECTOR_3))
    {
        sector = FLASH_SECTOR_3;
    }
    else if((address < ADDR_FLASH_SECTOR_5) && (address >= ADDR_FLASH_SECTOR_4))
    {
        sector = FLASH_SECTOR_4;
    }
    else if((address < ADDR_FLASH_SECTOR_6) && (address >= ADDR_FLASH_SECTOR_5))
    {
        sector = FLASH_SECTOR_5;
    }
    else if((address < ADDR_FLASH_SECTOR_7) && (address >= ADDR_FLASH_SECTOR_6))
    {
        sector = FLASH_SECTOR_6;
    }
    else /* (Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_7) */
    {
        sector = FLASH_SECTOR_7;
    }
    return sector;
}

static void HAL_FLASH_Interminal_Erase(uint32_t address, uint32_t length)
{
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t FirstSector = 0, NbOfSectors = 0, SECTORError = 0;

    HAL_FLASH_Unlock();
    FirstSector = FLASH_Interminal_Get_Sector(address);
    NbOfSectors = FLASH_Interminal_Get_Sector(address + length) - FirstSector + 1;

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = FirstSector;
    EraseInitStruct.NbSectors = NbOfSectors;
    HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError);
    HAL_FLASH_Lock();
}

void HAL_FLASH_Interminal_Read(uint32_t address, uint16_t *pdata, uint32_t datalen)
{
    uint32_t endAddress = address + datalen*2;
    uint16_t i = 0;

    while(address < endAddress)
    {
        pdata[i++] = (*(uint16_t*)address);
        address = address + 2;
    }
}

int HAL_FLASH_Interminal_Write(uint32_t address, uint16_t *pdata, uint32_t datalen)
{
    uint32_t endAddress = address + datalen * 2;
    uint16_t i = 0;

    HAL_StatusTypeDef flashStatus = HAL_OK;
    FLASH_Erase_Impl(ARGUMENT_PAGE0_BASE_ADDRESS, ARGUMENT_PAGE_SIZE);
    HAL_FLASH_Unlock();
    while((address < endAddress) && (flashStatus == HAL_OK))
    {
        flashStatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address, pdata[i++]);
        address = address + 2;
    }
    HAL_FLASH_Lock();
    return flashStatus;
}

