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

#ifndef FLASH_MAP_H_
#define FLASH_MAP_H_

#define INTERNAL_FLASH_START            ((uint32_t)0x08000000)
#define INTERNAL_FLASH_SIZE             (0x80000)
#define INTERNAL_FLASH_END_ADDRESS      ((uint32_t)INTERNAL_FLASH_START+INTERNAL_FLASH_SIZE)	//For 512KB Internal Flash

/*
 *  32k   bootloader
 *  32k   eeprom模拟区
 *  64k   应用系统参数区
 *  384k  应用程序区
 */


#define SYSTEM_PARAMS_START_ADDR         ((uint32_t)0x08010000)
#define SYSTEM_PARAMS_END_ADDR           ((uint32_t)0x0801FFFF)

#define BOOT_ADDR                        ((uint32_t)0x08000000)
#define EEPROM_ADDR                      ((uint32_t)0x08008000)
#define APP_ADDR                         ((uint32_t)0x08020000)

#endif /*FLASH_MAP_H_*/


