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
#include "sdkqueue.h"
#include "usbd_core.h"
#include "usbd_cdc_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define APP_RX_DATA_SIZE  2048

extern UART_HandleTypeDef UartHandleA;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USBD_CDC_LineCodingTypeDef LineCoding =
{
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* nb. of bits 8*/
};

extern SDK_QUEUE USART_Rx_Queue;
extern SDK_QUEUE USB_Rx_Queue;

extern uint8_t ESP8266_COM_MODE;

uint8_t UsartRxByte;
uint8_t UserRxBuffer[APP_RX_DATA_SIZE];/* Received Data over USB are stored in this buffer */

/* UART handler declaration */
UART_HandleTypeDef UartHandle;
/* TIM handler declaration */
TIM_HandleTypeDef  TimHandle;
/* USB handler declaration */
extern USBD_HandleTypeDef  USBD_Device;

/* Private function prototypes -----------------------------------------------*/
static int8_t CDC_Itf_Init(void);
static int8_t CDC_Itf_DeInit(void);
static int8_t CDC_Itf_Control(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Itf_Receive(uint8_t* pbuf, uint32_t *Len);

static void Error_Handler(void);
static void ComPort_Config(void);

USBD_CDC_ItfTypeDef USBD_CDC_fops =
{
    CDC_Itf_Init,
    CDC_Itf_DeInit,
    CDC_Itf_Control,
    CDC_Itf_Receive
};

int8_t Uart1_Init(void)
{
    /*##-1- Configure the UART peripheral ######################################*/
    /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
    /* USART configured as follow:
       - Word Length = 8 Bits
       - Stop Bit    = One Stop bit
       - Parity      = No parity
       - BaudRate    = 115200 baud
       - Hardware flow control disabled (RTS and CTS signals) */
    UartHandle.Instance          = USART1;
    UartHandle.Init.BaudRate     = 460800;
    //UartHandle.Init.BaudRate     = 115200;
    UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits     = UART_STOPBITS_1;
    UartHandle.Init.Parity       = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode         = UART_MODE_TX_RX;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

    if(HAL_UART_Init(&UartHandle) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }

    /*##-2- Put UART peripheral in IT reception process ########################*/
    /* Any data received will be stored in "UserTxBuffer" buffer  */
    if(HAL_UART_Receive_IT(&UartHandle, &UsartRxByte, 1) != HAL_OK)
    {
        /* Transfer error in reception process */
        Error_Handler();
    }
    sdkClearQueue(&USART_Rx_Queue);
    return 0;
}

/**
 * @brief  CDC_Itf_Init
 *         Initializes the CDC media low layer
 * @param  None
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t CDC_Itf_Init(void)
{
    /*##-1- Configure the UART peripheral ######################################*/
    /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
    /* USART configured as follow:
       - Word Length = 8 Bits
       - Stop Bit    = One Stop bit
       - Parity      = No parity
       - BaudRate    = 115200 baud
       - Hardware flow control disabled (RTS and CTS signals) */
    UartHandle.Instance          = USART1;
    if(1 == ESP8266_COM_MODE)
    {UartHandle.Init.BaudRate     = 460800;}
    else
    {UartHandle.Init.BaudRate     = 115200;}
    UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits     = UART_STOPBITS_1;
    UartHandle.Init.Parity       = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode         = UART_MODE_TX_RX;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

    if(HAL_UART_Init(&UartHandle) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }

    /*##-2- Put UART peripheral in IT reception process ########################*/
    /* Any data received will be stored in "UserTxBuffer" buffer  */
    if(HAL_UART_Receive_IT(&UartHandle, &UsartRxByte, 1) != HAL_OK)
    {
        /* Transfer error in reception process */
        Error_Handler();
    }

    sdkClearQueue(&USART_Rx_Queue);
    sdkClearQueue(&USB_Rx_Queue);
    /*##-5- Set Application Buffers ############################################*/
    USBD_CDC_SetRxBuffer(&USBD_Device, UserRxBuffer);
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
    /* DeInitialize the UART peripheral */
    if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }
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
            LineCoding.bitrate    = (uint32_t)(pbuf[0] | (pbuf[1] << 8) |\
                    (pbuf[2] << 16) | (pbuf[3] << 24));
            LineCoding.format     = pbuf[4];
            LineCoding.paritytype = pbuf[5];
            LineCoding.datatype   = pbuf[6];
            /* Set the new configuration */
            ComPort_Config();
            break;

        case CDC_GET_LINE_CODING:
            pbuf[0] = (uint8_t)(LineCoding.bitrate);
            pbuf[1] = (uint8_t)(LineCoding.bitrate >> 8);
            pbuf[2] = (uint8_t)(LineCoding.bitrate >> 16);
            pbuf[3] = (uint8_t)(LineCoding.bitrate >> 24);
            pbuf[4] = LineCoding.format;
            pbuf[5] = LineCoding.paritytype;
            pbuf[6] = LineCoding.datatype;
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
 * @brief  Rx Transfer completed callback
 * @param  huart: UART handle
 * @retval None
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    sdkInsertQueue(&USART_Rx_Queue, &UsartRxByte, 1);
    /* testI++; */
    /* HAL_UART_Transmit(&UartHandleA, &testI, 1, 5); */
    /* Start another reception: provide the buffer pointer with offset and the buffer size */
    HAL_UART_Receive_IT(huart, &UsartRxByte, 1);
    //print("1");
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
    sdkInsertQueue(&USB_Rx_Queue, Buf, *Len);
    USBD_CDC_ReceivePacket(&USBD_Device);
    return (USBD_OK);
}


void USBD_CDC_Process(void)
{
    uint8_t TxBuffer[128];
    uint32_t len=0;

    len=0;
    while(!sdkIsQueueEmpty(&USART_Rx_Queue))
    {
        if(len < sizeof(TxBuffer))
        {
            sdkGetQueueData(&USART_Rx_Queue, &TxBuffer[len++]);
        }
        else
        {
            break;
        }
    }
    if(len)
    {
        USBD_CDC_SetTxBuffer(&USBD_Device, TxBuffer, len);
        while(USBD_CDC_TransmitPacket(&USBD_Device) != USBD_OK);
    }

    len=0;
    while(!sdkIsQueueEmpty(&USB_Rx_Queue))
    {
        if(len < sizeof(TxBuffer))
        {
            sdkGetQueueData(&USB_Rx_Queue, &TxBuffer[len++]);
        }
        else
        {
            break;
        }
    }
    if(len)
    {
        HAL_UART_Transmit(&UartHandle, TxBuffer, len, 2000);//2000ms
    }
}

/**
 * @brief  ComPort_Config
 *         Configure the COM Port with the parameters received from host.
 * @param  None.
 * @retval None
 * @note   When a configuration is not supported, a default value is used.
 */
static void ComPort_Config(void)
{
    if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }

    /* set the Stop bit */
    switch (LineCoding.format)
    {
        case 0:
            UartHandle.Init.StopBits = UART_STOPBITS_1;
            break;
        case 2:
            UartHandle.Init.StopBits = UART_STOPBITS_2;
            break;
        default :
            UartHandle.Init.StopBits = UART_STOPBITS_1;
            break;
    }

    /* set the parity bit*/
    switch (LineCoding.paritytype)
    {
        case 0:
            UartHandle.Init.Parity = UART_PARITY_NONE;
            break;
        case 1:
            UartHandle.Init.Parity = UART_PARITY_ODD;
            break;
        case 2:
            UartHandle.Init.Parity = UART_PARITY_EVEN;
            break;
        default :
            UartHandle.Init.Parity = UART_PARITY_NONE;
            break;
    }

    /*set the data type : only 8bits and 9bits is supported */
    switch (LineCoding.datatype)
    {
        case 0x07:
            /* With this configuration a parity (Even or Odd) must be set */
            UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
            break;
        case 0x08:
            if(UartHandle.Init.Parity == UART_PARITY_NONE)
            {
                UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
            }
            else
            {
                UartHandle.Init.WordLength = UART_WORDLENGTH_9B;
            }
            break;
        default :
            UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
            break;
    }
    if(1 == ESP8266_COM_MODE)
    {UartHandle.Init.BaudRate     = 460800;}
    else
    {UartHandle.Init.BaudRate     = LineCoding.bitrate;}
    UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode         = UART_MODE_TX_RX;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

    if(HAL_UART_Init(&UartHandle) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }

    /* Start reception: provide the buffer pointer with offset and the buffer size */
    if(HAL_UART_Receive_IT(&UartHandle, &UsartRxByte, 1) != HAL_OK)
    {
        /* Transfer error in reception process */
        Error_Handler();
    }
}

/**
 * @brief  UART error callbacks
 * @param  UartHandle: UART handle
 * @retval None
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
    /* Transfer error occured in reception and/or transmission process */
    Error_Handler();
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void)
{
    /* Add your own code here */
}
#if 0
void debug(char *pdebug, uint32_t len)
{
   USBD_CDC_SetTxBuffer(&USBD_Device, pdebug, len);
   while(USBD_CDC_TransmitPacket(&USBD_Device) != USBD_OK);
}

void debug1(char *pdebug)
{
   USBD_CDC_SetTxBuffer(&USBD_Device, pdebug, strlen(pdebug));
   while(USBD_CDC_TransmitPacket(&USBD_Device) != USBD_OK);
}
#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
