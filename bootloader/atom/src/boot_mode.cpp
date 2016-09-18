#include "hw_config.h"
#include "boot_mode.h"
#include "memory_hal.h"
#include "params_hal.h"
#include "ui_hal.h"
#include "flash_map.h"
#include "system_config.h"
#include "boot_debug.h"


pFunction JumpToApplication;
uint32_t JumpAddress;

void start_app(void)
{
    if(((*(__IO uint32_t*)CORE_FW_ADDRESS) & APP_START_MASK ) == 0x20000000)
    {
        /* Jump to user application */
        JumpAddress = *(__IO uint32_t*) (CORE_FW_ADDRESS + 4);
        JumpToApplication = (pFunction) JumpAddress;

        /* Initialize user application's Stack Pointer */
        __set_MSP(*(__IO uint32_t*) CORE_FW_ADDRESS);

        JumpToApplication();
    }
}

void USBD_DFU_Init(void)
{
    USBD_Init(&USBD_Device, &DFU_Desc, 0);
    USBD_RegisterClass(&USBD_Device, USBD_DFU_CLASS);
    USBD_DFU_RegisterMedia(&USBD_Device, &USBD_DFU_fops);
    USBD_Start(&USBD_Device);

    USB_Cable_Config(DISABLE);
    delay(100);
    USB_Cable_Config(ENABLE);
}

void bootloader_dfu_downover_Handler(void)
{
    BOOT_DEBUG("bootloader_dfu_downover_Handler\r\n");
    start_app();
    while(1);
}

void Enter_DFU_Mode(void)
{
    HAL_UI_UserLED_Control(1);

    HAL_PARAMS_Set_Boot_boot_flag(BOOT_FLAG_NORMAL);
    HAL_PARAMS_Save_Params();

    //SetDfuDownOverHandler(&bootloader_dfu_downover_Handler);
    USBD_DFU_Init();
    while(1)
    {}
}

