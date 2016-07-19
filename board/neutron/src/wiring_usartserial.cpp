/**
 ******************************************************************************
 * @file     : wiring_usartserial.cpp
 * @author   : robin
 * @version	 : V1.0.0
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
#include "wiring_usartserial.h"

/*
 * USART mapping
 */
STM32_USART_Info USART_MAP[] =
{
    /*
    * USRAT_peripheral (USART1-USART2; not using others)
    * interrupt number (USART1_IRQn or USART2_IRQn)
    * TX pin info
    * RX pin info
    * UartHandle
    * <tx_buffer pointer> used internally and does not appear below
    * <rx_buffer pointer> used internally and does not appear below
    */
    { USART2, USART2_IRQn, {GPIOA, GPIO_PIN_2, GPIO_AF7_USART2}, {GPIOA, GPIO_PIN_3, GPIO_AF7_USART2} },
};


/*********************************************************************************
  *Function		: inline void store_char(unsigned char c, Ring_Buffer *buffer)
  *Description	: Store the data
  *Input		      : data
  *Output		: none
  *Return		: none
  *author		: lz
  *date			: 2015-2-1
  *Others		: none
**********************************************************************************/
inline void store_char(unsigned char c, Ring_Buffer *buffer)
{
    unsigned i = (unsigned int)(buffer->head + 1) % SERIAL_BUFFER_SIZE;

    if (i != buffer->tail)
    {
        buffer->buffer[buffer->head] = c;
        buffer->head = i;
    }
}

/*********************************************************************************
  *Function		: USARTSerial::USARTSerial(STM32_USART_Info *usartMapPtr)
  *Description	: usart initialize
  *Input		      : usart number
  *Output		: none
  *Return		: none
  *author		: lz
  *date			: 2015-2-1
  *Others		: none
**********************************************************************************/
USARTSerial::USARTSerial(STM32_USART_Info *usartMapPtr)
{
    usartMap = usartMapPtr;
    usartMap->UartHandle = &UartHandle;
    usartMap->usart_rx_buffer = &_rx_buffer;
    usartMap->usart_tx_buffer = &_tx_buffer;

    memset(&_rx_buffer, 0, sizeof(Ring_Buffer));
    memset(&_tx_buffer, 0, sizeof(Ring_Buffer));

    transmitting = false;
    USARTSerial_Enabled = false;
}

/*********************************************************************************
 *Function		: void USARTSerial::begin(unsigned long baud)
 *Description	: usart begin
 *Input		      : set baud rate
 *Output		: none
 *Return		: none
 *author		: lz
 *date			: 2015-2-1
 *Others		: none
 **********************************************************************************/
void USARTSerial::begin(unsigned long baud)
{
    if (started) {
        return;
    }
    started = true;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();
    GPIO_InitTypeDef  GPIO_InitStruct;
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = usartMap->tx_pin_info.gpio_pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = usartMap->tx_pin_info.alternate;
    HAL_GPIO_Init(usartMap->tx_pin_info.gpio_peripheral, &GPIO_InitStruct);
    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin = usartMap->rx_pin_info.gpio_pin;
    GPIO_InitStruct.Alternate = usartMap->rx_pin_info.alternate;
    HAL_GPIO_Init(usartMap->tx_pin_info.gpio_peripheral, &GPIO_InitStruct);

    UartHandle.Instance          = usartMap->usart_peripheral;
    UartHandle.Init.BaudRate     = baud;
    UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits     = UART_STOPBITS_1;
    UartHandle.Init.Parity       = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode         = UART_MODE_TX_RX;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&UartHandle);

    //Configure the NVIC for UART
    HAL_NVIC_SetPriority(usartMap->usart_int_n, 0x0f, 0);
    HAL_NVIC_EnableIRQ(usartMap->usart_int_n);
    __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXNE);
    USARTSerial_Enabled = true;
    transmitting = false;
}

/*********************************************************************************
  *Function		: void USARTSerial::begin(unsigned long baud, byte config)
  *Description	: usart baud rate
  *Input		      : baud rate
  *Output		: none
  *Return		: none
  *author		: lz
  *date			: 2015-2-1
  *Others		: none
**********************************************************************************/
void USARTSerial::begin(unsigned long baud, byte config)
{
    begin(baud);
}

/*********************************************************************************
  *Function		: void USARTSerial::begin(void)
  *Description	: usart begin  The default baud rate is 115200
  *Input		      : none
  *Output		: none
  *Return		: none
  *author		: lz
  *date			: 2015-2-1
  *Others		: none
**********************************************************************************/
void USARTSerial::begin(void)
{
    begin(115200);
}

/*********************************************************************************
  *Function		: void USARTSerial::end(void)
  *Description	: stop usart
  *Input		      : none
  *Output		: none
  *Return		: none
  *author		: lz
  *date			: 2015-2-1
  *Others		: none
**********************************************************************************/
void USARTSerial::end(void)
{
    if (started)
    {started = false;}

    // wait for transmission of outgoing data
    while (_tx_buffer.head != _tx_buffer.tail);
    // clear any received data
    _rx_buffer.head = _rx_buffer.tail;
    // null ring buffer pointers
    usartMap->usart_tx_buffer = NULL;
    usartMap->usart_rx_buffer = NULL;
    //Reset peripherals ##################################################*/
    __HAL_RCC_USART2_FORCE_RESET();
    __HAL_RCC_USART2_RELEASE_RESET();

    /* Configure UART Tx as alternate function */
    HAL_GPIO_DeInit(usartMap->tx_pin_info.gpio_peripheral, usartMap->tx_pin_info.gpio_pin);
    /* Configure UART Rx as alternate function */
    HAL_GPIO_DeInit(usartMap->rx_pin_info.gpio_peripheral, usartMap->rx_pin_info.gpio_pin);

    //Disable the NVIC for UART ##########################################*/
    HAL_NVIC_DisableIRQ(usartMap->usart_int_n);
    USARTSerial_Enabled = false;
}

/*********************************************************************************
  *Function		: int USARTSerial::available(void)
  *Description	: To judge whether the data effectively
  *Input		      : none
  *Output		: none
  *Return		: none
  *author		: lz
  *date			: 2015-2-1
  *Others		: none
**********************************************************************************/
int USARTSerial::available(void)
{
   if (USARTSerial_Enabled == false)
{return -1;}
    return (unsigned int)(SERIAL_BUFFER_SIZE + _rx_buffer.head - _rx_buffer.tail) % SERIAL_BUFFER_SIZE;
}

/*********************************************************************************
  *Function		: int USARTSerial::peek(void)
  *Description	: Returns the next byte (character) of incoming data without removing it from the internal buffer.
  *Input		      : none
  *Output		: none
  *Return		: int : the first byte of incoming data available (or -1 if no data is available)
  *author		: lz
  *date			: 2015-2-1
  *Others		: none
**********************************************************************************/
int USARTSerial::peek(void)
{
   if (USARTSerial_Enabled == false)
   {return -1;}
    if (_rx_buffer.head == _rx_buffer.tail)
    {
        return -1;
    }
    else
    {
        return _rx_buffer.buffer[_rx_buffer.tail];
    }
}

/*********************************************************************************
  *Function		: int USARTSerial::read(void)
  *Description	: none
  *Input		      : none
  *Output		: none
  *Return		: the recieved date
  *author		: lz
  *date			: 2015-2-1
  *Others		: none
**********************************************************************************/
int USARTSerial::read(void)
{
   if (USARTSerial_Enabled == false)
   {return -1;}
    // if the head isn't ahead of the tail, we don't have any characters
    if (_rx_buffer.head == _rx_buffer.tail)
    {
        return -1;
    }
    else
    {
        unsigned char c = _rx_buffer.buffer[_rx_buffer.tail];
        _rx_buffer.tail = (unsigned int)(_rx_buffer.tail + 1) % SERIAL_BUFFER_SIZE;
        return c;
    }
}

/*********************************************************************************
  *Function		: void USARTSerial::flush(void)
  *Description	: none
  *Input		      : none
  *Output		: none
  *Return		: none
  *author		: lz
  *date			: 2015-2-1
  *Others		: none
**********************************************************************************/
void USARTSerial::flush(void)
{
    if (USARTSerial_Enabled == false)
    {return;}
    transmitting = false;
}

/*********************************************************************************
  *Function		: size_t USARTSerial::write(uint8_t c)
  *Description	: usart send data
  *Input		      : data
  *Output		: none
  *Return		: none
  *author		: lz
  *date			: 2015-2-1
  *Others		: none
**********************************************************************************/
size_t USARTSerial::write(uint8_t c)
{
    if (USARTSerial_Enabled == false)
    {return -1;}
    HAL_UART_Transmit(&UartHandle, &c, 1, 5);//5ms
    return 1;
}

/*********************************************************************************
  *Function		: USARTSerial::operator bool(void)
  *Description	: none
  *Input		      : none
  *Output		: none
  *Return		: none
  *author		: lz
  *date			: 2015-2-1
  *Others		: none
**********************************************************************************/
USARTSerial::operator bool(void)
{
    return true;
}

/*********************************************************************************
  *Function		: bool USARTSerial::isEnabled(void)
  *Description	: check the usart serial port is working
  *Input		      : none
  *Output		: none
  *Return		: true: working    false: close
  *author		: lz
  *date			: 2015-2-1
  *Others		: none
**********************************************************************************/
bool USARTSerial::isEnabled(void)
{
    return USARTSerial_Enabled;
}

/*********************************************************************************
  *Function		: static void USART_Interrupt_Handler(STM32_USART_Info *usartMap)
  *Description	: This function handles USART2 global interrupt request.
  *Input		      : none
  *Output		: none
  *Return		: none
  *author		: lz
  *data			: 2015-2-1
  *Others		: none
**********************************************************************************/
static void USART_Interrupt_Handler(STM32_USART_Info *usartMap)
{
    if(__HAL_UART_GET_FLAG(usartMap->UartHandle, UART_FLAG_RXNE ) != RESET)
    {
        // Read byte from the receive data register
        unsigned char c = (uint16_t)(usartMap->usart_peripheral->DR & (uint16_t)0x00FF);
        store_char(c, usartMap->usart_rx_buffer);
    }
}

/*********************************************************************************
  *Function		: void Wiring_USART2_Interrupt_Handler(void)
  *Description	: This function handles USART1 global interrupt request.
  *Input		      : none
  *Output		: none
  *Return		: none
  *author		: lz
  *date			: 2015-2-1
  *Others		: none
**********************************************************************************/
void Wiring_USART2_Interrupt_Handler(void)
{
    USART_Interrupt_Handler(&USART_MAP[USART_TX_RX]);
}

// serial
USARTSerial Serial(&USART_MAP[USART_TX_RX]);

