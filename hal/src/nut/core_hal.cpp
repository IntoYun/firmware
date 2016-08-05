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
#include <string.h>
#include "hw_config.h"
#include "core_hal.h"
#include "rng_hal.h"
#include "rgbled_hal.h"
#include "bootloader.h"
#include "gpio_hal.h"
#include "interrupts_hal.h"
#include "syshealth_hal.h"
#include "rtc_hal.h"
#include "service_debug.h"
#include "delay_hal.h"

#include <Arduino.h>
#include "Schedule.h"
extern "C" {
#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "user_interface.h"
#include "cont.h"
}
#include <core_version.h>




/* Private typedef ----------------------------------------------------------*/

/* Private define -----------------------------------------------------------*/
void HAL_Core_Setup(void);

/* Private macro ------------------------------------------------------------*/
#define LOOP_TASK_PRIORITY 1
#define LOOP_QUEUE_SIZE    1
#define OPTIMISTIC_YIELD_TIME_US 16000

/* Private variables --------------------------------------------------------*/
struct rst_info resetInfo;

/* Extern variables ----------------------------------------------------------*/
/**
 * Updated by HAL_1Ms_Tick()
 */
extern volatile uint32_t TimingDelay;

int atexit(void (*func)()) {
    return 0;
}

void preloop_update_frequency() __attribute__((weak));
void preloop_update_frequency() {
#if defined(F_CPU) && (F_CPU == 160000000L)
    REG_SET_BIT(0x3ff00014, BIT(0));
    ets_update_cpu_frequency(160);
#endif
}

extern void (*__init_array_start)(void);
extern void (*__init_array_end)(void);

extern "C" void __gdb_init() {}
extern "C" void gdb_init(void) __attribute__ ((weak, alias("__gdb_init")));

extern "C" void __gdb_do_break(){}
extern "C" void gdb_do_break(void) __attribute__ ((weak, alias("__gdb_do_break")));

cont_t g_cont __attribute__ ((aligned (16)));
static os_event_t g_loop_queue[LOOP_QUEUE_SIZE];

static uint32_t g_micros_at_task_start;

extern "C" void esp_yield() {
    if (cont_can_yield(&g_cont)) {
        cont_yield(&g_cont);
    }
}

extern "C" void esp_schedule() {
    ets_post(LOOP_TASK_PRIORITY, 0, 0);
}

extern "C" void __yield() {
    if (cont_can_yield(&g_cont)) {
        esp_schedule();
        esp_yield();
    }
    else {
        //panic();
    }
}

extern "C" void yield(void) __attribute__ ((weak, alias("__yield")));

extern "C" void optimistic_yield(uint32_t interval_us) {
    if (cont_can_yield(&g_cont) &&
        (system_get_time() - g_micros_at_task_start) > interval_us)
    {
        yield();
    }
}

static void loop_wrapper() {
    preloop_update_frequency();
    app_loop();
    run_scheduled_functions();
    esp_schedule();
}

static void loop_task(os_event_t *events) {
    g_micros_at_task_start = system_get_time();
    cont_run(&g_cont, &loop_wrapper);
    if (cont_check(&g_cont) != 0) {
       // panic();
    }
}

static void do_global_ctors(void) {
    void (**p)(void) = &__init_array_end;
    while (p != &__init_array_start)
        (*--p)();
}


void init_done() {
    system_set_os_print(1);
    gdb_init();
    do_global_ctors();
    app_setup_and_loop_initial();
    esp_schedule();
}

extern "C" void user_init(void) {
    struct rst_info *rtc_info_ptr = system_get_rst_info();
    memcpy((void *) &resetInfo, (void *) rtc_info_ptr, sizeof(resetInfo));
    uart_div_modify(0, UART_CLK_FREQ / (115200));

    SysTick_Enable();

    cont_init(&g_cont);
    ets_task(loop_task, LOOP_TASK_PRIORITY, g_loop_queue, LOOP_QUEUE_SIZE);
    system_init_done_cb(&init_done);
}

void HAL_Core_Init(void)
{

}

void HAL_Core_Config(void)
{
    HAL_RTC_Initial();
    HAL_RNG_Initial();

    HAL_IWDG_Initial();
    HAL_LED_Initial();

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
    //NVIC_SystemReset();
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

/*******************************************************************************
 * Function Name  : SysTick_Handler
 * Description    : This function handles SysTick Handler.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void SysTick_Handler(void)
{
    //HAL_IncTick();
    //System1MsTick();

    if (TimingDelay != 0x00)
    {
        TimingDelay--;
    }

    HAL_SysTick_Handler();
}

