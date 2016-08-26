#include "memory_hal.h"
#include "hw_config.h"
#include "flash.h"
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
HAL_Flash_StatusTypeDef HAL_FLASH_Interminal_Erase(uint32_t sector)
{
    HAL_Flash_StatusTypeDef result = HAL_FLASH_STATUS_TIMEOUT;

    ets_isr_mask(FLASH_INT_MASK);
    int rlt = SPIEraseSector(sector);
    switch(rlt)
    {
        case 0:
            result = HAL_FLASH_STATUS_OK;
            break;
        case 2:
            result = HAL_FLASH_STATUS_TIMEOUT;
            break;
        default:
            result = HAL_FLASH_STATUS_ERROR;
            break;
    }
    ets_isr_unmask(FLASH_INT_MASK);
    return result;
}

HAL_Flash_StatusTypeDef HAL_FLASH_Interminal_Read(uint32_t address, uint32_t *pdata, uint32_t datalen)
{
    HAL_Flash_StatusTypeDef result = HAL_FLASH_STATUS_TIMEOUT;

    ets_isr_mask(FLASH_INT_MASK);
    int rlt = SPIRead(address, pdata, datalen);
    switch(rlt)
    {
        case 0:
            result = HAL_FLASH_STATUS_OK;
            break;
        case 2:
            result = HAL_FLASH_STATUS_TIMEOUT;
            break;
        default:
            result = HAL_FLASH_STATUS_ERROR;
            break;
    }
    ets_isr_unmask(FLASH_INT_MASK);
    return result;
}

HAL_Flash_StatusTypeDef HAL_FLASH_Interminal_Write(uint32_t address, uint32_t *pdata, uint32_t datalen)
{
    HAL_Flash_StatusTypeDef result = HAL_FLASH_STATUS_TIMEOUT;

    ets_isr_mask(FLASH_INT_MASK);
    int rlt = SPIWrite(address, pdata, datalen);
    switch(rlt)
    {
        case 0:
            result = HAL_FLASH_STATUS_OK;
            break;
        case 2:
            result = HAL_FLASH_STATUS_TIMEOUT;
            break;
        default:
            result = HAL_FLASH_STATUS_ERROR;
            break;
    }
    ets_isr_unmask(FLASH_INT_MASK);
    return result;
}

