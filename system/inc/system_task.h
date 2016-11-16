/**
 ******************************************************************************
 * @file    spark_wiring_wlan.h
 * @author  Satish Nair and Zachary Crockett
 * @version V1.0.0
 * @date    13-March-2013
 * @brief   Header for spark_wiring_wlan.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SYSTEM_TASK_H_
#define SYSTEM_TASK_H_

#include "socket_hal.h"
#include "system_cloud.h"
#include "wlan_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    BUTTON_MODE_NONE = 0,
    BUTTON_MODE_CONFIG,
    BUTTON_MODE_DEFFW,
    BUTTON_MODE_COM,
    BUTTON_MODE_FAC,
    BUTTON_MODE_NC,
    BUTTON_MODE_RESET
} button_mode_t;

void system_process_loop(void);
void ui_process_loop(void);

uint32_t HAL_NET_SetNetWatchDog(uint32_t timeOutInuS);
void Network_Setup();


extern volatile uint8_t g_intorobot_network_connected;
extern volatile uint8_t g_intorobot_cloud_connected;
extern volatile uint8_t INTOROBOT_WLAN_SLEEP;
extern volatile uint8_t INTOROBOT_WLAN_STARTED;

extern volatile uint8_t SYSTEM_POWEROFF;
extern volatile uint8_t SPARK_FLASH_UPDATE;
extern volatile uint32_t TimingFlashUpdateTimeout;


/**
 * Determines the backoff period after a number of failed connections.
 */
unsigned backoff_period(unsigned connection_attempts);

/**
 * This is for internal testing. Do not call this function since it is not
 * guaranteed to be preserved or backwards compatible between releases.
 */
void* system_internal(int item, void* reserved);

void manage_setup_config();

#ifdef __cplusplus
}
#endif

#endif  /*SYSTEM_TASK_H_*/
