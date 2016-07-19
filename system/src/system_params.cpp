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

#include "system_params.h"
//#include "wiring_flash_memory.h"
#include <string.h>
#include <stdio.h>

/*初始化bootloader参数区*/
void system_init_boot_params(boot_params_t *pboot_params)
{
#if 0
    memset((uint8_t *)pboot_params, 0, sizeof(boot_params_t));
    pboot_params->header = BOOT_PARAMS_HEADER;

    BootWriteArgument((uint8_t *)pboot_params, sizeof(boot_params_t));
#endif
}

/*加载bootloader参数区*/
void system_load_boot_params(boot_params_t *pboot_params)
{
#if 0
    memset(pboot_params, 0, sizeof(boot_params_t));
    BootReadArgument((uint8_t *)pboot_params, sizeof(boot_params_t));
    if( BOOT_PARAMS_HEADER != pboot_params->header )
    {
        initBootParams(pboot_params);
    }
#endif
}

/*保存bootloader参数区*/
void system_save_boot_params(boot_params_t *pboot_params)
{
#if 0
    BootWriteArgument((uint8_t *)pboot_params, sizeof(boot_params_t));
#endif
}

/*初始化系统参数区*/
void system_init_system_params(system_params_t *psystem_params)
{
#if 0
    memset((uint8_t *)psystem_params, 0, sizeof(system_params_t));
    psystem_params->header = SYSTEM_PARAMS_HEADER;
    psystem_params->config_flag = 1;
    psystem_params->zone = 8;
    saveSystemParams(psystem_params);
#endif
}

/*初始化系统参数区 保留密钥参数*/
void system_init_fac_system_params(system_params_t *psystem_params)
{
#if 0
    uint8_t  at_mode;                  // 是否已经灌装密钥  0:未灌装 1:已经灌装
    uint8_t  device_id[52]={0};        // 设备序列号
    uint8_t  access_token[52]={0};     // 设备access_token

    at_mode = psystem_params->at_mode;
    memcpy(device_id, psystem_params->device_id, sizeof(psystem_params->device_id));
    memcpy(access_token, psystem_params->access_token, sizeof(psystem_params->access_token));

    initSystemParams(psystem_params);

    psystem_params->at_mode = at_mode;
    memcpy(psystem_params->device_id, device_id, sizeof(psystem_params->device_id));
    memcpy(psystem_params->access_token, access_token, sizeof(psystem_params->access_token));

    saveSystemParams(psystem_params);
#endif
}

/*加载系统参数区*/
void system_init_load_system_params(system_params_t *psystem_params)
{
#if 0
    memset(psystem_params, 0, sizeof(system_params_t));
    SystemReadArgument(SYSTEM_ARGUMENT_ADDRESS, (uint16_t *)psystem_params, sizeof(system_params_t)/2);
    if( SYSTEM_PARAMS_HEADER != psystem_params->header )
    {
        initSystemParams(psystem_params);
    }
#endif
}

/*保存系统参数区*/
void system_save_system_params(system_params_t *psystem_params)
{
#if 0
    SystemWriteArgument(SYSTEM_ARGUMENT_ADDRESS, (uint16_t *)psystem_params, sizeof(system_params_t)/2);
#endif
}

boot_params_t intorobot_boot_param;         //bootloader参数
system_params_t intorobot_system_param;     //设备参数


