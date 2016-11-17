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
#define INTERNAL_FLASH_SIZE             (0x20000)
#define INTERNAL_FLASH_END_ADDRESS      ((uint32_t)INTERNAL_FLASH_START+INTERNAL_FLASH_SIZE-1)	//For 128KB Internal Flash

/*
 * 0000-4FFF   20K    bootloader
 * 5000-5FFF   4K     eeprom模拟区
 * 6000-63FF   1K     boot参数区
 * 6400-6FFF   3K     应用系统参数区
 * 7000-1FFFF  100k   应用程序区
 */


#define BOOT_ADDR                        ((uint32_t)0x08000000)//0000-4FFF
// #define BOOT_END_ADDR                    ((uint32_t)0x08004FFF)//0000-4FFF
// #define EEPROM_START_ADDR                ((uint32_t)0x08005000)//5000-5FFF
// #define EEPROM_END_ADDR                  ((uint32_t)0x08005FFF)//5000-5FFF
// #define BOOT_PARAMS_START_ADDR           ((uint32_t)0x08006000)//6000-63FF
// #define BOOT_PARAMS_END_ADDR             ((uint32_t)0x080063FF)//6000-63FF
#define SYSTEM_PARAMS_START_ADDR         ((uint32_t)0x08006400)//6400-6FFF
#define SYSTEM_PARAMS_END_ADDR           ((uint32_t)0x08006FFF)
#define APP_ADDR                         ((uint32_t)0x08007000)//100K

#define EEPROM_ADDR                      ((uint32_t)0x08005000)

#endif /*FLASH_MAP_H_*/


