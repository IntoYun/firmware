/**
 *******************************************************************************
 * @file    : variant.cpp
 * @author  : lz
 * @version : V1.0.0
 * @date    : 6-December-2014
 * @brief   : 
 *******************************************************************************
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

#include "variant.h"
#include "wiring.h"


/*
 * IntoRobot Atom Pin           | PORT                       | Label
 * ---------------------------------+--------+-------
 *   0              D0                  |  PB11                      | RX1   SDA1
 *   1              D1                  |  PB10                      | TX1   SCL1
 *   2              D2                  |  PB12                      | SS1
 *   3              D3                  |  PB13                      | SCK1
 *   4              D4                  |  PB14                      | MISO1
 *   5              D5                  |  PB15                      | MOSI1
 *   6              D6                  |  PB8                        | PWM   CANRX
 *   7              D7                  |  PB9                        | PWM   CANTX
 *   8              D8                  |  PB7                        | PWM   SDA
 *   9              D9                  |  PB6                        | PWM   SCL
 *  10              D10                |  PB5                        | 
 *  11              D11                |  PA8                        | 
 *  12              D12                |  PD2                        | 
 *  13              D13                |  PC6                        | 
 *  14              D14                |  PC7                        | 
 *  15              D15                |  PC8                        | 
 *  16              D16                |  PC9                        |
 *  17              D17                |  PC10                      |
 *  18              D18                |  PC11                      |
 *  19              D19                |  PC13                      |
 *  20              D20                |  PA13                      |JTMS
 *  21              D21                |  PA14                      |JTCK
 *  22              D22                |  PA16                      |JTDI
 *  23              D23                |  PB3                        |JTDO
 *  24              D24                |  PB4                        |JNTRST
 *  25              Reserve
 *  26              Reserve
 *  27              Reserve
 *  28              Reserve
 *  29              Reserve
 *  30              A0                 |  PA0                        |PWM
 *  31              A1                 |  PA1                        |PWM
 *  32              A2                 |  PA3                        |PWM   RX
 *  33              A3                 |  PA2                        |PWM   TX
 *  34              A4                 |  PA4                        |SS
 *  35              A5                 |  PA5                        |SCK
 *  36              A6                 |  PA6                        |PWM MISO
 *  37              A7                 |  PA7                        |PWM MOSI
 *  38              A8                 |  PB0                        |PWM
 *  39              A9                 |  PB1                        |PWM
 *  40              A10               |  PC0                        |
 *  41              A11               |  PC1                        |
 *  42              A12               |  PC2                        |
 *  43              A13               |  PC3                        |
 *  44              A14               |  PC4                        |
 *  45              A15               |  PC5                        |
 *  46                                  |  PA10                       |RX
 *  47                                  |  PA9                         |TX
 *  48              Reserve
 *  49              Reserve
 *  50              Reserve
 *  51              Reserve
 *  52              Reserve
 *  53              Reserve
 *  54              Reserve
 *  55              Reserve
 */


/*
 * Pin mapping
 */
STM32_Pin_Info PIN_MAP[TOTAL_PINS] =
{
    /*
    * gpio_peripheral (GPIOA or GPIOB or GPIOC or GPIOD)
    * gpio_pin (0-15)
    * adc_channel (0-15 or NONE. Note we don't define the peripheral because our chip only has one)
    * timer_peripheral (TIM1 - TIM4, or NONE)
    * timer_ch (1-4, or NONE)
    * pin_mode (NONE by default, can be set to OUTPUT, INPUT, or other types)
    * timer_ccr (0 by default, store the CCR value for TIM interrupt use)
    * user_property (0 by default, user variable storage)
    */
    /*0-9*/
    { GPIOB, GPIO_Pin_11, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { GPIOB, GPIO_Pin_10, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { GPIOB, GPIO_Pin_12, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { GPIOB, GPIO_Pin_13, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { GPIOB, GPIO_Pin_14, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { GPIOB, GPIO_Pin_15, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { GPIOB, GPIO_Pin_8, NONE, TIM4, TIM_Channel_3, (PinMode)NONE, 0, 0 },
    { GPIOB, GPIO_Pin_9, NONE, TIM4, TIM_Channel_4, (PinMode)NONE, 0, 0 },
    { GPIOB, GPIO_Pin_7, NONE, TIM4, TIM_Channel_2, (PinMode)NONE, 0, 0 },
    { GPIOB, GPIO_Pin_6, NONE, TIM4, TIM_Channel_1, (PinMode)NONE, 0, 0 },
    /*10-19*/
    { GPIOB, GPIO_Pin_5,  NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { GPIOA, GPIO_Pin_8,  NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { GPIOD, GPIO_Pin_2,  NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { GPIOC, GPIO_Pin_6,  NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { GPIOC, GPIO_Pin_7,  NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { GPIOC, GPIO_Pin_8,  NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { GPIOC, GPIO_Pin_9,  NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { GPIOC, GPIO_Pin_10, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { GPIOC, GPIO_Pin_11, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { GPIOC, GPIO_Pin_13, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    /*20-24*/
    { GPIOA, GPIO_Pin_13, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { GPIOA, GPIO_Pin_14, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { GPIOA, GPIO_Pin_15, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { GPIOB, GPIO_Pin_3,  NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { GPIOB, GPIO_Pin_4,  NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    /*25-29  reserve*/
    { NULL, GPIO_Pin_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { NULL, GPIO_Pin_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { NULL, GPIO_Pin_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { NULL, GPIO_Pin_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { NULL, GPIO_Pin_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    /*30-39*/
    { GPIOA, GPIO_Pin_0, ADC_Channel_0, TIM2, TIM_Channel_1, (PinMode)NONE, 0, 0 },
    { GPIOA, GPIO_Pin_1, ADC_Channel_1, TIM2, TIM_Channel_2, (PinMode)NONE, 0, 0 },
    { GPIOA, GPIO_Pin_3, ADC_Channel_3, TIM2, TIM_Channel_4, (PinMode)NONE, 0, 0 },
    { GPIOA, GPIO_Pin_2, ADC_Channel_2, TIM2, TIM_Channel_3, (PinMode)NONE, 0, 0 },
    { GPIOA, GPIO_Pin_4, ADC_Channel_4, NULL, (uint16_t)NULL, (PinMode)NONE, 0, 0 },
    { GPIOA, GPIO_Pin_5, ADC_Channel_5, NULL, (uint16_t)NULL, (PinMode)NONE, 0, 0 },
    { GPIOA, GPIO_Pin_6, ADC_Channel_6, TIM3, TIM_Channel_1, (PinMode)NONE, 0, 0 },
    { GPIOA, GPIO_Pin_7, ADC_Channel_7, TIM3, TIM_Channel_2, (PinMode)NONE, 0, 0 },
    { GPIOB, GPIO_Pin_0, ADC_Channel_8, TIM3, TIM_Channel_3, (PinMode)NONE, 0, 0 },
    { GPIOB, GPIO_Pin_1, ADC_Channel_9, TIM3, TIM_Channel_4, (PinMode)NONE, 0, 0 },
    /*40-45*/
    { GPIOC, GPIO_Pin_0, ADC_Channel_10, NULL, (uint16_t)NULL, (PinMode)NONE, 0, 0 },
    { GPIOC, GPIO_Pin_1, ADC_Channel_11, NULL, (uint16_t)NULL, (PinMode)NONE, 0, 0 },
    { GPIOC, GPIO_Pin_2, ADC_Channel_12, NULL, (uint16_t)NULL, (PinMode)NONE, 0, 0 },
    { GPIOC, GPIO_Pin_3, ADC_Channel_13, NULL, (uint16_t)NULL, (PinMode)NONE, 0, 0 },
    { GPIOC, GPIO_Pin_4, ADC_Channel_14, NULL, (uint16_t)NULL, (PinMode)NONE, 0, 0 },
    { GPIOC, GPIO_Pin_5, ADC_Channel_15, NULL, (uint16_t)NULL, (PinMode)NONE, 0, 0 },
    /*46-47  usart1*/ 
    { GPIOA, GPIO_Pin_10, NONE, TIM1, TIM_Channel_3, (PinMode)NONE, 0, 0 },
    { GPIOA, GPIO_Pin_9, NONE, TIM1, TIM_Channel_2, (PinMode)NONE, 0, 0 },
    /*48-49 reserve*/
    { NULL, GPIO_Pin_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { NULL, GPIO_Pin_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    /*50-55 reserve*/
    { NULL, GPIO_Pin_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { NULL, GPIO_Pin_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { NULL, GPIO_Pin_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { NULL, GPIO_Pin_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { NULL, GPIO_Pin_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    { NULL, GPIO_Pin_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 },
    /*56 reserve*/
    { NULL, GPIO_Pin_0, NONE, NULL, NONE, (PinMode)NONE, 0, 0 }
};

/*
 * USART mapping
 */
STM32_USART_Info USART_MAP[TOTAL_USARTS] =
{
    /*
    * USRAT_peripheral (USART1-USART2; not using others)
    * clock control register (APB1ENR or APB1ENR)
    * clock enable bit value (RCC_APB2Periph_USART1 or RCC_APB2Periph_USART2)
    * interrupt number (USART1_IRQn or USART2_IRQn)
    * TX pin
    * RX pin
    * GPIO Remap (RCC_APB2Periph_USART2 or GPIO_Remap_None )
    * <tx_buffer pointer> used internally and does not appear below
    * <rx_buffer pointer> used internally and does not appear below
    */
    { USART1, &RCC->APB2ENR, RCC_APB2Periph_USART1, USART1_IRQn, TX, RX, GPIO_Remap_None },
    { USART2, &RCC->APB1ENR, RCC_APB1Periph_USART2, USART2_IRQn, TX2, RX2, GPIO_Remap_None },
    { USART3, &RCC->APB1ENR, RCC_APB1Periph_USART3, USART3_IRQn, TX1, RX1, GPIO_Remap_None }
};

// serial 
USARTSerial Serial(&USART_MAP[USART_A2_A3]);
void serialEvent() __attribute__((weak));
void serialEvent() { }

USARTSerial Serial1(&USART_MAP[USART_D1_D0]);
void serialEvent1() __attribute__((weak));
void serialEvent1() { }

USARTSerial SerialBridge(&USART_MAP[USART_TX_RX]);
void serialEvent2() __attribute__((weak));
void serialEvent2() { }

/*********************************************************************************
  *Function       : void serialEventRun(void)
  *Description   : serial event run
  *Input           : none
  *Output         : none
  *Return         : none
  *author         : lz
  *date            : 6-December-2014
  *Others         : none        
**********************************************************************************/ 
void serialEventRun(void)
{
    if (Serial.available()) serialEvent();
    if (Serial1.available()) serialEvent1();
}

// ----------------------------------------------------------------------------
void __libc_init_array(void);


/*********************************************************************************
  *Function     : void RTC_Configuration(void)
  *Description  : RTC config
  *Input        : none
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       : none        
**********************************************************************************/ 
void RTC_Configuration(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Enable write access to Backup domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Should we execute System Standby mode */
    if(BKP_ReadBackupRegister(BKP_DR9) == 0xA5A5)
    {
        /* Clear Standby mode system flag */
        BKP_WriteBackupRegister(BKP_DR9, 0xFFFF);

        /* Request to enter STANDBY mode */
        PWR_EnterSTANDBYMode();

        /* Following code will not be reached */
        while(1);
    }
    
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure EXTI Line17(RTC Alarm) to generate an interrupt on rising edge */
    EXTI_ClearITPendingBit(EXTI_Line17);
    EXTI_InitStructure.EXTI_Line = EXTI_Line17;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable the RTC Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = RTC_IRQ_PRIORITY;			//OLD: 0x01
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;								//OLD: 0x01
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable the RTC Alarm Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = RTCALARM_IRQ_PRIORITY;		//OLD: 0x01
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;								//OLD: 0x02
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Check if the StandBy flag is set */
    if(PWR_GetFlagStatus(PWR_FLAG_SB) != RESET)
    {
        /* System resumed from STANDBY mode */

        /* Clear StandBy flag */
        PWR_ClearFlag(PWR_FLAG_SB);

        /* Wait for RTC APB registers synchronisation */
        RTC_WaitForSynchro();

        /* No need to configure the RTC as the RTC configuration(clock source, enable,
        prescaler,...) is kept after wake-up from STANDBY */
    }
    else
    {
        /* StandBy flag is not set */

        /* Enable LSE */
        RCC_LSEConfig(RCC_LSE_ON);

        /* Wait till LSE is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
        {
            //Do nothing
        }

        /* Select LSE as RTC Clock Source */
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

        /* Enable RTC Clock */
        RCC_RTCCLKCmd(ENABLE);

        /* Wait for RTC registers synchronization */
        RTC_WaitForSynchro();

        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();

        /* Set RTC prescaler: set RTC period to 1sec */
        RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();
    }

    /* Enable the RTC Second and RTC Alarm interrupt */
    RTC_ITConfig(RTC_IT_SEC | RTC_IT_ALR, ENABLE);

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
}

/*********************************************************************************
  *Function     : void IWDG_Reset_Enable(uint32_t msTimeout)
  *Description  : iwdg enable
  *Input        : msTimeout:set The overflow of time
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       : none        
**********************************************************************************/ 
void IWDG_Reset_Enable(uint32_t msTimeout)
{
    /* Enable write access to IWDG_PR and IWDG_RLR registers */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    /* IWDG counter clock: LSI/256 */
    IWDG_SetPrescaler(IWDG_Prescaler_256);

    /* IWDG timeout may vary due to LSI frequency dispersion */
    msTimeout = ((msTimeout * 40) / 256); //Assuming LSI Frequency = 40000
    if (msTimeout > 0xfff) msTimeout = 0xfff;   // 26214.4

    IWDG_SetReload((uint16_t)msTimeout);

    /* Reload IWDG counter */
    IWDG_ReloadCounter();

    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG_Enable();
}

/*********************************************************************************
  *Function     : static void DWT_Init(void)
  *Description  : DWT initiale
  *Input        : none
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       : none        
**********************************************************************************/ 
static void DWT_Init(void)
{
    DBGMCU->CR |= DBGMCU_SETTINGS;
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

/*********************************************************************************
  *Function     : void Delay_Microsecond(uint32_t uSec)
  *Description  : delay us
  *Input        : uSec:set delay time
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       : none        
**********************************************************************************/ 
void Delay_Microsecond(uint32_t uSec)
{
    volatile uint32_t DWT_START = DWT->CYCCNT;

    // keep DWT_TOTAL from overflowing (max 59.652323s w/72MHz SystemCoreClock)
    if (uSec > (UINT_MAX / SYSTEM_US_TICKS)) // UINT_MAX = 0xFFFFFFFF
    {
    uSec = (UINT_MAX / SYSTEM_US_TICKS);
    }

    volatile uint32_t DWT_TOTAL = (SYSTEM_US_TICKS * uSec);

    while((DWT->CYCCNT - DWT_START) < DWT_TOTAL)
    {
        KICK_WDT();
    }
}

/*********************************************************************************
  *Function     : void TIM1_Configuration(void)
  *Description  : TIM1 config
  *Input        : none
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       : none        
**********************************************************************************/ 
void TIM1_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; // 必须加

    TIM_DeInit(TIM1);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能
    TIM_InternalClockConfig(TIM1); // 配置时钟源
    TIM_TimeBaseStructure.TIM_Period = 5000-1;//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
    TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;//72-1; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    TIM_Cmd(TIM1, ENABLE); // 使能
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);


    NVIC_InitTypeDef NVIC_InitStructure;
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); // 中断优先级分组
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM1_UP_IRQ_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*********************************************************************************
  *Function     : void set_intorobot_loop_period(unsigned short time_ms)
  *Description  : set  intorobot_loop time
  *Input        : 100ms--6500ms
  *Output       : none
  *Return       : none
  *author       : lbz
  *date         : 6-December-2015
  *Others       : none        
**********************************************************************************/ 
void set_intorobot_loop_period(unsigned short time_ms)
{
    if(time_ms<100)  time_ms=100;
    if(time_ms>6500)  time_ms=6500;

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; // 必须加

    TIM_DeInit(TIM1);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能
    TIM_InternalClockConfig(TIM1); // 配置时钟源

    TIM_TimeBaseStructure.TIM_Period = (unsigned short)(time_ms*10-1);
    //TIM_TimeBaseStructure.TIM_Period = 2500-1;//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
    TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;//72-1; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    TIM_Cmd(TIM1, ENABLE); // 使能
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);


    NVIC_InitTypeDef NVIC_InitStructure;
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); // 中断优先级分组
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM1_UP_IRQ_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/*********************************************************************************
  *Function     : void init(void)
  *Description  : initiale
  *Input        : none
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       : none        
**********************************************************************************/ 
void init(void)
{
    //disable JTAG-DP,release pin 29(PB3),30(PB4),23(PA15)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
    
#ifdef DFU_BUILD_ENABLE
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x5000); // lz
#endif

#ifdef SWD_JTAG_DISABLE
    /* Disable the Serial Wire JTAG Debug Port SWJ-DP */
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
#endif

    // Set Systick to 1ms interval, common to all SAM3 variants
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        // Capture error
        while (true);
    }
    /* Configure the SysTick Handler Priority: Preemption priority and subpriority */
    NVIC_SetPriority(SysTick_IRQn, SYSTICK_IRQ_PRIORITY);	
    
    //digital pin  default output  low
    for (unsigned i = 0; i < FIRST_ANALOG_PIN+TOTAL_ANALOG_PINS; i++)
    {
        pinMode(i, OUTPUT);
        digitalWrite(i, LOW);
    }

    RTC_Configuration(); 

    DWT_Init();

#ifdef IWDG_RESET_ENABLE
    IWDG_Reset_Enable(3 * TIMING_IWDG_RELOAD);
#endif

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    TIM1_Configuration();
    TIM_Cmd(TIM1, DISABLE); //定时器关闭
    NVIC_DisableIRQ(TIM1_UP_IRQn);
}

