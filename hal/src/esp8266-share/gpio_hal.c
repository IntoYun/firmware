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

/* Includes -----------------------------------------------------------------*/
#include "hw_config.h"
#include "gpio_hal.h"
#include "pinmap_impl.h"
#include "esp8266-hal-gpio.h"

/* Private typedef ----------------------------------------------------------*/

/* Private define -----------------------------------------------------------*/

/* Private macro ------------------------------------------------------------*/

/* Private variables --------------------------------------------------------*/
PinMode digitalPinModeSaved = PIN_MODE_NONE;

/* Extern variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/

inline bool is_valid_pin(pin_t pin) __attribute__((always_inline));
inline bool is_valid_pin(pin_t pin)
{
    return pin<TOTAL_PINS;
}

PinMode HAL_Get_Pin_Mode(pin_t pin)
{
    return (!is_valid_pin(pin)) ? PIN_MODE_NONE : HAL_Pin_Map()[pin].pin_mode;
}

PinFunction HAL_Validate_Pin_Function(pin_t pin, PinFunction pinFunction)
{
    EESP8266_Pin_Info *PIN_MAP = HAL_Pin_Map();

    if (!is_valid_pin(pin))
        return PF_NONE;
    if (pinFunction==PF_ADC && PIN_MAP[pin].adc_channel!=ADC_CHANNEL_NONE)
        return PF_ADC;
    if (pinFunction==PF_TIMER && PIN_MAP[pin].timer_peripheral!=NONE)
        return PF_TIMER;
    return PF_DIO;
}

/*
 * @brief Set the mode of the pin to OUTPUT, INPUT, INPUT_PULLUP,
 * or INPUT_PULLDOWN
 */
void HAL_Pin_Mode(pin_t pin, PinMode setMode)
{
    uint8_t mode;
    EESP8266_Pin_Info *PIN_MAP = HAL_Pin_Map();
    pin_t gpio_pin = PIN_MAP[pin].gpio_pin;

    switch (setMode)
    {
        case OUTPUT:
            PIN_MAP[pin].pin_mode = OUTPUT;
            mode = ESP8266_OUTPUT;
            break;

        case INPUT:
            PIN_MAP[pin].pin_mode = INPUT;
            mode = ESP8266_INPUT;
            break;

        case INPUT_PULLUP:
            PIN_MAP[pin].pin_mode = INPUT_PULLUP;
            mode = ESP8266_INPUT_PULLUP;
            break;

        case INPUT_PULLDOWN:
            PIN_MAP[pin].pin_mode = INPUT_PULLDOWN;
            mode = ESP8266_INPUT_PULLDOWN_16;
            break;

        //Used internally begin
        case OUTPUT_OPEN_DRAIN:
            PIN_MAP[pin].pin_mode = OUTPUT_OPEN_DRAIN;
            mode = ESP8266_OUTPUT_OPEN_DRAIN;
            break;

        case WAKEUP_PULLUP:
            PIN_MAP[pin].pin_mode = WAKEUP_PULLUP;
            mode = ESP8266_WAKEUP_PULLUP;
            break;

        case WAKEUP_PULLDOWN:
            PIN_MAP[pin].pin_mode = WAKEUP_PULLDOWN;
            mode = ESP8266_WAKEUP_PULLDOWN;
            break;

        case SPECIAL:
            PIN_MAP[pin].pin_mode = SPECIAL;
            mode = ESP8266_SPECIAL;
            break;

        case FUNCTION_0:
            PIN_MAP[pin].pin_mode = FUNCTION_0;
            mode = ESP8266_FUNCTION_0;
            break;

        case FUNCTION_1:
            PIN_MAP[pin].pin_mode = FUNCTION_1;
            mode = ESP8266_FUNCTION_1;
            break;

        case FUNCTION_2:
            PIN_MAP[pin].pin_mode = FUNCTION_2;
            mode = ESP8266_FUNCTION_2;
            break;

        case FUNCTION_3:
            PIN_MAP[pin].pin_mode = FUNCTION_3;
            mode = ESP8266_FUNCTION_3;
            break;

        case FUNCTION_4:
            PIN_MAP[pin].pin_mode = FUNCTION_4;
            mode = ESP8266_FUNCTION_4;
            break;

        default:
            break;
    }
    __pinMode(gpio_pin, mode);
}

/*
 * @brief Saves a pin mode to be recalled later.
 */
void HAL_GPIO_Save_Pin_Mode(PinMode mode)
{
    digitalPinModeSaved = mode;
}

/*
 * @brief Recalls a saved pin mode.
 */
PinMode HAL_GPIO_Recall_Pin_Mode()
{
    return digitalPinModeSaved;
}

/*
 * @brief Sets a GPIO pin to HIGH or LOW.
 */
void HAL_GPIO_Write(uint16_t pin, uint8_t value)
{
    EESP8266_Pin_Info *PIN_MAP = HAL_Pin_Map();
    pin_t gpio_pin = PIN_MAP[pin].gpio_pin;
    __digitalWrite(gpio_pin, value);
}

/*
 * @brief Reads the value of a GPIO pin. Should return either 1 (HIGH) or 0 (LOW).
 */
int32_t HAL_GPIO_Read(uint16_t pin)
{
    EESP8266_Pin_Info *PIN_MAP = HAL_Pin_Map();

    pin_t gpio_pin = PIN_MAP[pin].gpio_pin;
    return __digitalRead(gpio_pin);
}

/*
 * @brief   blocking call to measure a high or low pulse
 * @returns uint32_t pulse width in microseconds up to 3 seconds,
 *          returns 0 on 3 second timeout error, or invalid pin.
 */
uint32_t HAL_Pulse_In(pin_t pin, uint16_t value)
{
    // FIXME: SYTME_TICK_COUNTER change to system_get_time which return the micro seconds
    volatile uint32_t timeoutStart = system_get_time(); // total 3 seconds for entire function!

    /* If already on the value we want to measure, wait for the next one.
     * Time out after 3 seconds so we don't block the background tasks
     */
    while (HAL_pinReadFast(pin) == value) {
        if (sytem_get_time() - timeoutStart > 3000000UL) {
            return 0;
        }
    }

    /* Wait until the start of the pulse.
     * Time out after 3 seconds so we don't block the background tasks
     */
    while (HAL_pinReadFast(pin) != value) {
        if (system_get_time() - timeoutStart > 3000000UL) {
            return 0;
        }
    }

    /* Wait until this value changes, this will be our elapsed pulse width.
     * Time out after 3 seconds so we don't block the background tasks
     */
    volatile uint32_t pulseStart = system_get_time();
    while (HAL_pinReadFast(pin) == value) {
        if (system_get_time()- timeoutStart > 3000000UL) {
            return 0;
        }
    }

    return (system_get_time() - pulseStart);
}

void HAL_pinSetFast(pin_t pin)
{
    HAL_GPIO_Write(pin, 1);
}

void HAL_pinResetFast(pin_t pin)
{
    HAL_GPIO_Write(pin, 0);
}

int32_t HAL_pinReadFast(pin_t pin)
{
    return HAL_GPIO_Read(pin);
}

