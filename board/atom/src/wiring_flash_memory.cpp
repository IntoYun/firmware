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
FLASH_Status SystemWriteArgument(uint32_t writeStartAddress, uint16_t *dataBuffer, uint32_t size)
{
    uint32_t address = writeStartAddress;
    uint32_t endAddress = writeStartAddress + size * 2;
    uint16_t i = 0;

    if((address < SYSTEM_ARGUMENT_ADDRESS)
    || (endAddress > SYSTEM_ARGUMENT_END_ADDRESS
    || ((address % 2) != 0)))
    {
        return FLASH_ERROR_PG;
    }

    FLASH_Status flashStatus = FLASH_COMPLETE;
    /* Unlock the Flash Program Erase Controller */
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    flashStatus = FLASH_ErasePage(ARGUMENT_PAGE0_BASE_ADDRESS);
    if(flashStatus != FLASH_COMPLETE)
    {
        return flashStatus;
    }

    flashStatus = FLASH_ErasePage(ARGUMENT_PAGE1_BASE_ADDRESS);
    if(flashStatus != FLASH_COMPLETE)
    {
        return flashStatus;
    }

    while((address < endAddress) && (flashStatus == FLASH_COMPLETE))
    {
        flashStatus = FLASH_ProgramHalfWord(address,dataBuffer[i++]);
        address = address + 2;
    }
    /* Locks the FLASH Program Erase Controller */
    FLASH_Lock();
    return flashStatus;
}

/*********************************************************************************
  *Function		:      readSystemParam
  *Description	:      read the system param
  *Input		      :      system_param :  the point of the struct system param
  *Output		:      none
  *Return		:      the return of result
  *author		:      robin
  *date			:      2015-03-27
  *Others		:
**********************************************************************************/
int readSystemParam(struct _system_param *system_param)
{
    memset(system_param, 0, sizeof(struct _system_param));
    SystemReadArgument(SYSTEM_ARGUMENT_ADDRESS, (uint16_t *)system_param, sizeof(struct _system_param)/2);
    return true;
}

/*********************************************************************************
  *Function		:      writeSystemParam
  *Description	:      write the system param
  *Input		      :      system_param :  the point of the struct system param
  *Output		:      none
  *Return		:      the return of result
  *author		:      robin
  *date			:      2015-03-27
  *Others		:
**********************************************************************************/
int writeSystemParam(struct _system_param *system_param)
{
    SystemWriteArgument(SYSTEM_ARGUMENT_ADDRESS, (uint16_t *)system_param, sizeof(struct _system_param)/2);
    return true;
}

struct _system_param intorobot_system_param;

