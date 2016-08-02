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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2S_HAL_H
#define __I2S_HAL_H

/* Includes ------------------------------------------------------------------*/
#include "pinmap_hal.h"

/* Exported types ------------------------------------------------------------*/
typedef enum HAL_I2S_Interface {
    HAL_I2S_INTERFACE1 = 0,
    HAL_I2S_INTERFACE2 = 1
} HAL_I2S_Interface;

/* Exported functions --------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
void HAL_I2S_Initial(HAL_I2S_Interface i2s);
void HAL_I2S_Begin(HAL_I2S_Interface i2s, uint32_t audioFreq);
void HAL_I2S_End(HAL_I2S_Interface i2s);
uint16_t HAL_I2S_Receive_Data(HAL_I2S_Interface i2s);
bool HAL_I2S_Is_Enabled(HAL_I2S_Interface i2s);
#ifdef __cplusplus
}
#endif

#endif  /* __I2S_HAL_H */
