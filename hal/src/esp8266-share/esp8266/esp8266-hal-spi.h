/* 
  SPI.h - SPI library for esp8266

  Copyright (c) 2015 Hristo Gochkov. All rights reserved.
  This file is part of the esp8266 core for Arduino environment.
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define SPI_HAS_TRANSACTION

// This defines are not representing the real Divider of the ESP8266
// the Defines match to an AVR Arduino on 16MHz for better compatibility
#define ESP8266_SPI_CLOCK_DIV2 		0x00101001 //8 MHz
#define ESP8266_SPI_CLOCK_DIV4 		0x00241001 //4 MHz
#define ESP8266_SPI_CLOCK_DIV8 		0x004c1001 //2 MHz
#define ESP8266_SPI_CLOCK_DIV16 	0x009c1001 //1 MHz
#define ESP8266_SPI_CLOCK_DIV32 	0x013c1001 //500 KHz
#define ESP8266_SPI_CLOCK_DIV64 	0x027c1001 //250 KHz
#define ESP8266_SPI_CLOCK_DIV128 	0x04fc1001 //125 KHz

#define ESP8266_SPI_MODE0  0x00 ///<  CPOL: 0  CPHA: 0
#define ESP8266_SPI_MODE1  0x01 ///<  CPOL: 0  CPHA: 1
#define ESP8266_SPI_MODE2  0x10 ///<  CPOL: 1  CPHA: 0
#define ESP8266_SPI_MODE3  0x11 ///<  CPOL: 1  CPHA: 1

void spi_selectSet(void);
bool spi_pins(int8_t sck, int8_t miso, int8_t mosi, int8_t ss);
void spi_begin(int8_t sck, int8_t miso, int8_t mosi, int8_t ss);
void spi_end();
void spi_setHwCs(bool use);
void spi_setBitOrder(uint8_t bitOrder);  
void spi_setDataMode(uint8_t dataMode);
void spi_setFrequency(uint32_t freq);
void spi_setClockDivider(uint32_t clockDiv);
uint8_t spi_transfer(uint8_t data);
uint16_t spi_transfer16(uint16_t data);
void spi_write(uint8_t data);
void spi_write16(uint16_t data);
void spi_write16Msb(uint16_t data, bool msb);
void spi_write32(uint32_t data);
void spi_write32Msb(uint32_t data, bool msb);
void spi_writeBytes(uint8_t * data, uint32_t size);
void spi_writePattern(uint8_t * data, uint8_t size, uint32_t repeat);
void spi_transferBytes(uint8_t * out, uint8_t * in, uint32_t size);
void spi_endTransaction(void);

void spi_writeBytes_(uint8_t * data, uint8_t size);
void spi_transferBytes_(uint8_t * out, uint8_t * in, uint8_t size);
inline void spi_setDataBits(uint16_t bits);


#endif
