#include <stdint.h>
#include "lib_system_all.h"

USBD_HandleTypeDef USBD_Device;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USB_RX_BUFFER_SIZE  256
#define RX_BUFFER_SIZE  128

uint8_t USBRxBuffer[USB_RX_BUFFER_SIZE];/* Received Data over USB are stored in this buffer */
uint32_t USBRxBufPtrIn = 0;
uint32_t USBRxBufPtrOut = 0;

uint8_t RxBuffer[RX_BUFFER_SIZE];/* Ready Data for USB are stored in this buffer */
uint32_t RxBuffLength;


static osMutexId usb_mutex;	//transfer all mutex


/*******************************************************************************
 * Function Name  : USB_USART_Init
 * Description    : Start USB-USART protocol.
 * Input          : baudRate.
 * Return         : None.
 *******************************************************************************/
void USB_USART_Init(uint32_t baudRate)
{

	//创建 usb mutex
  osMutexDef(USB_MUT);
  usb_mutex = osMutexCreate(osMutex(USB_MUT));

    LineCoding.bitrate = baudRate;
    /* Init Device Library */
    USBD_Init(&USBD_Device, &VCP_Desc, 0);
    /* Add Supported Class */
    USBD_RegisterClass(&USBD_Device, USBD_CDC_CLASS);
    /* Add CDC Interface Class */
    USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_fops);
    /* Start Device Process */
    USBD_Start(&USBD_Device);
}

/*******************************************************************************
 * Function Name  : USB_USART_Available_Data.
 * Description    : Return the length of available data received from USB.
 * Input          : None.
 * Return         : Length.
 *******************************************************************************/
int32_t USB_USART_Available_Data(void)
{
    return (unsigned int)(USB_RX_BUFFER_SIZE + USBRxBufPtrOut - USBRxBufPtrIn) % USB_RX_BUFFER_SIZE;
}

/*******************************************************************************
 * Function Name  : USB_USART_Read_Data.
 * Description    : Return data sent by USB Host.
 * Input          : None
 * Return         : Data.
 *******************************************************************************/
int32_t USB_USART_Read_Data(void)
{
    // if the head isn't ahead of the tail, we don't have any characters
    if (USBRxBufPtrIn == USBRxBufPtrOut )
    {
        return -1;
    }
    else
    {
        uint8_t c = USBRxBuffer[USBRxBufPtrOut];
        USBRxBufPtrOut = (unsigned int)(USBRxBufPtrOut + 1) % USB_RX_BUFFER_SIZE;
        return c;
    }
}

/*******************************************************************************
 * Function Name  : USB_USART_Send_Data.
 * Description    : Send Data from USB_USART to USB Host.
 * Input          : Data.
 * Return         : None.
 *******************************************************************************/

 
void USB_USART_Send_Data(uint8_t Data)
{

	osMutexWait(usb_mutex, osWaitForever);
	
	USBD_CDC_SetTxBuffer(&USBD_Device, &Data, 1);
	while(USBD_CDC_TransmitPacket(&USBD_Device)!=USBD_OK);

	osMutexRelease(usb_mutex);
	
}



