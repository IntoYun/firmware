/**
 ******************************************************************************
 * @file     : main.cpp
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

  
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "debug.h"
#include "stm32_it.h"
#include "ros.h"

extern "C" {
#include "usb_conf.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "usb_prop.h"


}

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t  USART_Rx_Buffer[USART_RX_DATA_SIZE];
uint32_t USART_Rx_ptr_in = 0;
uint32_t USART_Rx_ptr_out = 0;
uint32_t USART_Rx_length  = 0;

uint8_t USB_Rx_Buffer[VIRTUAL_COM_PORT_DATA_SIZE];
uint16_t USB_Rx_length = 0;
uint16_t USB_Rx_ptr = 0;

uint8_t  USB_Tx_State = 0;
uint8_t  USB_Rx_State = 0;

uint32_t USB_USART_BaudRate = 9600;

volatile uint32_t TimingIWDGReload;

static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len);

/* Extern variables ----------------------------------------------------------*/
extern LINE_CODING linecoding;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
 * Function Name  : USB_USART_Init
 * Description    : Start USB-USART protocol.
 * Input          : baudRate.
 * Return         : None.
 *******************************************************************************/
void USB_USART_Init(uint32_t baudRate)
{
    linecoding.bitrate = baudRate;
    USB_Disconnect_Config();
    USB_Cable_Config(DISABLE);
    Delay_Microsecond(100000);
    Set_USBClock();
    USB_Interrupts_Config();
    USB_Init();
}

/*******************************************************************************
 * Function Name  : USB_USART_Available_Data.
 * Description    : Return the length of available data received from USB.
 * Input          : None.
 * Return         : Length.
 *******************************************************************************/
uint8_t USB_USART_Available_Data(void)
{
    if(bDeviceState == CONFIGURED)
    {
        if(USB_Rx_State == 1)
        {
            return (USB_Rx_length - USB_Rx_ptr);
        }
    }
    return 0;
}

/*******************************************************************************
 * Function Name  : USB_USART_Receive_Data.
 * Description    : Return data sent by USB Host.
 * Input          : None
 * Return         : Data.
 *******************************************************************************/
int32_t USB_USART_Receive_Data(void)
{
    if(bDeviceState == CONFIGURED)
    {
        if(USB_Rx_State == 1)
        {
            if((USB_Rx_length - USB_Rx_ptr) == 1)
            {
                USB_Rx_State = 0;
                /* Enable the receive of data on EP3 */
                SetEPRxValid(ENDP3);
            }
            return USB_Rx_Buffer[USB_Rx_ptr++];
        }
    }
    return -1;
}

/*******************************************************************************
 * Function Name  : USB_USART_Send_Data.
 * Description    : Send Data from USB_USART to USB Host.
 * Input          : Data.
 * Return         : None.
 *******************************************************************************/
void USB_USART_Send_Data(uint8_t Data)
{
	if(bDeviceState == CONFIGURED)
	{
		USART_Rx_Buffer[USART_Rx_ptr_in] = Data;
		USART_Rx_ptr_in++;
		/* To avoid buffer overflow */
		if(USART_Rx_ptr_in == USART_RX_DATA_SIZE)
		{
			USART_Rx_ptr_in = 0;
		}
		Delay_Microsecond(100);
    }
}

/*******************************************************************************
 * Function Name  : Handle_USBAsynchXfer.
 * Description    : send data to USB.
 * Input          : None.
 * Return         : None.
 *******************************************************************************/
void Handle_USBAsynchXfer (void)
{
	uint16_t USB_Tx_ptr;
	uint16_t USB_Tx_length;

	if(USB_Tx_State != 1)
	{
		if (USART_Rx_ptr_out == USART_RX_DATA_SIZE)
		{
			USART_Rx_ptr_out = 0;
		}

		if(USART_Rx_ptr_out == USART_Rx_ptr_in)
		{
			USB_Tx_State = 0;
			return;
		}

		if(USART_Rx_ptr_out > USART_Rx_ptr_in) /* rollback */
		{
			USART_Rx_length = USART_RX_DATA_SIZE - USART_Rx_ptr_out;
		}
		else
		{
			USART_Rx_length = USART_Rx_ptr_in - USART_Rx_ptr_out;
		}

		if (USART_Rx_length > VIRTUAL_COM_PORT_DATA_SIZE)
		{
			USB_Tx_ptr = USART_Rx_ptr_out;
			USB_Tx_length = VIRTUAL_COM_PORT_DATA_SIZE;

			USART_Rx_ptr_out += VIRTUAL_COM_PORT_DATA_SIZE;
			USART_Rx_length -= VIRTUAL_COM_PORT_DATA_SIZE;
		}
		else
		{
			USB_Tx_ptr = USART_Rx_ptr_out;
			USB_Tx_length = USART_Rx_length;

			USART_Rx_ptr_out += USART_Rx_length;
			USART_Rx_length = 0;
		}
		USB_Tx_State = 1;
		UserToPMABufferCopy(&USART_Rx_Buffer[USB_Tx_ptr], ENDP1_TXADDR, USB_Tx_length);
		SetEPTxCount(ENDP1, USB_Tx_length);
		SetEPTxValid(ENDP1);
	}
}

/*******************************************************************************
 * Function Name  : Get_SerialNum.
 * Description    : Create the serial number string descriptor.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void Get_SerialNum(void)
{
	uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

	Device_Serial0 = *(uint32_t*)ID1;
	Device_Serial1 = *(uint32_t*)ID2;
	Device_Serial2 = *(uint32_t*)ID3;

	Device_Serial0 += Device_Serial2;

	if (Device_Serial0 != 0)
	{
		IntToUnicode (Device_Serial0, &Virtual_Com_Port_StringSerial[2] , 8);
		IntToUnicode (Device_Serial1, &Virtual_Com_Port_StringSerial[18], 4);
	}
}

/*******************************************************************************
 * Function Name  : HexToChar.
 * Description    : Convert Hex 32Bits value into char.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len)
{
	uint8_t idx = 0;

	for( idx = 0 ; idx < len ; idx ++)
	{
		if( ((value >> 28)) < 0xA )
		{
			pbuf[ 2* idx] = (value >> 28) + '0';
		}
		else
		{
			pbuf[2* idx] = (value >> 28) + 'A' - 10;
		}
		value = value << 4;
		pbuf[ 2* idx + 1] = 0;
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


static volatile system_tick_t system_1ms_tick = 0;

/*********************************************************************************
  *Function		:       
  *Description	:    main
  *Input		      :    
  *Output		:         
  *Return		:        
  *author		:        
  *date			:           
  *Others		:         
**********************************************************************************/
void System1MsTick(void)
{
    system_1ms_tick++;  
    
#ifdef IWDG_RESET_ENABLE
	if (TimingIWDGReload >= TIMING_IWDG_RELOAD)
	{
		TimingIWDGReload = 0;
		/* Reload WDG counter */
		KICK_WDT();
	}
	else
	{
		TimingIWDGReload++;
	}
#endif
}

/*********************************************************************************
  *Function		:    system_tick_t GetSystem1MsTick(void)   
  *Description	:    get system 1ms tick
  *Input		      :    
  *Output		:         
  *Return		:        
  *author		:        
  *date			:           
  *Others		:         
**********************************************************************************/
system_tick_t GetSystem1MsTick(void)
{
    return system_1ms_tick;
}

/*********************************************************************************
  *Function		:       
  *Description	:    main
  *Input		      :    
  *Output		:         
  *Return		:        
  *author		:        
  *date			:           
  *Others		:         
**********************************************************************************/
int main(void)
{
    //stm32 初始化
    init();
    //应用初始化
    intorobot_init();
    //打开TIME1 定时器
    Wiring_TIM1_Interrupt_Handler = intorobot_loop;
    TIM_Cmd(TIM1, ENABLE);

    while(1)
    {

#ifdef INTOROBOT_APP_ENABLE
        static uint8_t setup_done=0;
#ifdef INTOROBOT_WLAN_ENABLE
		//if(!intorobot_cloud_connect_flag || intorobot_cloud_connected_flag )
		{
#endif
            //Execute setup only once
            if((0 == setup_done) && (NULL != setup))
            {
                setup();
                setup_done=1;
            }
            //Execute loop
            if(NULL != loop)
            {
                loop();
            }
#ifdef INTOROBOT_WLAN_ENABLE
        }
#endif
#endif 
    }

}

void debug_output_(const char *pdebug)
{
	SerialUSB.print(pdebug);
}

