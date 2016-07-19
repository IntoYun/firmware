/**
 ******************************************************************************
 * @file     : platform_config.h
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
#ifndef   PLATFORM_CONFIG_H_
#define   PLATFORM_CONFIG_H_

#include "stm32f10x.h"


//define switch
//#define INTOROBOT_CLOUD_DEBUG
#define INTOROBOT_CONFIG_ENABLE
#define INTOROBOT_WLAN_ENABLE
#define INTOROBOT_APP_ENABLE
//#define IWDG_RESET_ENABLE
#define SWD_JTAG_DISABLE

#undef  RELEASE_BUILD
#undef  DEBUG_BUILD
//#define DEBUG_BUILD


#if !defined(RELEASE_BUILD) && !defined(DEBUG_BUILD)
//#warning  "Defaulting to Release Build"
#define RELEASE_BUILD
#undef  DEBUG_BUILD
#endif


#if defined(DEBUG_BUILD)
#define DBGMCU_SETTINGS (DBGMCU_CR_DBG_SLEEP|DBGMCU_CR_DBG_STOP|DBGMCU_CR_DBG_STANDBY|DBGMCU_CR_DBG_IWDG_STOP|DBGMCU_CR_DBG_WWDG_STOP)
#else
#define USE_ONLY_PANIC // Define to remove all Logging and only have Panic
#define DBGMCU_SETTINGS (DBGMCU_CR_DBG_IWDG_STOP|DBGMCU_CR_DBG_WWDG_STOP)
#endif

//Device Serial
#define         ID1          (0x1FFFF7E8)
#define         ID2          (0x1FFFF7EC)
#define         ID3          (0x1FFFF7F0)


//usb disconnect pin
#define USB_DISCONNECT_GPIO_PIN           	GPIO_Pin_12
#define USB_DISCONNECT_GPIO_PORT       		GPIOC
#define USB_DISCONNECT_GPIO_CLK		  		RCC_APB2Periph_GPIOC


//NVIC Priorities based on NVIC_PriorityGroup_4
#define USB_LP_IRQ_PRIORITY					0	//USB_LP_CAN1_RX0 Interrupt
#define RTCALARM_IRQ_PRIORITY				1	//RTC Alarm Interrupt
#define RTC_IRQ_PRIORITY					2	//RTC Seconds Interrupt
#define USART2_IRQ_PRIORITY				3	//USART2 Interrupt
#define SYSTICK_IRQ_PRIORITY				4	//CORTEX_M3 Systick Interrupt
#define EXTI0_IRQ_PRIORITY					5	//User Interrupt
#define EXTI1_IRQ_PRIORITY					6	//User Interrupt
#define EXTI2_IRQ_PRIORITY					7	//User Interrupt
#define EXTI3_IRQ_PRIORITY					8	//User Interrupt
#define EXTI4_IRQ_PRIORITY					9	//User Interrupt
#define EXTI9_5_IRQ_PRIORITY				10	//User Interrupt
#define EXTI15_10_IRQ_PRIORITY				11	//User Interrupt
#define SVCALL_IRQ_PRIORITY					12	//CORTEX_M3 SVCall Interrupt
#define PENDSV_IRQ_PRIORITY				13	//CORTEX_M3 PendSV Interrupt
#define TIM1_UP_IRQ_PRIORITY				14	//TIM1_UP Interrupt


#endif /* PLATFORM_CONFIG_H_ */

