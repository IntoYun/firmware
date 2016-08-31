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
#include "wiring_platform.h"
//#include "wiring_system.h"
#include "system_task.h"
#include "system_cloud.h"
#include "system_mode.h"
#include "system_network.h"
#include "system_network_internal.h"
#include "system_update.h"
#include "system_rgbled.h"
#include "system_config.h"
#include "intorobot_macros.h"
#include "string.h"
#include "system_tick_hal.h"
#include "watchdog_hal.h"
#include "wlan_hal.h"
#include "delay_hal.h"
#include "timer_hal.h"
#include "params_hal.h"
#include "service_debug.h"

#include "wiring_network.h"
#include "wiring_constants.h"
#include "wiring_cloud.h"
#include "system_threading.h"

using intorobot::Network;

extern volatile uint8_t INTOROBOT_IMLINK_CONFIG_IS_NEEDED; //进入配置模式
extern volatile uint8_t INTOROBOT_IMLINK_CONFIG_START; //配置模式开始

/**
 * Time in millis of the last cloud connection attempt.
 * The next attempt isn't made until the backoff period has elapsed.
 */
static int network_backoff_start = 0;
static int cloud_backoff_start = 0;

/**
 * The number of connection attempts.
 */
static uint8_t network_connection_attempts = 0;
static uint8_t cloud_failed_connection_attempts = 0;

void network_connection_attempt_init()
{
    network_connection_attempts=0;
    network_backoff_start = HAL_Timer_Get_Milli_Seconds();
}

void network_connection_attempted()
{
    if (network_connection_attempts<255)
        network_connection_attempts++;
    network_backoff_start = HAL_Timer_Get_Milli_Seconds();
}

inline uint8_t in_network_backoff_period()
{
    return (HAL_Timer_Get_Milli_Seconds()-network_backoff_start)<backoff_period(network_connection_attempts);
}

void cloud_connection_attempt_init()
{
    cloud_failed_connection_attempts=0;
    cloud_backoff_start = HAL_Timer_Get_Milli_Seconds();
}

void cloud_connection_failed()
{
    if (cloud_failed_connection_attempts<255)
        cloud_failed_connection_attempts++;
    cloud_backoff_start = HAL_Timer_Get_Milli_Seconds();
}

inline uint8_t in_cloud_backoff_period()
{
    return (HAL_Timer_Get_Milli_Seconds()-cloud_backoff_start)<backoff_period(cloud_failed_connection_attempts);
}

void Network_Setup(void)
{
#if !PARTICLE_NO_NETWORK
    network.setup();

    // don't automatically connect when threaded since we want the thread to start asap
    if(system_mode() == AUTOMATIC)
    {
        network.connect();
    }
    system_rgb_blink(RGB_COLOR_GREEN, 1000);//绿灯闪烁
    network_connection_attempt_init();
#endif

#if !SPARK_NO_CLOUD
    intorobot_cloud_init();
#endif
}

/**
 * Use usb serial ymodem flasher to update firmware.
 */
void manage_serial_flasher()
{

}

/**
 * Reset or initialize the network connection as required.
 */

void manage_network_connection()
{
    if (in_network_backoff_period())
        return;

    bool was_connected = network.connected();

    DEBUG_D("network: checking\r\n");
    if (network.status()) {
        DEBUG_D("network connected\r\n");
        if(!was_connected) {
            system_rgb_blink(RGB_COLOR_BLUE, 1000);//蓝灯闪烁
        }
    }
    else
    {
        DEBUG_D("network connection failed\r\n");
        if(was_connected) {
            g_intorobot_cloud_connected = 0;
            system_rgb_blink(RGB_COLOR_GREEN, 1000);//绿灯闪烁
        }
    }
    network_connection_attempted();
}

#ifndef SPARK_NO_CLOUD
/**
 * Establishes a socket connection to the cloud if not already present.
 * - attempts to open a socket to the cloud
 *
 * On return, SPARK_CLOUD_SOCKETED is set to true if the socket connection was successful.
 */

void establish_cloud_connection(void)
{
    if (network.connected())
    {
        if (!g_intorobot_cloud_connected)
        {
            if (in_cloud_backoff_period())
                return;

            DEBUG_D("Cloud: connecting\r\n");
            int connect_result = intorobot_cloud_connect();
            if (connect_result >= 0)
            {
                DEBUG_D("Cloud connected\r\n");
                g_intorobot_cloud_connected = 1;
                cloud_failed_connection_attempts = 0;
                system_rgb_blink(RGB_COLOR_WHITE, 2000); //白灯闪烁
            }
            else
            {
                DEBUG_D("Cloud connection failed: %d\r\n", connect_result);
                g_intorobot_cloud_connected = 0;
                intorobot_cloud_disconnect();
                cloud_connection_failed();
            }
        }
    }
}

void handle_cloud_connection(void)
{
    if (network.connected())
    {
        if (g_intorobot_cloud_connected)
        {
            int err = intorobot_cloud_handle();
            if (err) {
                DEBUG_D("Cloud disconnected\r\n");
                g_intorobot_cloud_connected = 0;
                intorobot_cloud_disconnect();
                system_rgb_blink(RGB_COLOR_BLUE, 1000);
            }
        }
    }
}

void manage_cloud_connection(void)
{
    if (intorobot_cloud_flag_auto_connect() == 0)
    {
        intorobot_cloud_disconnect();
    }
    else // cloud connection is wanted
    {
        establish_cloud_connection();
        handle_cloud_connection();
    }
}
#endif

// These are internal methods
bool manage_imlink_config()
{
    if(HAL_PARAMS_Get_System_config_flag())
    {
        if (!INTOROBOT_IMLINK_CONFIG_START)
        {
            DEBUG_D(("enter device config\r\n"));
            system_rgb_blink(RGB_COLOR_RED, 1000);
            DeviceConfigUsb.init();
            DeviceConfigUdp.init();
            INTOROBOT_IMLINK_CONFIG_START = true;
            INTOROBOT_IMLINK_CONFIG_IS_NEEDED = true;
        }
        if(DeviceConfigUsb.process()||DeviceConfigUdp.process())
        {
            DEBUG_D(("exit  device config\r\n"));
            HAL_PARAMS_Set_System_config_flag(0);
            HAL_PARAMS_Save_Params();
            INTOROBOT_IMLINK_CONFIG_START = false;
        }
        else {
            return false;
        }
    }
    if(true == INTOROBOT_IMLINK_CONFIG_IS_NEEDED) {
        Network_Setup();
        INTOROBOT_IMLINK_CONFIG_IS_NEEDED = false;
    }
    return true;
}

void system_process_loop(void)
{
#if PLATFORM_THREADING
    while (1) {
#endif
        if(manage_imlink_config())
        {
            manage_serial_flasher();
            manage_network_connection();
            manage_ip_config();
            CLOUD_FN(manage_cloud_connection(), (void)0);
        }
#if PLATFORM_THREADING
    }
#endif
}

