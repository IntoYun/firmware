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
#include "adc_hal.h"
#include "gpio_hal.h"
#include "pinmap_hal.h"
#include "pinmap_impl.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

#define ADC_DMA_BUFFERSIZE  10
#define ADC_SAMPLING_TIME ADC_SAMPLETIME_3CYCLES

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/*
__IO uint16_t ADC_ConvertedValues[ADC_DMA_BUFFERSIZE];
uint8_t adcInitFirstTime = true;
uint8_t adcChannelConfigured = ADC_CHANNEL_NONE;
static uint8_t ADC_Sample_Time = ADC_SAMPLING_TIME;
ADC_HandleTypeDef ADC_HandleStruct;
DMA_HandleTypeDef DMA_HandleStruct;
*/
/* Extern variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/*
 * @brief  Override the default ADC Sample time depending on requirement
 * @param  ADC_SampleTime: The sample time value to be set.
 * This parameter can be one of the following values:
 * @arg ADC_SampleTime_3Cycles: Sample time equal to 3 cycles
 * @arg ADC_SampleTime_15Cycles: Sample time equal to 15 cycles
 * @arg ADC_SampleTime_28Cycles: Sample time equal to 28 cycles
 * @arg ADC_SampleTime_56Cycles: Sample time equal to 56 cycles
 * @arg ADC_SampleTime_84Cycles: Sample time equal to 84 cycles
 * @arg ADC_SampleTime_112Cycles: Sample time equal to 112 cycles
 * @arg ADC_SampleTime_144Cycles: Sample time equal to 144 cycles
 * @arg ADC_SampleTime_480Cycles: Sample time equal to 480 cycles
 * @retval None
 */

void HAL_ADC_Set_Sample_Time(uint8_t ADC_SampleTime)
{
    /*
  if(ADC_SampleTime < ADC_SampleTime_3Cycles || ADC_SampleTime > ADC_SampleTime_480Cycles)
  {
    ADC_Sample_Time = ADC_SAMPLING_TIME;
  }
  else
  {
    ADC_Sample_Time = ADC_SampleTime;
  }*/
}

/*
 * @brief Read the analog value of a pin.
 * Should return a 16-bit value, 0-65536 (0 = LOW, 65536 = HIGH)
 * Note: ADC is 12-bit. Currently it returns 0-4096
 */
int32_t HAL_ADC_Read(uint16_t pin)
{
#if 0
    STM32_Pin_Info* PIN_MAP = HAL_Pin_Map();

    if (PIN_MAP[pin].pin_mode != AN_INPUT)
    {
        HAL_GPIO_Save_Pin_Mode(pin);
        HAL_Pin_Mode(pin, AN_INPUT);
    }

    if (adcInitFirstTime == true)
    {
        HAL_ADC_DMA_Init();
        adcInitFirstTime = false;
    }

    if (adcChannelConfigured != PIN_MAP[pin].adc_channel)
    {
        adcChannelConfigured = PIN_MAP[pin].adc_channel;

        __HAL_RCC_ADC1_CLK_ENABLE();
        /* Configure the ADC peripheral */
        ADC_HandleStruct.Instance = ADC1;

        ADC_HandleStruct.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
        ADC_HandleStruct.Init.Resolution = ADC_RESOLUTION_12B;
        ADC_HandleStruct.Init.ScanConvMode = DISABLE;
        ADC_HandleStruct.Init.ContinuousConvMode = ENABLE;
        ADC_HandleStruct.Init.DiscontinuousConvMode = DISABLE;
        ADC_HandleStruct.Init.NbrOfDiscConversion = 0;
        ADC_HandleStruct.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
        ADC_HandleStruct.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
        ADC_HandleStruct.Init.DataAlign = ADC_DATAALIGN_RIGHT;
        ADC_HandleStruct.Init.NbrOfConversion = 1;
        ADC_HandleStruct.Init.DMAContinuousRequests = ENABLE;
        ADC_HandleStruct.Init.EOCSelection = DISABLE;

        HAL_ADC_Init(&ADC_HandleStruct);

        /* Configure ADC regular channel */
        ADC_ChannelConfTypeDef ADC_ChannelConfStruct;

        ADC_ChannelConfStruct.Channel = PIN_MAP[pin].adc_channel;
        ADC_ChannelConfStruct.Rank = 1;
        ADC_ChannelConfStruct.SamplingTime = ADC_Sample_Time;
        ADC_ChannelConfStruct.Offset = 0;

        HAL_ADC_ConfigChannel(&ADC_HandleStruct, &ADC_ChannelConfStruct);
    }

    memset(ADC_ConvertedValues, 0, sizeof(ADC_ConvertedValues));

    /* Start the conversion process */
    HAL_ADC_Start(&ADC_HandleStruct);

    /* Start the conversion process */
    HAL_ADC_Start_DMA(&ADC_HandleStruct, (uint32_t*)&ADC_ConvertedValues, ADC_DMA_BUFFERSIZE);

    /* Wait for the end of conversion */
    HAL_ADC_PollForConversion(&ADC_HandleStruct, 10);

    /* Check if the continuous conversion of regular channel is finished */
    while((HAL_ADC_GetState(&ADC_HandleStruct) & HAL_ADC_STATE_EOC_REG) != HAL_ADC_STATE_EOC_REG);

    HAL_ADC_Stop(&ADC_HandleStruct);

    HAL_ADC_Stop_DMA(&ADC_HandleStruct);

    uint32_t ADC_SummatedValue = 0;
    uint16_t ADC_AveragedValue = 0;

    for(i = 0; i < ADC_DMA_BUFFERSIZE; i++)
    {
        ADC_SummatedValue += ADC_ConvertedValues[i];
    }

    ADC_AveragedValue = (uint16_t)(ADC_SummatedValue / ADC_DMA_BUFFERSIZE);

    // Return ADC averaged value
    return ADC_AveragedValue;
#endif
    return 0;
}

/*
 * @brief Initialize the ADC peripheral.
 */
void HAL_ADC_DMA_Init(void)
{
#if 0
    /* Enable DMA2 clock */
    __HAL_RCC_DMA2_CLK_ENABLE();

    /* Configure the DMA streams */
    DMA_HandleStruct.Instance = DMA2_Stream0;

    DMA_HandleStrcut.Init.Channel  = DMA_CHANNEL_0;
    DMA_HandleStrcut.Init.Direction = DMA_PERIPH_TO_MEMORY;
    DMA_HandleStrcut.Init.PeriphInc = DMA_PINC_DISABLE;
    DMA_HandleStrcut.Init.MemInc = DMA_MINC_ENABLE;
    DMA_HandleStrcut.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    DMA_HandleStrcut.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    DMA_HandleStrcut.Init.Mode = DMA_CIRCULAR;
    DMA_HandleStrcut.Init.Priority = DMA_PRIORITY_HIGH;
    DMA_HandleStrcut.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    DMA_HandleStrcut.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    DMA_HandleStrcut.Init.MemBurst = DMA_MBURST_SINGLE;
    DMA_HandleStrcut.Init.PeriphBurst = DMA_PBURST_SINGLE;

    HAL_DMA_Init(&DMA_HandleStrcut);

    /* Associate the initialized DMA handle to the ADC handle */
    __HAL_LINKDMA(ADC_HandleStruct, DMA_Handle, DMA_HandleStrcut);

    /* Configure the NVIC for DMA */
    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
#endif
}
