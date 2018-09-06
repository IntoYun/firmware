/**
 ******************************************************************************
 * @file    flash_mal.h
 * @author  Satish Nair
 * @version V1.0.0
 * @date    30-Jan-2015
 * @brief   Header for flash media access layer
 ******************************************************************************
 Copyright (c) 2015 Particle Industries, Inc.  All rights reserved.

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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_MAL_H
#define __FLASH_MAL_H

#include "hw_config.h"
#include "flash_device_hal.h"

/* Includes ------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macros ------------------------------------------------------------*/

/* Serial Flash Start Address */
#define SERIAL_FLASH_START        ((uint32_t)0x000000)
/* Serial Flash End Address */
#define SERIAL_FLASH_END          ((uint32_t)0x400000)

/* Serial Flash page size */
#define SERIAL_FLASH_SECTOR_SIZE    ((uint32_t)0x1000)      //4k Byte

#include "flash_access.h"

bool FLASH_EraseMemory(flash_device_t flashDeviceID, uint32_t startAddress, uint32_t length);
bool FLASH_WriteMemory(flash_device_t flashDeviceID, uint32_t startAddress, uint32_t *data, uint32_t length);
bool FLASH_ReadMemory(flash_device_t flashDeviceID, uint32_t startAddress, uint32_t *data, uint32_t length);

#ifdef __cplusplus
}
#endif


#endif  /*__FLASH_MAL_H*/
