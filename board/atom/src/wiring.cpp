/**
 ******************************************************************************
 * @file     : wiring.cpp
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

#include "wiring.h"
#include "wiring_interrupts.h"
#include "wiring_usartserial.h"
#include "wiring_spi.h"
#include "wiring_i2c.h"
#include "intorobot_api.h"
#include "openwrt_pin.h"

/*
 * Globals
 */
__IO uint32_t ADC_DualConvertedValues[ADC_DMA_BUFFERSIZE];
uint8_t adcInitFirstTime = true;
uint8_t adcChannelConfigured = NONE;
static  uint8_t ADC_Sample_Time = ADC_SAMPLING_TIME;

PinMode  digitalPinModeSaved = (PinMode)NONE;


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
    //E0~E5  and  other  exclude D0~D24  A0~A15
    if(PIN_MAP[pin].gpio_peripheral == NULL)
    {
        if((pin >= E0)&&(pin <= E5))
        {
            switch (setMode)
            {
                case OUTPUT:
                case AF_OUTPUT_PUSHPULL:
                case AF_OUTPUT_DRAIN:
                    PIN_MAP[pin].pin_mode = OUTPUT;
                    OpenwrtPin.openwrt_pinmode(pin, OUTPUT);
                    break;
                case INPUT:
                case INPUT_PULLUP:
                case INPUT_PULLDOWN:
                case AN_INPUT:
                    PIN_MAP[pin].pin_mode = INPUT;
                    OpenwrtPin.openwrt_pinmode(pin, INPUT);
                    break;
                default:
                    break;
            }
        }
        return;
    }

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

    GPIO_TypeDef *gpio_port = PIN_MAP[pin].gpio_peripheral;
    uint16_t gpio_pin = PIN_MAP[pin].gpio_pin;

    GPIO_InitTypeDef GPIO_InitStructure;

    if (gpio_port == GPIOA )
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    }
    else if (gpio_port == GPIOB )
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    }
    else if (gpio_port == GPIOC)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    }
    else if (gpio_port == GPIOD )
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    }

    GPIO_InitStructure.GPIO_Pin = gpio_pin;

    switch (setMode)
    {
        case OUTPUT:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            PIN_MAP[pin].pin_mode = OUTPUT;
            break;

        case INPUT:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
            PIN_MAP[pin].pin_mode = INPUT;
            break;

        case INPUT_PULLUP:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
            PIN_MAP[pin].pin_mode = INPUT_PULLUP;
            break;

        case INPUT_PULLDOWN:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
            PIN_MAP[pin].pin_mode = INPUT_PULLDOWN;
            break;

        case AF_OUTPUT_PUSHPULL:	//Used internally for Alternate Function Output PushPull(TIM, UART, SPI etc)
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            PIN_MAP[pin].pin_mode = AF_OUTPUT_PUSHPULL;
            break;

        case AF_OUTPUT_DRAIN:		//Used internally for Alternate Function Output Drain(I2C etc)
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            PIN_MAP[pin].pin_mode = AF_OUTPUT_DRAIN;
            break;

        case AN_INPUT:				//Used internally for ADC Input
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
            PIN_MAP[pin].pin_mode = AN_INPUT;
            break;

        default:
            break;
    }
    GPIO_Init(gpio_port, &GPIO_InitStructure);
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
    if (pin >= TOTAL_PINS || PIN_MAP[pin].pin_mode == INPUT
    || PIN_MAP[pin].pin_mode == INPUT_PULLUP || PIN_MAP[pin].pin_mode == INPUT_PULLDOWN
    || PIN_MAP[pin].pin_mode == AN_INPUT || PIN_MAP[pin].pin_mode == NONE)
    {
        return;
    }

    //E0~E5  and  other  exclude D0~D24  A0~A15
    if(PIN_MAP[pin].gpio_peripheral == NULL)
    {
        if((pin >= E0)&&(pin <= E5))
        {
            if(value != LOW)
            {value = HIGH;}
            OpenwrtPin.openwrt_digital_write(pin, value);
        }
        return;
    }
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

    //If the pin is used by analogWrite, we need to change the mode
    if(PIN_MAP[pin].pin_mode == AF_OUTPUT_PUSHPULL)
    {
        pinMode(pin, OUTPUT);
    }

    if (value == LOW)
    {
        PIN_MAP[pin].gpio_peripheral->BRR = PIN_MAP[pin].gpio_pin;
    } 
    else
    {
        PIN_MAP[pin].gpio_peripheral->BSRR = PIN_MAP[pin].gpio_pin;
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
    if (pin >= TOTAL_PINS || PIN_MAP[pin].pin_mode == NONE
    || PIN_MAP[pin].pin_mode == AF_OUTPUT_PUSHPULL || PIN_MAP[pin].pin_mode == AF_OUTPUT_DRAIN)
    {
        return LOW;
    }

    //E0~E5  and  other  exclude D0~D24  A0~A15
    if(PIN_MAP[pin].gpio_peripheral == NULL)
    {
        if((pin >= E0)&&(pin <= E5))
        {
            return OpenwrtPin.openwrt_digital_read(pin);
        }
        return LOW;
    }

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

    if(PIN_MAP[pin].pin_mode == AN_INPUT)
    {
        if(digitalPinModeSaved == NONE)
        {
            return LOW;
        }
        else
        {
            //Restore the PinMode after calling analogRead on same pin earlier
            pinMode(pin, digitalPinModeSaved);
        }
    }

    if(PIN_MAP[pin].pin_mode == OUTPUT)
    {
        return GPIO_ReadOutputDataBit(PIN_MAP[pin].gpio_peripheral, PIN_MAP[pin].gpio_pin);
    }

    return GPIO_ReadInputDataBit(PIN_MAP[pin].gpio_peripheral, PIN_MAP[pin].gpio_pin);
}

/*********************************************************************************
  *Function     : void ADC_DMA_Init()
  *Description  : Initialize the ADC-DMA peripheral.
  *Input        : none
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       :         
**********************************************************************************/                    
void ADC_DMA_Init()
{
    //Using "Dual Slow Interleaved ADC Mode" to achieve higher input impedance

    ADC_InitTypeDef ADC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;

    // ADCCLK = PCLK2/6 = 72/6 = 12MHz
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    // Enable DMA1 clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    // Enable ADC1 and ADC2 clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2, ENABLE);

    // DMA1 channel1 configuration
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS; // 数据来源地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_DualConvertedValues; // 存储数据的地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; // 外设寄存器作为数据源
    DMA_InitStructure.DMA_BufferSize = ADC_DMA_BUFFERSIZE; // 需传输的数据大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // 外设地址不自增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // 存储器数据递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; // 数据宽度为字
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word; // 数据宽度为字
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; // 正常缓存
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; // 通道优先级高
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; // 非存储器到存储器传输模式
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    // ADC1 configuration
    ADC_InitStructure.ADC_Mode = ADC_Mode_SlowInterl; // adc1 慢速交替模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE; // 扫描模式是多通道还是单通道 1 多通道
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // 转换是连续还是单次 1连续
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 外部触发启动 此处是软件触发 非外部
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // ADC数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    // ADC2 configuration
    ADC_InitStructure.ADC_Mode = ADC_Mode_SlowInterl; // 慢速交替模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC2, &ADC_InitStructure);

    // Enable ADC1
    ADC_Cmd(ADC1, ENABLE);

    // Enable ADC1 reset calibration register
    ADC_ResetCalibration(ADC1); // 复位校准

    // Check the end of ADC1 reset calibration register
    while(ADC_GetResetCalibrationStatus(ADC1)); // ADC重置校准状态

    // Start ADC1 calibration
    ADC_StartCalibration(ADC1); // 开始指定ADC的校准状态

    // Check the end of ADC1 calibration
    while(ADC_GetCalibrationStatus(ADC1)); // 获取校准状态

    // Enable ADC2
    ADC_Cmd(ADC2, ENABLE);

    // Enable ADC2 reset calibration register
    ADC_ResetCalibration(ADC2);

    // Check the end of ADC2 reset calibration register
    while(ADC_GetResetCalibrationStatus(ADC2));

    // Start ADC2 calibration
    ADC_StartCalibration(ADC2);

    // Check the end of ADC2 calibration
    while(ADC_GetCalibrationStatus(ADC2));
}

/*********************************************************************************
  *Function     : void ADC_DMA_Init()
  *Description  : Override the default ADC Sample time depending on requirement
  *Input        : ADC_SampleTime:The sample time value to be set.
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       : * This parameter can be one of the following values:
				  * @arg ADC_SampleTime_1Cycles5: Sample time equal to 1.5 cycles
				  * @arg ADC_SampleTime_7Cycles5: Sample time equal to 7.5 cycles
				  * @arg ADC_SampleTime_13Cycles5: Sample time equal to 13.5 cycles
				  * @arg ADC_SampleTime_28Cycles5: Sample time equal to 28.5 cycles
				  * @arg ADC_SampleTime_41Cycles5: Sample time equal to 41.5 cycles
				  * @arg ADC_SampleTime_55Cycles5: Sample time equal to 55.5 cycles
				  * @arg ADC_SampleTime_71Cycles5: Sample time equal to 71.5 cycles
				  * @arg ADC_SampleTime_239Cycles5: Sample time equal to 239.5 cycles
**********************************************************************************/
void setADCSampleTime(uint8_t ADC_SampleTime)
{
    if(ADC_SampleTime < ADC_SampleTime_1Cycles5 || ADC_SampleTime > ADC_SampleTime_239Cycles5)
    {
        ADC_Sample_Time = ADC_SAMPLING_TIME;
    }
    else
    {
        ADC_Sample_Time = ADC_SampleTime;
    }
}


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
int32_t analogRead(uint16_t pin)
{
    // Allow people to use 0-7 to define analog pins by checking to see if the values are too low.
    if (pin < FIRST_ANALOG_PIN)
    {
        pin = pin + FIRST_ANALOG_PIN;
    }

    //SPI safety check
    if ((SPI.isEnabled() == true && (pin == PIN_SPI_SCK || pin == PIN_SPI_MOSI || pin == PIN_SPI_MISO))
    ||(SPI_1.isEnabled() == true && (pin == PIN_SPI_SCK1 || pin == PIN_SPI_MOSI1 || pin == PIN_SPI_MISO1)))
    {
        return LOW;
    }
    //Wire safety check
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

    if (pin >= TOTAL_PINS || PIN_MAP[pin].adc_channel == NONE )
    {
        return LOW;
    }

    int i = 0;

    if (adcChannelConfigured != PIN_MAP[pin].adc_channel)
    {
        digitalPinModeSaved = PIN_MAP[pin].pin_mode;
        pinMode(pin, AN_INPUT);
    }

    if (adcInitFirstTime == true)
    {
        ADC_DMA_Init();
        adcInitFirstTime = false;
    }

    if (adcChannelConfigured != PIN_MAP[pin].adc_channel)
    {
        // ADC1 regular channel configuration
        ADC_RegularChannelConfig(ADC1, PIN_MAP[pin].adc_channel, 1, ADC_Sample_Time);
        // ADC2 regular channel configuration
        ADC_RegularChannelConfig(ADC2, PIN_MAP[pin].adc_channel, 1, ADC_Sample_Time);
        // Save the ADC configured channel
        adcChannelConfigured = PIN_MAP[pin].adc_channel;
    }

    for(i = 0 ; i < ADC_DMA_BUFFERSIZE ; i++)
    {
        ADC_DualConvertedValues[i] = 0;
    }

    // Reset the number of data units in the DMA1 Channel1 transfer
    DMA_SetCurrDataCounter(DMA1_Channel1, ADC_DMA_BUFFERSIZE);

    // Enable ADC2 external trigger conversion
    ADC_ExternalTrigConvCmd(ADC2, ENABLE);

    // Enable DMA1 Channel1
    DMA_Cmd(DMA1_Channel1, ENABLE);

    // Enable ADC1 DMA
    ADC_DMACmd(ADC1, ENABLE);

    // Start ADC1 Software Conversion
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    // Test on Channel 1 DMA1_FLAG_TC flag
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC1)); // 通道 1 传输完成标志

    // Clear Channel 1 DMA1_FLAG_TC flag
    DMA_ClearFlag(DMA1_FLAG_TC1);

    // Disable ADC1 DMA
    ADC_DMACmd(ADC1, DISABLE);

    // Disable DMA1 Channel1
    DMA_Cmd(DMA1_Channel1, DISABLE);

    uint16_t ADC1_ConvertedValue = 0;
    uint16_t ADC2_ConvertedValue = 0;
    uint32_t ADC_SummatedValue = 0;
    uint16_t ADC_AveragedValue = 0;

    for(int i = 0 ; i < ADC_DMA_BUFFERSIZE ; i++)
    {
        // Retrieve the ADC2 converted value and add to ADC_SummatedValue
        ADC2_ConvertedValue = ADC_DualConvertedValues[i] >> 16;
        ADC_SummatedValue += ADC2_ConvertedValue;

        // Retrieve the ADC1 converted value and add to ADC_SummatedValue
        ADC1_ConvertedValue = ADC_DualConvertedValues[i] & 0xFFFF;
        ADC_SummatedValue += ADC1_ConvertedValue;
    }

    ADC_AveragedValue = (uint16_t)(ADC_SummatedValue / (ADC_DMA_BUFFERSIZE * 2));
    // Return ADC averaged value
    return ADC_AveragedValue;
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
    if (pin >= TOTAL_PINS || PIN_MAP[pin].timer_peripheral == NULL)
    {
        return;
    }
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

    if(PIN_MAP[pin].pin_mode != OUTPUT && PIN_MAP[pin].pin_mode != AF_OUTPUT_PUSHPULL)
    {
        return;
    }

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    //PWM Frequency : 500 Hz
    uint16_t TIM_Prescaler = (uint16_t)(SystemCoreClock / 24000000) - 1;//TIM Counter clock = 24MHz
    uint16_t TIM_ARR = (uint16_t)(24000000 / TIM_PWM_FREQ) - 1;
    // LZ 72M 3分频 24M (1/24)*(TIM_ARR+1) = T = 1/f
    // TIM Channel Duty Cycle(%) = (TIM_CCR / TIM_ARR + 1) * 100
    uint16_t TIM_CCR = (uint16_t)(value * (TIM_ARR + 1) / 255);

    // AFIO clock enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    pinMode(pin, AF_OUTPUT_PUSHPULL);

    // TIM clock enable
    if(PIN_MAP[pin].timer_peripheral == TIM2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    }
    else if(PIN_MAP[pin].timer_peripheral == TIM3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    }
    else if(PIN_MAP[pin].timer_peripheral == TIM4)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    }

    // Time base configuration
    TIM_TimeBaseStructure.TIM_Period = TIM_ARR;
    TIM_TimeBaseStructure.TIM_Prescaler = TIM_Prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(PIN_MAP[pin].timer_peripheral, &TIM_TimeBaseStructure);

    // PWM1 Mode configuration
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = TIM_CCR;

    if(PIN_MAP[pin].timer_ch == TIM_Channel_1)
    {
        // PWM1 Mode configuration: Channel1
        TIM_OC1Init(PIN_MAP[pin].timer_peripheral, &TIM_OCInitStructure);
        TIM_OC1PreloadConfig(PIN_MAP[pin].timer_peripheral, TIM_OCPreload_Enable);
    }
    else if(PIN_MAP[pin].timer_ch == TIM_Channel_2)
    {
        // PWM1 Mode configuration: Channel2
        TIM_OC2Init(PIN_MAP[pin].timer_peripheral, &TIM_OCInitStructure);
        TIM_OC2PreloadConfig(PIN_MAP[pin].timer_peripheral, TIM_OCPreload_Enable);
    }
    else if(PIN_MAP[pin].timer_ch == TIM_Channel_3)
    {
        // PWM1 Mode configuration: Channel3
        TIM_OC3Init(PIN_MAP[pin].timer_peripheral, &TIM_OCInitStructure);
        TIM_OC3PreloadConfig(PIN_MAP[pin].timer_peripheral, TIM_OCPreload_Enable);
    }
    else if(PIN_MAP[pin].timer_ch == TIM_Channel_4)
    {
        // PWM1 Mode configuration: Channel4
        TIM_OC4Init(PIN_MAP[pin].timer_peripheral, &TIM_OCInitStructure);
        TIM_OC4PreloadConfig(PIN_MAP[pin].timer_peripheral, TIM_OCPreload_Enable);
    }

    TIM_ARRPreloadConfig(PIN_MAP[pin].timer_peripheral, ENABLE);

    // TIM enable counter
    TIM_Cmd(PIN_MAP[pin].timer_peripheral, ENABLE);
}

/*********
 * TIMING
*********/
void analogWriteAndSetF(uint16_t pin, uint8_t value,uint32_t fre)
{
    if (pin >= TOTAL_PINS || PIN_MAP[pin].timer_peripheral == NULL)
    {
        return;
    }

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

    if(PIN_MAP[pin].pin_mode != OUTPUT && PIN_MAP[pin].pin_mode != AF_OUTPUT_PUSHPULL)
    {
        return;
    }

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    //PWM Frequency : 500 Hz
    uint16_t TIM_Prescaler = (uint16_t)(SystemCoreClock / 24000000) - 1;//TIM Counter clock = 24MHz
    uint16_t TIM_ARR = (uint16_t)(24000000 / fre) - 1;
    // LZ 72M 3分频 24M (1/24)*(TIM_ARR+1) = T = 1/f
    // TIM Channel Duty Cycle(%) = (TIM_CCR / TIM_ARR + 1) * 100
    uint16_t TIM_CCR = (uint16_t)(value * (TIM_ARR + 1) / 255);

    // AFIO clock enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    pinMode(pin, AF_OUTPUT_PUSHPULL);

    // TIM clock enable
    if(PIN_MAP[pin].timer_peripheral == TIM2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    }
    else if(PIN_MAP[pin].timer_peripheral == TIM3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    }
    else if(PIN_MAP[pin].timer_peripheral == TIM4)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    }

    // Time base configuration
    TIM_TimeBaseStructure.TIM_Period = TIM_ARR;
    TIM_TimeBaseStructure.TIM_Prescaler = TIM_Prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(PIN_MAP[pin].timer_peripheral, &TIM_TimeBaseStructure);

    // PWM1 Mode configuration
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = TIM_CCR;

    if(PIN_MAP[pin].timer_ch == TIM_Channel_1)
    {
        // PWM1 Mode configuration: Channel1
        TIM_OC1Init(PIN_MAP[pin].timer_peripheral, &TIM_OCInitStructure);
        TIM_OC1PreloadConfig(PIN_MAP[pin].timer_peripheral, TIM_OCPreload_Enable);
    }
    else if(PIN_MAP[pin].timer_ch == TIM_Channel_2)
    {
        // PWM1 Mode configuration: Channel2
        TIM_OC2Init(PIN_MAP[pin].timer_peripheral, &TIM_OCInitStructure);
        TIM_OC2PreloadConfig(PIN_MAP[pin].timer_peripheral, TIM_OCPreload_Enable);
    }
    else if(PIN_MAP[pin].timer_ch == TIM_Channel_3)
    {
        // PWM1 Mode configuration: Channel3
        TIM_OC3Init(PIN_MAP[pin].timer_peripheral, &TIM_OCInitStructure);
        TIM_OC3PreloadConfig(PIN_MAP[pin].timer_peripheral, TIM_OCPreload_Enable);
    }
    else if(PIN_MAP[pin].timer_ch == TIM_Channel_4)
    {
        // PWM1 Mode configuration: Channel4
        TIM_OC4Init(PIN_MAP[pin].timer_peripheral, &TIM_OCInitStructure);
        TIM_OC4PreloadConfig(PIN_MAP[pin].timer_peripheral, TIM_OCPreload_Enable);
    }

    TIM_ARRPreloadConfig(PIN_MAP[pin].timer_peripheral, ENABLE);

    // TIM enable counter
    TIM_Cmd(PIN_MAP[pin].timer_peripheral, ENABLE);
}

/*********************************************************************************
  *Function     : system_tick_t millis(void)
  *Description  : Should return the number of milliseconds since the processor started up.
  *Input        : none
  *Output       : none
  *Return       : return the system tickong clock count 1ms
  *author       : lz
  *date         : 6-December-2014
  *Others       : This is useful for measuring the passage of time.
**********************************************************************************/
system_tick_t millis(void)
{
    return GetSystem1MsTick();
}

/*********************************************************************************
  *Function     : unsigned long micros(void)
  *Description  : Should return the number of microseconds since the processor started up.
  *Input        : none
  *Output       : none
  *Return       : return the DWT count us
  *author       : lz
  *date         : 6-December-2014
  *Others       : This is useful for measuring the passage of time.
**********************************************************************************/
unsigned long micros(void)
{
    return (DWT->CYCCNT / SYSTEM_US_TICKS);
}

/*********************************************************************************
  *Function     : void delay(unsigned long ms)
  *Description  : This should block for a certain number of milliseconds and also execute intorobot_loop
  *Input        : ms:delay time value
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       :        
**********************************************************************************/
void delay(unsigned long ms)
{
    volatile u32 delay_timer = timerGetId();
    while (1)
    {         
        KICK_WDT();

        if(timerIsEnd(delay_timer, ms))
        {
            break;
        }
    }
}
/*********************************************************************************
  *Function     : void delayMicroseconds(unsigned int us)
  *Description  : This should block for a certain number of microseconds.
  *Input        : us:delay time value
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       :        
**********************************************************************************/
void delayMicroseconds(unsigned int us)
{
    Delay_Microsecond(us);
}

/*********************************************************************************
  *Function      : u32 timerGetId(void)
  *Description  : This should block for a certain number of microseconds.
  *Input           : us:delay time value
  *Output         : none
  *Return         : none
  *author         : lz
  *date            : 6-December-2014
  *Others         :        
**********************************************************************************/
u32 timerGetId(void)
{
    return millis();
}

/*********************************************************************************
  *Function      : bool timerIsEnd(u32 timerID, u32 time)
  *Description  : This should block for a certain number of microseconds.
  *Input          : us:delay time value
  *Output        : none
  *Return        : none
  *author        : lz
  *date           : 6-December-2014
  *Others        :        
**********************************************************************************/
bool timerIsEnd(u32 timerID, u32 time)
{
    volatile system_tick_t current_millis = millis();
    volatile long elapsed_millis = current_millis - timerID;
    
    //Check for wrapping
    if (elapsed_millis < 0)
    {        
        elapsed_millis =  0xFFFFFFFF -timerID + current_millis;  
    }

    if (elapsed_millis >= (long)time)
    {
        return true;
    }
    return false;
  }

/*********************************************************************************
  *Function      : uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) 
  *Description  : Data shift receiving
  *Input          : dataPin:Data input port number
  				  clockPin:The clock port number
  				  bitOrder: 1 or 0
  *Output        : none
  *Return        : return data
  *author        : lz
  *date           : 6-December-2014
  *Others        : Can be used for communication
**********************************************************************************/
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) 
{
    uint8_t value = 0;
    uint8_t i;

    for (i = 0; i < 8; ++i) 
    {
        digitalWrite(clockPin, HIGH);
        if (bitOrder == LSBFIRST)
        value |= digitalRead(dataPin) << i;
        else
        value |= digitalRead(dataPin) << (7 - i);
        digitalWrite(clockPin, LOW);
    }
    return value;
}

/*********************************************************************************
  *Function     : void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
  *Description  : Data shift receiving
  *Input        : dataPin:Data input port number
  				  clockPin:The clock port number
  				  bitOrder: 1 or 0
  				  val:To send data
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       : Can be used for communication
**********************************************************************************/
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
    uint8_t i;

    for (i = 0; i < 8; i++)  
    {
        if (bitOrder == LSBFIRST)
        {digitalWrite(dataPin, !!(val & (1 << i)));}
        else
        {digitalWrite(dataPin, !!(val & (1 << (7 - i))));}

        digitalWrite(clockPin, HIGH);
        digitalWrite(clockPin, LOW);
    }
}

/*********************************************************************************
  *Function      : uint32_t pulseIn(uint8_t pin, uint8_t state, uint32_t timeout)
  *Description  : Measures the length (in microseconds) of a pulse on the pin
  *Input           : pin:Data input port number
  				  state:HIGH or LOW
  				  timeout: Set up wait for timeout 
  *Output         : none
  *Return         : return pluse width
  *author         : lz
  *date            : 6-December-2014
  *Others         : Works on pulses from 2-3 microseconds to 3 minutes in length, but must be called at least a few dozen microseconds
 				  before the start of the pulse. 
**********************************************************************************/
uint32_t pulseIn(uint8_t pin, uint8_t state, uint32_t timeout)
{ 
	/* timeout是超时 等不到测量的电平
       state 是需测量的电平 1或者 0
       timeout 超时时间 us    计数值 = timeout * SYSTEM_US_TICKS
       按指令计算时间
  	*/
  	
  	STM32_Pin_Info p = PIN_MAP[pin];
	uint32_t width = 0; 
	
	uint32_t numloops = 0;
	uint32_t maxloops = timeout * SYSTEM_US_TICKS; 
	
	// wait for any previous pulse to end
	while (GPIO_ReadInputDataBit(p.gpio_peripheral, p.gpio_pin) == state) // 读到的电平是测量的电平 等待
	{
		if (numloops++ == maxloops)
		{	
			return 0;
		}
	}

	// wait for the pulse to start
	while (GPIO_ReadInputDataBit(p.gpio_peripheral, p.gpio_pin) != state) // 读到的电平不是测试的电平 等待
	{
		if (numloops++ == maxloops)
		{
			return 0;
		}
	}

	// wait for the pulse to stop
	while (GPIO_ReadInputDataBit(p.gpio_peripheral, p.gpio_pin) == state)  // 经过等待之后 读到的电平是测试的电平 开始计时
	{
		if (numloops++ == maxloops)
		{
			return 0;
		}
		width++; // 28条指令时间
	}
	return ((width * 28)+148) / SYSTEM_US_TICKS; // 返回值 单位 us 148为误差
}

