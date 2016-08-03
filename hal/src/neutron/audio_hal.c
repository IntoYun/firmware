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
#include "i2s_hal.h"
#include "stm32f4xx.h"
#include "pdm_filter.h"
#include "service_debug.h"
#include "audio_hal.h"

extern I2S_HandleTypeDef *sensorI2SHandle;
/* Current state of the audio recorder interface intialization */
static uint32_t AudioRecInited = 0;
PDMFilter_InitStruct Filter;

/* Temporary data sample */
uint16_t InternalBuffer[128];
static uint32_t InternalBufferSize = 0;
uint16_t I2SPdmBuf[128];
uint32_t I2SPdmBufSize = 0;
uint8_t *p_send=(uint8_t *) &I2SPdmBuf[0];

/**
 * @brief  Initialize wave recording
 * @param  AudioFreq: Sampling frequency
 *         BitRes: Audio recording Samples format (from 8 to 16 bits)
 *         ChnlNbr: Number of input microphone channel
 * @retval None
 */
uint32_t WaveRecorderInit(uint32_t AudioFreq, uint32_t BitRes, uint32_t ChnlNbr)
{
    /* Check if the interface is already initialized */
    if(AudioRecInited)
    {
        /* No need for initialization */
        return 0;
    }
    else
    {
        RCC_PeriphCLKInitTypeDef rcc_ex_clk_init_struct;
        HAL_RCCEx_GetPeriphCLKConfig(&rcc_ex_clk_init_struct);
        rcc_ex_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_I2S;//RCC_I2SCLKSOURCE_PLLI2S;//RCC_PERIPHCLK_I2S;//I2S_CLOCK_PLL;//RCC_PERIPHCLK_I2S;//RCC_I2SCLKSOURCE_PLLI2S;// //;
        rcc_ex_clk_init_struct.PLLI2S.PLLI2SN = 429;//256;//131;//192;
        rcc_ex_clk_init_struct.PLLI2S.PLLI2SR = 6;//5;//2;//2;//2;
        HAL_RCCEx_PeriphCLKConfig(&rcc_ex_clk_init_struct);

        // /* Configure the PDM library */
        // /* Enable CRC module */
        __CRC_CLK_ENABLE();

        /* Filter LP & HP Init */
        uint32_t audio_freq = 16000;
        Filter.LP_HZ = audio_freq / 2;
        Filter.HP_HZ = 10;
        Filter.Fs = audio_freq;
        Filter.Out_MicChannels = 1;
        Filter.In_MicChannels = 1;

        PDM_Filter_Init((PDMFilter_InitStruct *)&Filter);

        /////////////////////////////////////////////////////////////////////////
        /* Configure the I2S peripheral */
        HAL_I2S_Begin(HAL_I2S_INTERFACE2, audio_freq); // select the sensor I2S

        /* Set state of the audio recorder to initialized */
        AudioRecInited = 1;

        HAL_I2S_Receive_DMA(sensorI2SHandle, (uint16_t *)&InternalBuffer[0], INTERNAL_BUFF_SIZE);
        /* Return 0 if all operations are OK */
        return 0;
    }
}

uint8_t sd_buf[4096];
int sd_idx = 0;
static mic_mems_t *p_mic_mems;

void HAL_Audio_Begin(mic_mems_t *p)
{
    p_mic_mems = p;
    if(WaveRecorderInit( 0, 0, 0))
    {
        DEBUG("Error WaveRecorderInit");
    }
}

void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    for( int i = 0; i < INTERNAL_BUFF_SIZE; i++)
    {
        InternalBuffer[i] = HTONS(InternalBuffer[i]);
    }
    PDM_Filter_64_LSB((uint8_t *)InternalBuffer, (uint16_t *)I2SPdmBuf, 64, (PDMFilter_InitStruct *)&Filter);

    for(int i = 1; i < 32; i += 4)
    {
       sd_buf[sd_idx++] = p_send[i];
    }
    if(sd_idx == 2048)
    {
        if(p_mic_mems->status_buf == 0)
        {
            p_mic_mems->status_buf = 1;
            p_mic_mems->P_buf = sd_buf;
        }
    }
    else if(sd_idx == 4096)
    {
        sd_idx = 0;
        if(p_mic_mems->status_buf == 0)
        {
            p_mic_mems->status_buf = 1;
            p_mic_mems->P_buf = sd_buf + sizeof(sd_buf)/2;
        }
    }
}
