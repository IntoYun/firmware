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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMER_HAL_H
#define __TIMER_HAL_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
typedef enum {
    TIMER_PRECISION_US, //milliseconds
    TIMER_PRECISION_MS  //microseconds
} timer_precision_t;

typedef void (*timer_callback_fn_t)(void);
typedef int32_t timer_handle_t;

/* Exported constants --------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

int HAL_Timer_Create(timer_handle_t *handle, uint32_t period, timer_callback_fn_t callback_fn, bool one_shot, timer_precision_t precision);
int HAL_Timer_Start(timer_handle_t handle);
int HAL_Timer_Stop(timer_handle_t handle);
int HAL_Timer_Reset(timer_handle_t handle);
int HAL_Timer_Attach_Interrupt(timer_handle_t handle, uint32_t period);
int HAL_Timer_Change_Period(timer_handle_t handle, uint32_t period);
int HAL_Timer_Is_Active(timer_handle_t handle);
uint32_t HAL_Timer_Get_Remain_Time(timer_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif  /* __TIMER_HAL_H */

