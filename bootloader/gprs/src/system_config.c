#include "hw_config.h"
#include "boot_debug.h"

#define GPRSA6_USART_QUEUE_SIZE              1024

UART_HandleTypeDef UartHandleDebug;
UART_HandleTypeDef UartHandleGPRSA6;


SDK_QUEUE USART_GPRSA6_Queue;


void log_output(const char* msg)
{
    while(*msg)
    {
        HAL_UART_Transmit(&UartHandleDebug, (uint8_t *)msg++, 1, 10);
    }
}

void usart_debug_initial(uint32_t baud)
{
    HAL_UART_DeInit(&UartHandleDebug);

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    GPIO_InitTypeDef  GPIO_InitStruct;
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;//GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    UartHandleDebug.Instance          = USART2;
    UartHandleDebug.Init.BaudRate     = baud;
    UartHandleDebug.Init.WordLength   = UART_WORDLENGTH_8B;
    UartHandleDebug.Init.StopBits     = UART_STOPBITS_1;
    UartHandleDebug.Init.Parity       = UART_PARITY_NONE;
    UartHandleDebug.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    UartHandleDebug.Init.Mode         = UART_MODE_TX_RX;
    UartHandleDebug.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&UartHandleDebug);
}

void usart_GPRSA6_initial(uint32_t baud)
{
    sdkReleaseQueue(&USART_GPRSA6_Queue);
    HAL_UART_DeInit(&UartHandleGPRSA6);

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();

    GPIO_InitTypeDef  GPIO_InitStruct;
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;//GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    UartHandleGPRSA6.Instance          = USART1;
    UartHandleGPRSA6.Init.BaudRate     = baud;
    UartHandleGPRSA6.Init.WordLength   = UART_WORDLENGTH_8B;
    UartHandleGPRSA6.Init.StopBits     = UART_STOPBITS_1;
    UartHandleGPRSA6.Init.Parity       = UART_PARITY_NONE;
    UartHandleGPRSA6.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    UartHandleGPRSA6.Init.Mode         = UART_MODE_TX_RX;
    UartHandleGPRSA6.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&UartHandleGPRSA6);

    sdkInitialQueue(&USART_GPRSA6_Queue, GPRSA6_USART_QUEUE_SIZE); //初始化esp8266接受缓冲队列
    //Configure the NVIC for UART
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    __HAL_UART_ENABLE_IT(&UartHandleGPRSA6, UART_IT_RXNE);
}

void HAL_USART1_GPRSA6_Handler(UART_HandleTypeDef *huart)
{
    uint8_t c;

    if((__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE) != RESET)
            && (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_RXNE) != RESET))
    {
        c = (uint16_t)(huart->Instance->DR & (uint16_t)0x00FF);
        sdkInsertQueue(&USART_GPRSA6_Queue, &c, 1);
    }
}

void HAL_System_Config(void)
{
    Set_System();
    usart_debug_initial(115200);
    set_logger_output(log_output, ALL_LEVEL); //注册debug实现函数
    HAL_RTC_Initial();
    HAL_UI_Initial();
    GPRSA6_GPIO_Initial();
    usart_GPRSA6_initial(115200);  //gprs a6通讯 采取115200波特率
    GPRSA6_Reset();
}

void GPRSA6_GPIO_Initial(void)
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

void GPRSA6_Reset(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_Delay(200);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}

void GPRSA6_Enter_UpdateMode(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_Delay(200);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
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

void System_Reset(void)
{
    NVIC_SystemReset();
}

