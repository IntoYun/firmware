/**
 ******************************************************************************
 * @file     : wiring_i2c.h
 * @author   : robin
 * @version  : V1.0.0
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


#define I2C_BUFFER_LENGTH 32
#define EVENT_TIMEOUT 100

#define CLOCK_SPEED_100KHZ (uint32_t)100000
#define CLOCK_SPEED_400KHZ (uint32_t)400000

/* typedef struct I2C_Ring_Buffer */
/* { */
/*         unsigned char buffer[I2C_BUFFER_SIZE]; */
/*         volatile unsigned int head; */
/*         volatile unsigned int tail; */
/* } I2C_Ring_Buffer; */

/* typedef enum I2C_Num_Def */
/* { */
/*         I2C1_SCL_SDA  = 0, */
/*         I2C3_SCL_SDA  = 1, */
/* } I2C_Num_Def; */

/* extern typedef struct STM32_I2C_Pin_Alt_Info */
/* { */
/*         GPIO_TypeDef* gpio_peripheral; */
/*         uint16_t gpio_pin; */
/*         uint32_t alternate; */
/* } STM32_I2C_Pin_Alt_Info; */

/* DMA1_Stream0_IRQn */
/* typedef struct STM32_I2C_Info */
/* { */
/*         I2C_TypeDef* i2c_peripheral; */

/*         IRQn_Type i2c_int_irqn; */
/*         IRQn_Type i2c_dma_tx_irqn; */
/*         IRQn_Type i2c_dma_rx_irqn; */

/*         STM32_I2C_Pin_Alt_Info scl_pin_info; */
/*         STM32_I2C_Pin_Alt_Info sda_pin_info; */

/*         void (*I2C_CLOCK_ENABLE) (void); */
/*         //i2chandle */
/*         I2C_HandleTypeDef *i2c_handle; */
/*         // Buffer pointers. These need to be global for IRQ handler access */
/*         I2C_Ring_Buffer* i2c_tx_buffer; */
/*         I2c_Ring_Buffer* i2c_rx_buffer; */
/* } STM32_I2C_Info; */

class TwoWire : public Stream
{
    private:
        I2C_HandleTypeDef I2CHandle;
        I2C_TypeDef *I2C_Type;
        uint32_t I2C_NoStretchMode;

        uint32_t I2C_ClockSpeed;
        bool I2C_SetAsSlave;
        bool I2C_Enabled;
        bool I2C_Initialized;

        // RX Buffer
        uint8_t rxBuffer[I2C_BUFFER_LENGTH];
        uint8_t rxBufferIndex;
        uint8_t rxBufferLength;

        // TX Buffer
        uint8_t txAddress;
        uint8_t txBuffer[I2C_BUFFER_LENGTH];
        uint8_t txBufferIndex;
        uint8_t txBufferLength;

        uint8_t transmitting;

        // Callback user functions
        void (*user_onRequest)(void);
        void (*user_onReceive)(int);
        void onRequestService(void);
        void onReceiveService(uint8_t*, int);
        void init(uint8_t);

    public:
        TwoWire(I2C_TypeDef *I2Cx);
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

        bool isEnabled(void);
};

/* #if I2C_INTERFACES_COUNT > 0 */
extern TwoWire Wire1;
/* #endif */
//#if I2C_INTERFACES_COUNT > 1
extern TwoWire Wire;
//#endif


#endif /* WIRING_I2C_H_ */
