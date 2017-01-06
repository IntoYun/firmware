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

#if 0
typedef enum PinMode {
    INPUT,
    OUTPUT,
    INPUT_PULLUP,
    INPUT_PULLDOWN,
    AF_OUTPUT_PUSHPULL, //Used internally for Alternate Function Output PushPull(TIM, UART, SPI etc)
    AF_OUTPUT_DRAIN,    //Used internally for Alternate Function Output Drain(I2C etc). External pullup resistors required.
    AN_INPUT,           //Used internally for ADC Input
    AN_OUTPUT,          //Used internally for DAC Output
    PIN_MODE_NONE=0xFF
} PinMode;
#endif


#if  1
typedef enum PinMode {
        INPUT = 0x1,
        OUTPUT = 0x2,
        PULLUP = 0x4,
        INPUT_PULLUP = 0x5,
        PULLDOWN = 0x08,
        INPUT_PULLDOWN = 0x09,
        OPEN_DRAIN = 0x10,
        OUTPUT_OPEN_DRAIN = 0x12,
        SPECIAL = 0xF0,
        FUNCTION_1 = 0x00,
        FUNCTION_2 = 0x20,
        FUNCTION_3 = 0x40,
        FUNCTION_4 = 0x60,
        FUNCTION_5 = 0x80,
        FUNCTION_6 = 0xA0,
        ANALOG = 0xc0,
        AF_OUTPUT_PUSHPULL, //Used internally for Alternate Function Output PushPull(TIM, UART, SPI etc)
        AF_OUTPUT_DRAIN,    //Used internally for Alternate Function Output Drain(I2C etc). External pullup resistors required.
        AN_INPUT,           //Used internally for ADC Input
        AN_OUTPUT,          //Used internally for DAC Output
        PIN_MODE_NONE=0xFF
} PinMode;
#endif

typedef enum {
    PF_NONE,
    PF_DIO,
    PF_TIMER,
    PF_ADC,
    PF_DAC
} PinFunction;

PinFunction HAL_Validate_Pin_Function(pin_t pin, PinFunction pinFunction);

typedef struct EESP32_Pin_Info  EESP32_Pin_Info;

EESP32_Pin_Info* HAL_Pin_Map(void);

/* Exported macros -----------------------------------------------------------*/

/*
* Pin mapping. Borrowed from Wiring
*/

#define TOTAL_PINS           36 //26
#define TOTAL_ANALOG_PINS    10
#define TOTAL_DIGITAL_PINS   11 // include KEY, TXD0, RXD0 RST, and D7 is D6
#define FIRST_ANALOG_PIN     14

#define GPIO0 0
#define GPIO1 1
#define GPIO2 2
#define GPIO3 3
#define GPIO4 4
#define GPIO5 5
// #define GPIO12 6
#define GPIO12 12
// #define GPIO13 7
#define GPIO13 13
// #define GPIO14 8
#define GPIO14 14
// #define GPIO15 9
#define GPIO15 15
// #define GPIO16 10
#define GPIO16 16
// #define GPIO17 11
#define GPIO17 17
// #define GPIO18 12
#define GPIO18 18
// #define GPIO19 13
#define GPIO19 19
// #define GPIO21 14
#define GPIO21 21
// #define GPIO22 15
#define GPIO22 22
// #define GPIO23 16
#define GPIO23 23
// #define GPIO25 17
#define GPIO25 25
// #define GPIO26 18
#define GPIO26 26
// #define GPIO27 19
#define GPIO27 27
// #define GPIO32 20
#define GPIO32 32
// #define GPIO33 21
#define GPIO33 33
// #define GPIO34 22
#define GPIO34 34
// #define GPIO35 23
#define GPIO35 35

// #define D0  0
// #define D1  1
// #define D2  2
// #define D3  3
// #define D4  4
// #define D5  5
// #define D6  6
// #define D7  7
#define LED_R 0 //10
#define LED_G 0//11
#define LED_B 0//12
#define KEY   0//13

// #define A0  14
// #define A1  15
// #define A2  16
// #define A3  17
// #define A4  18
// #define A5  19
// #define A6  20
// #define A7  21
// #define A8  22
// #define A9  23

// Serial pins
#define TX  GPIO1
#define RX  GPIO3

#define TX1  GPIO17
#define RX1  GPIO16

#define TX2  GPIO25
#define RX2  GPIO26

// SPI pins
#define SS   GPIO5  //D5
#define SCK  GPIO18 //D2
#define MISO GPIO19 //D0
#define MOSI GPIO23 //D1

// SPI1 pins
#define SS1   GPIO26 //A4
#define SCK1  GPIO15 //A5
#define MISO1 GPIO14 //A6
#define MOSI1 GPIO12 //A7


// I2C pins
#define SDA  GPIO18 //D0
#define SCL  GPIO19 //D1

// I2C_1 pins
#define SDA1  GPIO25 //A2
#define SCL1  GPIO26 //A3

// The following pins are only defined for easy access during development.
// Will be removed later as they are internal I/O and users
// should not have too easy of access or bad code could do harm.

// #define LED_USER  D6

#define TOTAL_USARTS       3
#define TOTAL_I2CS         2
#define TOTAL_SPIS         2

#define LSBFIRST 0
#define MSBFIRST 1



/*************************************************************/
/*defined for firmware test only, not for users*/
#define INTOROBOT_TEST_PIN_DIGITAL          GPIO19
#define INTOROBOT_TEST_PIN_ADC              GPIO35
#define INTOROBOT_TEST_PIN_DAC              GPIO25



#ifdef __cplusplus
}
#endif

#endif  /* PINMAP_HAL_H_ */
