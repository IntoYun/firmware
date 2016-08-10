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

void HAL_PARAMS_Init_System_Params(void);
void HAL_PARAMS_Init_Fac_System_Params(void);
void HAL_PARAMS_Init_Boot_Params(void);
void HAL_PARAMS_Load_System_Params(void);
void HAL_PARAMS_Load_Boot_Params(void);

uint16_t HAL_PARAMS_Get_InitParam_Flag(void);
int HAL_PARAMS_Set_InitParam_Flag(uint16_t flag);
uint16_t HAL_PARAMS_Get_Boot_Flag(void);
int HAL_PARAMS_Set_Boot_Flag(uint16_t flag);

uint16_t HAL_PARAMS_Get_DeviceID(char* buffer, uint16_t len);
int HAL_PARAMS_Set_DeviceID(const char* buffer);
uint16_t HAL_PARAMS_Get_AccessToken(char* buffer, uint16_t len);
int HAL_PARAMS_Set_AccessToken(const char* buffer);
uint16_t HAL_PARAMS_Get_At_Mode_Flag(void);
int HAL_PARAMS_Set_At_Mode_Flag(uint16_t flag);
float HAL_PARAMS_Get_Time_Zone(void);
int HAL_PARAMS_Set_Time_Zone(float zone);
uint16_t HAL_PARAMS_Get_Server_Domain(char* buffer, uint16_t len);
int HAL_PARAMS_Set_Server_Domain(const char* buffer);
int HAL_PARAMS_Get_Server_Port(void);
int HAL_PARAMS_Set_Server_Port(int port);
uint16_t HAL_PARAMS_Get_Down_Domain(char* buffer, uint16_t len);
int HAL_PARAMS_Set_Down_Domain(const char* buffer);
uint16_t HAL_PARAMS_Get_Server_Select_Flag(void);
int HAL_PARAMS_Set_Server_Select_Flag(uint16_t flag);

uint16_t HAL_PARAMS_Get_FirmwareLib_Version(char* buffer, uint16_t len);
int HAL_PARAMS_Set_FirmwareLib_Version(const char* buffer);
uint16_t HAL_PARAMS_Get_Subsys_Version(char* buffer, uint16_t len);
int HAL_PARAMS_Set_Subsys_Version(const char* buffer);


#ifdef __cplusplus
}
#endif

#endif
