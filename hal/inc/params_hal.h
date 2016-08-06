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
#ifndef PARAMS_HAL_H_
#define PARAMS_HAL_H_

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif


void HAL_PARAMS_Init_System_Params(void);
void HAL_PARAMS_Init_Fac_System_Params(void);
void HAL_PARAMS_Init_Boot_Params(void);
void HAL_PARAMS_Load_System_Params(void);
void HAL_PARAMS_Load_Boot_Params(void);
void HAL_PARAMS_Save_System_Params(void);
void HAL_PARAMS_Save_Boot_Params(void);
boot_params_t &HAL_Boot_Param(void);
system_params_t &HAL_System_Param(void);

extern boot_params_t intorobot_boot_params;         //bootloader参数
extern system_params_t intorobot_system_params;     //设备参数


#ifdef __cplusplus
}
#endif

#endif
