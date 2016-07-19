/**
 ******************************************************************************
 * @file     : main.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef   MAIN_H_
#define   MAIN_H_


#include "variant.h"
#include "intorobot_api.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "hw_config.h"


extern volatile uint32_t TimingIWDGReload;


#define USART_RX_DATA_SIZE			256

void USB_USART_Init(uint32_t baudRate);
uint8_t USB_USART_Available_Data(void);
int32_t USB_USART_Receive_Data(void);
void USB_USART_Send_Data(uint8_t Data);
void Handle_USBAsynchXfer(void);
void Get_SerialNum(void);


void System1MsTick(void);
system_tick_t GetSystem1MsTick();


void setup() __attribute__ ((weak));
void loop() __attribute__ ((weak));

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H_ */

