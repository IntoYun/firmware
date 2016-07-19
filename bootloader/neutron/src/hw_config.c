/**
 ******************************************************************************
 * @file    hw_config.c
 * @author  Satish Nair, Zachary Crockett and Mohit Bhoite
 * @version V1.0.0
 * @date    13-March-2013
 * @brief   Hardware Configuration & Setup
 ******************************************************************************
 Copyright (c) 2013-2015 Particle Industries, Inc.  All rights reserved.

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation, either
 version 3 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this program; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "hw_config.h"
#include "system_params.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

USBD_HandleTypeDef  USBD_Device;
RTC_HandleTypeDef RtcHandle;

volatile uint32_t TimingBUTTON;
volatile uint32_t TimingLED;
extern uint8_t DEFAULT_FIRMWARE_MODE;
extern uint8_t OTA_FIRMWARE_MODE;
extern uint8_t FACTORY_RESET_MODE;
extern uint8_t ALL_RESET_MODE;


boot_params_t intorobot_boot_params;  	//设备标识参数


/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
UART_HandleTypeDef  UartHandleA;
extern UART_HandleTypeDef UartHandle;


extern void print(char *p);
extern void printn(char *p, int n);

void usartA2A3begin(uint baud)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();
    GPIO_InitTypeDef  GPIO_InitStruct;
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_2;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    UartHandleA.Instance          = USART2;
    UartHandleA.Init.BaudRate     = baud;
    UartHandleA.Init.WordLength   = UART_WORDLENGTH_8B;
    UartHandleA.Init.StopBits     = UART_STOPBITS_1;
    UartHandleA.Init.Parity       = UART_PARITY_NONE;
    UartHandleA.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    UartHandleA.Init.Mode         = UART_MODE_TX_RX;
    UartHandleA.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&UartHandleA);

    //Configure the NVIC for UART
    /* HAL_NVIC_SetPriority(usartMap->usart_int_n, 0x0f, 0); */
    /* HAL_NVIC_EnableIRQ(usartMap->usart_int_n); */
    /* __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXNE); */
}

/**
 * @brief  Configures Main system clocks & power.
 * @param  None
 * @retval None
 */
void Set_System(void)
{
    HAL_Init();
    SystemClock_Config();
    LED_Init();
    RTC_Init();
    UI_Timer_Configure();
    BUTTON_Init(FAC_BUTTON, BUTTON_MODE_EXTI);
    GPIO_ESP8266_Init();
    Esp8266_Reset();
}

/**
 * @brief  System Clock Configuration
 * @param  96M
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    __PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 13;
    RCC_OscInitStruct.PLL.PLLN = 192;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 8;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
}

void USBD_DFU_Init(void)
{
    USBD_Init(&USBD_Device, &DFU_Desc, 0);
    USBD_RegisterClass(&USBD_Device, USBD_DFU_CLASS);
    USBD_DFU_RegisterMedia(&USBD_Device, &USBD_DFU_fops);
    USBD_Start(&USBD_Device);
}

void USBD_CDC_Init(void)
{
    USBD_Init(&USBD_Device, &VCP_Desc, 0);
    USBD_RegisterClass(&USBD_Device, USBD_CDC_CLASS);
    USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_fops);
    USBD_Start(&USBD_Device);
}

/*******************************************************************************
 * Function Name  : Timing_Decrement
 * Description    : Decrements the various Timing variables related to SysTick.
 This function is called every 1mS.
 * Input          : None
 * Output         : Timing
 * Return         : None
 *******************************************************************************/
void HAL_SYSTICK_Callback(void)
{
    TimingBUTTON++;

    if (TimingLED != 0x00)
    {
        TimingLED--;
    }
    else if(DEFAULT_FIRMWARE_MODE || OTA_FIRMWARE_MODE || FACTORY_RESET_MODE || ALL_RESET_MODE)
    {
        LED_Toggle(LED_RGB);
        TimingLED = 50;
    }
}

void RTC_Init(void)
{
    RtcHandle.Instance = RTC;
    RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
    RtcHandle.Init.AsynchPrediv = 0x7F;
    RtcHandle.Init.SynchPrediv = 0x00FF;
    RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
    RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

    HAL_RTC_Init(&RtcHandle);
    HAL_PWR_EnableBkUpAccess();
}

void UI_Timer_Configure(void)
{
    TIM_HandleTypeDef TimHandle;
    TIM_OC_InitTypeDef sConfig;

    __HAL_RCC_TIM1_CLK_ENABLE();
    uint32_t uhPrescalerValue = (uint32_t)(SystemCoreClock / 1000000) - 1;
    uint32_t uhPeriod = (1000000 / UI_TIMER_FREQUENCY) - 1;

    TimHandle.Instance = TIM1;
    TimHandle.Init.Prescaler         = uhPrescalerValue;
    TimHandle.Init.Period            = uhPeriod;
    TimHandle.Init.ClockDivision     = 0;
    TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
    TimHandle.Init.RepetitionCounter = 0;
    HAL_TIM_PWM_Init(&TimHandle);

    sConfig.OCMode       = TIM_OCMODE_PWM1;
    sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
    sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
    sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;

    sConfig.Pulse = 0;
    HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1);
    HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_2);
    HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&TimHandle, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&TimHandle, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&TimHandle, TIM_CHANNEL_3);
}

/**
 * @brief  Configures LED GPIO.
 * @param  Led: Specifies the Led to be configured.
 *   This parameter can be one of following parameters:
 *     @arg LED1, LED2, LED3, LED4
 * @retval None
 */
void LED_Init(void)
{
    //ledred
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    //ledgreen
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    //ledblue
    GPIO_InitStruct.Pin = GPIO_PIN_14;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void Set_RGB_LED_Values(uint16_t r, uint16_t g, uint16_t b) {
    TIM1->CCR3 = r;
    TIM1->CCR1 = g;
    TIM1->CCR2 = b;
}

void Get_RGB_LED_Values(uint16_t* values) {
    values[0] = TIM1->CCR3;
    values[1] = TIM1->CCR1;
    values[2] = TIM1->CCR2;
}

void Set_User_LED(uint8_t state) {
    if (state)
    {HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);}
    else
    {HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);}
}

void Toggle_User_LED() {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
}

uint16_t Get_RGB_LED_Max_Value() {
    return (TIM1->ARR + 1);
}

/**
 * @brief  Configures Button GPIO, EXTI Line and DEBOUNCE Timer.
 * @param  Button: Specifies the Button to be configured.
 *   This parameter can be one of following parameters:
 *     @arg BUTTON1: Button1
 *     @arg BUTTON2: Button2
 * @param  Button_Mode: Specifies Button mode.
 *   This parameter can be one of following parameters:
 *     @arg BUTTON_MODE_GPIO: Button will be used as simple IO
 *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
 *                     generation capability
 * @retval None
 */
void BUTTON_Init(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode)
{
    GPIO_InitTypeDef   GPIO_InitStruct;

    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
 * @brief  Returns the selected Button non-filtered state.
 * @param  Button: Specifies the Button to be checked.
 *   This parameter can be one of following parameters:
 *     @arg BUTTON1: Button1
 *     @arg BUTTON2: Button2
 * @retval Actual Button Pressed state.
 */
uint8_t BUTTON_GetState(Button_TypeDef Button)
{
    return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2);
}

void GPIO_ESP8266_Init(void)
{
    GPIO_InitTypeDef   GPIO_InitStruct;
    //esp8266 gpio1
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    //esp8266 reset
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void Esp8266_Reset(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_Delay(200);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}

void Esp8266_Enter_UpdateMode(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_Delay(200);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}

void Load_BootParams(void)
{
    loadBootParams(&intorobot_boot_params);
}

void Save_BootParams(void)
{
    saveBootParams(&intorobot_boot_params);
}

void Init_BootParams(void)
{
    initBootParams(&intorobot_boot_params);
}

/**
 * @brief  Gets the sector of a given address
 * @param  None
 * @retval The sector of a given address
 */
static uint32_t GetSector(uint32_t Address)
{
    uint32_t sector = 0;

    if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
    {
        sector = FLASH_SECTOR_0;
    }
    else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
    {
        sector = FLASH_SECTOR_1;
    }
    else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
    {
        sector = FLASH_SECTOR_2;
    }
    else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
    {
        sector = FLASH_SECTOR_3;
    }
    else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
    {
        sector = FLASH_SECTOR_4;
    }
    else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
    {
        sector = FLASH_SECTOR_5;
    }
    else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
    {
        sector = FLASH_SECTOR_6;
    }
    else /* (Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_7) */
    {
        sector = FLASH_SECTOR_7;
    }
    return sector;
}

void FLASH_Erase_Impl(uint32_t address, uint32_t length)
{
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t FirstSector = 0, NbOfSectors = 0, SECTORError = 0;

    HAL_FLASH_Unlock();
    /* Get the 1st sector to erase */
    FirstSector = GetSector(address);
    /* Get the number of sector to erase from 1st sector*/
    NbOfSectors = GetSector(address + length) - FirstSector + 1;
    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = FirstSector;
    EraseInitStruct.NbSectors = NbOfSectors;
    HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError);
    HAL_FLASH_Lock();
}

void FLASH_Erase()
{
    FLASH_Erase_Impl(CORE_FW_ADDRESS, (INTERNAL_FLASH_END_ADDRESS - CORE_FW_ADDRESS));
}

bool FLASH_Restore(Firmware_TypeDef FmType)
{
    #define PACKAGE_UNIT 16384  //16k
    int PacketNum,Count;

    PacketNum=ESP8266_Firmware_Size(FmType, PACKAGE_UNIT);
    if(PacketNum)
    {
        print("erase begin\r\n");
        FLASH_Erase_Impl(CORE_FW_ADDRESS, PacketNum * PACKAGE_UNIT) ;
        LED_SetSignalingColor(RGB_COLOR_YELLOW);
        LED_Signaling_Start();
        print("erase over\r\n");
        HAL_FLASH_Unlock();
        for(Count = 0; Count < PacketNum; Count++)
        {
            if(ESP8266_RSP_SUCCESS != ESP8266_Firware_Packet(FmType, PACKAGE_UNIT, Count, 1000))
            {
                return false;
            }
        }
        HAL_FLASH_Lock();
        return true;
    }
    return false;
}

bool FACTORY_Flash_Reset(void)
{
    return FLASH_Restore(DEFAULT_FIRWARE);
}

bool DEFAULT_Flash_Reset(void)
{
    return FLASH_Restore(DEFAULT_FIRWARE);
}

bool OTA_Flash_Reset(void)
{
    return FLASH_Restore(OTA_FIRWARE);
}

system_tick_t millis(void)
{
    return HAL_GetTick();
}

void delay(uint32_t ms)
{
    unsigned long startTime = millis();
    while( millis() - startTime < ms)
    {

    }
}

void RestoreEsp8266(void)
{
    char restoreCmd[] = "AT+RESTORE\r\n";
    HAL_UART_Transmit(&UartHandle, (uint8_t *)restoreCmd, strlen(restoreCmd), 2000);
}


