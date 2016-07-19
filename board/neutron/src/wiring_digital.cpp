/**
 ******************************************************************************
 @file     : wiring_digital.h
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

#include "wiring_digital.h"


/*********************************************************************************
 *Function      : void pinMode(uint16_t pin, PinMode setMode)
 *Description  : Set the pin mode
 *Input          : pin: port number
setMode: INPUT,OUTPUT,INPUT_PULLUP,INPUT_PULLDOWN,AF_OUTPUT_PUSHPULL,AN_INPUT
 *Output        : none
 *Return        : void
 *author        : lz
 *date           : 6-December-2014
 *Others        : io in STM32_Pin_Info PIN_MAP[TOTAL_PINS]
 **********************************************************************************/
void pinMode(uint16_t pin, PinMode setMode)
{
    if (pin >= TOTAL_PINS || setMode == NONE )
    {
        return;
    }
    GPIO_TypeDef *gpio_port = PIN_MAP[pin].gpio_peripheral;
    uint16_t gpio_pin = PIN_MAP[pin].gpio_pin;

    /*//SPI safety check
      if ((SPI.isEnabled() == true && (pin == PIN_SPI_SCK || pin == PIN_SPI_MOSI || pin == PIN_SPI_MISO))
      ||(SPI_1.isEnabled() == true && (pin == PIN_SPI_SCK1 || pin == PIN_SPI_MOSI1 || pin == PIN_SPI_MISO1)))
      {
      return;
      }
    //Wire safety check
    if ((Wire.isEnabled() == true && (pin == PIN_I2C_SCL || pin == PIN_I2C_SDA))
    ||(Wire1.isEnabled() == true && (pin == PIN_I2C_SCL1 || pin == PIN_I2C_SDA1)))
    {
    return;
    }
    //Serial safety check
    if (Serial.isEnabled() == true && (pin == RX2 || pin == TX2))
    {
    return;
    }
    //Serial1 safety check
    if (Serial1.isEnabled() == true && (pin == RX1 || pin == TX1))
    {
    return;
    }
    //SerialBridge safety check
    if (SerialBridge.isEnabled() == true && (pin == RX || pin == TX))
    {
    return;
    }
    */

    if (gpio_port == GPIOA )
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }
    else if (gpio_port == GPIOB )
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }
    else if (gpio_port == GPIOC)
    {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }
    else if (gpio_port == GPIOH )
    {
        __HAL_RCC_GPIOH_CLK_ENABLE();
    }
    else
    {
        return;
    }

    /* Configure the GPIO_LED pin */
    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.Pin = gpio_pin;
    switch (setMode)
    {
        case OUTPUT:
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
            GPIO_InitStruct.Pull = GPIO_PULLUP;
            GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
            PIN_MAP[pin].pin_mode = OUTPUT;
            break;

        case INPUT:
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
            PIN_MAP[pin].pin_mode = INPUT;
            break;

        case INPUT_PULLUP:
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
            GPIO_InitStruct.Pull = GPIO_PULLUP;
            GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
            PIN_MAP[pin].pin_mode = INPUT;
            break;

        case INPUT_PULLDOWN:
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
            GPIO_InitStruct.Pull = GPIO_PULLDOWN;
            GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
            PIN_MAP[pin].pin_mode = INPUT;
            break;

        case AF_OUTPUT_PUSHPULL:	//Used internally for Alternate Function Output PushPull(TIM, UART, SPI etc)
            GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
            PIN_MAP[pin].pin_mode = AF_OUTPUT_PUSHPULL;
            break;

        case AF_OUTPUT_DRAIN:		//Used internally for Alternate Function Output Drain(I2C etc)
            GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
            GPIO_InitStruct.Pull = GPIO_PULLUP;
            GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
            PIN_MAP[pin].pin_mode = AF_OUTPUT_DRAIN;
            break;

        case AN_INPUT:				//Used internally for ADC Input
            GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
            PIN_MAP[pin].pin_mode = AN_INPUT;
            break;

        default:
            break;
    }
    HAL_GPIO_Init(PIN_MAP[pin].gpio_peripheral, &GPIO_InitStruct);
    HAL_GPIO_WritePin(PIN_MAP[pin].gpio_peripheral, PIN_MAP[pin].gpio_pin, GPIO_PIN_RESET);
}

/*********************************************************************************
 *Function     : void digitalWrite(uint16_t pin, uint8_t value)
 *Description  : Sets a GPIO pin to HIGH or LOW.
 *Input:         pin: port number
value: 1 or 0
 *Output       : none
 *Return       : void
 *author       : lz
 *date         : 6-December-2014
 *Others       :
 **********************************************************************************/
void digitalWrite(uint16_t pin, uint8_t value)
{
    if (pin >= TOTAL_PINS || PIN_MAP[pin].pin_mode != OUTPUT)
    {
    //    return;
    }
    /*
    //SPI safety check
    if ((SPI.isEnabled() == true && (pin == PIN_SPI_SCK || pin == PIN_SPI_MOSI || pin == PIN_SPI_MISO))
    ||(SPI_1.isEnabled() == true && (pin == PIN_SPI_SCK1 || pin == PIN_SPI_MOSI1 || pin == PIN_SPI_MISO1)))
    {
    return;
    }
    //Wire safety check
    if ((Wire.isEnabled() == true && (pin == PIN_I2C_SCL || pin == PIN_I2C_SDA))
    ||(Wire1.isEnabled() == true && (pin == PIN_I2C_SCL1 || pin == PIN_I2C_SDA1)))
    {
    return;
    }
    //Serial safety check
    if (Serial.isEnabled() == true && (pin == RX2 || pin == TX2))
    {
    return;
    }
    //Serial1 safety check
    if (Serial1.isEnabled() == true && (pin == RX1 || pin == TX1))
    {
    return;
    }
    //SerialBridge safety check
    if (SerialBridge.isEnabled() == true && (pin == RX || pin == TX))
    {
    return;
    }
    */
    if (value == LOW)
    {
        HAL_GPIO_WritePin(PIN_MAP[pin].gpio_peripheral, PIN_MAP[pin].gpio_pin, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(PIN_MAP[pin].gpio_peripheral, PIN_MAP[pin].gpio_pin, GPIO_PIN_SET);
    }
}

/*********************************************************************************
 *Function     : int32_t digitalRead(uint16_t pin)
 *Description  : Reads the value of a GPIO pin.
 *Input        : pin: port number
 *Output       : none
 *Return       : Should return either 1 (HIGH) or 0 (LOW).
 *author       : lz
 *date         : 6-December-2014
 *Others       :
 **********************************************************************************/
int32_t digitalRead(uint16_t pin)
{
    if (pin >= TOTAL_PINS || PIN_MAP[pin].pin_mode != INPUT)
    {
        return LOW;
    }
    /*
    // SPI safety check
    if ((SPI.isEnabled() == true && (pin == PIN_SPI_SCK || pin == PIN_SPI_MOSI || pin == PIN_SPI_MISO))
    ||(SPI_1.isEnabled() == true && (pin == PIN_SPI_SCK1 || pin == PIN_SPI_MOSI1 || pin == PIN_SPI_MISO1)))
    {
    return LOW;
    }
    // I2C safety check
    if ((Wire.isEnabled() == true && (pin == PIN_I2C_SCL || pin == PIN_I2C_SDA))
    ||(Wire1.isEnabled() == true && (pin == PIN_I2C_SCL1 || pin == PIN_I2C_SDA1)))
    {
    return LOW;
    }
    //Serial safety check
    if (Serial.isEnabled() == true && (pin == RX2 || pin == TX2))
    {
    return LOW;
    }
    //Serial1 safety check
    if (Serial1.isEnabled() == true && (pin == RX1 || pin == TX1))
    {
    return LOW;
    }
    //SerialBridge safety check
    if (SerialBridge.isEnabled() == true && (pin == RX || pin == TX))
    {
    return LOW;
    }
    */
    return HAL_GPIO_ReadPin(PIN_MAP[pin].gpio_peripheral, PIN_MAP[pin].gpio_pin);
}