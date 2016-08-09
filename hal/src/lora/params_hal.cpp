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

#include <string.h>
#include <stdio.h>
#include "params_impl.h"
#include "params_hal.h"
#include "eeprom_hal.h"
#include "memory_hal.h"
#include "intorobot_macros.h"


#define FLASH_SYSTEM_PARAMS_START_ADDRESS              ((uint32_t)0x08010000)
#define FLASH_SYSTEM_PARAMS_END_ADDRESS          ((uint32_t)0x0801FFFF)

#define EEPROM_BOOT_PARAMS_MAX_SIZE                        (512)    //参数区大小

boot_params_t intorobot_boot_params;         //bootloader参数
system_params_t intorobot_system_params;     //设备参数

/*初始化bootloader参数区*/
void init_boot_params(boot_params_t *pboot_params) {
    memset((uint8_t *)pboot_params, 0, sizeof(boot_params_t));
    pboot_params->header = BOOT_PARAMS_HEADER;
}

/*初始化系统参数区*/
void init_system_params(system_params_t *psystem_params) {
    memset((uint8_t *)psystem_params, 0, sizeof(system_params_t));
    psystem_params->header = SYSTEM_PARAMS_HEADER;
    psystem_params->config_flag = 1;
    psystem_params->zone = 8;
}

/*初始化系统参数区 保留密钥参数*/
void init_fac_system_params(system_params_t *psystem_params) {
    uint8_t  at_mode;                  // 是否已经灌装密钥  0:未灌装 1:已经灌装
    uint8_t  device_id[52]={0};        // 设备序列号
    uint8_t  access_token[52]={0};     // 设备access_token

    at_mode = psystem_params->at_mode;
    memcpy(device_id, psystem_params->device_id, sizeof(psystem_params->device_id));
    memcpy(access_token, psystem_params->access_token, sizeof(psystem_params->access_token));

    init_system_params(psystem_params);

    psystem_params->at_mode = at_mode;
    memcpy(psystem_params->device_id, device_id, sizeof(psystem_params->device_id));
    memcpy(psystem_params->access_token, access_token, sizeof(psystem_params->access_token));
}

void read_boot_params(boot_params_t *pboot_params) {
    uint32_t len = sizeof(boot_params_t);
    uint32_t address = HAL_EEPROM_Length() - EEPROM_BOOT_PARAMS_MAX_SIZE;
    uint8_t *pboot = (uint8_t *)pboot_params;

    memset(pboot, 0, len);
    if(len > EEPROM_BOOT_PARAMS_MAX_SIZE) {
        return;
    }

    for (int num = 0; num<len; num++) {
        pboot[num] = HAL_EEPROM_Read(address+num);
    }
}

/*
 * 保存bootloader参数区
 * */
void save_boot_params(boot_params_t *pboot_params) {
    uint32_t len = sizeof(boot_params_t);
    uint32_t address = HAL_EEPROM_Length() - EEPROM_BOOT_PARAMS_MAX_SIZE;
    uint8_t *pboot = (uint8_t *)pboot_params;

    if(len > EEPROM_BOOT_PARAMS_MAX_SIZE) {
        return;
    }

    for (int num = 0; num<len; num++) {
        HAL_EEPROM_Write(address+num, pboot[num]);
    }
}

/*
 * 加载系统参数区
 * */
void read_system_params(system_params_t *psystem_params) {
    uint32_t len = sizeof(system_params_t);

    memset(psystem_params, 0, sizeof(system_params_t));
    if(len > (FLASH_SYSTEM_PARAMS_END_ADDRESS-FLASH_SYSTEM_PARAMS_START_ADDRESS)) {
        return;
    }
    HAL_FLASH_Interminal_Read(FLASH_SYSTEM_PARAMS_START_ADDRESS, (uint16_t *)psystem_params, len/2);
    if( SYSTEM_PARAMS_HEADER != psystem_params->header ) {
        init_system_params(psystem_params);
        HAL_FLASH_Interminal_Read(FLASH_SYSTEM_PARAMS_START_ADDRESS, (uint16_t *)psystem_params, len/2);
    }
}

/*
 * 保存系统参数区
 * */
void save_system_params(system_params_t *psystem_params) {
    uint32_t len = sizeof(system_params_t);

    if(len > (FLASH_SYSTEM_PARAMS_END_ADDRESS - FLASH_SYSTEM_PARAMS_START_ADDRESS)) {
        return;
    }
    HAL_FLASH_Interminal_Write(FLASH_SYSTEM_PARAMS_START_ADDRESS, (uint16_t *)psystem_params, len/2);
}

void HAL_PARAMS_Init_System_Params(void) {
    init_system_params(&intorobot_system_params);
    save_system_params(&intorobot_system_params);
}

void HAL_PARAMS_Init_Fac_System_Params(void) {
    init_fac_system_params(&intorobot_system_params);
    save_system_params(&intorobot_system_params);
}

void HAL_PARAMS_Init_Boot_Params(void) {
    init_boot_params(&intorobot_boot_params);
    save_boot_params(&intorobot_boot_params);
}

void HAL_PARAMS_Load_System_Params(void) {
    read_system_params(&intorobot_system_params);
}

void HAL_PARAMS_Load_Boot_Params(void) {
    read_boot_params(&intorobot_boot_params);
}

uint16_t HAL_PARAMS_Get_InitParam_Flag(void) {
    return intorobot_boot_params.initparam_flag;
}

int HAL_PARAMS_Set_InitParam_Flag(uint16_t flag) {
    intorobot_boot_params.initparam_flag = flag;
    save_boot_params(&intorobot_boot_params);
    return 0;
}

uint16_t HAL_PARAMS_Get_DeviceID(char* buffer, uint16_t len) {
    uint16_t templen;

    if (buffer!=NULL && len>0) {
        templen = MIN(strlen(intorobot_system_params.device_id), len-1);
        memcpy(buffer, intorobot_system_params.device_id, templen);
        return templen;
    }
    return 0;
}

int HAL_PARAMS_Set_DeviceID(const char* buffer) {
    uint16_t templen;

    if (buffer!=NULL) {
        templen = MIN(sizeof(intorobot_system_params.device_id)-1, strlen(buffer));
        memcpy(intorobot_system_params.device_id, 0, sizeof(intorobot_system_params.device_id));
        memcpy(intorobot_system_params.device_id, buffer, templen);
        save_system_params(&intorobot_system_params);
        return 0;
    }
    return -1;
}

uint16_t HAL_PARAMS_Get_AccessToken(char* buffer, uint16_t len) {
    uint16_t templen;

    if (buffer!=NULL && len>0) {
        templen = MIN(strlen(intorobot_system_params.access_token), len-1);
        memcpy(buffer, intorobot_system_params.access_token, templen);
        return templen;
    }
    return 0;
}

int HAL_PARAMS_Set_AccessToken(const char* buffer) {
    uint16_t templen;

    if (buffer!=NULL) {
        templen = MIN(sizeof(intorobot_system_params.access_token)-1, strlen(buffer));
        memcpy(intorobot_system_params.access_token, 0, sizeof(intorobot_system_params.access_token));
        memcpy(intorobot_system_params.access_token, buffer, templen);
        save_system_params(&intorobot_system_params);
        return 0;
    }
    return -1;
}

