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

#ifndef DEVICEID_HAL_H
#define	DEVICEID_HAL_H

#include <stdint.h>
#include <stddef.h>

#ifdef	__cplusplus
extern "C" {
#endif


uint32_t HAL_device_ID(uint8_t* dest, uint32_t destLen);
uint32_t HAL_Board_Type(uint8_t* dest, uint32_t destLen, uint8_t type);
uint32_t HAL_Platform_ID(void);
int HAL_Get_Device_Identifier(const char** name, char* buf, size_t buflen, unsigned index, void* reserved);


#ifdef	__cplusplus
}
#endif

#endif	/* DEVICEID_HAL_H */

