/*
 ******************************************************************************
 *  Copyright (c) 2015 Particle Industries, Inc.  All rights reserved.
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

#include "flash_mal.h"
#include "string.h"

/**
 * Implements access to the internal flash, providing the interface expected by dcd.h
 */
class InternalFlashStore
{
public:
    int erase(unsigned address, unsigned size)
    {
        return !FLASH_EraseMemory(FLASH_SERIAL, address, size);
    }

    int eraseSector(unsigned address)
    {
        return !FLASH_EraseMemory(FLASH_SERIAL, address, 1);
    }

    int write(const unsigned offset, const void* data, const unsigned size)
    {
        bool result = FLASH_WriteMemory(FLASH_SERIAL, offset, (uint32_t *)data, size);
        return !result ? -1 : 0;
    }

    const uint8_t* dataAt(unsigned address)
    {
        return (const uint8_t*)address;
    }

    int read(unsigned offset, void* data, unsigned size)
    {
        bool result = FLASH_ReadMemory(FLASH_SERIAL, offset, (uint32_t *)data, size);
        return !result ? -1 : 0;
    }
};
