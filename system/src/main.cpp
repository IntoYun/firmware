/**
 ******************************************************************************
 * @file    main.cpp

 * @version V1.0.0
 * @date    13-March-2013
 *
 * Updated: 14-Feb-2014 David Sidrane <david_s5@usa.net>
 *
 * @brief   Main program body.
 ******************************************************************************
 Copyright (c) 2013-2015 IntoRobot Industries, Inc.  All rights reserved.

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
#include "system_rgbled.h"
#include "params_hal.h"
#include "wiring_interrupts.h"
#include "wiring_cellular.h"
#include "wiring_cellular_printable.h"
#include "system_version.h"
#include "system_ex_usermode.h"
#include "system_product.h"

using namespace intorobot;

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
void app_loop(void)
{
    static bool first_time = true;
    if (first_time == true){
        setup();
        first_time = false;
    }
    loop();

    #if 0
    DECLARE_SYS_HEALTH(ENTERED_WLAN_Loop);

    static uint8_t INTOROBOT_WIRING_APPLICATION = 0;
    if ((INTOROBOT_WIRING_APPLICATION != 1))
    {
        //Execute user application setup only once
        DECLARE_SYS_HEALTH(ENTERED_Setup);
        if (system_mode()!=SAFE_MODE)
            setup();
        INTOROBOT_WIRING_APPLICATION = 1;
        _post_loop();
    }

    // Execute user application loop
    DECLARE_SYS_HEALTH(ENTERED_Loop);
    if (system_mode()!=SAFE_MODE) {
        loop();
        DECLARE_SYS_HEALTH(RAN_Loop);
        _post_loop();
    }
    #endif
}

static void load_system_fwlib_version(void)
{
    //保存固件库版本号
    char fw_ver1[32] = {0}, fw_ver2[32] = {0};
    system_version(fw_ver1);
    HAL_PARAMS_Get_System_fwlib_ver(fw_ver2, sizeof(fw_ver2));
    if(strcmp(fw_ver1, fw_ver2))
    {
        HAL_PARAMS_Set_System_fwlib_ver(fw_ver1);
    }
    HAL_PARAMS_Save_Params();
}

/*******************************************************************************
 * Function Name  : main.
 * Description    : main routine.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void app_setup_and_loop_initial(void)
{
    HAL_Core_Init();
    // We have running firmware, otherwise we wouldn't have gotten here
    DECLARE_SYS_HEALTH(ENTERED_Main);
    // load params
    load_system_fwlib_version();

    set_system_mode(DEFAULT);

    DEBUG_D("welcome from IntoRobot!\r\n");

#if defined (START_DFU_FLASHER_SERIAL_SPEED) || defined (START_YMODEM_FLASHER_SERIAL_SPEED)
#ifdef configHAL_USB_CDC_ENABLE
    USB_USART_LineCoding_BitRate_Handler(system_lineCodingBitRateHandler);
#endif
#endif

#ifdef configSETUP_ENABLE
    manage_setup_config();
#endif

#if 1
    product_details_t product_details;
    system_product_instance().get_product_details(product_details);
    DEBUG_D("product_firmware_version=%d\r\n", product_details .product_firmware_version);
    DEBUG_D("product_id=%s\r\n", product_details.product_id);
    DEBUG_D("product_secret=%s\r\n", product_details.product_secret);
    DEBUG_D("platform_id=%s\r\n", product_details.platform_id);
    DEBUG_D("platform_name=%s\r\n", product_details.platform_name);
    DEBUG_D("product_button_pressed=%d\r\n", product_details.product_button_pressed);
    DEBUG_D("product_button_pin=%d\r\n", product_details.product_button_pin);
    DEBUG_D("product_button_pressed_time=%d\r\n", product_details.product_button_pressed_time);
    DEBUG_D("product_indicator_pin=%d\r\n", product_details.product_indicator_pin);
    DEBUG_D("product_indicator_period=%d\r\n", product_details.product_indicator_period);
#endif

    NEWORK_FN(Network_Setup(), (void)0);

#if PLATFORM_THREADING
    create_system_task();
#else
    HAL_Core_Set_System_Loop_Handler(&system_process_loop);
#endif

}

/*******************************************************************************
 * Function Name  : main.
 * Description    : main routine.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void app_setup_and_loop(void)
{
    app_setup_and_loop_initial();
    /* Main loop */
    while (1) {
        app_loop();
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
