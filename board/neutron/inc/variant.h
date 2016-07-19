/**
 ******************************************************************************
 * @file     : variant.h
 * @author   : robin
 * @version	 : V1.0.0
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
#ifndef   VARIANT_H_
#define   VARIANT_H_

#include "stm32f4xx_hal.h"

/** Frequency of the board main oscillator */
#define VARIANT_MAINOSC		26000000

/** Master clock frequency */
#define VARIANT_MCK			72000000

/** @addtogroup Exported_types
  * @{
  */

/*!< STM32F10x Standard Peripheral Library old types (maintained for legacy purpose) */
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */

#define UINT_MAX	0xffffffff

#define HIGH 	0x1
#define LOW 	0x0

#define ON      0x1
#define OFF     0x0

#ifndef __cplusplus
//extern "C" {
//#endif

typedef enum
{
  false = 0, true  = !false
}bool;

//#ifdef __cplusplus
//} /* end of extern "C" */
#endif

//#define bool  uint8_t
#define boolean bool

#define NONE ((uint8_t)0xFF)

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

/*
* Pin mapping. Borrowed from Wiring
*/
#define TOTAL_PINS 				40
#define TOTAL_DIGITAL_PINS 		8
#define FIRST_ANALOG_PIN 		30
#define TOTAL_ANALOG_PINS 		8

#define D0 	0
#define D1 	1
#define D2 	2
#define D3 	3
#define D4 	4
#define D5 	5
#define D6 	6
#define D7 	7

#define ESP_BOOT 8
#define ESP_RST 9
#define BT 10
#define LR 11
#define LG 12
#define LB 13
#define PO188 14

#define LED_USER  D7

#define A0 	30
#define A1 	31
#define A2 	32
#define A3 	33
#define A4 	34
#define A5 	35
#define A6 	36
#define A7 	37

// Timer pins
#define TIMER5_CH1 A0
#define TIMER5_CH2 A1
#define TIMER5_CH3 A2
#define TIMER5_CH4 A3

#define TIMER2_CH1 A5
#define TIMER3_CH1 A6
#define TIMER3_CH2 A7

#define TIMER1_CH1 9
#define TIMER4_CH2 8
#define TIMER4_CH3 6
#define TIMER4_CH4 7

// SPI pins
#define PIN_SPI_SS          A4
#define PIN_SPI_SCK         A5
#define PIN_SPI_MISO        A6
#define PIN_SPI_MOSI        A7

// I2C pins
#define PIN_I2C_SDA         D2
#define PIN_I2C_SCL         D1

// usart pins
#define SERIAL_INTERFACES_COUNT  2

#define RX    A2 // usart
#define TX    A3

#define RX1   D0 // usart1
#define TX1   D1


#define ADC1_DR_ADDRESS		((uint32_t)0x4001244C)
#define ADC_DMA_BUFFERSIZE	10
#define ADC_SAMPLING_TIME	ADC_SampleTime_7Cycles5	//Allowed values: 1.5, 7.5 and 13.5 for "Dual slow interleaved mode"

#define TIM_PWM_FREQ	500 //500Hz

#define LSBFIRST	0
#define MSBFIRST	1

typedef unsigned char byte;
typedef uint32_t system_tick_t;
#define SYSTEM_US_TICKS		(SystemCoreClock / 1000000)//cycles per microsecond

typedef enum PinMode
{
    OUTPUT,
    INPUT,
    INPUT_PULLUP,
    INPUT_PULLDOWN,
    AF_OUTPUT_PUSHPULL,	//Used internally for Alternate Function Output PushPull(TIM, UART, SPI etc)
    AF_OUTPUT_DRAIN,    //Used internally for Alternate Function Output Drain(I2C etc). External pullup resistors required.
    AN_INPUT  			//Used internally for ADC Input
} PinMode;

typedef struct STM32_Pin_Info
{
    GPIO_TypeDef* gpio_peripheral;
    uint16_t gpio_pin;
    uint8_t adc_channel;
    TIM_TypeDef* timer_peripheral;
    uint16_t timer_ch;
    PinMode pin_mode;
    uint16_t timer_ccr;
    int32_t user_property;
} STM32_Pin_Info;



extern STM32_Pin_Info PIN_MAP[];

#endif /* VARIANT_INTOROBOT_ATOM_H_ */

