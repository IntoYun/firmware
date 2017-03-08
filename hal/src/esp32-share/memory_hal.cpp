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
extern "C" {
#include "rom/spi_flash.h"
#include "esp_spi_flash.h"
#include "esp_err.h"
#include "esp_intr.h"
}

/**
 * @brief  Gets the sector of a given address
 * @param  None
 * @retval The sector of a given address
 */
uint32_t HAL_FLASH_Interminal_Get_Sector(uint32_t address)
{
    return address/SPI_FLASH_SEC_SIZE;
}

/**
 * @brief  Gets the sector of a given address
 * @param  None
 * @retval The sector of a given address
 */
HAL_Flash_StatusTypeDef HAL_FLASH_Interminal_Erase(uint32_t sector)
{
    HAL_Flash_StatusTypeDef result = HAL_FLASH_STATUS_TIMEOUT;

    ESP_INTR_DISABLE(10);
    //esp_err_t rlt = spi_flash_erase_sector(sector);
    esp_err_t rlt = ESP_OK;
    switch(rlt)
    {
        case ESP_OK:
            result = HAL_FLASH_STATUS_OK;
            break;
        case ESP_ERR_FLASH_OP_TIMEOUT :
            result = HAL_FLASH_STATUS_TIMEOUT;
            break;
        default:
            result = HAL_FLASH_STATUS_ERROR;
            break;
    }
    ESP_INTR_ENABLE(10);
    return result;
}

HAL_Flash_StatusTypeDef HAL_FLASH_Interminal_Read(uint32_t address, uint32_t *pdata, uint32_t datalen)
{
    HAL_Flash_StatusTypeDef result = HAL_FLASH_STATUS_TIMEOUT;

    ESP_INTR_DISABLE(10);
    //esp_err_t rlt = spi_flash_read(address, pdata, datalen);
    esp_err_t rlt = ESP_OK;
    switch(rlt)
    {
        case ESP_OK:
            result = HAL_FLASH_STATUS_OK;
            break;
        case ESP_ERR_FLASH_OP_TIMEOUT :
            result = HAL_FLASH_STATUS_TIMEOUT;
            break;
        default:
            result = HAL_FLASH_STATUS_ERROR;
            break;
    }
    ESP_INTR_ENABLE(10);
    return result;
}

HAL_Flash_StatusTypeDef HAL_FLASH_Interminal_Write(uint32_t address, uint32_t *pdata, uint32_t datalen)
{
    HAL_Flash_StatusTypeDef result = HAL_FLASH_STATUS_TIMEOUT;

    ESP_INTR_DISABLE(10);
    //esp_err_t rlt = spi_flash_write(address, pdata, datalen);
    esp_err_t rlt = ESP_OK;
    switch(rlt)
    {
        case ESP_OK:
            result = HAL_FLASH_STATUS_OK;
            break;
        case ESP_ERR_FLASH_OP_TIMEOUT :
            result = HAL_FLASH_STATUS_TIMEOUT;
            break;
        default:
            result = HAL_FLASH_STATUS_ERROR;
            break;
    }
    ESP_INTR_ENABLE(10);
    return result;
}

