/**
 ******************************************************************************
 * @file    platform_config.h
 * @authors Satish Nair, Brett Walach
 * @version V1.0.0
 * @date    22-Oct-2014
 * @brief   Board specific configuration file.
 ******************************************************************************
  Copyright (c) 2013-2015 Particle Industries, Inc.  All rights reserved.

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
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

#include "platforms.h"

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define sFLASH_PAGESIZE     0x1000 /* 4096 bytes sector size that needs to be erased */
#define sFLASH_PAGECOUNT    1024   /* 4MByte storage */

/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */
