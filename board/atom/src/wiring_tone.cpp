/**
 ******************************************************************************
 * @file     : wiring_tone.cpp
 * @author   : robin
 * @version	 : V1.0.0
 * @date     : 6-December-2014
 * @brief    : Generates a square wave of the specified frequency and duration
 * 	  	  	   (and 50% duty cycle) on a timer channel pin
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
**/
#include "stm32_it.h"
#include "wiring_tone.h"
#include "wiring_usartserial.h"
#include "wiring_spi.h"
#include "wiring_i2c.h"

/* Private function prototypes */
void TIM2_Tone_Interrupt_Handler(void);
void TIM3_Tone_Interrupt_Handler(void);
void TIM4_Tone_Interrupt_Handler(void);

/*********************************************************************************
  *Function      : void tone(uint8_t pin, unsigned int frequency, unsigned long duration)
  *Description  : Generates a square wave of the specified frequency and duration
  	  	  	      (and 50% duty cycle) on a timer channel pin
  *Input          : pin: pin number
  				  frequency:
  				  duration:
  *Output        : none
  *Return         : none
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
void tone(uint8_t pin, unsigned int frequency, unsigned long duration)
{
    if (pin >= TOTAL_PINS || PIN_MAP[pin].timer_peripheral == NULL)
    {
        return;
    }

    // LZ 修改
    if(pin == 46 || pin == 47)
    {
        return; // 46 47为 stm32与7620n 串口通讯 IO 用户不可操作
    }

    // SPI safety check
    if ((SPI.isEnabled() == true && (pin == PIN_SPI_SCK || pin == PIN_SPI_MOSI || pin == PIN_SPI_MISO))
    ||(SPI_1.isEnabled() == true && (pin == PIN_SPI_SCK1 || pin == PIN_SPI_MOSI1 || pin == PIN_SPI_MISO1)))
    {
        return;
    }


    // I2C safety check
    if ((Wire.isEnabled() == true && (pin == PIN_I2C_SCL || pin == PIN_I2C_SDA))
    ||(Wire1.isEnabled() == true && (pin == PIN_I2C_SCL1 || pin == PIN_I2C_SDA1)))
    {
        return;
    }

    // Serial1 safety check
    if (SerialBridge.isEnabled() == true && (pin == RX || pin == TX))
    {
        return;
    }

    // LZ 修改
    if (Serial.isEnabled() == true && (pin == RX2 || pin == TX2))
    {
        return;
    }

    if (Serial1.isEnabled() == true && (pin == RX1 || pin == TX1))
    {
        return;
    }

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    uint16_t TIM_Prescaler = (uint16_t)(SystemCoreClock / 1000000) - 1;//TIM Counter clock = 1MHz
    uint16_t TIM_CCR = (uint16_t)(1000000 / (2 * frequency));
    int32_t timer_channel_toggle_count = -1;

    // Calculate the toggle count
    if (duration > 0)
    {
        timer_channel_toggle_count = 2 * frequency * duration / 1000;
    }

    PIN_MAP[pin].timer_ccr = TIM_CCR;
    PIN_MAP[pin].user_property = timer_channel_toggle_count;

    // AFIO clock enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    pinMode(pin, AF_OUTPUT_PUSHPULL);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    // TIM clock enable
    if(PIN_MAP[pin].timer_peripheral == TIM2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
        Wiring_TIM2_Interrupt_Handler = TIM2_Tone_Interrupt_Handler;
    }
    else if(PIN_MAP[pin].timer_peripheral == TIM3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
        Wiring_TIM3_Interrupt_Handler = TIM3_Tone_Interrupt_Handler;
    }
    else if(PIN_MAP[pin].timer_peripheral == TIM4)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
        Wiring_TIM4_Interrupt_Handler = TIM4_Tone_Interrupt_Handler;
    }

    NVIC_Init(&NVIC_InitStructure);

    // Time base configuration
    TIM_TimeBaseStructure.TIM_Period = 65535;
    TIM_TimeBaseStructure.TIM_Prescaler = TIM_Prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(PIN_MAP[pin].timer_peripheral, &TIM_TimeBaseStructure);

    // Output Compare Toggle Mode configuration
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_Pulse = TIM_CCR;

    if(PIN_MAP[pin].timer_ch == TIM_Channel_1)
    {
        // Channel1 configuration
        TIM_OC1Init(PIN_MAP[pin].timer_peripheral, &TIM_OCInitStructure);
        TIM_OC1PreloadConfig(PIN_MAP[pin].timer_peripheral, TIM_OCPreload_Disable);
        TIM_ITConfig(PIN_MAP[pin].timer_peripheral, TIM_IT_CC1, ENABLE);
    }
    else if(PIN_MAP[pin].timer_ch == TIM_Channel_2)
    {
        // Channel2 configuration
        TIM_OC2Init(PIN_MAP[pin].timer_peripheral, &TIM_OCInitStructure);
        TIM_OC2PreloadConfig(PIN_MAP[pin].timer_peripheral, TIM_OCPreload_Disable);
        TIM_ITConfig(PIN_MAP[pin].timer_peripheral, TIM_IT_CC2, ENABLE);
    }
    else if(PIN_MAP[pin].timer_ch == TIM_Channel_3)
    {
        // Channel3 configuration
        TIM_OC3Init(PIN_MAP[pin].timer_peripheral, &TIM_OCInitStructure);
        TIM_OC3PreloadConfig(PIN_MAP[pin].timer_peripheral, TIM_OCPreload_Disable);
        TIM_ITConfig(PIN_MAP[pin].timer_peripheral, TIM_IT_CC3, ENABLE);
    }
    else if(PIN_MAP[pin].timer_ch == TIM_Channel_4)
    {
        // Channel4 configuration
        TIM_OC4Init(PIN_MAP[pin].timer_peripheral, &TIM_OCInitStructure);
        TIM_OC4PreloadConfig(PIN_MAP[pin].timer_peripheral, TIM_OCPreload_Disable);
        TIM_ITConfig(PIN_MAP[pin].timer_peripheral, TIM_IT_CC4, ENABLE);
    }

    // TIM enable counter
    TIM_Cmd(PIN_MAP[pin].timer_peripheral, ENABLE);
}

/*********************************************************************************
  *Function      : void noTone(uint8_t pin)
  *Description  : none pwm
  *Input           : pin: pin number
  *Output         : none
  *Return         : none
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
void noTone(uint8_t pin)
{
    if (pin >= TOTAL_PINS || PIN_MAP[pin].timer_peripheral == NULL)
    {
        return;
    }

    if(PIN_MAP[pin].timer_ch == TIM_Channel_1)
    {
        TIM_ITConfig(PIN_MAP[pin].timer_peripheral, TIM_IT_CC1, DISABLE);
    }
    else if(PIN_MAP[pin].timer_ch == TIM_Channel_2)
    {
        TIM_ITConfig(PIN_MAP[pin].timer_peripheral, TIM_IT_CC2, DISABLE);
    }
    else if(PIN_MAP[pin].timer_ch == TIM_Channel_3)
    {
        TIM_ITConfig(PIN_MAP[pin].timer_peripheral, TIM_IT_CC3, DISABLE);
    }
    else if(PIN_MAP[pin].timer_ch == TIM_Channel_4)
    {
        TIM_ITConfig(PIN_MAP[pin].timer_peripheral, TIM_IT_CC4, DISABLE);
    }

    TIM_CCxCmd(PIN_MAP[pin].timer_peripheral, PIN_MAP[pin].timer_ch, TIM_CCx_Disable);
    PIN_MAP[pin].timer_ccr = 0;
    PIN_MAP[pin].user_property = 0;
}

/*********************************************************************************
  *Function      : void TIM2_Tone_Interrupt_Handler(void)
  *Description  : set duty cycle
  *Input          : none
  *Output        : none
  *Return        : none
  *author        : lz
  *date           : 6-December-2013
  *Others        :         
**********************************************************************************/
void TIM2_Tone_Interrupt_Handler(void)
{
    uint16_t capture;

    if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1 );
        capture = TIM_GetCapture1(TIM2);
        TIM_SetCompare1(TIM2, capture + PIN_MAP[30].timer_ccr);
        if(PIN_MAP[30].user_property != -1)
        {
            if (PIN_MAP[30].user_property > 0)
            {
                PIN_MAP[30].user_property -= 1;
            }
            else
            {
                noTone(30);
            }
        }
    }

    if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
        capture = TIM_GetCapture2(TIM2);
        TIM_SetCompare2(TIM2, capture + PIN_MAP[31].timer_ccr);
        if(PIN_MAP[31].user_property != -1)
        {
            if (PIN_MAP[31].user_property > 0)
            {
                PIN_MAP[31].user_property -= 1;
            }
            else
            {
                noTone(31);
            }
        }
    }

    if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
        capture = TIM_GetCapture3(TIM2);
        TIM_SetCompare3(TIM2, capture + PIN_MAP[33].timer_ccr);
        if(PIN_MAP[33].user_property != -1)
        {
            if (PIN_MAP[33].user_property > 0)
            {
                PIN_MAP[33].user_property -= 1;
            }
            else
            {
                noTone(33);
            }
        }
    }

    if (TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
        capture = TIM_GetCapture4(TIM2);
        TIM_SetCompare4(TIM2, capture + PIN_MAP[32].timer_ccr);
        if(PIN_MAP[32].user_property != -1)
        {
            if (PIN_MAP[32].user_property > 0)
            {
                PIN_MAP[32].user_property -= 1;
            }
            else
            {
                noTone(32);
            }
        }
    }
}

/*********************************************************************************
  *Function      : void TIM3_Tone_Interrupt_Handler(void)
  *Description  : set duty cycle
  *Input          : none
  *Output        : none
  *Return        : none
  *author        : lz
  *date           : 6-December-2013
  *Others        :         
**********************************************************************************/
void TIM3_Tone_Interrupt_Handler(void)
{
    uint16_t capture;

    if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 );
        capture = TIM_GetCapture1(TIM3);
        TIM_SetCompare1(TIM3, capture + PIN_MAP[36].timer_ccr);
        if(PIN_MAP[36].user_property != -1)
        {
            if (PIN_MAP[36].user_property > 0)
            {
                PIN_MAP[36].user_property -= 1;
            }
            else
            {
                noTone(36);
            }
        }
    }

    if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
        capture = TIM_GetCapture2(TIM3);
        TIM_SetCompare2(TIM3, capture + PIN_MAP[37].timer_ccr);
        if(PIN_MAP[37].user_property != -1)
        {
            if (PIN_MAP[37].user_property > 0)
            {
                PIN_MAP[37].user_property -= 1;
            }
            else
            {
                noTone(37);
            }
        }
    }

    if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
        capture = TIM_GetCapture3(TIM3);
        TIM_SetCompare3(TIM3, capture + PIN_MAP[38].timer_ccr);
        if(PIN_MAP[38].user_property != -1)
        {
            if (PIN_MAP[38].user_property > 0)
            {
                PIN_MAP[38].user_property -= 1;
            }
            else
            {
                noTone(38);
            }
        }
    }

    if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
        capture = TIM_GetCapture4(TIM3);
        TIM_SetCompare4(TIM3, capture + PIN_MAP[39].timer_ccr);
        if(PIN_MAP[39].user_property != -1)
        {
            if (PIN_MAP[39].user_property > 0)
            {
                PIN_MAP[39].user_property -= 1;
            }
            else
            {
                noTone(39);
            }
        }
    }
}

/*********************************************************************************
  *Function      : void TIM4_Tone_Interrupt_Handler(void)
  *Description  : set duty cycle
  *Input          : none
  *Output        : none
  *Return        : none
  *author        : lz
  *date           : 6-December-2013
  *Others        :         
**********************************************************************************/
void TIM4_Tone_Interrupt_Handler(void)
{
    uint16_t capture;

    if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC1 );
        capture = TIM_GetCapture1(TIM4);
        TIM_SetCompare1(TIM4, capture + PIN_MAP[9].timer_ccr);
        if(PIN_MAP[9].user_property != -1)
        {
            if (PIN_MAP[9].user_property > 0)
            {
                PIN_MAP[9].user_property -= 1;
            }
            else
            {
                noTone(9);
            }
        }
    }

    if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
        capture = TIM_GetCapture2(TIM4);
        TIM_SetCompare2(TIM4, capture + PIN_MAP[8].timer_ccr);
        if(PIN_MAP[8].user_property != -1)
        {
            if (PIN_MAP[8].user_property > 0)
            {
                PIN_MAP[8].user_property -= 1;
            }
            else
            {
                noTone(8);
            }
        }
    }

    if (TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
        capture = TIM_GetCapture2(TIM4);
        TIM_SetCompare2(TIM4, capture + PIN_MAP[6].timer_ccr);
        if(PIN_MAP[6].user_property != -1)
        {
            if (PIN_MAP[6].user_property > 0)
            {
                PIN_MAP[6].user_property -= 1;
            }
            else
            {
                noTone(6);
            }
        }
    }

    if (TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
        capture = TIM_GetCapture2(TIM4);
        TIM_SetCompare2(TIM4, capture + PIN_MAP[7].timer_ccr);
        if(PIN_MAP[7].user_property != -1)
        {
            if (PIN_MAP[7].user_property > 0)
            {
                PIN_MAP[7].user_property -= 1;
            }
            else
            {
                noTone(7);
            }
        }
    }
}

