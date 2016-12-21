#if 0
// pull in the sources from the HAL. It's a bit of a hack, but is simpler than trying to link the
// full hal library.
#include "../src/neutron/memory_hal.cpp"

#else

#include "memory_hal.h"
#include "hw_config.h"

/**
 * @brief  Gets the sector of a given address
 * @param  None
 * @retval The sector of a given address
 */
uint32_t HAL_FLASH_Interminal_Get_Sector(uint32_t address)
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

HAL_Flash_StatusTypeDef HAL_FLASH_Interminal_Erase(uint32_t sector)
{
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SECTORError = 0;

    HAL_FLASH_Unlock();

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = sector;
    EraseInitStruct.NbSectors = 1;
    HAL_StatusTypeDef rslt = HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError);
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

#endif
