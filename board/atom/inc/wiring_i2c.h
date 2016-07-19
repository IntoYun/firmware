/**
 ******************************************************************************
 * @file     : wiring_i2c.h
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
**/

#ifndef   WIRING_I2C_H_
#define   WIRING_I2C_H_

#include "wiring_stream.h"
#include "wiring.h"


#define BUFFER_LENGTH 32
#define EVENT_TIMEOUT 100

#define CLOCK_SPEED_100KHZ (uint32_t)100000
#define CLOCK_SPEED_400KHZ (uint32_t)400000

class TwoWire : public Stream
{
    private:
        // TWI instance
        I2C_InitTypeDef I2C_InitStructure;
        I2C_TypeDef *I2C_Type;

        uint32_t I2C_ClockSpeed;
        bool I2C_SetAsSlave;
        bool I2C_Enabled;
        bool I2C_Initialized;

        // RX Buffer
        uint8_t rxBuffer[BUFFER_LENGTH];
        uint8_t rxBufferIndex;
        uint8_t rxBufferLength;

        // TX Buffer
        uint8_t txAddress;
        uint8_t txBuffer[BUFFER_LENGTH];
        uint8_t txBufferIndex;
        uint8_t txBufferLength;

        uint8_t transmitting;

        // Callback user functions
        void (*user_onRequest)(void);
        void (*user_onReceive)(int);

        void (*initCb)(uint8_t);               //初始化函数指针

        void init(uint8_t);

    public:
        TwoWire(I2C_TypeDef *_twi, void(*_initCb)(uint8_t));
        void setSpeed(uint32_t);
        void stretchClock(bool);
        void begin(void);
        void begin(uint8_t);
        void begin(int);
        void beginTransmission(uint8_t);
        void beginTransmission(int);
        uint8_t endTransmission(void);
        uint8_t endTransmission(uint8_t);
        uint8_t requestFrom(uint8_t, uint8_t);
        uint8_t requestFrom(uint8_t, uint8_t, uint8_t);
        uint8_t requestFrom(int, int);
        uint8_t requestFrom(int, int, int);
        virtual size_t write(uint8_t);
        virtual size_t write(const uint8_t *, size_t);
        virtual int available(void);
        virtual int read(void);
        virtual int peek(void);
        virtual void flush(void);
        void onReceive( void (*)(int) );
        void onRequest( void (*)(void) );

        inline size_t write(unsigned long n) { return write((uint8_t)n); }
        inline size_t write(long n) { return write((uint8_t)n); }
        inline size_t write(unsigned int n) { return write((uint8_t)n); }
        inline size_t write(int n) { return write((uint8_t)n); }
        using Print::write;

        void slaveEventHandler(void);
        bool isEnabled(void);
        // lz
        void DMA_Control(I2C_TypeDef *I2Cx,uint32_t direction,FunctionalState NewState);
        FlagStatus GetDMAFlagStatus(I2C_TypeDef *I2Cx,uint32_t direction);
        void ClearDMAFlag(I2C_TypeDef *I2Cx,uint32_t direction);
        uint16_t GetDMACurrDataCounter(I2C_TypeDef *I2Cx);
};

#if I2C_INTERFACES_COUNT > 0
extern TwoWire Wire;
#endif
#if I2C_INTERFACES_COUNT > 1
extern TwoWire Wire1;
#endif


#endif /* WIRING_I2C_H_ */

