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
#include "rgbled_hal.h"
#include "bootloader.h"
#include "gpio_hal.h"
#include "interrupts_hal.h"
#include "hw_config.h"
#include "syshealth_hal.h"
#include "rtc_hal.h"
#include "stm32f4xx_it.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "service_debug.h"
#include "cmsis_os.h"
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


/* Private function prototypes -----------------------------------------------*/
/**
 * 当freertos没有运行起来的时候 xPortSysTickHandler(); 在SysTick_Handler中不能调用,否则会出现异常
 * 例如：用户程序调用SerialDebugOutput debugOutput(115200, ALL_LEVEL);程序将卡死.
 */
static uint8_t  is_os_running=0;

static TaskHandle_t  app_thread_handle;

#define APP_TREAD_STACK_SIZE            6144
#define SYSTEM_TREAD_STACK_SIZE         6144
#define UI_TREAD_STACK_SIZE             configMINIMAL_STACK_SIZE

/**
 * The mutex to ensure only one thread manipulates the heap at a given time.
 */

static osSemaphoreId malloc_protect_sem;     //申请释放内存保护信号量

static void init_malloc_semaphore(void)
{
    //创建信号量
    osSemaphoreDef(MALLOC_SEM);
    malloc_protect_sem = osSemaphoreCreate(osSemaphore(MALLOC_SEM) , 1);
}

void __malloc_lock(void* ptr)
{
    //DEBUG("__malloc_lock=%d",malloc_protect_sem);
    if (malloc_protect_sem)
      while(osSemaphoreWait(malloc_protect_sem, 0) != osOK) {}
}

void __malloc_unlock(void* ptr)
{
    if (malloc_protect_sem)
        osSemaphoreRelease(malloc_protect_sem);
    //DEBUG("__malloc_unlock=%d",malloc_protect_sem);
}

static void app_task_start(void const *argument)
{
    is_os_running = 1;
    HAL_Core_Setup();
    app_setup_and_loop();
}

/**
 * Called from startup_stm32f2xx.s at boot, main entry point.
 */
int main(void)
{
    /*
       char _buffer[256]={0};

    //int a = HAL_Timer_Get_Milli_Seconds();
    int a = 12242;
    float i = a*0.34;
    sprintf(_buffer, "%f\r\n", i);
    while(1);
    */
#if 0
    HAL_Core_Setup();
    app_setup_and_loop();
    while(1);
    return 0;
#endif
#if 1
    init_malloc_semaphore();
    init_modem_semaphore();
    /*app_tread*/
    osThreadDef(APP_THREAD, app_task_start, osPriorityNormal,0,APP_TREAD_STACK_SIZE/sizeof( portSTACK_TYPE ));
    osThreadCreate(osThread(APP_THREAD),NULL);
#if 0
    /*system_tread*/
    osThreadDef(SYSTEM_THEARD, system_task_start, osPriorityNormal, 0, SYSTEM_TREAD_STACK_SIZE/sizeof( portSTACK_TYPE ));
    osThreadCreate(osThread(SYSTEM_THEARD),NULL);

    /*user_interface_thread   rgb and button*/
    osThreadDef(UI_THREAD, ui_task_start, osPriorityNormal, 0, UI_TREAD_STACK_SIZE/sizeof( portSTACK_TYPE ));
    osThreadCreate(osThread(UI_THREAD),NULL);
#endif
    /* Start scheduler */
    osKernelStart();
    /* we should never get here */
    while (1);
    return 0;
#endif

#if 0
    init_malloc_mutex();
    xTaskCreate( app_task_start, "app_thread", APP_TREAD_STACK_SIZE/sizeof( portSTACK_TYPE ), NULL, 2, &app_thread_handle);

    vTaskStartScheduler();
    /* we should never get here */
    while (1);
    return 0;
#endif
}

void HAL_1Ms_Tick()
{
    if (TimingDelay != 0x00)
    {
        __sync_sub_and_fetch(&TimingDelay, 1);
    }
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
    USE_SYSTEM_FLAGS = 1;
#endif

    //Wiring pins default to inputs
#if !defined(USE_SWD_JTAG) && !defined(USE_SWD)
    for (pin_t pin=0; pin<=7; pin++)
//        HAL_Pin_Mode(pin, INPUT);
    for (pin_t pin=30; pin<=37; pin++)
//        HAL_Pin_Mode(pin, INPUT);
#endif

    HAL_RTC_Initial();
    HAL_RNG_Initial();

    HAL_IWDG_Initial();
    HAL_LED_Initial();

#ifdef DFU_BUILD_ENABLE
    //Load_SystemFlags();
#endif

    HAL_LED_RGB_Color(255, 255, 255);
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


/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
    HAL_IncTick();
    System1MsTick();
#if 1
    if (1 == is_os_running){
        osSystickHandler();
    }
    /* Handle short and generic tasks for the device HAL on 1ms ticks */
    HAL_1Ms_Tick();
    HAL_SysTick_Handler();
    //HAL_System_Interrupt_Trigger(SysInterrupt_SysTick, NULL);
#endif
}
