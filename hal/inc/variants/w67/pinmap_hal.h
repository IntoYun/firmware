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
    AF_OUTPUT_PUSHPULL, //Used internally for Alternate Function Output PushPull(TIM, UART, SPI etc)
    AF_OUTPUT_DRAIN,    //Used internally for Alternate Function Output Drain(I2C etc). External pullup resistors required.
    AN_INPUT,           //Used internally for ADC Input
    AN_OUTPUT,          //Used internally for DAC Output
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

typedef struct EESP82666_Pin_Info  EESP82666_Pin_Info;

EESP82666_Pin_Info* HAL_Pin_Map(void);

/* Exported macros -----------------------------------------------------------*/

/*
* Pin mapping. Borrowed from Wiring
*/

#define TOTAL_PINS              31
#define TOTAL_ANALOG_PINS       1
#define TOTAL_DIGITAL_PINS      10
#define FIRST_ANALOG_PIN        30

// #define D0 0
// #define D1 1
// #define D2 2
// #define D3 3
// #define D4 4
// #define D5 5
// #define D6 6
// #define D7 7

#define GPIO17 30
#define A0  GPIO17

#define GPIO2  0
#define GPIO4  1
#define GPIO14 2
#define GPIO12 3
#define GPIO13 4
#define GPIO15 5
#define GPIO16 6
#define GPIO1  8
#define GPIO3  9
#define GPIO5  10
#define GPIO0  11

#define D0 GPIO2
#define D1 GPIO4
#define D2 GPIO14
#define D3 GPIO12
#define D4 GPIO13
#define D5 GPIO15
#define D6 GPIO16
#define D7 D67


#define TX   GPIO1
#define RX   GPIO3
#define LED  GPIO5
#define KEY  GPIO0

// SPI pins
#define SS   GPIO15 //D5
#define SCK  GPIO14 //D2
#define MISO GPIO12 //D3
#define MOSI GPIO13 //D4

// I2C pins
#define SDA  GPIO4 //D1
#define SCL  GPIO2 //D0

// The following pins are only defined for easy access during development.
// Will be removed later as they are internal I/O and users
// should not have too easy of access or bad code could do harm.

#define LED_USER  GPIO16 // D6

#define TOTAL_USARTS        2
#define TOTAL_SPIS          1
#define TOTAL_I2CS          1

#define LSBFIRST 0
#define MSBFIRST 1

#ifdef __cplusplus
}
#endif

#endif  /* PINMAP_HAL_H_ */
