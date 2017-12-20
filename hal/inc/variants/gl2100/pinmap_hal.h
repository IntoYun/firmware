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
#ifndef PINMAP_HAL_H_
#define PINMAP_HAL_H_

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include "intorobot_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/

typedef uint16_t pin_t;

typedef enum PinMode {
    INPUT,
    OUTPUT,
    INPUT_PULLUP,
    INPUT_PULLDOWN,
    //Used internally begin
    AF_OUTPUT_PUSHPULL, //Used internally for Alternate Function Output PushPull(TIM, UART, SPI etc)
    AF_OUTPUT_DRAIN,    //Used internally for Alternate Function Output Drain(I2C etc). External pullup resistors required.
    AN_INPUT,           //Used internally for ADC Input
    AN_OUTPUT,          //Used internally for DAC Output
    //Used internally end
    PIN_MODE_NONE=0xFF
} PinMode;

typedef enum {
    PF_NONE,
    PF_DIO,
    PF_TIMER,
    PF_ADC,
    PF_DAC
} PinFunction;

PinFunction HAL_Validate_Pin_Function(pin_t pin, PinFunction pinFunction);

typedef struct STM32_Pin_Info  STM32_Pin_Info;

STM32_Pin_Info* HAL_Pin_Map(void);

/* Exported macros -----------------------------------------------------------*/

/*
* Pin mapping. Borrowed from Wiring
*/
#define TOTAL_PINS             17
#define FIRST_DIGITAL_PIN      0
#define TOTAL_DIGITAL_PINS     17
#define FIRST_ANALOG_PIN       30
#define TOTAL_ANALOG_PINS      0

#define D0  0   //L7 DIO0
#define D1  1   //L7 DIO1
#define D2  2   //L7 DIO2
#define D3  3   //L7 DIO3
#define D4  4   //L7 RXTX
#define D5  5   //L7 RSET
#define D6  6   //transceiver indicator led
#define D7  7   //status indicator led
#define D8  8   //mode pin
#define D9  9   //ver0
#define D10 10  //ver1
#define D11 11
#define D12 12
#define D13 13
#define D14 14
#define D15 15
#define D16 16

// Serial pins
#define TX   D11
#define RX   D12

#define TXD  TX
#define RXD  RX

// SPI pins
#define SS   D13
#define SCK  D14
#define MISO D15
#define MOSI D16

#define LED_USER    D7
#define LED_BUILTIN LED_USER

#define TOTAL_USARTS        1
#define TOTAL_SPIS          1
#define TOTAL_I2CS          0

#define LSBFIRST 0
#define MSBFIRST 1

/*************************************************************/
/*defined for firmware test only, not for users*/
#define INTOROBOT_TEST_PIN_DIGITAL          D0
#define INTOROBOT_TEST_PIN_ADC              A0



#ifdef __cplusplus
}
#endif

#endif  /* PINMAP_HAL_H_ */
