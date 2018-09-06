/*
 ******************************************************************************
 *  Copyright (c) 2016 Particle Industries, Inc.  All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

#include "flash_storage_impl.h"

//EEPROM固定起始地址
#define EEPROM_START_ADDRESS   0x016000

#define EEPROM_SIZE           SPI_FLASH_SEC_SIZE   //4096

extern "C" {

uint8_t* _data;
size_t _size;
bool _dirty;

void HAL_EEPROM_Init(void)
{
    size_t size = (EEPROM_SIZE + 3) & (~3);
    InternalFlashStore flashStore;

    if (_data) {
        delete[] _data;
    }

    _data = new uint8_t[size];
    _size = size;

    flashStore.read(EEPROM_START_ADDRESS, (uint32_t*)_data, _size);
}

uint8_t HAL_EEPROM_Read(uint32_t address)
{
    if (address < 0 || (size_t)address >= _size)
        return 0xFF;
    if(!_data)
        return 0xFF;

    return _data[address];
}

void HAL_EEPROM_Write(uint32_t address, uint8_t data)
{
    if (address < 0 || (size_t)address >= _size)
        return;
    if(!_data)
        return;

    // Optimise _dirty. Only flagged if data written is different.
    uint8_t* pData = &_data[address];
    if (*pData != data) {
        *pData = data;
        _dirty = true;
    }
}

size_t HAL_EEPROM_Length()
{
    return _size;
}

void HAL_EEPROM_Get(uint32_t address, void *data, size_t length)
{
    if (address < 0 || address + length > _size)
      return;

    memcpy((uint8_t*)data, _data + address, length);
}

void HAL_EEPROM_Put(uint32_t address, const void *data, size_t length)
{
    if (address < 0 || address + length > _size)
      return;

    memcpy(_data + address, (const uint8_t*)data, length);
    _dirty = true;
}

bool HAL_EEPROM_Commit(void)
{
    bool ret = false;
    InternalFlashStore flashStore;

    if (!_size)
        return false;
    if(!_dirty)
        return true;
    if(!_data)
        return false;

    if(0 == flashStore.erase(EEPROM_START_ADDRESS, _size)) {
        if(0 == flashStore.write(EEPROM_START_ADDRESS, (uint32_t*)_data, _size)) {
            _dirty = false;
            ret = true;
        }
    }
    return ret;
}

void HAL_EEPROM_Clear()
{
    InternalFlashStore flashStore;

    flashStore.erase(EEPROM_START_ADDRESS, _size);
}

bool HAL_EEPROM_Has_Pending_Erase()
{
    return false;
}

void HAL_EEPROM_Perform_Pending_Erase()
{

}

}
