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
#ifndef __SOFTWARE_TIMERS_HAL_H
#define __SOFTWARE_TIMERS_HAL_H

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif
typedef void* hal_timer_t;

int HAL_Software_Timers_Create(hal_timer_t *timer, unsigned period, void (*callback)(hal_timer_t timer), void* const timer_id, bool one_shot);
int HAL_Software_Timers_Start(hal_timer_t timer, bool fromISR, unsigned block);
int HAL_Software_Timers_Stop(hal_timer_t timer, bool fromISR, unsigned block);
int HAL_Software_Timers_Reset(hal_timer_t timer, bool fromISR, unsigned block);
int HAL_Software_Timers_Change_Period(hal_timer_t timer, unsigned period, bool fromISR, unsigned block);
int HAL_Software_Timers_Dispose(hal_timer_t timer);
int HAL_Software_Timers_Is_Active(hal_timer_t timer);
int HAL_Software_Timers_Get_Id(hal_timer_t timer, void **timer_id);

#ifdef __cplusplus
}
#endif

#endif  /* __SOFTWARE_TIMERS_HAL_H */
