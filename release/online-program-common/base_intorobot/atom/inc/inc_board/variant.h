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
#ifndef   VARIANT_INTOROBOT_ATOM_H_
#define   VARIANT_INTOROBOT_ATOM_H_

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

//#include "application.h"

#include "stm32f10x.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "hw_config.h"
#ifdef __cplusplus
}
#endif
#include "wiring_usartserial.h"


/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/

/** Frequency of the board main oscillator */
#define VARIANT_MAINOSC		8000000

/** Master clock frequency */
#define VARIANT_MCK			72000000

//#define UINT_MAX	0xffffffff

#define HIGH 	0x1
#define LOW 	0x0

#define ON      0x1
#define OFF     0x0

#define boolean bool

//#define NULL ((void *)0)
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
#define TOTAL_PINS 				57
#define TOTAL_DIGITAL_PINS 		25
#define FIRST_ANALOG_PIN 		30
#define TOTAL_ANALOG_PINS 		16
#define FIRST_EXTEND_PIN        50
#define TOTAL_EXTEND_PINS 		6

#define D0 	0
#define D1 	1
#define D2 	2
#define D3 	3
#define D4 	4
#define D5 	5
#define D6 	6
#define D7 	7
#define D8 	8
#define D9 	9
#define D10 10
#define D11 11
#define D12 12
#define D13 13
#define D14 14
#define D15 15
#define D16 16
#define D17 17
#define D18 18
#define D19 19
#define D20 20
#define D21 21
#define D22 22
#define D23 23
#define D24 24

#define LED_USER  D7

#define A0 	30
#define A1 	31
#define A2 	32
#define A3 	33
#define A4 	34
#define A5 	35
#define A6 	36
#define A7 	37
#define A8 	38
#define A9 	39
#define A10 40
#define A11 41
#define A12 42
#define A13 43
#define A14 44
#define A15 45

#define E0  50
#define E1  51
#define E2  52
#define E3  53
#define E4  54
#define E5  55

// Timer pins
#define TIMER1_CH1 11

#define TIMER2_CH1 30
#define TIMER2_CH2 31
#define TIMER2_CH3 33
#define TIMER2_CH4 32

#define TIMER3_CH1 36
#define TIMER3_CH2 37
#define TIMER3_CH3 38
#define TIMER3_CH4 39

#define TIMER4_CH1 9
#define TIMER4_CH2 8
#define TIMER4_CH3 6
#define TIMER4_CH4 7

// SPI pins
#define SPI_INTERFACES_COUNT 	2

#define SPI_INTERFACE1        	SPI1
#define PIN_SPI_SS             	34
#define PIN_SPI_SCK           	35
#define PIN_SPI_MISO         	36
#define PIN_SPI_MOSI         	37

#define SPI_INTERFACE2        	SPI2
#define PIN_SPI_SS1            	2
#define PIN_SPI_SCK1          	3
#define PIN_SPI_MISO1        	4
#define PIN_SPI_MOSI1        	5

// I2C pins
#define I2C_INTERFACES_COUNT 	2

#define I2C_INTERFACE1        I2C1
#define PIN_I2C_SDA           8
#define PIN_I2C_SCL           9

#define I2C_INTERFACE2        I2C2
#define PIN_I2C_SDA1           0
#define PIN_I2C_SCL1           1

// usart pins
#define SERIAL_INTERFACES_COUNT 3

#define RX 46 // usart1
#define TX 47

#define RX1 D0 // usart3
#define TX1 D1

#define RX2 A2 // usart2
#define TX2 A3



#define ADC1_DR_ADDRESS		((uint32_t)0x4001244C)
#define ADC_DMA_BUFFERSIZE	10
#define ADC_SAMPLING_TIME	ADC_SampleTime_7Cycles5	//Allowed values: 1.5, 7.5 and 13.5 for "Dual slow interleaved mode"

#define TIM_PWM_FREQ	500 //500Hz

#define LSBFIRST	0
#define MSBFIRST	1

typedef unsigned char byte;

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

#ifdef __cplusplus
extern "C" {
#endif

#define TIMING_IWDG_RELOAD	1000 //one sec


extern USARTSerial Serial;       // USART2 on PA3/2 (IntoRobot A2, A3)
extern USARTSerial Serial1;      // USART3 on alternate PB10/11 (IntoRobot D1, D0)
extern USARTSerial SerialBridge; // USART1 on alternate PA9/10

void init( void );
void Delay_Microsecond(uint32_t uSec);

void set_intorobot_loop_period(unsigned short time_ms);



#ifdef __cplusplus
} /* end of extern "C" */
#endif


#endif /* VARIANT_INTOROBOT_ATOM_H_ */

