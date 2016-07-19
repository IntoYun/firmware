#include "system_params.h"
#include "wiring_flash_memory.h"


/*初始化bootloader参数区*/
void initBootParams(boot_params_t *pboot_params)
{
    memset((uint8_t *)pboot_params, 0, sizeof(boot_params_t));
    pboot_params->header = BOOT_PARAMS_HEADER;

    BootWriteArgument((uint8_t *)pboot_params, sizeof(boot_params_t));
}

/*加载bootloader参数区*/
void loadBootParams(boot_params_t *pboot_params)
{
    memset(pboot_params, 0, sizeof(boot_params_t));
    BootReadArgument((uint8_t *)pboot_params, sizeof(boot_params_t));
    if( BOOT_PARAMS_HEADER != pboot_params->header )
    {
        initBootParams(pboot_params);
    }
}

/*保存bootloader参数区*/
void saveBootParams(boot_params_t *pboot_params)
{
    BootWriteArgument((uint8_t *)pboot_params, sizeof(boot_params_t));
}

/*初始化系统参数区*/
void initSystemParams(system_params_t *psystem_params)
{
    memset((uint8_t *)psystem_params, 0, sizeof(system_params_t));
    psystem_params->header = SYSTEM_PARAMS_HEADER;
    psystem_params->config_flag = 1;
    psystem_params->zone = 8;
    saveSystemParams(psystem_params);
}

/*初始化系统参数区 保留密钥参数*/
void initFacSystemParams(system_params_t *psystem_params)
{
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
}

/*加载系统参数区*/
void loadSystemParams(system_params_t *psystem_params)
{
    memset(psystem_params, 0, sizeof(system_params_t));
    SystemReadArgument(SYSTEM_ARGUMENT_ADDRESS, (uint16_t *)psystem_params, sizeof(system_params_t)/2);
    if( SYSTEM_PARAMS_HEADER != psystem_params->header )
    {
        initSystemParams(psystem_params);
    }
}

/*保存系统参数区*/
void saveSystemParams(system_params_t *psystem_params)
{
    SystemWriteArgument(SYSTEM_ARGUMENT_ADDRESS, (uint16_t *)psystem_params, sizeof(system_params_t)/2);
}



