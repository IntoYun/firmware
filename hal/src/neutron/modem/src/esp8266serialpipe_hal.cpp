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

#include "esp8266serialpipe_hal.h"
#include "hw_config.h"
#include "usart_hal.h"
#include "pinmap_impl.h"
#include "gpio_hal.h"
#include "mdm_hal.h"

#ifdef putc
#undef putc
#undef getc
#endif


UART_HandleTypeDef UartHandle_ESP8266;

Esp8266SerialPipe::Esp8266SerialPipe(int rxSize, int txSize) :
    _pipeRx( rxSize ),
    _pipeTx( txSize )
{
}

Esp8266SerialPipe::~Esp8266SerialPipe(void)
{
    // wait for transmission of outgoing data
    __HAL_RCC_USART1_FORCE_RESET();
    __HAL_RCC_USART1_RELEASE_RESET();

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9);
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_10);

    HAL_NVIC_DisableIRQ(USART1_IRQn);
}

void Esp8266SerialPipe::begin(unsigned int baud)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();

    GPIO_InitTypeDef  GPIO_InitStruct;
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_9;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    UartHandle_ESP8266.Instance          = USART1;
    UartHandle_ESP8266.Init.BaudRate     = baud;
    UartHandle_ESP8266.Init.WordLength   = UART_WORDLENGTH_8B;
    UartHandle_ESP8266.Init.StopBits     = UART_STOPBITS_1;
    UartHandle_ESP8266.Init.Parity       = UART_PARITY_NONE;
    UartHandle_ESP8266.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    UartHandle_ESP8266.Init.Mode         = UART_MODE_TX_RX;
    UartHandle_ESP8266.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_DeInit(&UartHandle_ESP8266);
    HAL_UART_Init(&UartHandle_ESP8266);

    //Configure the NVIC for UART
    HAL_NVIC_SetPriority(USART1_IRQn, 0x05, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    __HAL_UART_ENABLE_IT(&UartHandle_ESP8266, UART_IT_RXNE);
}

// tx channel
int Esp8266SerialPipe::writeable(void)
{
    return 1;
}

int Esp8266SerialPipe::putc(int c)
{
    uint8_t data = c;
    HAL_UART_Transmit(&UartHandle_ESP8266, &data, 1, 1000);//1s  带操作系统待验证
    return c;
}

int Esp8266SerialPipe::put(const void* buffer, int length, bool blocking)
{
    int n;
    const char* ptr = (const char*)buffer;

    for(n=0; n<length; n++)
    {
        putc(ptr[n]);
    }
    return length;
}

// rx channel
int Esp8266SerialPipe::readable(void)
{
    return _pipeRx.readable();
}

int Esp8266SerialPipe::getc(void)
{
    if (!_pipeRx.readable())
        return EOF;
    return _pipeRx.getc();
}

int Esp8266SerialPipe::get(void* buffer, int length, bool blocking)
{
    return _pipeRx.get((char*)buffer,length,blocking);
}

void Esp8266SerialPipe::rxIrqBuf(void)
{
    uint8_t c = (uint16_t)(UartHandle_ESP8266.Instance->DR & (uint16_t)0x00FF);
    if (_pipeRx.writeable())
        _pipeRx.putc(c);
    else
        /* overflow */;
    DEBUG_D("%c ", c);
}

extern "C"
{
    void HAL_USART1_Handler(UART_HandleTypeDef *huart)
    {
        if((__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE) != RESET)
                && (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_RXNE) != RESET))
        {
            esp8266MDM.rxIrqBuf();
        }
    }

    // Serial1 interrupt handler
    void USART1_IRQHandler(void)
    {
        HAL_USART1_Handler(&UartHandle_ESP8266);
    }
}
