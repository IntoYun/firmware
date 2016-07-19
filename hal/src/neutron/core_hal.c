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
#include "gpio_hal.h"
#include "interrupts_hal.h"
//#include "hw_config.h"
//#include "syshealth_hal.h"
#include "rtc_hal.h"
#include "stm32f4xx_it.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


/* Private typedef ----------------------------------------------------------*/

/* Private define -----------------------------------------------------------*/

/* Private macro ------------------------------------------------------------*/

/* Private variables --------------------------------------------------------*/

/* Extern variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static TaskHandle_t  app_thread_handle;
#define APPLICATION_STACK_SIZE 6144

/**
 * The mutex to ensure only one thread manipulates the heap at a given time.
 */
xSemaphoreHandle malloc_mutex = 0;

static void init_malloc_mutex(void)
{
    malloc_mutex = xSemaphoreCreateRecursiveMutex();
}

void __malloc_lock(void* ptr)
{
    if (malloc_mutex)
        while (!xSemaphoreTakeRecursive(malloc_mutex, 0xFFFFFFFF)) {}
}

void __malloc_unlock(void* ptr)
{
    if (malloc_mutex)
        xSemaphoreGiveRecursive(malloc_mutex);
}

void application_task_start(void* arg)
{
	//application_start();
}

/**
 * Called from startup_stm32f2xx.s at boot, main entry point.
 */
int main(void)
{
    init_malloc_mutex();
    xTaskCreate( application_task_start, "app_thread", APPLICATION_STACK_SIZE/sizeof( portSTACK_TYPE ), NULL, 2, &app_thread_handle);

    vTaskStartScheduler();

    /* we should never get here */
    while (1);

    return 0;
}

void SysTickChain()
{
    /*
    void (*chain)(void) = (void (*)(void))((uint32_t*)&link_interrupt_vectors_location)[SysTick_Handler_Idx];

    chain();

    SysTickOverride();
    */
}

void HAL_1Ms_Tick()
{
    /*
    if (TimingDelay != 0x00)
    {
        __sync_sub_and_fetch(&TimingDelay, 1);
    }
    */
}
/**
 * Called by HAL_Core_Init() to pre-initialize any low level hardware before
 * the main loop runs.
 */
void HAL_Core_Init_finalize(void)
{
}


/**
 * Called by HAL_Core_Setup() to perform any post-setup config after the
 * watchdog has been disabled.
 */
void HAL_Core_Setup_finalize(void)
{
    /*
    uint32_t* isrs = (uint32_t*)&link_ram_interrupt_vectors_location;
    isrs[SysTick_Handler_Idx] = (uint32_t)SysTickChain;
    // retained memory is critical for efficient data use on the electron
    HAL_Feature_Set(FEATURE_RETAINED_MEMORY, ENABLE);
    */
}

void HAL_Core_Init(void)
{
}

void HAL_Core_Config(void)
{

}

uint16_t HAL_Core_Mode_Button_Pressed_Time()
{
    return 0;
}

bool HAL_Core_Mode_Button_Pressed(uint16_t pressedMillisDuration)
{
    return false;
}

void HAL_Core_Mode_Button_Reset(void)
{
}

void HAL_Core_System_Reset(void)
{
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

int32_t HAL_Core_Backup_Register(uint32_t BKP_DR)
{
    return 0;
}

void HAL_Core_Write_Backup_Register(uint32_t BKP_DR, uint32_t Data)
{
}

uint32_t HAL_Core_Read_Backup_Register(uint32_t BKP_DR)
{
    return 0;
}

// todo find a technique that allows accessor functions to be inlined while still keeping
// hardware independence.
bool HAL_watchdog_reset_flagged()
{
    return false;
}

void HAL_Notify_WDT()
{
}

void HAL_Bootloader_Lock(bool lock)
{
}


unsigned HAL_Core_System_Clock(HAL_SystemClock clock, void* reserved)
{
    return 1;
}

/*
int main()
{
    while(1);
}
*/

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