/**
 ******************************************************************************
 * @file     : wiring_spi.cpp
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
#include "wiring_spi.h"


SPIClass::SPIClass(SPI_TypeDef *_spi):SPI_Type(_spi)
{
    SPI_Bit_Order_Set = false;
    SPI_Data_Mode_Set = false;
    SPI_Clock_Divider_Set = false;
    SPI_Enabled = false;
    SPI_Initialized = false;
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
    SpiHandle.Instance = SPI_Type;

    // SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
    // SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
    // SpiHandle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
    SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    SpiHandle.Init.CRCPolynomial     = 7;
    SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
    // SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    SpiHandle.Init.NSS               = SPI_NSS_SOFT;
    SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;

    SpiHandle.Init.Mode              = SPI_MODE_MASTER;
    //SpiHandle.Init.Mode              = SPI_MODE_SLAVE;


    // CLOCK and pin

    GPIO_InitTypeDef GPIO_InitStruct;
    if(SpiHandle.Instance == SPI1)
    {
        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* NSS PA4; SCK PA5; MISO PA6; MOSI PA7; AF5 */
        __HAL_RCC_GPIOA_CLK_ENABLE(); // SCK, MISO, MOSI
        /* Enable SPI Clock */
        __HAL_RCC_SPI1_CLK_ENABLE();

        /*##-2- Configure peripheral GPIO ##########################################*/
        /* SPI SCK GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_5;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* SPI MISO GPIO pin configuration  */
        GPIO_InitStruct.Pin = GPIO_PIN_6;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* SPI MOSI GPIO pin configuration  */
        GPIO_InitStruct.Pin = GPIO_PIN_7;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    }
    else if(SpiHandle.Instance == SPI3)
    {
        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* NSS PA15; SCK PB3; MISO PB4; MOSI PB5;  AF6*/
        __HAL_RCC_GPIOB_CLK_ENABLE(); // SCK, MISO, MOSI
        /* Enable SPI Clock */
        __HAL_RCC_SPI3_CLK_ENABLE();

        /*##-2- Configure peripheral GPIO ##########################################*/
        /* SPI SCK GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_3;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
        GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;

        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* SPI MISO GPIO pin configuration  */
        GPIO_InitStruct.Pin = GPIO_PIN_4;
        GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;

        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* SPI MOSI GPIO pin configuration  */
        GPIO_InitStruct.Pin = GPIO_PIN_5;
        GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;

        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }

    // pinMode(ss_pin, OUTPUT);
    // digitalWrite(ss_pin, HIGH);

    if(SPI_Data_Mode_Set != true)
    {
        setDataMode(SPI_MODE3);
    }

    if(SPI_Clock_Divider_Set != true)
    {
        setClockDivider(SPI_BAUDRATEPRESCALER_16);
    }

    if(SPI_Bit_Order_Set != true)
    {
        setBitOrder( SPI_FIRSTBIT_MSB);
    }

    if(HAL_SPI_Init(&SpiHandle) != HAL_OK)
    {
        while(1)
        {}
    }
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

        if(SpiHandle.Instance == SPI1)
        {
            /*##-1- Reset peripherals ##################################################*/
            __HAL_RCC_SPI1_FORCE_RESET();
            __HAL_RCC_SPI1_RELEASE_RESET();

            /*##-2- Disable peripherals and GPIO Clocks ################################*/
            /* Configure SPI SCK as alternate function  */
            HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);
            /* Configure SPI MISO as alternate function  */
            HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6);
            /* Configure SPI MOSI as alternate function  */
            HAL_GPIO_DeInit(GPIOA, GPIO_PIN_7);
        }
        else if(SpiHandle.Instance == SPI3)
        {
            /*##-1- Reset peripherals ##################################################*/
            __HAL_RCC_SPI3_FORCE_RESET();
            __HAL_RCC_SPI3_RELEASE_RESET();

            /*##-2- Disable peripherals and GPIO Clocks ################################*/
            /* Configure SPI SCK as alternate function  */
            HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3);
            /* Configure SPI MISO as alternate function  */
            HAL_GPIO_DeInit(GPIOB, GPIO_PIN_4);
            /* Configure SPI MOSI as alternate function  */
            HAL_GPIO_DeInit(GPIOB, GPIO_PIN_5);
        }
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
        SpiHandle.Init.FirstBit = SPI_FIRSTBIT_LSB;
    }
    else
    {
        SpiHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    }

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
    switch(mode)
    {
        case SPI_MODE0:
            SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
            SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
            break;
        case SPI_MODE1:
            SpiHandle.Init.CLKPhase          = SPI_PHASE_2EDGE;
            SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
            break;
        case SPI_MODE2:
            SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
            SpiHandle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
            break;
        case SPI_MODE3:
            SpiHandle.Init.CLKPhase          = SPI_PHASE_2EDGE;
            SpiHandle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
            break;
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
    SpiHandle.Init.BaudRatePrescaler = rate;
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
    byte RxData;
    if(HAL_SPI_TransmitReceive(&SpiHandle, &_data, &RxData, 1, 5) != HAL_OK)// 5ms
    {
        while(1)
        {}
    }
    return RxData;
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

//#if SPI_INTERFACES_COUNT > 0

SPIClass SPI(SPI1);

void Wiring_SPI1_Interrupt_Handler(void)
{
    //To Do
}
//#endif//>0

#if SPI_INTERFACES_COUNT > 1

SPIClass SPI_1(SPI3);

void Wiring_SPI2_Interrupt_Handler(void)
{
    //To Do
}
#endif//>1
