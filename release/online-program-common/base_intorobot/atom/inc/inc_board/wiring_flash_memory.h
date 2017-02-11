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

/******
存储区后2K地址存储  device id product id token 等信息
****/
#define SYSTEM_ARGUMENT_ADDRESS			((uint32_t)0x0801F800)  // 系统参数
#define SYSTEM_ARGUMENT_END_ADDRESS  	((uint32_t)0x0801FFFF)

// 1K Byte
#define ARGUMENT_PAGE_SIZE  (uint16_t)0x400

/* Pages 0 and 1 base and end addresses */
#define ARGUMENT_PAGE0_BASE_ADDRESS      ((uint32_t)SYSTEM_ARGUMENT_ADDRESS)
#define ARGUMENT_PAGE0_END_ADDRESS       ((uint32_t)(SYSTEM_ARGUMENT_ADDRESS + (ARGUMENT_PAGE_SIZE - 1)))

#define ARGUMENT_PAGE1_BASE_ADDRESS      ((uint32_t)(SYSTEM_ARGUMENT_ADDRESS + ARGUMENT_PAGE_SIZE))
#define ARGUMENT_PAGE1_END_ADDRESS       ((uint32_t)(SYSTEM_ARGUMENT_ADDRESS + (2 * ARGUMENT_PAGE_SIZE - 1)))


struct _system_param
{
	char product_id[52];
	char device_id[52];
	char access_token[52];
	char server[28];
	float zone;

	char at_mode;
	char sv_select;
	char sv_domain[52];
	int sv_port;
	char dw_domain[52];
};

void SystemReadArgument(uint32_t readStartAddress, uint16_t *dataBuffer, uint32_t size);
FLASH_Status SystemWriteArgument(uint32_t writeStartAddress, uint16_t *dataBuffer, uint32_t size);

int readSystemParam(struct _system_param *system_param);
int writeSystemParam(struct _system_param *system_param);

extern struct _system_param intorobot_system_param;

#endif /*WIRING_FLASH_MEMORY_H_*/

