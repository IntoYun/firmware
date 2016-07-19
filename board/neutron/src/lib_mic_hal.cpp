#include "lib_system_all.h"
#include "pdm_filter.h"
#include "lib_mic_hal.h"

I2S_HandleTypeDef I2sHandle;

/* Current state of the audio recorder interface intialization */
static uint32_t AudioRecInited = 0;
PDMFilter_InitStruct Filter;

/* Temporary data sample */
uint16_t InternalBuffer[128];

static uint32_t InternalBufferSize = 0;

uint16_t I2SPdmBuf[128];
uint32_t I2SPdmBufSize = 0;

uint8_t *p_send=(uint8_t *) &I2SPdmBuf[0];

static void I2S2_MspInit()
{
    static DMA_HandleTypeDef hdma_i2s_rx;
    GPIO_InitTypeDef  gpio_init_structure;

    /* Enable I2S clock */
    __HAL_RCC_SPI2_CLK_ENABLE();

    /* Enable SCK and SD GPIO clock */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* CODEC_I2S pins configuration: SCK and SD pins */
    gpio_init_structure.Mode = GPIO_MODE_AF_PP;
    gpio_init_structure.Pull = GPIO_NOPULL;
    gpio_init_structure.Speed = GPIO_SPEED_FAST;

    gpio_init_structure.Pin = GPIO_PIN_10;
    gpio_init_structure.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &gpio_init_structure);

    gpio_init_structure.Pin = GPIO_PIN_15;
    gpio_init_structure.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &gpio_init_structure);

    /* Enable the DMA clock */
    __HAL_RCC_DMA1_CLK_ENABLE();

    if(I2sHandle.Instance == SPI2)
    {
        /* Configure the hdma_i2s_rx handle parameters */
        hdma_i2s_rx.Init.Channel             = DMA_CHANNEL_0;
        hdma_i2s_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma_i2s_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_i2s_rx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_i2s_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_i2s_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        hdma_i2s_rx.Init.Mode                = DMA_CIRCULAR;//DMA_NORMAL;//DMA_CIRCULAR;
        hdma_i2s_rx.Init.Priority            = DMA_PRIORITY_HIGH;
        hdma_i2s_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        hdma_i2s_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
        hdma_i2s_rx.Init.MemBurst            = DMA_MBURST_SINGLE;
        hdma_i2s_rx.Init.PeriphBurst         = DMA_MBURST_SINGLE;

        hdma_i2s_rx.Instance = DMA1_Stream3;

        /* Associate the DMA handle */
        __HAL_LINKDMA(&I2sHandle, hdmarx, hdma_i2s_rx);

        /* Deinitialize the Stream for new transfer */
        HAL_DMA_DeInit(&hdma_i2s_rx);

        /* Configure the DMA Stream */
        HAL_DMA_Init(&hdma_i2s_rx);
    }

    /* I2S DMA IRQ Channel configuration */
    HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
    //__DMA
}


static void I2S2_Init(uint32_t AudioFreq)
{
    /* Initialize the hAudioInI2s Instance parameter */
    I2sHandle.Instance = SPI2;

    /* Disable I2S block */
    __HAL_I2S_DISABLE(&I2sHandle);

    /* I2S2 peripheral configuration */
    I2sHandle.Init.AudioFreq = 2*AudioFreq;//32000;//192000;////32000;//192000;//2 * AudioFreq;
    I2sHandle.Init.ClockSource = I2S_CLOCK_PLL;
    I2sHandle.Init.CPOL = I2S_CPOL_HIGH;
    I2sHandle.Init.DataFormat = I2S_DATAFORMAT_16B;
    I2sHandle.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
    I2sHandle.Init.Mode = I2S_MODE_MASTER_RX;
    I2sHandle.Init.Standard = I2S_STANDARD_LSB;

    /* Initialize the I2S peripheral with the structure above */
    if(HAL_I2S_GetState(&I2sHandle) == HAL_I2S_STATE_RESET)
    {
        I2S2_MspInit();
    }

    /* Init the I2S */
    HAL_I2S_Init(&I2sHandle);
}

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
        ////////////////////////////////////////////////////////////////////////
        RCC_PeriphCLKInitTypeDef rcc_ex_clk_init_struct;
        HAL_RCCEx_GetPeriphCLKConfig(&rcc_ex_clk_init_struct);
        rcc_ex_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_I2S;//RCC_I2SCLKSOURCE_PLLI2S;//RCC_PERIPHCLK_I2S;//I2S_CLOCK_PLL;//RCC_PERIPHCLK_I2S;//RCC_I2SCLKSOURCE_PLLI2S;// //;
        rcc_ex_clk_init_struct.PLLI2S.PLLI2SN = 429;//256;//131;//192;
        rcc_ex_clk_init_struct.PLLI2S.PLLI2SR = 6;//5;//2;//2;//2;
        HAL_RCCEx_PeriphCLKConfig(&rcc_ex_clk_init_struct);

        // /* Configure the PDM library */
        // PDMDecoder_Init(AudioFreq, ChnlNbr);
        ///////////////////////////////////// /////////////////////////////////
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
        I2S2_Init(audio_freq);

        MO_INFO(("--------------"));
        /* Set state of the audio recorder to initialized */
        AudioRecInited = 1;

        HAL_I2S_Receive_DMA(&I2sHandle, (uint16_t *)&InternalBuffer[0], INTERNAL_BUFF_SIZE);
        /* Return 0 if all operations are OK */
        return 0;
    }
}


/**
 * @brief  This function handles AUDIO_REC_SPI global interrupt request.
 * @param  None
 * @retval None
 */

uint8_t sd_buf[4096];
int sd_idx = 0;
static mic_mems_t *p_mic_mems;

void MicClass_Start_Hal(mic_mems_t *p)
{
    p_mic_mems=p;
    if(WaveRecorderInit(0,0,0))
    {
        MO_ERROR(("error WaveRecorderInit"));
    }

}

// void MicClass_Stop_Hal()
// {
//   if(WaveRecorderStop())
//   {
//     MO_ERROR(("WaveRecorderStop"));
//   }
// }

void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
    // for( int i = 0; i < INTERNAL_BUFF_SIZE; i++)
    // {
    //     InternalBuffer[i] = HTONS(InternalBuffer[i]);
    // }
    // PDM_Filter_64_LSB((uint8_t *)InternalBuffer, (uint16_t *)I2SPdmBuf, 64, (PDMFilter_InitStruct *)&Filter);

    // for(int i=1; i < 32; i += 4)
    // {
    //     sd_buf[sd_idx++] = p_send[i];
    // }

    // if(sd_idx == 2048)
    // {
    //     if(p_mic_mems->status_buf == 0)
    //     {
    //         p_mic_mems->status_buf = 1;
    //         p_mic_mems->P_buf = sd_buf;
    //     }
    // }
    // else if(sd_idx == 4096)
    // {
    //     sd_idx = 0;
    //     if(p_mic_mems->status_buf == 0)
    //     {
    //         p_mic_mems->status_buf = 1;
    //         p_mic_mems->P_buf = sd_buf + sizeof(sd_buf)/2;
    //     }
    // }
}


void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    // for( int i = 0; i < INTERNAL_BUFF_SIZE; i++)
    // {
    //     InternalBuffer[i + INTERNAL_BUFF_SIZE] = HTONS(InternalBuffer[i + INTERNAL_BUFF_SIZE]);
    // }
    // PDM_Filter_64_LSB((uint8_t *)(InternalBuffer + INTERNAL_BUFF_SIZE), (uint16_t *)I2SPdmBuf, 64, (PDMFilter_InitStruct *)&Filter);

    // for(int i=1; i < 32; i += 4)
    // {
    //     sd_buf[sd_idx++] = p_send[i];
    // }

    // if(sd_idx == 2048)
    // {
    //     if(p_mic_mems->status_buf == 0)
    //     {
    //         p_mic_mems->status_buf = 1;
    //         p_mic_mems->P_buf = sd_buf;
    //     }
    // }
    // else if(sd_idx == 4096)
    // {
    //     sd_idx = 0;
    //     if(p_mic_mems->status_buf == 0)
    //     {
    //         p_mic_mems->status_buf = 1;
    //         p_mic_mems->P_buf = sd_buf + sizeof(sd_buf)/2;
    //     }
    // }



    for( int i = 0; i < INTERNAL_BUFF_SIZE; i++)
    {
        InternalBuffer[i] = HTONS(InternalBuffer[i]);
    }
    PDM_Filter_64_LSB((uint8_t *)InternalBuffer, (uint16_t *)I2SPdmBuf, 64, (PDMFilter_InitStruct *)&Filter);

    for(int i = 1; i < 32; i += 4)
    {
       sd_buf[sd_idx++] = p_send[i];

    }
    // for(int i = 0; i < 32; i ++)
    // {
    //    sd_buf[sd_idx++] = p_send[i];
    // }

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
