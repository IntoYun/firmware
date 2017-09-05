#include "hw_config.h"
#include "flash_map.h"
#include "flash_storage_impl.h"
#include "eeprom_hal.h"

#define     EEPROM_SIZE           4096    //4096

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

    flashStore.read(EEPROM_START_ADDR, (uint32_t*)_data, _size);
}

uint8_t HAL_EEPROM_Read(uint32_t address)
{
    if (address < 0 || (size_t)address >= _size)
        return 0;
    if(!_data)
        return 0;

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

    if(0 == flashStore.eraseSector(EEPROM_START_ADDR)) {
        if(0 == flashStore.write(EEPROM_START_ADDR, (uint32_t*)_data, _size)) {
            _dirty = false;
            ret = true;
        }
    }
    return ret;
}

void HAL_EEPROM_Clear()
{
    InternalFlashStore flashStore;

    flashStore.eraseSector(EEPROM_START_ADDR);
}

bool HAL_EEPROM_Has_Pending_Erase()
{
    return false;
}

void HAL_EEPROM_Perform_Pending_Erase()
{

}

