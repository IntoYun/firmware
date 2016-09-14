#include "hw_config.h"
#include "ui_hal.h"

UART_HandleTypeDef UartHandleDebug;

void log_output(const char* msg)
{
    while(*msg)
    {
        HAL_UART_Transmit(&UartHandleDebug, (uint8_t *)msg++, 1, 10);
    }
}

void usart_debug_initial(uint32_t baud)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    GPIO_InitTypeDef  GPIO_InitStruct;
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_2;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    //GPIO_InitStruct.Pin       = GPIO_PIN_3;
    //GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
    //HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    UartHandleDebug.Instance          = USART2;
    UartHandleDebug.Init.BaudRate     = baud;
    UartHandleDebug.Init.WordLength   = UART_WORDLENGTH_8B;
    UartHandleDebug.Init.StopBits     = UART_STOPBITS_1;
    UartHandleDebug.Init.Parity       = UART_PARITY_NONE;
    UartHandleDebug.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    UartHandleDebug.Init.Mode         = UART_MODE_TX_RX;

    HAL_UART_DeInit(&UartHandleDebug);
    HAL_UART_Init(&UartHandleDebug);

    HAL_UART_Transmit(&UartHandleDebug, (uint8_t *)&"1", 1, 0xFFFF);
}

void HAL_System_Config(void)
{
    Set_System();
    //HAL_RTC_Initial();
    HAL_UI_Initial();
    usart_debug_initial(115200);
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

