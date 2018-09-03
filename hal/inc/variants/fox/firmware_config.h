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

#ifndef FIRMWARE_CONFIG_H_
#define FIRMWARE_CONFIG_H_

/* config hal function. */
#define FIRMWARE_CONFIG_HAL_USB_CDC_ENABLE 1
//#define FIRMWARE_CONFIG_HAL_USB_HID_ENABLE

/* config wiring communication type. */
//#define FIRMWARE_CONFIG_WIRING_WIFI_ENABLE
#define FIRMWARE_CONFIG_WIRING_CELLULAR_ENABLE
//#define FIRMWARE_CONFIG_WIRING_LORA_ENABLE
//#define FIRMWARE_CONFIG_WIRING_IPV6_ENABLE

/* config wiring usb serial. */
#define FIRMWARE_CONFIG_WIRING_USBSERIAL_ENABLE
//#define FIRMWARE_CONFIG_WIRING_USBMOUSE_ENABLE
//#define FIRMWARE_CONFIG_WIRING_USBKEYBOARD_ENABLE

/* config wiring usart serial. */
#define FIRMWARE_CONFIG_WIRING_USARTSERIAL_ENABLE
//#define FIRMWARE_CONFIG_WIRING_USARTSERIAL1_ENABLE
//#define FIRMWARE_CONFIG_WIRING_USARTSERIAL2_ENABLE
//#define FIRMWARE_CONFIG_WIRING_USARTSERIAL3_ENABLE
//#define FIRMWARE_CONFIG_WIRING_USARTSERIAL4_ENABLE
//#define FIRMWARE_CONFIG_WIRING_USARTSERIAL5_ENABLE

/* config wring wire */
#define FIRMWARE_CONFIG_WIRING_WIRE_ENABLE
//#define FIRMWARE_CONFIG_WIRING_WIRE1_ENABLE
//#define FIRMWARE_CONFIG_WIRING_WIRE2_ENABLE

/* config wring spi */
#define FIRMWARE_CONFIG_WIRING_SPI_ENABLE
#define FIRMWARE_CONFIG_WIRING_SPI1_ENABLE
//#define FIRMWARE_CONFIG_WIRING_SPI2_ENABLE

/* config wiring i2s. */
#define FIRMWARE_CONFIG_WIRING_I2S_ENABLE

/* config wiring can. */
//#define FIRMWARE_CONFIG_WIRING_CAN_ENABLE
//#define FIRMWARE_CONFIG_WIRING_CAN1_ENABLE

/* config setup */
#define FIRMWARE_CONFIG_SETUP_ENABLE
#define FIRMWARE_CONFIG_SETUP_USBSERIAL_ENABLE
#define FIRMWARE_CONFIG_SETUP_USARTSERIAL_ENABLE
//#define FIRMWARE_CONFIG_SETUP_TCP_ENABLE
//#define FIRMWARE_CONFIG_SETUP_UDP_ENABLE

/* config network */
//#define FIRMWARE_CONFIG_SYSTEM_NETWORK

/* config cloud */
//#define FIRMWARE_CONFIG_SYSTEM_CLOUD

/* config cloud */
#define FIRMWARE_CONFIG_SYSTEM_LORAWAN

/* config rgb ui */
//#define FIRMWARE_CONFIG_SYSTEM_RGB

/* config setup button ui */
//#define FIRMWARE_CONFIG_SYSTEM_BUTTON


#define FIRMWARE_CONFIG_TIM_PWM_FREQ                      500 //500Hz
#define FIRMWARE_CONFIG_SERVO_TIM_PWM_FREQ                50  //50Hz


#endif /* FIRMWARE_CONFIG_H */

