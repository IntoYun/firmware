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
#define TOTAL_PINS             45
#define FIRST_DIGITAL_PIN      0
#define TOTAL_DIGITAL_PINS     10
#define FIRST_ANALOG_PIN       30
#define TOTAL_ANALOG_PINS      6


#define D0 0
#define D1 1
#define D2 2
#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7

#define A0 30
#define A1 31
#define A2 32
#define A3 33
#define A4 34
#define A5 35

// lora sx1278 pins
#define SX1278_RESET    36
#define SX1278_NSS      37
#define SX1278_DIO0     38
#define SX1278_DIO1     39
#define SX1278_DIO2     40
#define SX1278_DIO3     41
#define SX1278_DIO4     42
#define SX1278_DIO5     43
#define SX1278_RXTX     44
#define SX1278_BATTERY_POWER    A4

// Serial pins
#define TX     8
#define RX     9

#define TXD    TX
#define RXD    RX

// Serial1 pins
#define TX1    A0
#define RX1    A1

#define TXD1   TX1
#define RXD1   RX1

// Serial2 pins
#define TX2    D0
#define RX2    D1

#define TXD2   TX2
#define RXD2   RX2

//SPI pins
#define SS   D5
#define SCK  D2
#define MISO D3
#define MOSI D4

// I2C pins
#define SDA  D1
#define SCL  D0

// DAC pins
#define DAC1 A4
#define DAC2 A5

#define LED_USER    D7
#define LED_BUILTIN LED_USER

#define TOTAL_USARTS    3
#define TOTAL_SPIS      1
#define TOTAL_I2CS      1

#define LSBFIRST 0
#define MSBFIRST 1

#ifdef __cplusplus
}
#endif

#endif  /* PINMAP_HAL_H_ */
