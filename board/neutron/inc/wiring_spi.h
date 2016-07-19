/**
 ******************************************************************************
 * @file     : wiring_spi.h
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
  *******************************************************************************/
#ifndef   WIRING_SPI_H_
#define   WIRING_SPI_H_

#include "wiring.h"

#define SPI_MODE0			0x00
#define SPI_MODE1			0x01
#define SPI_MODE2			0x02
#define SPI_MODE3			0x03

#define SPI_CLOCK_DIV2		SPI_BAUDRATEPRESCALER_2
#define SPI_CLOCK_DIV4		SPI_BAUDRATEPRESCALER_4
#define SPI_CLOCK_DIV8		SPI_BAUDRATEPRESCALER_8
#define SPI_CLOCK_DIV16		SPI_BAUDRATEPRESCALER_16
#define SPI_CLOCK_DIV32		SPI_BAUDRATEPRESCALER_32
#define SPI_CLOCK_DIV64		SPI_BAUDRATEPRESCALER_64
#define SPI_CLOCK_DIV128	SPI_BAUDRATEPRESCALER_128
#define SPI_CLOCK_DIV256	SPI_BAUDRATEPRESCALER_256

class SPIClass
{
    private:
        SPI_HandleTypeDef SpiHandle;
        SPI_TypeDef *SPI_Type;
        SPI_InitTypeDef SPI_InitStructure;
        bool SPI_Bit_Order_Set;
        bool SPI_Data_Mode_Set;
        bool SPI_Clock_Divider_Set;
        bool SPI_Enabled;
        bool SPI_Initialized;
    public:
        SPIClass(SPI_TypeDef *_spi);
        void begin(void);
        void end(void);

        void setBitOrder(uint8_t);
        void setDataMode(uint8_t);
        void setClockDivider(uint8_t);

        byte transfer(byte _data);

        bool isEnabled(void);
};

// #if SPI_INTERFACES_COUNT > 0
extern SPIClass SPI;
// #endif
#if SPI_INTERFACES_COUNT > 1
extern SPIClass SPI_1;
#endif

#endif /*WIRING_SPI_H_ */
