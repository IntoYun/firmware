/**
 ******************************************************************************
 * @file     : wiring_spi.cpp
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
  *******************************************************************************/
#include "wiring_spi.h"

/*********************************************************************************
  *Function       : SPIClass::SPIClass(SPI_TypeDef *_spi, uint8_t _spi_ss, void(*_initCb)(void)) :SPI_Type(_spi), SPI_Default_SS(_spi_ss), initCb(_initCb)
  *Description   : initalize SPIClass
  *Input           : _spi:SPI1 or SPI2 
  				  _spi_ss:From the machine can make the feet
  				  _initCb:call back SPI_Init
  *Output         : none
  *Return         : none
  *author         : lz
  *date            : 6-December-2013
  *Others         :        
**********************************************************************************/
SPIClass::SPIClass(SPI_TypeDef *_spi, uint8_t _spi_ss, void(*_initCb)(void)) :SPI_Type(_spi), SPI_Default_SS(_spi_ss), initCb(_initCb)
{
    SPI_Bit_Order_Set = false;
    SPI_Data_Mode_Set = false;
    SPI_Clock_Divider_Set = false;
    SPI_Enabled = false;
    SPI_Initialized = false;
}

/*********************************************************************************
  *Function      : void SPIClass::init(void) 
  *Description  : initalize 
  *Input          : none
  *Output         : none
  *Return         : none
  *author         : lz
  *date            : 6-December-2013
  *Others         :        
**********************************************************************************/
void SPIClass::init(void) 
{
    if (SPI_Initialized)
    {return;}

    if(initCb)
    {initCb();}

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_CRCPolynomial = 7;

    SPI_Init(SPI_Type,&SPI_InitStructure);
    SPI_Cmd(SPI_Type,ENABLE);
    SPI_Initialized = true;
}

/*********************************************************************************
  *Function     : void SPIClass::begin() 
  *Description  : SPI config 
  *Input          : none
  *Output        : none
  *Return        : none
  *author        : lz
  *date           : 6-December-2013
  *Others        : Call this function if you want to use SPI test configuration
**********************************************************************************/
void SPIClass::begin(void) 
{
    begin(SPI_Default_SS);
}

//带参数 开始函数
void SPIClass::begin(uint16_t ss_pin) 
{
    if (ss_pin >= TOTAL_PINS )
    {return;}

    //初始化
    init();

    //初始化SS片选引脚
    pinMode(ss_pin, OUTPUT);
    digitalWrite(ss_pin, HIGH);
    //设置通讯模式
    if(SPI_Data_Mode_Set != true)
    {setDataMode(SPI_MODE3);}
    //设置波特率分频值
    if(SPI_Clock_Divider_Set != true)
    {setClockDivider(SPI_BaudRatePrescaler_16);}
    //设置数据传输位顺序
    if(SPI_Bit_Order_Set != true)
    {setBitOrder( SPI_FirstBit_MSB);}

    SPI_Init(SPI_Type, &SPI_InitStructure);
    SPI_Cmd(SPI_Type, ENABLE);

    SPI_Enabled = true;
}

/*********************************************************************************
  *Function      : void SPIClass::end() 
  *Description  : SPI disable 
  *Input          : none
  *Output        : none
  *Return        : none
  *author        : lz
  *date           : 6-December-2013
  *Others        : disable SPI
**********************************************************************************/
void SPIClass::end(void) 
{
    if(SPI_Enabled != false)
    {
        SPI_Cmd(SPI_Type, DISABLE);
        SPI_Enabled = false;
    }
    SPI_Initialized = false;
}

/*********************************************************************************
  *Function     : void SPIClass::setBitOrder(uint8_t bitOrder)
  *Description  : Set the data transmission bit order 
  *Input          : bitOrder: SPI_FirstBit_MSB or SPI_FirstBit_LSB
  *Output        : none
  *Return        : none
  *author        : lz
  *date           : 6-December-2013
  *Others        :         
**********************************************************************************/
void SPIClass::setBitOrder(uint8_t bitOrder)
{
    if(bitOrder == LSBFIRST)
    {
        SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
    }
    else
    {
        SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    }

    SPI_Init(SPI_Type, &SPI_InitStructure);
    SPI_Bit_Order_Set = true;
}

/*********************************************************************************
  *Function     : void SPIClass::setDataMode(uint8_t mode)
  *Description  : Set the data mode
  *Input          : mode: Data acquisition way
  *Output       : none
  *Return       : none
  *author       : lz
  *date          : 6-December-2013
  *Others       :         
**********************************************************************************/
void SPIClass::setDataMode(uint8_t mode)
{
    if(SPI_Enabled != false)
    {
        SPI_Cmd(SPI_Type, DISABLE);
    }

    switch(mode)
    {
        case SPI_MODE0:
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
            break;
        case SPI_MODE1:
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
            break;
        case SPI_MODE2:
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
            break;
        case SPI_MODE3:
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
            break;
    }
    SPI_Init(SPI_Type, &SPI_InitStructure);
    if(SPI_Enabled != false)
    {
        SPI_Cmd(SPI_Type, ENABLE);
    }
    SPI_Data_Mode_Set = true;
}

/*********************************************************************************
  *Function     : void SPIClass::setClockDivider(uint8_t rate)
  *Description  : Set the baud rate
  *Input        : rate: baud rate
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2013
  *Others       :         
**********************************************************************************/
void SPIClass::setClockDivider(uint8_t rate)
{
    SPI_InitStructure.SPI_BaudRatePrescaler = rate;
    SPI_Init(SPI_Type, &SPI_InitStructure);
    SPI_Clock_Divider_Set = true;
}

/*********************************************************************************
  *Function     : byte SPIClass::transfer(byte _data) 
  *Description  : transfer data
  *Input        : _data:
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2013
  *Others       :         
**********************************************************************************/
byte SPIClass::transfer(byte _data) 
{
    /* Wait for SPI1 Tx buffer empty */
    while (SPI_I2S_GetFlagStatus(SPI_Type, SPI_I2S_FLAG_TXE) == RESET);
    /* Send SPI1 data */
    SPI_I2S_SendData(SPI_Type, _data);

    /* Wait for SPI1 data reception */
    while (SPI_I2S_GetFlagStatus(SPI_Type, SPI_I2S_FLAG_RXNE) == RESET);
    /* Read and return SPI1 received data */
    return SPI_I2S_ReceiveData(SPI_Type);
}

void SPIClass::attachInterrupt(void) 
{
    //To Do
}

void SPIClass::detachInterrupt(void) 
{
    //To Do
}

/*********************************************************************************
  *Function     : bool SPIClass::isEnabled(void)  
  *Description  : return data status
  *Input        : _data:
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2013
  *Others       :         
**********************************************************************************/
bool SPIClass::isEnabled(void) 
{
    return SPI_Enabled;
}

#if SPI_INTERFACES_COUNT > 0
/*********************************************************************************
  *Function      : static void SPI_Init(void) 
  *Description  : initialize SPI
  *Input          : _data:
  *Output        : none
  *Return        : none
  *author        : lz
  *date           : 6-December-2013
  *Others        :         
**********************************************************************************/
static void SPI_Init(void) 
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
    pinMode(PIN_SPI_SCK, AF_OUTPUT_PUSHPULL);
    pinMode(PIN_SPI_MOSI, AF_OUTPUT_PUSHPULL);
    pinMode(PIN_SPI_MISO, INPUT);
}

SPIClass SPI(SPI_INTERFACE1, PIN_SPI_SS, SPI_Init);

void Wiring_SPI1_Interrupt_Handler(void)
{
    //To Do
}
#endif//>0

#if SPI_INTERFACES_COUNT > 1
static void SPI1_Init(void) 
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
    pinMode(PIN_SPI_SCK1, AF_OUTPUT_PUSHPULL);
    pinMode(PIN_SPI_MOSI1, AF_OUTPUT_PUSHPULL);
    pinMode(PIN_SPI_MISO1, INPUT);
}

SPIClass SPI_1(SPI_INTERFACE2, PIN_SPI_SS1, SPI1_Init);

void Wiring_SPI2_Interrupt_Handler(void)
{
    //To Do
}
#endif//>1

