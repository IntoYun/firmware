/**
 ******************************************************************************
 * @file    eeprom_hal.c
 * @author  Matthew McGowan
 * @version V1.0.0
 * @date    27-Sept-2014
 * @brief
 ******************************************************************************
  Copyright (c) 2013-2015 IntoRobot Industries, Inc.  All rights reserved.

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

/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"
#include "eeprom_hal.h"
#include "spi_flash.h"
#include "flash_map.h"

/*
 * only use the eeprom the first sector (eeprom)
 */
#define EEPROM_SIZE SPI_FLASH_SEC_SIZE   //4096
/* extern "C" uint32_t _SPIFFS_end; */
extern uint32_t _SPIFFS_end;
static uint32_t sector = 0;
static uint8_t* eeprom_mem = NULL;
static size_t size;

void HAL_EEPROM_Init(void)
{
    sector = EEPROM_SEC_START;

    size = (EEPROM_SIZE + 3) & (~3);
    if (eeprom_mem) {
        free(eeprom_mem);
    }
    eeprom_mem = (uint8_t *)malloc(size);
    /* noInterrupts(); */
    /* spi_flash_read(sector * SPI_FLASH_SEC_SIZE, reinterpret_cast<uint32_t*>(eeprom_mem), size); */
    spi_flash_read(sector * SPI_FLASH_SEC_SIZE, (uint32_t*)(eeprom_mem), size);
    /* interrupts(); */
}

uint8_t HAL_EEPROM_Read(uint32_t address)
{
    if (address < 0 || (size_t)address >= size) {
        return 0xFF;
    }
    if (!eeprom_mem) {
        return 0xFF;
    }
    return eeprom_mem[address];
}


// EEPROM write and commit
void HAL_EEPROM_Write(uint32_t address, uint8_t data)
{
    if (address < 0 || (size_t)address >= size) {
        return;
    }
    if (!eeprom_mem) {
        return;
    }

    // Optimise _dirty. Only flagged if data written is different.
    uint8_t* pData = &eeprom_mem[address];
    if (*pData != data) {
        *pData = data;
    }

    /* noInterrupts(); */
    if(spi_flash_erase_sector(sector) == SPI_FLASH_RESULT_OK) {
        /* spi_flash_write(sector * SPI_FLASH_SEC_SIZE, reinterpret_cast<uint32_t*>(eeprom_mem), size); */
        spi_flash_write(sector * SPI_FLASH_SEC_SIZE, (uint32_t*)(eeprom_mem), size);
    }
    /* interrupts(); */
}

size_t HAL_EEPROM_Length()
{
    return size;
}
