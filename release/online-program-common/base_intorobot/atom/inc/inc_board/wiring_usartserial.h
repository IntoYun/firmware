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
    USART_A2_A3,
    USART_D1_D0
} USART_Num_Def;

#define TOTAL_USARTS 3

#define GPIO_Remap_None 0

typedef struct STM32_USART_Info 
{
    USART_TypeDef* usart_peripheral;
    __IO uint32_t* usart_apbReg;
    uint32_t usart_clock_en; 

    IRQn usart_int_n;

    uint16_t usart_tx_pin;
    uint16_t usart_rx_pin;

    uint32_t usart_pin_remap;

    // Buffer pointers. These need to be global for IRQ handler access
    Ring_Buffer* usart_tx_buffer;
    Ring_Buffer* usart_rx_buffer;
} STM32_USART_Info;

extern STM32_USART_Info USART_MAP[TOTAL_USARTS];

//extern STM32_Pin_Info PIN_MAP[];

class USARTSerial : public Stream
{
    private:
        USART_InitTypeDef USART_InitStructure;
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

#endif /*WIRING_USARTSERIAL_H_*/

