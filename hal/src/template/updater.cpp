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

#include "updater_hal.h"

bool HAL_Updater_Run(const char *host, const char *uri, const char * md5, updater_mode_t mode)
{
    return true;
}

void HAL_Updater_Set_Handle(THandlerFunction_Progress fn)
{
}

void HAL_Updater_Set_Flag(uint32_t size)
{
}

uint32_t HAL_Updater_Start_Address(void)
{
    return 0;
}

uint32_t HAL_Updater_Flash_Length(void)
{
    return 0;
}

int HAL_Updater_Flash(const uint8_t *pBuffer, uint32_t address, uint32_t length, void* reserved)
{
    return false;
}

