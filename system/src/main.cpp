/**
 ******************************************************************************
 * @file    main.cpp
 * @author  Satish Nair, Zachary Crockett, Zach Supalla and Mohit Bhoite
 * @version V1.0.0
 * @date    13-March-2013
 *
 * Updated: 14-Feb-2014 David Sidrane <david_s5@usa.net>
 *
 * @brief   Main program body.
 ******************************************************************************
  Copyright (c) 2013-2015 Particle Industries, Inc.  All rights reserved.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program; if not, see <http://www.gnu.org/licenses/>.
  ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "service_debug.h"
#include "system_event.h"
#include "system_mode.h"
#include "system_task.h"
#include "system_network.h"
#include "system_network_internal.h"
//#include "system_cloud_internal.h"
#include "system_sleep.h"
#include "system_threading.h"
#include "system_user.h"
#include "system_update.h"
#include "core_hal.h"
#include "delay_hal.h"
#include "syshealth_hal.h"
#include "watchdog_hal.h"
#include "usb_hal.h"
#include "system_mode.h"
#include "rgbled_hal.h"
//#include "ledcontrol.h"
//#include "wiring_power.h"
//#include "wiring_fuel.h"
#include "wiring_interrupts.h"
#include "wiring_cellular.h"
#include "wiring_cellular_printable.h"
#include "system_rgbled.h"

using namespace intorobot;

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#if defined(DEBUG_BUTTON_WD)
#define BUTTON_WD_DEBUG(x,...) DEBUG(x,__VA_ARGS__)
#else
#define BUTTON_WD_DEBUG(x,...)
#endif

static volatile uint32_t button_timeout_start;
static volatile uint32_t button_timeout_duration;

inline void ARM_BUTTON_TIMEOUT(uint32_t dur) {
    button_timeout_start = HAL_Timer_Get_Milli_Seconds();
    button_timeout_duration = dur;
    BUTTON_WD_DEBUG("Button WD Set %d",(dur));
}
inline bool IS_BUTTON_TIMEOUT() {
    return button_timeout_duration && ((HAL_Timer_Get_Milli_Seconds()-button_timeout_start)>button_timeout_duration);
}

inline void CLR_BUTTON_TIMEOUT() {
    button_timeout_duration = 0;
    BUTTON_WD_DEBUG("Button WD Cleared, was %d",button_timeout_duration);
}

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static volatile uint32_t TimingLED;
static volatile uint32_t TimingIWDGReload;

/**
 * When non-zero, causes the system to play out a count on the LED.
 * For even values, the system displays a bright color for a short period,
 * while for odd values the system displays a dim color for a longer period.
 * The value is then decremented and the process repeated until 0.
 */
static volatile uint8_t SYSTEM_LED_COUNT;

//RGBLEDState led_state;

/**
 * KNowing the current listen mode isn't sufficient to determine the correct action (since that may or may not have changed)
 * we need also to know the listen mode at the time the button was pressed.
 */
static volatile bool wasListeningOnButtonPress;
/**
 * The lower 16-bits of the time when the button was first pressed.
 */
static volatile uint16_t pressed_time;

uint16_t system_button_pushed_duration(uint8_t button, void*)
{
    if (button || network.listening())
        return 0;
    return pressed_time ? HAL_Timer_Get_Milli_Seconds()-pressed_time : 0;
}

static volatile uint8_t button_final_clicks = 0;
static uint8_t button_current_clicks = 0;

void reset_button_click()
{
    const uint8_t clicks = button_current_clicks;
    button_current_clicks = 0;
    CLR_BUTTON_TIMEOUT();
    if (clicks > 0) {
        system_notify_event(button_final_click, clicks);
        button_final_clicks = clicks;
    }
}

void handle_button_click(uint16_t depressed_duration)
{
    bool reset = true;
    if (depressed_duration < 30) { // Likely a spurious click due to switch bouncing
        reset = false;
    } else if (depressed_duration < 500) { // a short button press
        button_current_clicks++;
        if (button_current_clicks < 5) { // 5 clicks "ought to be enough for anybody"
            // If next button click doesn't come within 1 second, declare a
            // final button click.
            ARM_BUTTON_TIMEOUT(1000);
            reset = false;
        }
        system_notify_event(button_click, button_current_clicks);
    }
    if (reset) {
        reset_button_click();
    }
}

// this is called on multiple threads - ideally need a mutex
void HAL_Notify_Button_State(uint8_t button, uint8_t pressed)
{
    if (button==0)
    {
        if (pressed)
        {
            wasListeningOnButtonPress = network.listening();
            pressed_time = HAL_Timer_Get_Milli_Seconds();
            if (!wasListeningOnButtonPress)             // start of button press
            {
                system_notify_event(button_status, 0);
            }
        }
        else
        {
            int release_time = HAL_Timer_Get_Milli_Seconds();
            uint16_t depressed_duration = release_time - pressed_time;

            if (!network.listening()) {
                system_notify_event(button_status, depressed_duration);
                handle_button_click(depressed_duration);
            }
            pressed_time = 0;
            if (depressed_duration>3000 && depressed_duration<8000 && wasListeningOnButtonPress && network.listening())
                network.listen(true);
        }
    }
}

inline bool system_led_override()
{
	return SYSTEM_LED_COUNT;
}

/*******************************************************************************
 * Function Name  : HAL_SysTick_Handler
 * Description    : Decrements the various Timing variables related to SysTick.
 * Input          : None
 * Output         : None.
 * Return         : None.
 ************************************************
 *******************************/
extern "C" void HAL_SysTick_Handler(void)
{
#if 0
    if (LED_RGB_IsOverRidden() && !system_led_override())
    {
#ifndef SPARK_NO_CLOUD
        if (LED_Spark_Signal != 0)
        {
            LED_Signaling_Override();
        }
#endif
    }
    else if (TimingLED != 0x00)
    {
        TimingLED--;
    }
    else if(SPARK_FLASH_UPDATE || Spark_Error_Count || network.listening())
    {
        //Do nothing
    }
    else if (SYSTEM_POWEROFF)
    {
        LED_SetRGBColor(RGB_COLOR_GREY);
        LED_On(LED_RGB);
    }
    else if (SYSTEM_LED_COUNT)
    {
		SPARK_LED_FADE = 0;
		if (SYSTEM_LED_COUNT==255)
		{
			// hold the LED on this color until the actual number of bars is set
  			LED_SetRGBColor(0<<16 | 10<<8 | 0);
    	        LED_On(LED_RGB);
    			TimingLED = 100;
		}
		else if (SYSTEM_LED_COUNT & 128)
		{
			LED_SetRGBColor(0<<16 | 10<<8 | 0);
			LED_On(LED_RGB);
			TimingLED = 1000;
			SYSTEM_LED_COUNT &= ~128;
		}
		else
		{
			--SYSTEM_LED_COUNT;
			if (SYSTEM_LED_COUNT==0)
			{
				led_state.restore();
			}
			else if (!(SYSTEM_LED_COUNT&1))
			{
				LED_SetRGBColor(0<<16 | 255<<8 | 0);
				LED_On(LED_RGB);
				TimingLED = 40;
			}
			else
			{
				LED_SetRGBColor(0<<16 | 10<<8 | 0);
				LED_On(LED_RGB);
				TimingLED = SYSTEM_LED_COUNT==1 ? 750 : 350;
			}
		}
    }
    else if(SPARK_LED_FADE && (!SPARK_CLOUD_CONNECTED || system_cloud_active()))
    {
        LED_Fade(LED_RGB);
        TimingLED = 20;//Breathing frequency kept constant
    }
    else if(SPARK_CLOUD_CONNECTED)
    {
        LED_SetRGBColor(system_mode()==SAFE_MODE ? RGB_COLOR_MAGENTA : RGB_COLOR_CYAN);
        LED_On(LED_RGB);
        SPARK_LED_FADE = 1;
    }
    else
    {
        LED_Toggle(LED_RGB);
        if(SPARK_CLOUD_SOCKETED || ( network.connected() && !network.ready()))
            TimingLED = 50;         //50ms
        else
            TimingLED = 100;        //100ms
    }

    if(SPARK_FLASH_UPDATE)
    {
#ifndef SPARK_NO_CLOUD
        if (TimingFlashUpdateTimeout >= TIMING_FLASH_UPDATE_TIMEOUT)
        {
            //Reset is the only way now to recover from stuck OTA update
            HAL_Core_System_Reset();
        }
        else
        {
            TimingFlashUpdateTimeout++;
        }
#endif
    }
    else if(network.listening() && HAL_Core_Mode_Button_Pressed(10000))
    {
        network.listen_command();
    }
    // determine if the button press needs to change the state (and hasn't done so already))
    else if(!network.listening() && HAL_Core_Mode_Button_Pressed(3000) && !wasListeningOnButtonPress)
    {
        network.connect_cancel(true);
        // fire the button event to the user, then enter listening mode (so no more button notifications are sent)
        // there's a race condition here - the HAL_notify_button_state function should
        // be thread safe, but currently isn't.
        HAL_Notify_Button_State(0, false);
        network.listen();
        HAL_Notify_Button_State(0, true);
    }

#ifdef IWDG_RESET_ENABLE
    if (TimingIWDGReload >= TIMING_IWDG_RELOAD)
    {
        TimingIWDGReload = 0;

        /* Reload WDG counter */
        HAL_Notify_WDT();
        DECLARE_SYS_HEALTH(CLEARED_WATCHDOG);
    }
    else
    {
        TimingIWDGReload++;
    }
#endif

    if (IS_BUTTON_TIMEOUT())
    {
        reset_button_click();
    }
#endif
}

void manage_safe_mode()
{
#if 0
    uint16_t flag = (HAL_Bootloader_Get_Flag(BOOTLOADER_FLAG_STARTUP_MODE));
    if (flag != 0xFF) { // old bootloader
        if (flag & 1) {
            set_system_mode(SAFE_MODE);
            // explicitly disable multithreading
            system_thread_set_state(spark::feature::DISABLED, NULL);
            uint8_t value = 0;
            system_get_flag(SYSTEM_FLAG_STARTUP_SAFE_LISTEN_MODE, &value, nullptr);
            if (value)
            {
                // disable logging so that it doesn't interfere with serial output
                set_logger_output(nullptr, NO_LOG_LEVEL);
                system_set_flag(SYSTEM_FLAG_STARTUP_SAFE_LISTEN_MODE, 0, 0);
                // flag listening mode
                network_listen(0, 0, 0);
            }
        }
    }
#endif
}

void app_loop(bool threaded)
{
#if 0
    static uint8_t INTOROBOT_WIRING_APPLICATION = 0;
    if ((INTOROBOT_WIRING_APPLICATION != 1))
    {
        //Execute user application setup only once
        setup();
        INTOROBOT_WIRING_APPLICATION = 1;
        //_post_loop();
    }

    //Execute user application loop
    loop();
    //_post_loop();
#endif
#if 1
    DECLARE_SYS_HEALTH(ENTERED_WLAN_Loop);
    if (!threaded)
        IntoRobot_Idle();

    static uint8_t INTOROBOT_WIRING_APPLICATION = 0;
    if(threaded || SPARK_WLAN_SLEEP || !intorobot_cloud_flag_auto_connect() || intorobot_cloud_flag_connected() || INTOROBOT_WIRING_APPLICATION || (system_mode()!=AUTOMATIC))
    {
        if(threaded || !SPARK_FLASH_UPDATE)
        {
            if ((INTOROBOT_WIRING_APPLICATION != 1))
            {
                //Execute user application setup only once
                DECLARE_SYS_HEALTH(ENTERED_Setup);
                if (system_mode()!=SAFE_MODE)
                    setup();
                INTOROBOT_WIRING_APPLICATION = 1;
                _post_loop();
            }

            //Execute user application loop
            DECLARE_SYS_HEALTH(ENTERED_Loop);
            if (system_mode()!=SAFE_MODE) {
                loop();
                DECLARE_SYS_HEALTH(RAN_Loop);
                _post_loop();
            }
        }
    }
#endif
}


#if PLATFORM_THREADING
// This is the application loop ActiveObject.
void app_thread_idle()
{
    DEBUG_D("app_thread_idle\r\n");
    app_loop(true);
}

// don't wait to get items from the queue, so the application loop is processed as often as possible
// timeout after attempting to put calls into the application queue, so the system thread does not deadlock  (since the application may also
// be trying to put events in the system queue.)
ActiveObjectCurrentThreadQueue ApplicationThread(ActiveObjectConfiguration(app_thread_idle,
		0, /* take time */
		5000, /* put time */
		20 /* queue size */));

#endif

/*******************************************************************************
 * Function Name  : main.
 * Description    : main routine.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void app_setup_and_loop(void)
{
    HAL_Core_Init();
    // We have running firmware, otherwise we wouldn't have gotten here
    DECLARE_SYS_HEALTH(ENTERED_Main);

    DEBUG("welcome from IntoRobot!\r\n");
    String s = intorobot_deviceID();
    INFO("Device %s started", s.c_str());

    manage_safe_mode();

#if defined (START_DFU_FLASHER_SERIAL_SPEED) || defined (START_YMODEM_FLASHER_SERIAL_SPEED)
    USB_USART_LineCoding_BitRate_Handler(system_lineCodingBitRateHandler);
#endif

    bool threaded = system_thread_get_state(NULL) != intorobot::feature::DISABLED &&
        (system_mode()!=SAFE_MODE);

    Network_Setup(threaded);

#if PLATFORM_THREADING
    if (threaded)
    {
        SystemThread.start();
        ApplicationThread.start();
    }
    else
    {
        SystemThread.setCurrentThread();
        ApplicationThread.setCurrentThread();
    }
#endif
    if(!threaded) {
        DEBUG_D("threaded=%d\r\n", threaded);
        /* Main loop */
        while (1) {
            app_loop(false);
        }
    }
}

#ifdef USE_FULL_ASSERT

/*******************************************************************************
 * Function Name  : assert_failed
 * Description    : Reports the name of the source file and the source line number
 *                  where the assert_param error has occurred.
 * Input          : - file: pointer to the source file name
 *                  - line: assert_param error line source number
 * Output         : None
 * Return         : None
 *******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif
