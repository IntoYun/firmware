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

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
//#include <stdatomic.h>
#include <string.h>
#include "core_hal.h"
#include "watchdog_hal.h"
#include "rng_hal.h"
// #include "rgbled_hal.h"
#include "bootloader.h"
#include "gpio_hal.h"
#include "interrupts_hal.h"
#include "hw_config.h"
#include "syshealth_hal.h"
#include "rtc_hal.h"
// #include "stm32l1xx_it.h"
#include "service_debug.h"

/* Private typedef ----------------------------------------------------------*/

/* Private define -----------------------------------------------------------*/
void HAL_Core_Setup(void);

/* Private macro ------------------------------------------------------------*/

/* Private variables --------------------------------------------------------*/

/* Extern variables ----------------------------------------------------------*/
/**
 * Updated by HAL_1Ms_Tick()
 */
extern volatile uint32_t TimingDelay;
volatile bool systick_hook_enabled = false;

void HAL_SysTick_Hook(void) __attribute__((weak));

void HAL_SysTick_Hook(void)
{

}

void HAL_Hook_Main() __attribute__((weak));

void HAL_Hook_Main()
{
    // nada

}

int main() {
    // the rtos systick can only be enabled after the system has been initialized
    systick_hook_enabled = true;
    HAL_Hook_Main();
    app_setup_and_loop();
    return 0;
}

void HAL_Core_Init(void)
{

}

void HAL_Core_Config(void)
{
//    DECLARE_SYS_HEALTH(ENTERED_SparkCoreConfig);
    Set_System();

#ifdef DFU_BUILD_ENABLE
    //Currently this is done through WICED library API so commented.
    //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x20000);
#endif

    //Wiring pins default to inputs
#if !defined(USE_SWD_JTAG) && !defined(USE_SWD)
    for (pin_t pin=0; pin<=7; pin++)
//        HAL_Pin_Mode(pin, INPUT);
    for (pin_t pin=30; pin<=37; pin++)
//        HAL_Pin_Mode(pin, INPUT);
#endif

    HAL_RTC_Initial();
    // HAL_RNG_Initial();

    // HAL_IWDG_Initial();
    // HAL_LED_Initial();

#ifdef DFU_BUILD_ENABLE
    //Load_SystemFlags();
#endif

    // HAL_LED_RGB_Color(255, 255, 255);
}

void HAL_Core_Setup(void)
{
    HAL_IWDG_Config(DISABLE);
    bootloader_update_if_needed();
    HAL_Bootloader_Lock(true);
}

void HAL_Core_System_Reset(void)
{
    NVIC_SystemReset();
}

void HAL_Core_System_Reset_Ex(int reason, uint32_t data, void *reserved)
{
    if (HAL_Feature_Get(FEATURE_RESET_INFO))
    {
        // Save reset info to backup registers
        //RTC_WriteBackupRegister(RTC_BKP_DR2, reason);
        //RTC_WriteBackupRegister(RTC_BKP_DR3, data);
    }
    HAL_Core_System_Reset();
}
void HAL_Core_Enter_DFU_Mode(bool persist)
{
    // true  - DFU mode persist if firmware upgrade is not completed
    // false - Briefly enter DFU bootloader mode (works with latest bootloader only )
    //         Subsequent reset or power off-on will execute normal firmware
    #if 0
    if (persist)
        {
            HAL_PARAMS_Set_Boot_boot_flag(BOOT_FLAG_USB_DFU);
            HAL_PARAMS_Save_Params();
        }
    else
        {
            HAL_Core_Write_Backup_Register(BKP_DR_01, 0x7DEA);
        }
    HAL_Core_System_Reset();
    #endif
}

void HAL_Core_Enter_Factory_Reset_Mode(void)
{
    // HAL_PARAMS_Set_Boot_boot_flag(BOOT_FLAG_FACTORY_RESET);
    // HAL_PARAMS_Save_Params();
    // HAL_Core_System_Reset();
}

void HAL_Core_Factory_Reset(void)
{
}

void HAL_Core_Enter_Safe_Mode(void* reserved)
{
}

void HAL_Core_Enter_Bootloader(bool persist)
{
}

void HAL_Core_Enter_Stop_Mode(uint16_t wakeUpPin, uint16_t edgeTriggerMode, long seconds)
{
}

void HAL_Core_Execute_Stop_Mode(void)
{
}

void HAL_Core_Enter_Standby_Mode(void)
{
}

void HAL_Core_Execute_Standby_Mode(void)
{
}

uint32_t HAL_Core_Compute_CRC32(const uint8_t *pBuffer, uint32_t bufferSize)
{
    return 0;
}

void HAL_Bootloader_Lock(bool lock)
{
}

unsigned HAL_Core_System_Clock(HAL_SystemClock clock, void* reserved)
{
    return 1;
}

int HAL_Feature_Set(HAL_Feature feature, bool enabled)
{
    return -1;
}

bool HAL_Feature_Get(HAL_Feature feature)
{
    return false;
}

int HAL_Set_System_Config(hal_system_config_t config_item, const void* data, unsigned length)
{
    return -1;
}

void HAL_Core_Set_System_Loop_Handler(void (*handler)(void))
{
   //APP_LineCodingBitRateHandler = handler;
}

uint16_t HAL_Core_Get_Subsys_Version(char* buffer, uint16_t len)
{
    return 0;
}
/*******************************************************************************
 * Function Name  : SysTick_Handler
 * Description    : This function handles SysTick Handler.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void SysTick_Handler(void)
{
    HAL_IncTick();
    System1MsTick();

    if (TimingDelay != 0x00)
    {
        TimingDelay--;
    }

    // another hook for an rtos
    if (systick_hook_enabled)
        HAL_SysTick_Hook();

    HAL_SysTick_Handler();
}

