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
#include "wiring.h"

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
    if (started)
        return;
    started = true;
    // AFIO clock enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    // Enable USART Clock
    *usartMap->usart_apbReg |=  usartMap->usart_clock_en;

    NVIC_InitTypeDef NVIC_InitStructure;
    // Enable the USART Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = usartMap->usart_int_n;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART2_IRQ_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // Configure USART Rx as input floating
    pinMode(usartMap->usart_rx_pin, INPUT);
    // Configure USART Tx as alternate function push-pull
    pinMode(usartMap->usart_tx_pin, AF_OUTPUT_PUSHPULL);

    // Remap USARTn to alternate pins EG. USART1 to pins TX/PB6, RX/PB7
    //  GPIO_PinRemapConfig(usartMap->usart_pin_remap, ENABLE);

    // USART default configuration
    // USART configured as follow:
    // - BaudRate = (set baudRate as 9600 baud)
    // - Word Length = 8 Bits
    // - One Stop Bit
    // - No parity
    // - Hardware flow control disabled (RTS and CTS signals)
    // - Receive and transmit enabled
    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    // Configure USART
    USART_Init(usartMap->usart_peripheral, &USART_InitStructure);

    // Enable USART Receive and Transmit interrupts
    USART_ITConfig(usartMap->usart_peripheral, USART_IT_RXNE, ENABLE);
    //USART_ITConfig(usartMap->usart_peripheral, USART_IT_TXE, ENABLE);
    // Enable UART interrupt in NVIC
    NVIC_EnableIRQ( usartMap->usart_int_n ) ;
    // Enable the USART
    USART_Cmd(usartMap->usart_peripheral, ENABLE);

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
        started = false;
    // wait for transmission of outgoing data
    while (_tx_buffer.head != _tx_buffer.tail);
    // Disable USART Receive and Transmit interrupts
    USART_ITConfig(usartMap->usart_peripheral, USART_IT_RXNE, DISABLE);
    //USART_ITConfig(usartMap->usart_peripheral, USART_IT_TXE, DISABLE);
    // Disable the USART
    USART_Cmd(usartMap->usart_peripheral, DISABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    // Disable the USART Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = usartMap->usart_int_n;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
    // clear any received data
    _rx_buffer.head = _rx_buffer.tail;
    // null ring buffer pointers
    usartMap->usart_tx_buffer = NULL;
    usartMap->usart_rx_buffer = NULL;
    // Undo any pin re-mapping done for this USART
    //GPIO_PinRemapConfig(usartMap->usart_pin_remap, DISABLE);
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
    return -1;
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
    return -1;
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
    return -1;
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
    return;
    // Loop until USART DR register is empty
    while ( _tx_buffer.head != _tx_buffer.tail );
    // Loop until last frame transmission complete
    while (transmitting && (USART_GetFlagStatus(usartMap->usart_peripheral, USART_FLAG_TC) == RESET));
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
    return -1;
    // Send one byte from USART
    USART_SendData(usartMap->usart_peripheral, c);

    // Loop until USART DR register is empty
    while(USART_GetFlagStatus(usartMap->usart_peripheral, USART_FLAG_TXE) == RESET)
    {
    }
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
    if(USART_GetITStatus(usartMap->usart_peripheral, USART_IT_RXNE) != RESET)
    {
        // Read byte from the receive data register
        unsigned char c = USART_ReceiveData(usartMap->usart_peripheral);
        store_char(c, usartMap->usart_rx_buffer);
    }
}

/*********************************************************************************
  *Function		: void Wiring_USART1_Interrupt_Handler(void)      
  *Description	: This function handles USART1 global interrupt request.    
  *Input		      : none         
  *Output		: none        
  *Return		: none       
  *author		: lz       
  *date			: 2015-2-1          
  *Others		: none        
**********************************************************************************/
void Wiring_USART1_Interrupt_Handler(void)
{
    USART_Interrupt_Handler(&USART_MAP[USART_TX_RX]);
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
    USART_Interrupt_Handler(&USART_MAP[USART_A2_A3]);
}

/*********************************************************************************
  *Function		: void Wiring_USART3_Interrupt_Handler(void)      
  *Description	: This function handles USART1 global interrupt request.   
  *Input		      : none         
  *Output		: none        
  *Return		: none       
  *author		: lz       
  *date			: 2015-2-1          
  *Others		: none        
**********************************************************************************/
void Wiring_USART3_Interrupt_Handler(void)
{
    USART_Interrupt_Handler(&USART_MAP[USART_D1_D0]);
}

