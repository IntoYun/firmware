#include "flash_map.h"
#include "ui_hal.h"
#include "boot_debug.h"

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
    HAL_UI_SysTick_Handler();
}

/**
 * @brief  Configures Main system clocks & power.
 * @param  None
 * @retval None
 */
void Set_System(void)
{
    HAL_UI_Initial();
}

void delay(uint32_t ms)
{
    ets_delay_us(ms*1000);
}

void System_Reset(void)
{
}

/* empty rtc_printf implementation, to work with librtc
   linking. Can be removed once -lrtc is removed from bootloader's
   main component.mk.
*/
int rtc_printf(void)
{
    return 0;
}

