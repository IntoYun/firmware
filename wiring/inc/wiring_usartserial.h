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

#ifndef WIRING_USARTSERIAL_H_
#define WIRING_USARTSERIAL_H_

#include "firmware_platform_config.h"

#if FIRMWARE_CONFIG_WIRING_USART > 0

#include "wiring_stream.h"
#include "usart_hal.h"

class USARTSerial : public Stream
{
    private:
        HAL_USART_Serial _serial;
        bool _blocking;

    public:
        USARTSerial(HAL_USART_Serial serial);
        virtual ~USARTSerial() {};
        void begin(unsigned long);
        void begin(unsigned long, uint8_t);
        void halfduplex(bool);
        void end();

        virtual void blockOnOverrun(bool);

        virtual int availableForWrite(void);
        virtual int available(void);
        virtual int peek(void);
        virtual int read(void);
        virtual void flush(void);
        size_t write(uint16_t);
        virtual size_t write(uint8_t);

        inline size_t write(unsigned long n) { return write((uint8_t)n); }
        inline size_t write(long n) { return write((uint8_t)n); }
        inline size_t write(unsigned int n) { return write((uint8_t)n); }
        inline size_t write(int n) { return write((uint8_t)n); }

        using Print::write; // pull in write(str) and write(buf, size) from Print

        operator bool();

        bool isEnabled(void);
};

// 不要改成类 为了保证类构造函数使用时，已经初始化
extern USARTSerial &__fetch_global_serial();
#define Serial __fetch_global_serial()

void serialEvent() __attribute__((weak));
#endif

#if FIRMWARE_CONFIG_WIRING_USART > 1
extern USARTSerial &__fetch_global_serial1();
#define Serial1 __fetch_global_serial1()

void serialEvent1(void) __attribute__((weak));
#endif

#if FIRMWARE_CONFIG_WIRING_USART > 2
extern USARTSerial &__fetch_global_serial2();
#define Serial2 __fetch_global_serial2()

void serialEvent2(void) __attribute__((weak));
#endif

#if FIRMWARE_CONFIG_WIRING_USART > 3
extern USARTSerial &__fetch_global_serial3();
#define Serial3 __fetch_global_serial3()

void serialEvent3(void) __attribute__((weak));
#endif

#if FIRMWARE_CONFIG_WIRING_USART > 4
extern USARTSerial &__fetch_global_serial4();
#define Serial4 __fetch_global_serial4()

void serialEvent4(void) __attribute__((weak));
#endif

#if FIRMWARE_CONFIG_WIRING_USART > 5
extern USARTSerial &__fetch_global_serial5();
#define Serial5 __fetch_global_serial5()

void serialEvent5(void) __attribute__((weak));
#endif

#endif

