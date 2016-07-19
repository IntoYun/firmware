/**
 ******************************************************************************
 * @file     : wiring_flash_memory.cpp
 * @author   : robin
 * @version	 : V1.0.0
 * @date     : 6-December-2014
 * @brief    :
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
#include "wiring_flash_memory.h"
#include "usbd_dfu_if.h"
#include "wiring_eeprom.h"

/**
 * @brief  Gets the sector of a given address
 * @param  None
 * @retval The sector of a given address
 */
static uint32_t GetSector(uint32_t Address)
{
    uint32_t sector = 0;

    if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
    {
        sector = FLASH_SECTOR_0;
    }
    else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
    {
        sector = FLASH_SECTOR_1;
    }
    else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
    {
        sector = FLASH_SECTOR_2;
    }
    else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
    {
        sector = FLASH_SECTOR_3;
    }
    else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
    {
        sector = FLASH_SECTOR_4;
    }
    else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
    {
        sector = FLASH_SECTOR_5;
    }
    else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
    {
        sector = FLASH_SECTOR_6;
    }
    else /* (Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_7) */
    {
        sector = FLASH_SECTOR_7;
    }
    return sector;
}

static void FLASH_Erase_Impl(uint32_t address, uint32_t length)
{
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t FirstSector = 0, NbOfSectors = 0, SECTORError = 0;

    HAL_FLASH_Unlock();
    /* Get the 1st sector to erase */
    FirstSector = GetSector(address);
    /* Get the number of sector to erase from 1st sector*/
    NbOfSectors = GetSector(address + length) - FirstSector + 1;
    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = FirstSector;
    EraseInitStruct.NbSectors = NbOfSectors;
    HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError);
    HAL_FLASH_Lock();
}

/*********************************************************************************
 *Function		: void SystemReadArgument(uint32_t readStartAddress, uint16_t *dataBuffer, uint32_t size)
 *Description	: Read data from the flash
 *Input              : address data size
 *Output		: none
 *Return		: none
 *author		: lz
 *date			: 2015-2-1
 *Others		: none
 **********************************************************************************/
void SystemReadArgument(uint32_t readStartAddress, uint16_t *dataBuffer, uint32_t size)
{
    //uint32_t remainder = 0;
    uint32_t address = readStartAddress;
    uint32_t endAddress = readStartAddress + size*2;

    if((address < SYSTEM_ARGUMENT_ADDRESS)
            || (endAddress > SYSTEM_ARGUMENT_END_ADDRESS
                || ((address % 2) != 0)))
    {
        return;
    }

    uint16_t i = 0;
    while(address < endAddress)
    {
        dataBuffer[i++] = (*(uint16_t*)address);
        address = address + 2;
    }
}

/*********************************************************************************
 *Function		: FLASH_Status SystemWriteArgument(uint32_t writeStartAddress, uint16_t *dataBuffer, uint32_t size)
 *Description	: For storing data in a flash
 *Input		      : address data number
 *Output		: none
 *Return		: none
 *author		: lz
 *date			: 2015-2-1
 *Others		: none
 **********************************************************************************/
int SystemWriteArgument(uint32_t writeStartAddress, uint16_t *dataBuffer, uint32_t size)
{
    uint32_t address = writeStartAddress;
    uint32_t endAddress = writeStartAddress + size * 2;
    uint16_t i = 0;

    if((address < SYSTEM_ARGUMENT_ADDRESS)
            || (endAddress > SYSTEM_ARGUMENT_END_ADDRESS
                || ((address % 2) != 0)))
    {
        return 2;//FLASH_ERROR_PG;
    }


    HAL_StatusTypeDef flashStatus = HAL_OK;
    FLASH_Erase_Impl(ARGUMENT_PAGE0_BASE_ADDRESS, ARGUMENT_PAGE_SIZE);
    HAL_FLASH_Unlock();
    while((address < endAddress) && (flashStatus == HAL_OK))
    {
        flashStatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address, dataBuffer[i++]);
        address = address + 2;
    }
    /* Locks the FLASH Program Erase Controller */
    HAL_FLASH_Lock();
    return flashStatus;
}


/*********************************************************************************
 *Function		: void SystemReadArgument(uint32_t readStartAddress, uint16_t *dataBuffer, uint32_t size)
 *Description	: Read data from the flash
 *Input              : address data size
 *Output		: none
 *Return		: none
 *author		: lz
 *date			: 2015-2-1
 *Others		: none
 **********************************************************************************/
void BootReadArgument(uint8_t *dataBuffer, uint32_t size)
{
    int num;

    for (num = 0; num<size; num++)
    {
        dataBuffer[num] = EEPROM.read(Param_Eeprom_ADDR+num);
    }
}

/*********************************************************************************
 *Function		: FLASH_Status SystemWriteArgument(uint32_t writeStartAddress, uint16_t *dataBuffer, uint32_t size)
 *Description	: For storing data in a flash
 *Input		      : address data number
 *Output		: none
 *Return		: none
 *author		: lz
 *date			: 2015-2-1
 *Others		: none
 **********************************************************************************/
int BootWriteArgument(uint8_t *dataBuffer, uint32_t size)
{
    int num;

    for (num = 0; num<size; num++)
    {
        EEPROM.write_system(Param_Eeprom_ADDR+num, dataBuffer[num]);
    }
}

