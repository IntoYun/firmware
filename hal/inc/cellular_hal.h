/*
 ******************************************************************************
  Copyright (c) 2015 IntoRobot Industries, Inc.  All rights reserved.

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

#ifndef CELLULAR_HAL_H
#define CELLULAR_HAL_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "net_hal.h"
#include "inet_hal.h"
#include "system_tick_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __attribute__((__packed__))  _CellularConfig_t {
    uint16_t size;
    NetworkConfig nw;
} CellularConfig;

typedef int (*_CALLBACKPTR_MDM)(int type, const char* buf, int len, void* param);

typedef void (*_CELLULAR_SMS_CB_MDM)(void* data, int index);

/**
 * Power on and initialize the cellular module,
 * if USART3 not initialized, will be done on first call.
 */
int HAL_Cellular_On(void* reserved);

/**
 * Power off the cellular module.
 */
int HAL_Cellular_Off(void* reserved);

void HAL_Cellular_Drive(void);

#ifdef __cplusplus
// Todo - is storing raw string pointers correct here? These will only be valid
// If they are stored as constants in the application.
struct CellularCredentials
{
    uint16_t size;
    const char* apn = "";
    const char* username = "";
    const char* password = "";

    CellularCredentials()
    {
        size = sizeof(*this);
    }
};
#else
typedef struct CellularCredentials CellularCredentials;
#endif

int HAL_Cellular_Connect(void* reserved);

int HAL_Cellular_Disconnect(void* reserved);

/**
 * Fetch the ip configuration.
 */
int HAL_Cellular_Fetch_Ipconfig(CellularConfig* config, void* reserved);

#ifdef __cplusplus
struct CellularDevice
{
    uint16_t size;
    char iccid[21];
    char imei[16];

    CellularDevice()
    {
        memset(this, 0, sizeof(*this));
        size = sizeof(*this);
    }
};
#else
typedef struct CellularDevice CellularDevice;
#endif

/**
 * Retrieve cellular module info, must be initialized first.
 */
int HAL_Cellular_Get_Device_Info(CellularDevice* device, void* reserved);

/**
 * Set cellular connection parameters
 */
int HAL_Cellular_Set_Credentials(const char* apn, const char* username, const char* password, void* reserved);

/**
 * Get cellular connection parameters
 */
CellularCredentials* HAL_Cellular_Get_Credentials(void* reserved);

/**
 * Attempts to stop/resume the cellular modem from performing AT operations.
 * Called from another thread or ISR context.
 *
 * @param cancel: true to cancel AT operations, false will resume AT operations.
 *        calledFromISR: true if called from ISR, false if called from main system thread.
 *        reserved: pass NULL. Allows future expansion.
 */
void HAL_Cellular_Cancel(bool cancel, bool calledFromISR, void* reserved);

#ifdef __cplusplus
struct CellularSignalHal
{
    int rssi = 0;
    int qual = 0;
};
#else
typedef struct CellularSignalHal CellularSignalHal;
#endif

/**
 * Retrieve cellular signal strength info
 */
int HAL_Cellular_Get_Signal(CellularSignalHal &signal, void* reserved);

/**
 * Send an AT command and wait for response, optionally specify a callback function to parse the results
 */
int HAL_Cellular_Command(_CALLBACKPTR_MDM cb, void* param, system_tick_t timeout_ms, const char* format, ...);

/**
 * Set the SMS received callback handler
 */
int HAL_Cellular_Set_SMS_Received_Handler(_CELLULAR_SMS_CB_MDM cb, void* data, void* reserved);

/**
 * Pauses cellular modem serial communication
 */
int HAL_Cellular_Pause(void* reserved);

/**
 * Resumes cellular modem serial communication
 */
int HAL_Cellular_Resume(void* reserved);

#ifdef __cplusplus
}
#endif

#endif	/* CELLULAR_HAL_H */

