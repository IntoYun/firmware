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

typedef struct EESP32_Pin_Info  EESP32_Pin_Info;

EESP32_Pin_Info* HAL_Pin_Map(void);


/* Exported macros -----------------------------------------------------------*/

/*
* Pin mapping. Borrowed from Wiring
*/
#define TOTAL_PINS           46
#define FIRST_DIGITAL_PIN    0
#define TOTAL_DIGITAL_PINS   10
#define FIRST_ANALOG_PIN     30
#define TOTAL_ANALOG_PINS    16


// digital pin
#define GPIO1          0
#define GPIO3          1
#define GPIO5          2
#define GPIO16         3
#define GPIO17         4
#define GPIO18         5
#define GPIO19         6
#define GPIO21         7
#define GPIO22         8
#define GPIO23         9

// analog pin
#define GPIO0          30
#define GPIO2          31
#define GPIO4          32
#define GPIO12         33
#define GPIO13         34
#define GPIO14         35
#define GPIO15         36
#define GPIO25         37
#define GPIO26         38
#define GPIO27         39
#define GPIO32         40
#define GPIO33         41
#define GPIO34         42
#define GPIO35         43
#define GPIO36         44
#define GPIO39         45

// Serial pins
#define TX    GPIO1
#define RX    GPIO3

#define TXD   TX
#define RXD   RX

// Serial1 pin
#define TX1   GPIO17
#define RX1   GPIO16

#define TXD1  TX1
#define RXD1  RX1

// SPI pins
#define SS    GPIO5
#define SCK   GPIO18
#define MISO  GPIO19
#define MOSI  GPIO23

// SPI1 pins
#define SS1   GPIO15
#define SCK1  GPIO14
#define MISO1 GPIO12
#define MOSI1 GPIO13

// I2C pins
#define SDA   GPIO23
#define SCL   GPIO19

// I2C_1 pins
#define SDA1  GPIO26
#define SCL1  GPIO25

// The following pins are only defined for easy access during development.
// Will be removed later as they are internal I/O and users
// should not have too easy of access or bad code could do harm.

#define LED_USER     GPIO4
#define LED_BUILTIN  LED_USER

#define TOTAL_USARTS    2
#define TOTAL_SPIS      2
#define TOTAL_I2CS      2


#define LSBFIRST        0
#define MSBFIRST        1

#ifdef __cplusplus
}
#endif

#endif  /* PINMAP_HAL_H_ */
