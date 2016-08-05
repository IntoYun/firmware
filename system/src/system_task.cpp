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
#include "wiring_system.h"
#include "wiring_usbserial.h"
#include "system_task.h"
#include "system_cloud.h"
//#include "system_cloud_internal.h"
#include "system_mode.h"
#include "system_network.h"
#include "system_network_internal.h"
#include "system_update.h"
#include "intorobot_macros.h"
#include "string.h"
#include "system_tick_hal.h"
#include "watchdog_hal.h"
#include "wlan_hal.h"
#include "delay_hal.h"
#include "timer_hal.h"
#include "rgbled_hal.h"
#include "service_debug.h"

#include "wiring_network.h"
#include "wiring_constants.h"
#include "wiring_cloud.h"
#include "system_threading.h"


void system_process_loop(void)
{
    //DEBUG_D("system_process_loop\r\n");
#if PLATFORM_THREADING
    while (1) {
        //DEBUG_D("process_loop");
    }
#endif
}

void ui_process_loop(void)
{
    DEBUG_D("ui_process_loop\r\n");

}

#if 0




using intorobot::Network;

volatile system_tick_t spark_loop_total_millis = 0;

// Auth options are WLAN_SEC_UNSEC, WLAN_SEC_WPA, WLAN_SEC_WEP, and WLAN_SEC_WPA2
unsigned char _auth = WLAN_SEC_WPA2;

unsigned char wlan_profile_index;

volatile uint8_t SPARK_LED_FADE = 1;

volatile uint8_t Spark_Error_Count;
volatile uint8_t SYSTEM_POWEROFF;


void Network_Setup(bool threaded)
{
#if !PARTICLE_NO_NETWORK
    network.setup();

    // don't automatically connect when threaded since we want the thread to start asap
    if ((!threaded && system_mode() == AUTOMATIC) || system_mode()==SAFE_MODE)
    {
        network.connect();
    }
#endif

#ifndef SPARK_NO_CLOUD
    //Initialize spark protocol callbacks for all System modes
    //Spark_Protocol_Init();
#endif
}

static int cfod_count = 0;

/**
 * Use usb serial ymodem flasher to update firmware.
 */
void manage_serial_flasher()
{
    if(SPARK_FLASH_UPDATE == 3)
    {
        system_firmwareUpdate(&Serial);
    }
}

/**
 * Reset or initialize the network connection as required.
 */
void manage_network_connection()
{
    if (SPARK_WLAN_RESET || SPARK_WLAN_SLEEP || WLAN_WD_TO())
    {
        if (SPARK_WLAN_STARTED)
        {
            WARN("!! Resetting WLAN due to %s", (WLAN_WD_TO()) ? "WLAN_WD_TO()":((SPARK_WLAN_RESET) ? "SPARK_WLAN_RESET" : "SPARK_WLAN_SLEEP"));
            auto was_sleeping = SPARK_WLAN_SLEEP;
            auto was_disconnected = network.manual_disconnect();
            cloud_disconnect();
            network.off();
            CLR_WLAN_WD();
            SPARK_WLAN_RESET = 0;
            SPARK_WLAN_SLEEP = was_sleeping;
            network.set_manual_disconnect(was_disconnected);
            cfod_count = 0;
        }
    }
    else
    {
        if (!SPARK_WLAN_STARTED || (intorobot_cloud_flag_auto_connect() && !network.connected()))
        {
            INFO("Network Connect: %s", (!SPARK_WLAN_STARTED) ? "!SPARK_WLAN_STARTED" : "SPARK_CLOUD_CONNECT && !network.connected()");
            network.connect();
        }
    }
}

#ifndef SPARK_NO_CLOUD

/**
 * Time in millis of the last cloud connection attempt.
 * The next attempt isn't made until the backoff period has elapsed.
 */
static int cloud_backoff_start = 0;

/**
 * The number of connection attempts.
 */
static uint8_t cloud_failed_connection_attempts = 0;

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

void handle_cloud_errors()
{
#if 0
    // cfod resets in orange since they are soft errors
    LED_SetRGBColor(Spark_Error_Count > 1 ? RGB_COLOR_ORANGE : RGB_COLOR_RED);

    while (Spark_Error_Count != 0)
    {
        LED_On(LED_RGB);
        HAL_Delay_Milliseconds(500);
        LED_Off(LED_RGB);
        HAL_Delay_Milliseconds(500);
        Spark_Error_Count--;
    }

    // TODO Send the Error Count to Cloud: NVMEM_Spark_File_Data[ERROR_COUNT_FILE_OFFSET]

    // Reset Error Count
    network.set_error_count(0);
#endif
}

void handle_cfod()
{
#if 0
    if ((cfod_count += RESET_ON_CFOD) == MAX_FAILED_CONNECTS)
    {
        SPARK_WLAN_RESET = RESET_ON_CFOD;
        ERROR("Resetting CC3000 due to %d failed connect attempts", MAX_FAILED_CONNECTS);
    }

    if (Internet_Test() < 0)
    {
        // No Internet Connection
        if ((cfod_count += RESET_ON_CFOD) == MAX_FAILED_CONNECTS)
        {
            SPARK_WLAN_RESET = RESET_ON_CFOD;
            ERROR("Resetting CC3000 due to %d failed connect attempts", MAX_FAILED_CONNECTS);
        }

        Spark_Error_Count = 2;
    }
    else
    {
        // Cloud not Reachable
        Spark_Error_Count = 3;
    }
#endif
}

/**
 * Establishes a socket connection to the cloud if not already present.
 * - handles previous connection errors by flashing the LED
 * - attempts to open a socket to the cloud
 * - handles the CFOD
 *
 * On return, SPARK_CLOUD_SOCKETED is set to true if the socket connection was successful.
 */

void establish_cloud_connection()
{
#if 0
    if (network.ready() && !SPARK_WLAN_SLEEP && !SPARK_CLOUD_SOCKETED)
    {
        if (Spark_Error_Count)
            handle_cloud_errors();

        SPARK_LED_FADE = 0;
        LED_SetRGBColor(RGB_COLOR_CYAN);
        if (in_cloud_backoff_period())
            return;

        INFO("Cloud: connecting");
        LED_On(LED_RGB);
        int connect_result = spark_cloud_socket_connect();
        if (connect_result >= 0)
        {
            cfod_count = 0;
            SPARK_CLOUD_SOCKETED = 1;
            INFO("Cloud socket connected");
        }
        else
        {
            WARN("Cloud socket connection failed: %d", connect_result);
            SPARK_CLOUD_SOCKETED = 0;
            // if the user put the networkin listening mode via the button,
            // the cloud connect may have been cancelled.
            if (SPARK_WLAN_RESET || network.listening())
                return;

            cloud_connection_failed();
            handle_cfod();
            network.set_error_count(Spark_Error_Count);
        }
    }
#endif
}

int cloud_handshake()
{
#if 0
	bool udp = HAL_Feature_Get(FEATURE_CLOUD_UDP);
	bool presence_announce = !udp;
	int err = Spark_Handshake(presence_announce);
	return err;
#endif
}

/**
 * Manages the handshake and cloud events when the cloud has a socket connected.
 * @param force_events
 */
void handle_cloud_connection(bool force_events)
{
#if 0
    if (SPARK_CLOUD_SOCKETED)
    {
        if (!SPARK_CLOUD_CONNECTED)
        {
        		int err = cloud_handshake();
            if (err)
            {
            		if (!SPARK_WLAN_RESET && !network.listening())
            		{
					cloud_connection_failed();
					uint32_t color = RGB_COLOR_RED;
					if (particle::protocol::DECRYPTION_ERROR==err)
							color = RGB_COLOR_ORANGE;
					else if (particle::protocol::AUTHENTICATION_ERROR==err)
							color = RGB_COLOR_MAGENTA;
					WARN("Cloud handshake failed, code=%d", err);
					LED_SetRGBColor(color);
					LED_On(LED_RGB);
					// delay a little to be sure the user sees the LED color, since
					// the socket may quickly disconnect and the connection retried, turning
					// the LED back to cyan
					system_tick_t start = HAL_Timer_Get_Milli_Seconds();
					// allow time for the LED to be flashed
					while ((HAL_Timer_Get_Milli_Seconds()-start)<250);
            		}
				cloud_disconnect();
            }
            else
            {
                INFO("Cloud connected");
                SPARK_CLOUD_CONNECTED = 1;
                cloud_failed_connection_attempts = 0;
            }
        }

        if (SPARK_FLASH_UPDATE || force_events || System.mode() != MANUAL || system_thread_get_state(NULL)==spark::feature::ENABLED)
        {
            Spark_Process_Events();
        }
    }
#endif
}

void manage_cloud_connection(bool force_events)
{
    if (intorobot_cloud_flag_auto_connect() == 0)
    {
        cloud_disconnect();
    }
    else // cloud connection is wanted
    {
        establish_cloud_connection();

        handle_cloud_connection(force_events);
    }
}
#endif

#if Wiring_SetupButtonUX
extern void system_handle_button_click();
#endif

void IntoRobot_Idle_Events(bool force_events/*=false*/)
{
    DEBUG_D("IntoRobot_Idle_Events\r\n");
#if 0
    HAL_IWDG_Feed();

    ON_EVENT_DELTA();
    spark_loop_total_millis = 0;

    if (!SYSTEM_POWEROFF) {

#if Wiring_SetupButtonUX
        system_handle_button_click();
#endif
        manage_serial_flasher();

        manage_network_connection();

        manage_smart_config();

        manage_ip_config();

        CLOUD_FN(manage_cloud_connection(force_events), (void)0);
    }
    else
    {
        system_pending_shutdown();
    }
    system_shutdown_if_needed();
#endif
}
#endif
#if 1

void system_delay_ms(unsigned long ms, bool force_no_background_loop=false)
{
    HAL_Delay_Milliseconds(ms);
}


void cloud_disconnect(bool closeSocket)
{
#if 0
#ifndef SPARK_NO_CLOUD

    if (SPARK_CLOUD_SOCKETED || SPARK_CLOUD_CONNECTED)
    {
        INFO("Cloud: disconnecting");
        if (closeSocket)
        intorobot_cloud_socket_disconnect();

        SPARK_FLASH_UPDATE = 0;
        SPARK_CLOUD_CONNECTED = 0;
        SPARK_CLOUD_SOCKETED = 0;

        if (!network.manual_disconnect() && !network.listening())
        {
            LED_SetRGBColor(RGB_COLOR_GREEN);
            LED_On(LED_RGB);
        }
        INFO("Cloud: disconnected");
    }
    Spark_Error_Count = 0;  // this is also used for CFOD/WiFi reset, and blocks the LED when set.

#endif
#endif
}
#endif
