/**
 ******************************************************************************
 * @file     : wiring_usartserial.h
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
#ifndef   WIRING_USARTSERIAL_H_
#define   WIRING_USARTSERIAL_H_

#include "wiring_print.h"
#include "wiring_printable.h"
#include "wiring_stream.h"

#define SERIAL_BUFFER_SIZE 64

typedef struct Ring_Buffer
{
    unsigned char buffer[SERIAL_BUFFER_SIZE];
    volatile unsigned int head;
    volatile unsigned int tail;
} Ring_Buffer;

typedef enum USART_Num_Def
{
    USART_TX_RX =0,
} USART_Num_Def;

typedef struct STM32_Pin_Alt_Info
{
    GPIO_TypeDef* gpio_peripheral;
    uint16_t gpio_pin;
    uint32_t alternate;
} STM32_Pin_Alt_Info;


typedef struct STM32_USART_Info
{
    USART_TypeDef* usart_peripheral;
    IRQn_Type usart_int_n;
    STM32_Pin_Alt_Info tx_pin_info;
    STM32_Pin_Alt_Info rx_pin_info;
    //uarthandle
    UART_HandleTypeDef *UartHandle;
    // Buffer pointers. These need to be global for IRQ handler access
    Ring_Buffer* usart_tx_buffer;
    Ring_Buffer* usart_rx_buffer;
} STM32_USART_Info;


class USARTSerial : public Stream
{
    private:
        UART_HandleTypeDef UartHandle;
        bool USARTSerial_Enabled;
        bool transmitting;
        Ring_Buffer _rx_buffer;
        Ring_Buffer _tx_buffer;
        STM32_USART_Info *usartMap; // pointer to USART_MAP[] containing USART peripheral register locations (etc)
        bool started;

    public:
        USARTSerial(STM32_USART_Info *usartMapPtr);
        virtual ~USARTSerial() {};
        void begin(void);
        void begin(unsigned long);
        void begin(unsigned long, uint8_t);
        void end(void);
        void stop(void);  // for deInit the UART, zhangbaoxian,2016.06.22

        virtual int available(void);
        virtual int peek(void);
        virtual int read(void);
        virtual void flush(void);
        virtual size_t write(uint8_t);

        inline size_t write(unsigned long n) { return write((uint8_t)n); }
        inline size_t write(long n) { return write((uint8_t)n); }
        inline size_t write(unsigned int n) { return write((uint8_t)n); }
        inline size_t write(int n) { return write((uint8_t)n); }

        using Print::write; // pull in write(str) and write(buf, size) from Print

        operator bool(void);

        bool isEnabled(void);
};


extern "C" {

void Wiring_USART1_Interrupt_Handler(void);
void Wiring_USART2_Interrupt_Handler(void);

}
extern USARTSerial Serial;       // USART2 on PA3/2 (IntoRobot A2, A3)

#endif /*WIRING_USARTSERIAL_H_*/

