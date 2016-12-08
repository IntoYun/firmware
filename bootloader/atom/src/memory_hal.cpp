#if 0
// pull in the sources from the HAL. It's a bit of a hack, but is simpler than trying to link the
// full hal library.
#include "../src/atom/memory_hal.cpp"

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

#endif
