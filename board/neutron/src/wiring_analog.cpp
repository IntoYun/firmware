/**
******************************************************************************
* @file     : wiring_analog.h
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

#include "wiring_analog.h"
#include "wiring_spi.h"
#include "wiring_usartserial.h"
#include "application.h"

uint8_t adcInitFirstTime = true;
__IO uint32_t uhADCxConvertedValue;

#define useAdcDMA 0
/*********************************************************************************
 *Function     : int32_t analogRead(uint16_t pin)
 *Description  : Read the analog value of a pin.
 *Input        : pin:port number
 *Output       : none
 *Return       : Should return a 16-bit value, 0-65536 (0 = LOW, 65536 = HIGH)
 *author       : lz
 *date         : 6-December-2014
 *Others       : ADC is 12-bit. Currently it returns 0-4095
 **********************************************************************************/


void ADC_DMA_Init( ADC_HandleTypeDef *hadc)
{
    /* A0 - A7: ADC1_IN0 - ADC_IN7, DMA2 Stream0 Channel0 */
    DMA_HandleTypeDef  hdma_adc;

    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable DMA2 clock */
    __HAL_RCC_DMA2_CLK_ENABLE();

    /*##-3- Configure the DMA streams ##########################################*/
    /* Set the parameters to be configured */
    hdma_adc.Instance = DMA2_Stream0;

    hdma_adc.Init.Channel  = DMA_CHANNEL_0;
    hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_adc.Init.Mode = DMA_CIRCULAR;
    hdma_adc.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_adc.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    hdma_adc.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    hdma_adc.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_adc.Init.PeriphBurst = DMA_PBURST_SINGLE;

    HAL_DMA_Init(&hdma_adc);

    /* Associate the initialized DMA handle to the ADC handle */
    __HAL_LINKDMA(hadc, DMA_Handle, hdma_adc);

    // /*##-4- Configure the NVIC for DMA #########################################*/
    // /* NVIC configuration for DMA transfer complete interrupt */
    // HAL_NVIC_SetPriority(ADCx_DMA_IRQn, 0, 0);
    // HAL_NVIC_EnableIRQ(ADCx_DMA_IRQn);
}



int32_t analogRead(uint16_t pin)
{
    if(  ((pin >= A0)&&(pin <= A7)) || (pin==PO188) )
    {
        if( (SPI.isEnabled() == true ) && ((pin == PIN_SPI_SS) ||  (pin == PIN_SPI_SCK) || (pin == PIN_SPI_MISO) || (pin == PIN_SPI_MOSI) ))
        {
            return LOW;
        }

        if( (Serial.isEnabled() == true) && ( (pin == RX) || (pin == TX) ))
        {
            return LOW;
        }
        // initial the pin
        /* A0 - A7: ADC1_IN0 - ADC_IN7, DMA2 Stream0 Channel0 */
        GPIO_InitTypeDef          GPIO_InitStruct;

        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* ADC1 Periph clock enable */
        __HAL_RCC_ADC1_CLK_ENABLE();
        /* Enable GPIO clock ****************************************/
        __HAL_RCC_GPIOA_CLK_ENABLE();

        if(pin==PO188)
        {
        	__HAL_RCC_GPIOB_CLK_ENABLE();
        }

        /*##-2- Configure peripheral GPIO ##########################################*/
        /* ADC Channel GPIO pin configuration */
        GPIO_InitStruct.Pin = PIN_MAP[pin].gpio_pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(PIN_MAP[pin].gpio_peripheral, &GPIO_InitStruct);
        // if (adcInitFirstTime == true)
        // {
        ADC_HandleTypeDef AdcHandle;
#if(useAdcDMA)
        ADC_DMA_Init(&AdcHandle);
#endif
        ADC_ChannelConfTypeDef sConfig;

        AdcHandle.Instance = ADC1;

        AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;   // F411 the maximum ADCCLK = 36M, typical ADCCLK = 30M, the ADCCLK set to 24M
        AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
        AdcHandle.Init.ScanConvMode          = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
#if(useAdcDMA)
        AdcHandle.Init.ContinuousConvMode    = ENABLE;                        /* Continuous mode disabled to have only 1 conversion at each conversion trig */
#endif
#if(~useAdcDMA)
        AdcHandle.Init.ContinuousConvMode    = DISABLE;
#endif
        AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
        AdcHandle.Init.NbrOfDiscConversion   = 0;
        AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;        /* Conversion start trigged at each external event */
        AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
        AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;

#if(useAdcDMA)
        AdcHandle.Init.NbrOfConversion       = 1;
        AdcHandle.Init.DMAContinuousRequests = ENABLE;
#endif
#if(~useAdcDMA)
        AdcHandle.Init.NbrOfConversion       = 0;
        AdcHandle.Init.DMAContinuousRequests = DISABLE;
#endif
        AdcHandle.Init.EOCSelection          = DISABLE;

        if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
        {
            /* ADC initialization Error */
            // Error_Handler();
            while(1)
            {}
        }

        /*##-2- Configure ADC regular channel ######################################*/
        sConfig.Channel      = PIN_MAP[pin].adc_channel;
        sConfig.Rank         = 1;
        sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
        sConfig.Offset       = 0;

        if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
        {
            /* Channel Configuration Error */
            // Error_Handler();
            while(1)
            {}
        }

#if(useAdcDMA)
        /*##-3- Start the conversion process #######################################*/
        /* Note: Considering IT occurring after each number of ADC conversions      */
        /*       (IT by DMA end of transfer), select sampling time and ADC clock    */
        /*       with sufficient duration to not create an overhead situation in    */
        /*        IRQHandler. */
        if(HAL_ADC_Start_DMA(&AdcHandle, (uint32_t*)&uhADCxConvertedValue, 1) != HAL_OK)
        {
            /* Start Conversation Error */
            // Error_Handler();
            while(1)
            {}
        }

        /*##-3- Start the conversion process #######################################*/
        if (HAL_ADC_Start(&AdcHandle) != HAL_OK)
        {
            /* Start Conversation Error */
            Error_Handler();
        }
        /*##-4- Wait for the end of conversion #####################################*/
        /*  Before starting a new conversion, you need to check the current state of
            the peripheral; if it聮s busy you need to wait for the end of current
            conversion before starting a new one.
            For simplicity reasons, this example is just waiting till the end of the
            conversion, but application may perform other tasks while conversion
            operation is ongoing. */
        if (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK)
        {
            /* End Of Conversion flag not set on time */
            Error_Handler();
        }
/* Check if the continuous conversion of regular channel is finished */
        if ((HAL_ADC_GetState(&AdcHandle) & HAL_ADC_STATE_EOC_REG) == HAL_ADC_STATE_EOC_REG)
        {
            /*##-5- Get the converted value of regular channel  ########################*/
            uhADCxConvertedValue = HAL_ADC_GetValue(&AdcHandle);
        }
#endif

#if(~useAdcDMA)
        /*##-3- Start the conversion process #######################################*/
        if (HAL_ADC_Start(&AdcHandle) != HAL_OK)
        {
            /* Start Conversation Error */
            // Error_Handler();
            while(1)
            {}
        }

        /*##-4- Wait for the end of conversion #####################################*/
        /*  Before starting a new conversion, you need to check the current state of
            the peripheral; if it聮s busy you need to wait for the end of current
            conversion before starting a new one.
            For simplicity reasons, this example is just waiting till the end of the
            conversion, but application may perform other tasks while conversion
            operation is ongoing. */
        if (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK)
        {
            /* End Of Conversion flag not set on time */
            //Error_Handler();
            while(1)
            {}
        }

        /* Check if the continuous conversion of regular channel is finished */
        if ((HAL_ADC_GetState(&AdcHandle) & HAL_ADC_STATE_EOC_REG) == HAL_ADC_STATE_EOC_REG)
        {
            /*##-5- Get the converted value of regular channel  ########################*/
            uhADCxConvertedValue = HAL_ADC_GetValue(&AdcHandle);
        }
#endif
        // adcTinitFirstTime = false;
        return uhADCxConvertedValue;

    }
    else
    {
        //return error;
        while(1)
        {}
    }
}

/*********************************************************************************
 *Function     : void analogWrite(uint16_t pin, uint8_t value)
 *Description  : Should take an integer 0-255 and create a PWM signal with a duty cycle from 0-100%.
 *Input        : pin:port number
value:0-255
*Output       : none
*Return       : none
*author       : lz
*date         : 6-December-2014
*Others       : TIM_PWM_FREQ is set at 500 Hz
**********************************************************************************/
void analogWrite(uint16_t pin, uint8_t value)
{
    if(PIN_MAP[pin].timer_peripheral != NULL && PIN_MAP[pin].timer_peripheral != TIM1)
    {
        if( (SPI.isEnabled() == true ) && ( (pin == PIN_SPI_SCK) || (pin == PIN_SPI_MISO) || (pin == PIN_SPI_MOSI) ))
        {
            // Error
            while(1)
            {}
        }

        if( (Serial.isEnabled() == true) && ( (pin == RX) || (pin == TX) ))
        {
            while(1)
            {}
            // Error
        }

        /* Common configuration for all channles */
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

        /* Timer Clock enable */
        // if( (PIN_MAP[pin].timer_peripheral == TIM1) )
        // {
        //     __HAL_RCC_TIM1_CLK_ENABLE();

        //     GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
        //     GPIO_InitStruct.Pin       = PIN_MAP[pin].gpio_pin;
        //     /* Port Clock enable */
        //     if( (PIN_MAP[pin].gpio_peripheral == GPIOA) )
        //     {
        //         // __HAL_RCC_GPIOA_CLK_DISABLE();
        //         __HAL_RCC_GPIOA_CLK_ENABLE();
        //         HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        //     }
        //     else if( (PIN_MAP[pin].gpio_peripheral == GPIOB) )
        //     {
        //         // __HAL_RCC_GPIOB_CLK_DISABLE();
        //         __HAL_RCC_GPIOB_CLK_ENABLE();
        //         HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        //     }

        // }
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
        }


        /*##-1- Configure the TIM peripheral #######################################*/
        uint32_t TIM_Prescaler = (uint32_t)((SystemCoreClock / 24000000) - 1);

        uint32_t TIM_ARR = (uint32_t)((24000000 / 500) - 1);
        uint32_t TIM_CCR = (uint32_t)(value * (TIM_ARR + 1) / 255);

        TIM_HandleTypeDef TimHandle;
        TIM_OC_InitTypeDef sConfig;

        TimHandle.Instance = PIN_MAP[pin].timer_peripheral;

        TimHandle.Init.Prescaler         = TIM_Prescaler;
        TimHandle.Init.Period            = TIM_ARR;
        TimHandle.Init.ClockDivision     = 0;
        TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
        TimHandle.Init.RepetitionCounter = 0;
        if (HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
        {
            // Error
            while(1)
            {}
        }

        /*##-2- Configure the PWM channels #b########################################*/
        /* Common configuration for all channels */
        sConfig.OCMode       = TIM_OCMODE_PWM1;
        sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
        sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
        sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;

        // if(PIN_MAP[pin].timer_peripheral == TIM1)
        // {
        //     sConfig.OCIdleState  = TIM_OCIDLESTATE_SET;
        //     sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
        // }

        /* Set the pulse value for channel 1 */
        sConfig.Pulse = TIM_CCR;
        if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, PIN_MAP[pin].timer_ch) != HAL_OK)
        {
            /* Configuration Error */
            //Error_Handler();
            while(1)
            {}
        }

        /* Start channel */
        if (HAL_TIM_PWM_Start(&TimHandle, PIN_MAP[pin].timer_ch) != HAL_OK)
        {
            /* PWM Generation Error */
            //Error_Handler();
            while(1)
            {}
        }
    }
    else
    {
        // Error
        while(1)
        {}
    }
}

/*********
 * TIMING
 *********/
void analogWriteAndSetF(uint16_t pin, uint8_t value,uint32_t fre)
{

}
