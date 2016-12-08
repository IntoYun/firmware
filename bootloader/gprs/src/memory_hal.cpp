#if 0
// pull in the sources from the HAL. It's a bit of a hack, but is simpler than trying to link the
// full hal library.
#include "../src/gprs/memory_hal.cpp"

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
#if 0
    uint32_t sector = 0;
// STM32L151C8T6 16K-SRAM 64K-Flash
    if((address < ADDR_FLASH_SECTOR_1) && (address >= ADDR_FLASH_SECTOR_0))
    {
        sector = OB_WRP1_PAGES0TO15;//LASH_SECTOR_0;
    }
    else if((address < ADDR_FLASH_SECTOR_2) && (address >= ADDR_FLASH_SECTOR_1))
    {
        sector = OB_WRP1_PAGES16TO31;//LASH_SECTOR_1;
    }
    else if((address < ADDR_FLASH_SECTOR_3) && (address >= ADDR_FLASH_SECTOR_2))
    {
        sector = OB_WRP1_PAGES16TO31;//FLASH_SECTOR_2;
    }
    else
        sector = OB_WRP1_PAGES16TO31;//FLASH_SECTOR_3;
    }

    return sector;
#endif
}

HAL_Flash_StatusTypeDef HAL_FLASH_Interminal_Erase(uint32_t sector)
{
#if 0
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t FirstSector = 0, SECTORError = 0;

    HAL_FLASH_Unlock();
    FirstSector = FLASH_Interminal_Get_Sector(address);

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = FirstSector;
    EraseInitStruct.NbSectors = 1;
    HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError);
    HAL_FLASH_Lock();
#endif
}

HAL_Flash_StatusTypeDef HAL_FLASH_Interminal_Read(uint32_t address, uint32_t *pdata, uint32_t datalen)
{
#if 0
    uint32_t endAddress = address + datalen*2;
    uint16_t i = 0;

    while(address < endAddress)
    {
        pdata[i++] = (*(uint16_t*)address);
        address = address + 2;
    }
#endif
}

HAL_Flash_StatusTypeDef HAL_FLASH_Interminal_Write(uint32_t address, uint32_t *pdata, uint32_t datalen)
{
#if 0
    uint32_t endAddress = address + datalen * 2;
    uint16_t i = 0;

    HAL_StatusTypeDef flashStatus = HAL_OK;
    HAL_FLASH_Interminal_Erase(address);
    HAL_FLASH_Unlock();
    while((address < endAddress) && (flashStatus == HAL_OK))
    {
        flashStatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address, pdata[i++]);
        address = address + 2;
    }
    HAL_FLASH_Lock();
    return flashStatus;
#endif
}

#endif
