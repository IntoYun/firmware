/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, SEMTECH SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * Copyright (C) SEMTECH S.A.
 */
/*!
 * \file       spi.c
 * \brief      SPI hardware driver
 *
 * \version    1.0
 * \date       Feb 12 2010
 * \author     Miguel Luis
 */
#include <stdint.h>
#include "stm32l1xx.h"
#include "spi.h"

static SPI_HandleTypeDef spiHandle;

void SpiInit( void )
{
    //使用ＳＰＩ１
    //ＰＡ１５－－－ＮＳＳ
    //ＰＢ３－－ＳＣＫ
    //ＰＢ４－－ＭＩＳＯ
    //ＰＢ５－－ＭＯＳＩ

    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /* SPI_INTERFACE Config -------------------------------------------------------------*/
    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.Pin       = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;//GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;//GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    spiHandle.Instance                = SPI1;
    spiHandle.Init.Mode               = SPI_MODE_MASTER;
    spiHandle.Init.Direction          = SPI_DIRECTION_2LINES;
    spiHandle.Init.DataSize           = SPI_DATASIZE_8BIT; // SPI_DATASIZE_8BIT; // SPI_DATASIZE_16BIT
    spiHandle.Init.CLKPolarity        = SPI_POLARITY_LOW;
    spiHandle.Init.CLKPhase           = SPI_PHASE_1EDGE;
    spiHandle.Init.NSS                = SPI_NSS_SOFT;
    spiHandle.Init.BaudRatePrescaler  = SPI_BAUDRATEPRESCALER_16;
    spiHandle.Init.FirstBit           = SPI_FIRSTBIT_MSB;
    spiHandle.Init.TIMode             = SPI_TIMODE_DISABLE;
    spiHandle.Init.CRCCalculation     = SPI_CRCCALCULATION_DISABLE;
    spiHandle.Init.CRCPolynomial      = 7;

    HAL_SPI_Init(&spiHandle);
}

uint8_t SpiInOut( uint8_t outData )
{
    uint8_t txData = outData;
    uint8_t rxData = 0;
    HAL_SPI_TransmitReceive(&spiHandle, &txData, &rxData, 1, 100);
    return rxData;
}
