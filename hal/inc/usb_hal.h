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
#ifndef __USB_HAL_H
#define __USB_HAL_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macros ------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

#include "intorobot_config.h"

#ifdef configHAL_USB_CDC_ENABLE
/**
 * Initialize or deinitialize USB serial
 * @param baudRate  The data rate of the connection. If 0, the connection is
 * uninitialized.
 */
void USB_USART_Initial(uint32_t baudRate);

unsigned int USB_USART_Baud_Rate(void);

void USB_USART_LineCoding_BitRate_Handler(void (*handler)(uint32_t bitRate));

/**
 * Retrieves the number of bytes of data available.
 * @return
 */
int32_t USB_USART_Available_Data(void);

/**
 * Retrieves the number of bytes of data available in the TX buffer.
 * @return
 */
int32_t USB_USART_Available_Data_For_Write(void);

/**
 * Reads data from the input buffer.
 * @param peek  If the data should be peeked reather than fetched.
 * The default, `false` means fetch, where data is removed from the buffer.
 * When `true`, the data byte is left in the buffer.
 * @return
 */
int32_t USB_USART_Receive_Data(uint8_t peek);

/**
 * Sends data to the USB serial.
 * @param Data      The data to write.
 * @return
 */
void USB_USART_Send_Data(uint8_t Data);

/**
 * Flushes TX buffer
 * @return
 */
void USB_USART_Flush_Data(void);

#endif

#ifdef configHAL_USB_HID_ENABLE
void USB_HID_Send_Report(void *pHIDReport, size_t reportSize);
#endif

#ifdef __cplusplus
}
#endif


#endif  /* __USB_HAL_H */
