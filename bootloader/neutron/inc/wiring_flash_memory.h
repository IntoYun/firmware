/**
 ******************************************************************************
 * @file     : wiring_flash_memory.h
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

#ifndef WIRING_FLASH_MEMORY_H_
#define WIRING_FLASH_MEMORY_H_

#include "variant.h"

/*0x8010000 开始的64k 为系统参数区 */
#define SYSTEM_ARGUMENT_ADDRESS			((uint32_t)0x08010000)  // 系统参数
#define SYSTEM_ARGUMENT_END_ADDRESS  	((uint32_t)0x0801FFFF)
/* #endif */

// 64K Byte
#define ARGUMENT_PAGE_SIZE  (uint16_t)0x10000

/* Pages 0base and end addresses */
#define ARGUMENT_PAGE0_BASE_ADDRESS      ((uint32_t)SYSTEM_ARGUMENT_ADDRESS)
#define ARGUMENT_PAGE0_END_ADDRESS       ((uint32_t)(SYSTEM_ARGUMENT_ADDRESS + (ARGUMENT_PAGE_SIZE - 1)))

#ifdef __cplusplus
 extern "C" {
#endif

void SystemReadArgument(uint32_t readStartAddress, uint16_t *dataBuffer, uint32_t size);
int SystemWriteArgument(uint32_t writeStartAddress, uint16_t *dataBuffer, uint32_t size);

void BootReadArgument(uint8_t *dataBuffer, uint32_t size);
int BootWriteArgument(uint8_t *dataBuffer, uint32_t size);

#ifdef __cplusplus
}
#endif


#endif /*WIRING_FLASH_MEMORY_H_*/

