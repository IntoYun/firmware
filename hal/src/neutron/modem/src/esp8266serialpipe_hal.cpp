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
#include "stm32f4xx.h"
#include "usart_hal.h"
#include "pinmap_impl.h"
#include "gpio_hal.h"
#include "mdm_hal.h"

#undef putc
#undef getc

static UART_HandleTypeDef UartHandle;

extern "C" {
static void USART_SendData(UART_HandleTypeDef *huart, uint16_t Data);
static uint16_t USART_ReceiveData(UART_HandleTypeDef *huart);
}

Esp8266SerialPipe::Esp8266SerialPipe(int rxSize, int txSize) :
    _pipeRx( rxSize ),
    _pipeTx( txSize )
{
}

Esp8266SerialPipe::~Esp8266SerialPipe(void)
{
    // wait for transmission of outgoing data
    while (_pipeTx.readable())
    {
        char c = _pipeTx.getc();
        HAL_UART_Transmit(&UartHandle,(uint8_t *)&c,1,5);//5ms
    }

    __HAL_RCC_USART2_FORCE_RESET();
    __HAL_RCC_USART2_RELEASE_RESET();

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

    UartHandle.Instance          = USART1;
    UartHandle.Init.BaudRate     = baud;
    UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits     = UART_STOPBITS_1;
    UartHandle.Init.Parity       = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode         = UART_MODE_TX_RX;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&UartHandle);

    //Configure the NVIC for UART
    HAL_NVIC_SetPriority(USART1_IRQn, 0x05, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXNE);
}

// tx channel
int Esp8266SerialPipe::writeable(void)
{
    return _pipeTx.free();
}

int Esp8266SerialPipe::putc(int c)
{
    c = _pipeTx.putc(c);
    txStart();
    return c;
}

int Esp8266SerialPipe::put(const void* buffer, int length, bool blocking)
{
    int count = length;
    const char* ptr = (const char*)buffer;
    if (count)
    {
        do
        {
            int written = _pipeTx.put(ptr, count, false);
            if (written) {
                ptr += written;
                count -= written;
                txStart();
            }
            else if (!blocking)
                break;
            /* nothing / just wait */;
        }
        while (count);
    }
    return (length - count);
}

void Esp8266SerialPipe::txCopy(void)
{
    if (_pipeTx.readable()) {
        char c = _pipeTx.getc();
        USART_SendData(&UartHandle, c);
    }
}

void Esp8266SerialPipe::txIrqBuf(void)
{
    txCopy();
    // detach tx isr if we are done
    if (!_pipeTx.readable())
    __HAL_UART_DISABLE_IT(&UartHandle, UART_IT_TXE);
}

void Esp8266SerialPipe::txStart(void)
{
    // disable the tx isr to avoid interruption
    __HAL_UART_DISABLE_IT(&UartHandle, UART_IT_TXE);
    txCopy();
    // attach the tx isr to handle the remaining data
    if (_pipeTx.readable())
    __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_TXE);
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
    char c = USART_ReceiveData(&UartHandle);
    if (_pipeRx.writeable())
        _pipeRx.putc(c);
    else
        /* overflow */;
}

static void USART_SendData(UART_HandleTypeDef *huart, uint16_t Data)
{
    huart->Instance->DR = (uint16_t)(Data & (uint16_t)0x01FF);
}

static uint16_t USART_ReceiveData(UART_HandleTypeDef *huart)
{
    return (uint16_t)(huart->Instance->DR & (uint16_t)0x01FF);
}


/*******************************************************************************
 * Function Name  : HAL_USART3_Handler
 * Description    : This function handles USART3 global interrupt request.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
extern "C" void HAL_USART1_Handler(UART_HandleTypeDef *huart)
{
    if((__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE) != RESET)
            && (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_RXNE) != RESET))
    {
        esp8266MDM.rxIrqBuf();
    }

    if((__HAL_UART_GET_FLAG(huart, UART_FLAG_TXE) != RESET)
            && (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_TXE) != RESET))
    {
        esp8266MDM.txIrqBuf();
    }
}

