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

/* Includes ------------------------------------------------------------------*/
#include "tone_hal.h"
#include "pinmap_impl.h"
#include "stm32f4xx.h"

#define TONE_TIM_COUNTER_CLOCK_FREQ 1000000 ////TIM Counter clock = 1MHz
extern void (*TIM2_IRQHandler)(void);
extern void (*TIM3_IRQHandler)(void);
extern void (*TIM4_HIRQandler)(void);
extern void (*TIM5_HIRQandler)(void);

/*void (*TIM2_IRQHandler)(void);*/
/*void (*TIM3_IRQHandler)(void);*/
/*void (*TIM4_HIRQandler)(void);*/
/*void (*TIM5_HIRQandler)(void);*/

//static void Tone_TIM2_Handler(void);
//static void Tone_TIM3_Handler(void);
//static void Tone_TIM4_Handler(void);
//static void Tone_TIM5_Handler(void);
TIM_HandleTypeDef TimHandle;


void HAL_Tone_Start(uint8_t pin, uint32_t frequency, uint32_t duration)
{
    if(frequency < 20 || frequency > 20000)
    {
        return;//no tone for frequency outside of human audible range
    }
    //Map the Spark pin to the appropriate port and pin on the STM32
    STM32_Pin_Info* PIN_MAP = HAL_Pin_Map();

    if(PIN_MAP[pin].timer_peripheral != NULL && PIN_MAP[pin].timer_peripheral != TIM1)
    {
        /* Common configuration for all channles */
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull  = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

        // else if( (PIN_MAP[pin].timer_peripheral == TIM2) )
        if( (PIN_MAP[pin].timer_peripheral == TIM2) )
        {
            __HAL_RCC_TIM2_CLK_ENABLE();

            GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
            GPIO_InitStruct.Pin       = PIN_MAP[pin].gpio_pin;
            /* Port Clock enable */
            if( (PIN_MAP[pin].gpio_peripheral == GPIOA) )
            {
                __HAL_RCC_GPIOA_CLK_ENABLE();
                HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
            }
            else if( (PIN_MAP[pin].gpio_peripheral == GPIOB) )
            {
                __HAL_RCC_GPIOB_CLK_ENABLE();
                HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
            }
            //HAL_TIM2_Handler = Tone_TIM2_Handler;

        }
        else if( (PIN_MAP[pin].timer_peripheral == TIM3) )
        {
            __HAL_RCC_TIM3_CLK_ENABLE();
            GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
            GPIO_InitStruct.Pin       = PIN_MAP[pin].gpio_pin;

            /* Port Clock enable */
            if( (PIN_MAP[pin].gpio_peripheral == GPIOA) )
            {
                __HAL_RCC_GPIOA_CLK_ENABLE();
                HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
            }
            else if( (PIN_MAP[pin].gpio_peripheral == GPIOB) )
            {
                __HAL_RCC_GPIOB_CLK_ENABLE();
                HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
            }
            //HAL_TIM3_Handler = Tone_TIM3_Handler;
        }
        else if( (PIN_MAP[pin].timer_peripheral == TIM4) )
        {
            __HAL_RCC_TIM4_CLK_ENABLE();
            GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
            GPIO_InitStruct.Pin       = PIN_MAP[pin].gpio_pin;

            /* Port Clock enable */
            if( (PIN_MAP[pin].gpio_peripheral == GPIOA) )
            {
                __HAL_RCC_GPIOA_CLK_ENABLE();
                HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
            }
            else if( (PIN_MAP[pin].gpio_peripheral == GPIOB) )
            {
                __HAL_RCC_GPIOB_CLK_ENABLE();
                HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
            }
            //HAL_TIM4_Handler = Tone_TIM4_Handler;
        }
        else if( (PIN_MAP[pin].timer_peripheral == TIM5) )
        {
            __HAL_RCC_TIM5_CLK_ENABLE();

            GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
            GPIO_InitStruct.Pin       = PIN_MAP[pin].gpio_pin;

            /* Port Clock enable */
            if( (PIN_MAP[pin].gpio_peripheral == GPIOA) )
            {
                __HAL_RCC_GPIOA_CLK_ENABLE();
                HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
            }
            else if( (PIN_MAP[pin].gpio_peripheral == GPIOB) )
            {
                __HAL_RCC_GPIOB_CLK_ENABLE();
                HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
            }
            //HAL_TIM5_Handler = Tone_TIM5_Handler;
        }

        /*##-1- Configure the TIM peripheral #######################################*/
        //uint32_t TIM_Prescaler = (uint32_t)((SystemCoreClock / 24000000) - 1);
        uint32_t TIM_Prescaler = (uint32_t)((SystemCoreClock / TONE_TIM_COUNTER_CLOCK_FREQ) - 1);

        //uint32_t TIM_ARR = (uint32_t)((24000000 / frequency) - 1);
        uint32_t TIM_CCR = (uint32_t)(TONE_TIM_COUNTER_CLOCK_FREQ / (2 * frequency));

        int32_t timer_channel_toggle_count = -1;

        // Calculate the toggle count
        if (duration > 0)
        {
            timer_channel_toggle_count = 2 * frequency * duration / 1000;
        }

        PIN_MAP[pin].timer_ccr = TIM_CCR;
        PIN_MAP[pin].user_property = timer_channel_toggle_count;

        //TIM_HandleTypeDef TimHandle;
        TIM_OC_InitTypeDef sConfig;

        TimHandle.Instance = PIN_MAP[pin].timer_peripheral;

        TimHandle.Init.Period            = 0xFFFF; // 16 bit timer 65535
        TimHandle.Init.Prescaler         = TIM_Prescaler;
        TimHandle.Init.ClockDivision     = 0;
        TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
        HAL_TIM_OC_Init(&TimHandle);

        /*##-2- Configure the Output Compare channels ##############################*/
        /* Common configuration for all channels */
        sConfig.OCMode       = TIM_OCMODE_TOGGLE;
        sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;

        /* Set the pulse value for channel 1 */
        sConfig.Pulse = TIM_CCR;
        if (HAL_TIM_OC_ConfigChannel(&TimHandle, &sConfig, PIN_MAP[pin].timer_ch) != HAL_OK)
        {
            /* Configuration Error */
        }

        /* Start channel */
        if (HAL_TIM_OC_Start_IT(&TimHandle, PIN_MAP[pin].timer_ch) != HAL_OK)
        {
            //Error_Handler();
        }
    }
    else
    {
        // Error
    }
}

void HAL_Tone_Stop(uint8_t pin)
{
    STM32_Pin_Info* PIN_MAP = HAL_Pin_Map();
    HAL_TIM_OC_Stop_IT(&TimHandle, PIN_MAP[pin].timer_ch);
    PIN_MAP[pin].timer_ccr = 0;
    PIN_MAP[pin].user_property = 0;
}

uint32_t HAL_Tone_Get_Frequency(uint8_t pin)
{
    uint16_t TIM_CCR = 0;
    uint16_t Tone_Frequency = 0;

    STM32_Pin_Info* PIN_MAP = HAL_Pin_Map();
    if(PIN_MAP[pin].timer_ch == TIM_CHANNEL_1)
    {
        TIM_CCR = PIN_MAP[pin].timer_peripheral->CCR1;
    }
    else if(PIN_MAP[pin].timer_ch == TIM_CHANNEL_2)
    {
        TIM_CCR = PIN_MAP[pin].timer_peripheral->CCR2;
    }
    else if(PIN_MAP[pin].timer_ch == TIM_CHANNEL_3)
    {
        TIM_CCR = PIN_MAP[pin].timer_peripheral->CCR3;
    }
    else if(PIN_MAP[pin].timer_ch == TIM_CHANNEL_4)
    {
        TIM_CCR = PIN_MAP[pin].timer_peripheral->CCR4;
    }
    else
    {
        return Tone_Frequency;
    }

    Tone_Frequency = (uint16_t)(TONE_TIM_COUNTER_CLOCK_FREQ / TIM_CCR);

    return Tone_Frequency;
}

bool HAL_Tone_Is_Stopped(uint8_t pin)
{
    STM32_Pin_Info* PIN_MAP = HAL_Pin_Map();
    if(PIN_MAP[pin].timer_ccr > 0)
    {
        return false;
    }

    return true;
}
