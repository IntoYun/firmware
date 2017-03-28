/**
 ******************************************************************************
 * @file    interrupts_hal.c
 * @author  Satish Nair, Brett Walach
 * @version V1.0.0
 * @date    12-Sept-2014
 * @brief
 ******************************************************************************
  Copyright (c) 2013-2015 IntoRobot Industries, Inc.  All rights reserved.

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

/* Includes ------------------------------------------------------------------*/
#include "interrupts_hal.h"
#include "gpio_hal.h"
#include "pinmap_impl.h"
#include "rom/gpio.h"
#include "soc/gpio_struct.h"
#include "soc/gpio_reg.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_intr.h"
#include "esp32-hal-gpio.h"


#if 0
#define ETS_GPIO_INUM       4
typedef void (*voidFuncPtr)(void);
static voidFuncPtr __pinInterruptHandlers[GPIO_PIN_COUNT] = {0,};

/* static void IRAM_ATTR __onPinInterrupt(void *arg) */
static void  __onPinInterrupt(void *arg)
{
    uint32_t gpio_intr_status_l=0;
    uint32_t gpio_intr_status_h=0;

    gpio_intr_status_l = GPIO.status;
    gpio_intr_status_h = GPIO.status1.val;
    GPIO.status_w1tc = gpio_intr_status_l;//Clear intr for gpio0-gpio31
    GPIO.status1_w1tc.val = gpio_intr_status_h;//Clear intr for gpio32-39

    uint8_t pin=0;
    if(gpio_intr_status_l) {
        do {
            if(gpio_intr_status_l & ((uint32_t)1 << pin)) {
                if(__pinInterruptHandlers[pin]) {
                    __pinInterruptHandlers[pin]();
                }
            }
        } while(++pin<32);
    }
    if(gpio_intr_status_h) {
        pin=32;
        do {
            if(gpio_intr_status_h & ((uint32_t)1 << (pin - 32))) {
                if(__pinInterruptHandlers[pin]) {
                    __pinInterruptHandlers[pin]();
                }
            }
        } while(++pin<GPIO_PIN_COUNT);
    }
}
#endif

void HAL_Interrupts_Attach(uint16_t pin, HAL_InterruptHandler handler, void* data, InterruptMode mode, HAL_InterruptExtraConfiguration* config)
{
    EESP32_Pin_Info* PIN_MAP = HAL_Pin_Map();
    pin_t gpio_pin = PIN_MAP[pin].gpio_pin;
    if (gpio_pin > 39) {
        return;
    }

    #if 1
    uint8_t interruptMode;
    switch(mode)
    {
        case CHANGE:
            interruptMode = ESP32_CHANGE;
            break;

        case RISING:
            interruptMode = ESP32_RISING;
            break;

        case FALLING:
            interruptMode = ESP32_FALLING;
            break;

        default:
            break;
    }
    __attachInterrupt(gpio_pin,handler,mode);
    #endif

    #if 0
    static bool interrupt_initialized = false;
    static int core_id = 0;

    if(!interrupt_initialized) {
        interrupt_initialized = true;
        core_id = xPortGetCoreID();
        ESP_INTR_DISABLE(ETS_GPIO_INUM);
        intr_matrix_set(core_id, ETS_GPIO_INTR_SOURCE, ETS_GPIO_INUM);
        xt_set_interrupt_handler(ETS_GPIO_INUM, &__onPinInterrupt, NULL);
        ESP_INTR_ENABLE(ETS_GPIO_INUM);
    }
    /* __pinInterruptHandlers[gpio_pin] = (voidFuncPtr)handler; */
    __pinInterruptHandlers[gpio_pin] = handler;
    ESP_INTR_DISABLE(ETS_GPIO_INUM);
    if(core_id) { //APP_CPU
        GPIO.pin[gpio_pin].int_ena = 1;
    } else { //PRO_CPU
        GPIO.pin[gpio_pin].int_ena = 4;
    }

    if (mode == CHANGE) {
        /* GPIO.pin[gpio_pin].int_type = GPIO_PIN_INTR_ANYEGDE; */
        GPIO.pin[gpio_pin].int_type = 3;
    }
    else if(mode == RISING) {
        /* GPIO.pin[gpio_pin].int_type = GPIO_PIN_INTR_POSEDGE; */
        GPIO.pin[gpio_pin].int_type = 1;
    }
    else if(mode== FALLING) {
        /* GPIO.pin[gpio_pin].int_type = GPIO_PIN_INTR_NEGEDGE; */
        GPIO.pin[gpio_pin].int_type = 2;
    }

    ESP_INTR_ENABLE(ETS_GPIO_INUM);
    #endif
}

void HAL_Interrupts_Detach(uint16_t pin)
{
    EESP32_Pin_Info* PIN_MAP = HAL_Pin_Map();
    pin_t gpio_pin = PIN_MAP[pin].gpio_pin;
    if (gpio_pin > 39) {
        return;
    }

    __detachInterrupt(gpio_pin);
    #if 0
    __pinInterruptHandlers[gpio_pin] = NULL;
    ESP_INTR_DISABLE(ETS_GPIO_INUM);
    GPIO.pin[gpio_pin].int_ena = 0;
    GPIO.pin[gpio_pin].int_type = 0;
    ESP_INTR_ENABLE(ETS_GPIO_INUM);
    #endif
}

void HAL_Interrupts_Enable_All(void)
{
}

void HAL_Interrupts_Disable_All(void)
{
}

void HAL_Interrupts_Suspend(void)
{
}

void HAL_Interrupts_Restore(void)
{
}

/*******************************************************************************
 * Function Name  : HAL_EXTI_Handler (Declared as weak in stm32_it.cpp)
 * Description    : This function is called by any of the interrupt handlers. It
                                                 essentially fetches the user function pointer from the array
                                                 and calls it.
 * Input          : EXTI_Line (Supported: 0, 1, 3, 4, 5, 6, 7, 13, 14 and 15)
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void HAL_EXTI_Handler(uint8_t EXTI_Line)
{
}


int HAL_disable_irq()
{
    return 0;
}

void HAL_enable_irq(int is)
{
}

uint8_t HAL_IsISR()
{
	return 0;
}
