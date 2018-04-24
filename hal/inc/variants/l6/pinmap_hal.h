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
#define TOTAL_PINS             51
#define FIRST_DIGITAL_PIN      0
#define TOTAL_DIGITAL_PINS     9
#define FIRST_ANALOG_PIN       30
#define TOTAL_ANALOG_PINS      12

//数字接口
#define PA9   0
#define PA10  1
#define PA11  2
#define PA12  3
#define PA13  4
#define PA14  5
#define PB2   6
#define PB10  7
#define PB11  8

//数字模拟接口
#define PA0   30
#define PA1   31
#define PA2   32
#define PA3   33
#define PA4   34
#define PA5   35
#define PA6   36
#define PA7   37
#define PB12  38
#define PB13  39
#define PB14  40
#define PB15  41

// lora pins
#define SX1278_RESET    42
#define SX1278_NSS      43
#define SX1278_DIO0     44
#define SX1278_DIO1     45
#define SX1278_DIO2     46
#define SX1278_DIO3     47
#define SX1278_DIO4     48
#define SX1278_DIO5     49
#define SX1278_RXTX     50
#define SX1278_BATTERY_POWER    PA5

// Serial pins
#define TX    PA9
#define RX    PA10

#define TXD   TX
#define RXD   RX

// Serial1 pins
#define TX1   PA2
#define RX1   PA3

#define TXD1  TX1
#define RXD1  RX1

// Serial2 pins
#define TX2   PB10
#define RX2   PB11

#define TXD2  TX2
#define RXD2  RX2

//SPI pins
#define SS    PB12
#define SCK   PB13
#define MISO  PB14
#define MOSI  PB15

// I2C pins
#define SDA   PB10
#define SCL   PB11

// DAC pins
#define DAC   PA4
#define DAC1  PA5

#define LED_USER        PA14
#define LED_BUILTIN     LED_USER

#define TOTAL_USARTS    3
#define TOTAL_SPIS      1
#define TOTAL_I2CS      1

#define LSBFIRST        0
#define MSBFIRST        1


/*************************************************************/
/*defined for firmware test only, not for users*/
#define INTOROBOT_TEST_PIN_DIGITAL          PB2
#define INTOROBOT_TEST_PIN_ADC              PA2


#ifdef __cplusplus
}
#endif

#endif  /* PINMAP_HAL_H_ */
