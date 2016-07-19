/**
 ******************************************************************************
 * @file     : wiring.c
 * @author   : robin
 * @version  : V1.0.0
 * @date     : 6-December-2014
 * @brief    :
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

#include "wiring.h"
#include "cmsis_os.h"
/*********************************************************************************
 *Function     : system_tick_t millis(void)
 *Description  : Should return the number of milliseconds since the processor started up.
 *Input        : none
 *Output       : none
 *Return       : return the system tickong clock count 1ms
 *author       : lz
 *date         : 6-December-2014
 *Others       : This is useful for measuring the passage of time.
 **********************************************************************************/
system_tick_t millis(void)
{
    return HAL_GetTick();
}

/*********************************************************************************
 *Function     : unsigned long micros(void)
 *Description  : Should return the number of microseconds since the processor started up.
 *Input        : none
 *Output       : none
 *Return       : return the DWT count us
 *author       : lz
 *date         : 6-December-2014
 *Others       : This is useful for measuring the passage of time.
 **********************************************************************************/
unsigned long micros(void)
{
    return (DWT->CYCCNT / SYSTEM_US_TICKS);
}

/*********************************************************************************
 *Function     : void delay(unsigned long ms)
 *Description  : This should block for a certain number of milliseconds and also execute intorobot_loop
 *Input        : ms:delay time value
 *Output       : none
 *Return       : none
 *author       : lz
 *date         : 6-December-2014
 *Others       :
 **********************************************************************************/
void delay(unsigned long ms)
{
    //osDelay(ms);
    #if 1
    volatile u32 delay_timer = timerGetId();
    while (1)
    {
       // KICK_WDT();
        if(timerIsEnd(delay_timer, ms))
        {
            break;
        }
    }
    #endif
}

void DWT_Init(void)
{
    //DBGMCU->CR |= DBGMCU_SETTINGS;
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

/*********************************************************************************
  *Function     : void delayMicroseconds(unsigned int us)
  *Description  : This should block for a certain number of microseconds.
  *Input        : us:delay time value
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       :
**********************************************************************************/
void delayMicroseconds(unsigned int us)
{
    volatile uint32_t DWT_START = DWT->CYCCNT;

    // keep DWT_TOTAL from overflowing (max 59.652323s w/72MHz SystemCoreClock)
    if (us > (UINT_MAX / SYSTEM_US_TICKS)) // UINT_MAX = 0xFFFFFFFF
    {
        us = (UINT_MAX / SYSTEM_US_TICKS);
    }

    volatile uint32_t DWT_TOTAL = (SYSTEM_US_TICKS * us);

    while((DWT->CYCCNT - DWT_START) < DWT_TOTAL)
    {
        //        KICK_WDT();
    }
}

/*********************************************************************************
 *Function      : u32 timerGetId(void)
 *Description  : This should block for a certain number of microseconds.
 *Input           : us:delay time value
 *Output         : none
 *Return         : none
 *author         : lz
 *date            : 6-December-2014
 *Others         :
 **********************************************************************************/
u32 timerGetId(void)
{
    return millis();
}

/*********************************************************************************
 *Function      : bool timerIsEnd(u32 timerID, u32 time)
 *Description  : This should block for a certain number of microseconds.
 *Input          : us:delay time value
 *Output        : none
 *Return        : none
 *author        : lz
 *date           : 6-December-2014
 *Others        :
 **********************************************************************************/
bool timerIsEnd(u32 timerID, u32 time)
{
    volatile system_tick_t current_millis = millis();
    volatile long elapsed_millis = current_millis - timerID;

    //Check for wrapping
    if (elapsed_millis < 0)
    {
        elapsed_millis =  0xFFFFFFFF -timerID + current_millis;
    }

    if (elapsed_millis >= (long)time)
    {
        return true;
    }
    return false;
}

