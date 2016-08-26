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

uint16_t HAL_Board_Type(char* dest, uint16_t destLen, uint8_t type);
uint32_t HAL_Platform_ID(void);

void HAL_PARAMS_Init_All_System_Params(void);
void HAL_PARAMS_Init_Fac_System_Params(void);
void HAL_PARAMS_Init_Boot_Params(void);
void HAL_PARAMS_Load_System_Params(void);
void HAL_PARAMS_Load_Boot_Params(void);
void HAL_PARAMS_Save_Params(void);

uint32_t HAL_PARAMS_Get_Boot_boot_version(void);
int HAL_PARAMS_Set_Boot_boot_version(uint32_t version);
uint16_t HAL_PARAMS_Get_Boot_boot_flag(void);
int HAL_PARAMS_Set_Boot_boot_flag(uint16_t flag);
uint16_t HAL_PARAMS_Get_Boot_initparam_flag(void);
int HAL_PARAMS_Set_Boot_initparam_flag(uint16_t flag);

uint16_t HAL_PARAMS_Get_System_device_id(char* buffer, uint16_t len);
int HAL_PARAMS_Set_System_device_id(const char* buffer);
uint16_t HAL_PARAMS_Get_System_access_token(char* buffer, uint16_t len);
int HAL_PARAMS_Set_System_access_token(const char* buffer);
uint16_t HAL_PARAMS_Get_System_at_mode(void);
int HAL_PARAMS_Set_System_at_mode(uint16_t flag);

float HAL_PARAMS_Get_System_zone(void);
int HAL_PARAMS_Set_System_zone(float zone);
uint16_t HAL_PARAMS_Get_System_sv_domain(char* buffer, uint16_t len);
int HAL_PARAMS_Set_System_sv_domain(const char* buffer);
int HAL_PARAMS_Get_System_sv_port(void);
int HAL_PARAMS_Set_System_sv_port(int port);
uint16_t HAL_PARAMS_Get_System_dw_domain(char* buffer, uint16_t len);
int HAL_PARAMS_Set_System_dw_domain(const char* buffer);
uint16_t HAL_PARAMS_Get_System_sv_select(void);
int HAL_PARAMS_Set_System_sv_select(uint16_t flag);

uint16_t HAL_PARAMS_Get_System_fwlib_ver(char* buffer, uint16_t len);
int HAL_PARAMS_Set_System_fwlib_ver(const char* buffer);
uint16_t HAL_PARAMS_Get_System_subsys_ver(char* buffer, uint16_t len);
int HAL_PARAMS_Set_System_subsys_ver(const char* buffer);

uint16_t HAL_PARAMS_Get_System_config_flag(void);
int HAL_PARAMS_Set_System_config_flag(uint16_t flag);


#ifdef __cplusplus
}
#endif

#endif
