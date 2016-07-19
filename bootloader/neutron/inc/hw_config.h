/**
 ******************************************************************************
 * @file    hw_config.h
 * @author  Satish Nair, Zachary Crockett and Mohit Bhoite
 * @version V1.0.0
 * @date    13-March-2013
 * @brief   Hardware Configuration & Setup
 ******************************************************************************
  Copyright (c) 2013-2015 Particle Industries, Inc.  All rights reserved.

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
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H
/* Includes ------------------------------------------------------------------*/

#include "variant.h"
#include "rgbled.h"
#include "esp8266.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
typedef enum
{
	FAC_BUTTON = 0
} Button_TypeDef;

typedef enum
{
	BUTTON_MODE_GPIO = 0, BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

#define BUTTON_PRESSED                      0x00

#define UI_TIMER_FREQUENCY					100							/* 100Hz -> 10ms */
#define BUTTON_DEBOUNCE_INTERVAL			1000 / UI_TIMER_FREQUENCY


/* Exported constants --------------------------------------------------------*/
/* Exported macros ------------------------------------------------------------*/
#define INTERNAL_FLASH_START            ((uint32_t)0x08000000)
#define INTERNAL_FLASH_SIZE             (0x80000)
#define INTERNAL_FLASH_END_ADDRESS      ((uint32_t)INTERNAL_FLASH_START+INTERNAL_FLASH_SIZE)	//For 512KB Internal Flash

#define USB_DFU_ADDRESS                 INTERNAL_FLASH_START
/* #define CORE_FW_ADDRESS                 ((uint32_t)0x08005000) */
#define CORE_FW_ADDRESS                 ((uint32_t)0x08020000)


/* Exported functions ------------------------------------------------------- */
void Set_System(void);
void SystemClock_Config(void);

void USBD_DFU_Init(void);
void USBD_CDC_Init(void);

void RTC_Init(void);
void UI_Timer_Configure(void);
void BUTTON_Init(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint8_t BUTTON_GetState(Button_TypeDef Button);

void GPIO_ESP8266_Init(void);
void Esp8266_Reset(void);
void Esp8266_Enter_UpdateMode(void);

void Init_BootParams(void);
void Load_BootParams(void);
void Save_BootParams(void);

void FLASH_Erase(void);
bool FLASH_Restore(Firmware_TypeDef FmType);

bool FACTORY_Flash_Reset(void);
bool DEFAULT_Flash_Reset(void);
bool OTA_Flash_Reset(void);

uint16_t Bootloader_Get_Version(void);
void Bootloader_Update_Version(uint16_t bootloaderVersion);


system_tick_t millis(void);
void delay(uint32_t ms);

void RestoreEsp8266(void);


void param_info_reset(void);
#ifdef __cplusplus
}
#endif


#endif  /*__HW_CONFIG_H*/
