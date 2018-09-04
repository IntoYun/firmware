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

#ifndef FIRMWARE_PLATFORM_CONFIG_H_
#define FIRMWARE_PLATFORM_CONFIG_H_

#define FIRMWARE_CONFIG_HAL_USB_CDC 1
#define FIRMWARE_CONFIG_HAL_USB_HID 0

#define FIRMWARE_CONFIG_WIRING_WIFI 1
#define FIRMWARE_CONFIG_WIRING_CELLULAR 0
#define FIRMWARE_CONFIG_WIRING_LORA 0
#define FIRMWARE_CONFIG_WIRING_IPV6 0
#define FIRMWARE_CONFIG_WIRING_USB_USART 1
#define FIRMWARE_CONFIG_WIRING_USBMOUSE 0
#define FIRMWARE_CONFIG_WIRING_USBKEYBOARD 0
#define FIRMWARE_CONFIG_WIRING_USART 3
#define FIRMWARE_CONFIG_WIRING_WIRE 2
#define FIRMWARE_CONFIG_WIRING_SPI 2
#define FIRMWARE_CONFIG_WIRING_I2S 0
#define FIRMWARE_CONFIG_WIRING_CAN 1

#define FIRMWARE_CONFIG_SYSTEM_SETUP 1
#define FIRMWARE_CONFIG_SYSTEM_SETUP_USB 1
#define FIRMWARE_CONFIG_SYSTEM_SETUP_USART 0
#define FIRMWARE_CONFIG_SYSTEM_SETUP_TCP 1
#define FIRMWARE_CONFIG_SYSTEM_SETUP_UDP 1

#define FIRMWARE_CONFIG_SYSTEM_NETWORK 1
#define FIRMWARE_CONFIG_SYSTEM_CLOUD 1
#define FIRMWARE_CONFIG_SYSTEM_LORAWAN 0
#define FIRMWARE_CONFIG_SYSTEM_RGB 1
#define FIRMWARE_CONFIG_SYSTEM_BUTTON 1

#define FIRMWARE_CONFIG_TIM_PWM_FREQ 500 //500Hz
#define FIRMWARE_CONFIG_SERVO_TIM_PWM_FREQ 50 //50Hz

#define FIRMWARE_CONFIG_LOG_DEFAULT_LEVEL 5

#endif

