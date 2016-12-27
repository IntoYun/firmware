/**
 ******************************************************************************
 * @file    delay_hal.c
 * @author  Matthew McGowan
 * @version V1.0.0
 * @date    25-Sept-2014
 * @brief
 ******************************************************************************
  Copyright (c) 2013-2015 IntoRobot Industries, Inc.  All rights reserved.

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

#include "delay_hal.h"
#include "hw_config.h"
#include "watchdog_hal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp32-hal.h"
#include "service_debug.h"
/**
 * Updated by HAL_1Ms_Tick()
 */
volatile uint32_t TimingDelay;

/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nTime: specifies the delay time length, in milliseconds.
* Output         : None
* Return         : None
*******************************************************************************/
void HAL_Delay_Milliseconds(uint32_t millis)
{
#if 0
    vTaskDelay(millis / portTICK_PERIOD_MS);
#else
    volatile system_tick_t start_millis, current_millis, elapsed_millis;

    start_millis = HAL_Timer_Get_Milli_Seconds();
    while (1)
    {
        current_millis = HAL_Timer_Get_Milli_Seconds();
        elapsed_millis = current_millis - start_millis;
        //Check for wrapping
        if (elapsed_millis < 0)
        {
            elapsed_millis =  0xFFFFFFFF - start_millis + current_millis;
        }

        if (elapsed_millis >= (long)millis)
        {
            break;
        }
        system_loop_handler(100);
    }
#endif
}

/**
 * @brief  delay time in microseconds using 32-bit DWT->CYCCNT
 * @param  uSec: specifies the delay time length, in milliseconds.
 * @retval None
 */
void HAL_Delay_Microseconds(uint32_t micros)
{
    if(micros) {
        unsigned long endat = HAL_Timer_Get_Micro_Seconds();
        endat += micros;
        while(HAL_Timer_Get_Micro_Seconds() < endat) {
            NOP();
        }
    }
}

