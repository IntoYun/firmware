/**
 ******************************************************************************
 * @file    USB_Device/CDC_Standalone/Src/usbd_cdc_interface.c
 * @author  MCD Application Team
 * @version V1.3.2
 * @date    13-November-2015
 * @brief   Source file for USBD CDC interface
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
#include "variant.h"

extern void enter_dfu(bool persist);
extern PCD_HandleTypeDef hpcd;

#define USB_RX_BUFFER_SIZE  256
#define RX_BUFFER_SIZE  128

#ifdef START_DFU_FLASHER_SERIAL_SPEED
static uint32_t start_dfu_flasher_serial_speed = START_DFU_FLASHER_SERIAL_SPEED;
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USBD_CDC_LineCodingTypeDef LineCoding =
{
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* nb. of bits 8*/
};

extern uint8_t USBRxBuffer[USB_RX_BUFFER_SIZE];/* Received Data over USB are stored in this buffer */
extern uint32_t USBRxBufPtrIn;
extern uint32_t USBRxBufPtrOut;

extern uint8_t RxBuffer[RX_BUFFER_SIZE];/* Ready Data for USB are stored in this buffer */
extern uint32_t RxBuffLength;
/* USB handler declaration */
extern USBD_HandleTypeDef  USBD_Device;

/* Private function prototypes -----------------------------------------------*/
static int8_t CDC_Itf_Init(void);
static int8_t CDC_Itf_DeInit(void);
static int8_t CDC_Itf_Control(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Itf_Receive(uint8_t* pbuf, uint32_t *Len);

USBD_CDC_ItfTypeDef USBD_CDC_fops =
{
    CDC_Itf_Init,
    CDC_Itf_DeInit,
    CDC_Itf_Control,
    CDC_Itf_Receive
};

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  CDC_Itf_Init
 *         Initializes the CDC media low layer
 * @param  None
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t CDC_Itf_Init(void)
{
   /*##-5- Set Application Buffers ############################################*/
    USBD_CDC_SetRxBuffer(&USBD_Device, RxBuffer);
    USBD_CDC_ReceivePacket(&USBD_Device);
    return (USBD_OK);
}

/**
 * @brief  CDC_Itf_DeInit
 *         DeInitializes the CDC media low layer
 * @param  None
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t CDC_Itf_DeInit(void)
{
   return (USBD_OK);
}

/**
 * @brief  CDC_Itf_Control
 *         Manage the CDC class requests
 * @param  Cmd: Command code
 * @param  Buf: Buffer containing command data (request parameters)
 * @param  Len: Number of data to be sent (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t CDC_Itf_Control (uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
    switch (cmd)
    {
        case CDC_SEND_ENCAPSULATED_COMMAND:
            /* Add your code here */
            break;
        case CDC_GET_ENCAPSULATED_RESPONSE:
            /* Add your code here */
            break;
        case CDC_SET_COMM_FEATURE:
            /* Add your code here */
            break;
        case CDC_GET_COMM_FEATURE:
            /* Add your code here */
            break;
        case CDC_CLEAR_COMM_FEATURE:
            /* Add your code here */
            break;
        case CDC_SET_LINE_CODING:
#ifdef START_DFU_FLASHER_SERIAL_SPEED
            LineCoding.bitrate    = (uint32_t)(pbuf[0] | (pbuf[1] << 8) |\
                    (pbuf[2] << 16) | (pbuf[3] << 24));
            if (LineCoding.bitrate == start_dfu_flasher_serial_speed)
            {
                //Reset device and briefly enter DFU bootloader mode
                enter_dfu(false);
            }
#endif
            break;
        case CDC_GET_LINE_CODING:
            break;
        case CDC_SET_CONTROL_LINE_STATE:
            /* Add your code here */
            break;
        case CDC_SEND_BREAK:
            /* Add your code here */
            break;
        default:
            break;
    }
    return (USBD_OK);
}

/**
 * @brief  CDC_Itf_DataRx
 *         Data received over USB OUT endpoint are sent over CDC interface
 *         through this function.
 * @param  Buf: Buffer of data to be transmitted
 * @param  Len: Number of data received (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t CDC_Itf_Receive(uint8_t* Buf, uint32_t *Len)
{
    uint32_t Index;

    for(Index=0; Index< *Len; Index++)
    {
        uint32_t i = (uint32_t)(USBRxBufPtrIn + 1) % USB_RX_BUFFER_SIZE;

        if (i != USBRxBufPtrOut)
        {
            USBRxBuffer[USBRxBufPtrIn] = Buf[Index];
            USBRxBufPtrIn = i;
        }
        else
        {
            break;
        }
    }
    USBD_CDC_SetRxBuffer(&USBD_Device, RxBuffer);
    USBD_CDC_ReceivePacket(&USBD_Device);
    return (USBD_OK);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
