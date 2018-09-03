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

/* Includes ------------------------------------------------------------------*/
#include "usb_hal.h"
#include <stdint.h>

/* Private typedef -----------------------------------------------------------*/

#ifdef FIRMWARE_CONFIG_HAL_USB_CDC
/*******************************************************************************
 * Function Name  : USB_USART_Init
 * Description    : Start USB-USART protocol.
 * Input          : baudRate.
 * Return         : None.
 *******************************************************************************/
void HAL_USB_USART_Initial(uint32_t baudRate)
{
}

/*******************************************************************************
 * Function Name  : HAL_USB_USART_Available_Data.
 * Description    : Return the length of available data received from USB.
 * Input          : None.
 * Return         : Length.
 *******************************************************************************/
int32_t HAL_USB_USART_Available_Data(void)
{
    return 0;
}

/*******************************************************************************
 * Function Name  : HAL_USB_USART_Receive_Data.
 * Description    : Return data sent by USB Host.
 * Input          : None
 * Return         : Data.
 *******************************************************************************/
int32_t HAL_USB_USART_Receive_Data(uint8_t peek)
{
    return 0;
}

/*******************************************************************************
 * Function Name  : HAL_USB_USART_Available_Data_For_Write.
 * Description    : Return the length of available space in TX buffer
 * Input          : None.
 * Return         : Length.
 *******************************************************************************/
int32_t HAL_USB_USART_Available_Data_For_Write(void)
{
    return 0;
}

/*******************************************************************************
 * Function Name  : HAL_USB_USART_Send_Data.
 * Description    : Send Data from USB_USART to USB Host.
 * Input          : Data.
 * Return         : None.
 *******************************************************************************/
void HAL_USB_USART_Send_Data(uint8_t Data)
{
}

/*******************************************************************************
 * Function Name  : HAL_USB_USART_Flush_Data.
 * Description    : Flushes TX buffer
 * Input          : None.
 * Return         : None.
 *******************************************************************************/
void HAL_USB_USART_Flush_Data(void)
{
}
#endif

#ifdef FIRMWARE_CONFIG_HAL_USB_HID
/*******************************************************************************
 * Function Name : HAL_USB_HID_Send_Report.
 * Description   : Send HID Report Info to Host.
 * Input         : pHIDReport and reportSize.
 * Output        : None.
 * Return value  : None.
 *******************************************************************************/
void HAL_USB_HID_Send_Report(void *pHIDReport, size_t reportSize)
{
}
#endif

uint32_t HAL_USB_USART_Baud_Rate(void)
{
    return 115200;
}

void HAL_USB_USART_LineCoding_BitRate_Handler(void (*handler)(uint32_t bitRate))
{
}

int32_t HAL_USB_USART_Flush_Output(unsigned timeout, void* reserved)
{
    return 0;
}
