#ifndef   WIRING_USBSERIAL_HAL_H_
#define   WIRING_USBSERIAL_HAL_H_

#include "stdint.h"
#ifdef __cplusplus
 extern "C" {
#endif

#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"



 void USB_USART_Init(uint32_t baudRate);
 int32_t USB_USART_Available_Data(void);
 int32_t USB_USART_Read_Data(void);
 void USB_USART_Send_Data(uint8_t Data);

#ifdef __cplusplus
 }
#endif



#endif /*WIRING_USBSERIAL_H_*/

