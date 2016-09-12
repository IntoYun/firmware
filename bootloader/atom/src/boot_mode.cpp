#include "boot_mode.h"
#include "memory_hal.h"
#include "params_hal.h"
#include "ui_hal.h"
#include "flash_map.h"
#include "system_config.h"
#include "boot_debug.h"

// extern UART_HandleTypeDef UartHandleEsp8266;
// extern SDK_QUEUE USART_Esp8266_Queue;

pFunction JumpToApplication;
uint32_t JumpAddress;

void start_app(void)
{
    if(((*(__IO uint32_t*)APP_ADDR) & 0x2FFE0000 ) == 0x20000000)
    {
        /* Jump to user application */
        JumpAddress = *(__IO uint32_t*) (APP_ADDR + 4);
        JumpToApplication = (pFunction) JumpAddress;

        /* Initialize user application's Stack Pointer */
        __set_MSP(*(__IO uint32_t*) APP_ADDR);
        JumpToApplication();
    }
}

void USBD_DFU_Init(void)
{
    USBD_Init(&USBD_Device, &DFU_Desc, 0);
    USBD_RegisterClass(&USBD_Device, USBD_DFU_CLASS);
    USBD_DFU_RegisterMedia(&USBD_Device, &USBD_DFU_fops);
    USBD_Start(&USBD_Device);
}


void Enter_DFU_Mode(void)
{
    HAL_UI_RGB_Color(RGB_COLOR_MAGENTA);
    USBD_DFU_Init();
    while(1)
    {}
}

