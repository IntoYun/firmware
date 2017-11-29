/**
 ******************************************************************************
 * @file    spi_flash.c
 * @author  Satish Nair
 * @version V1.0.0
 * @date    10-Dec-2014
 * @brief   spi flash driver
 ******************************************************************************
  Copyright (c) 2013-2015 Particle Industries, Inc.  All rights reserved.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"
#include "sx1278_comm.h"
#include "system_config.h"
#include "boot_debug.h"

SPI_HandleTypeDef Sx1278SpiHandle;

static void SX1278_RESET(void);
static void SX1278_CS_LOW(void);
static void SX1278_CS_HIGH(void);
static uint8_t SX1278_SendByte(uint8_t byte);

/**
 * @brief  DeInitializes the peripherals used by the SPI FLASH driver.
 * @param  None
 * @retval None
 */
static void SX1278_SPI_DeInit(void)
{
    __HAL_RCC_SPI1_FORCE_RESET();
    __HAL_RCC_SPI1_RELEASE_RESET();

    HAL_GPIO_DeInit(SX1278_SPI_SCK_GPIO_PORT, SX1278_SPI_SCK_GPIO_PIN);
    HAL_GPIO_DeInit(SX1278_SPI_MISO_GPIO_PORT, SX1278_SPI_MISO_GPIO_PIN);
    HAL_GPIO_DeInit(SX1278_SPI_MOSI_GPIO_PORT, SX1278_SPI_MOSI_GPIO_PIN);
    HAL_GPIO_DeInit(SX1278_SPI_CS_GPIO_PORT, SX1278_SPI_CS_GPIO_PIN);
}

/**
 * @brief  Initializes the peripherals used by the SPI FLASH driver.
 * @param  None
 * @retval None
 */
static void SX1278_SPI_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* SPI SCK GPIO pin configuration  */
    GPIO_InitStruct.Pin       = SX1278_SPI_SCK_GPIO_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = SX1278_SPI_SCK_AF;
    HAL_GPIO_Init(SX1278_SPI_SCK_GPIO_PORT, &GPIO_InitStruct);

    /* SPI MISO GPIO pin configuration  */
    GPIO_InitStruct.Pin = SX1278_SPI_MISO_GPIO_PIN;
    GPIO_InitStruct.Alternate = SX1278_SPI_MISO_AF;
    HAL_GPIO_Init(SX1278_SPI_MISO_GPIO_PORT, &GPIO_InitStruct);

    /* SPI MOSI GPIO pin configuration  */
    GPIO_InitStruct.Pin = SX1278_SPI_MOSI_GPIO_PIN;
    GPIO_InitStruct.Alternate = SX1278_SPI_MOSI_AF;
    HAL_GPIO_Init(SX1278_SPI_MOSI_GPIO_PORT, &GPIO_InitStruct);

    /* SPI CS GPIO pin configuration  */
    GPIO_InitStruct.Pin = SX1278_SPI_CS_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(SX1278_SPI_CS_GPIO_PORT, &GPIO_InitStruct);

    /* SX1278 RESET GPIO pin configuration  */
    GPIO_InitStruct.Pin = SX1278_RESET_GPIO_PIN;
    HAL_GPIO_Init(SX1278_RESET_GPIO_PORT, &GPIO_InitStruct);

    /* Chip RESET */
    SX1278_RESET();

    /* Chip Select high */
    SX1278_CS_HIGH();

    Sx1278SpiHandle.Instance               = SX1278_SPI;
    Sx1278SpiHandle.Init.BaudRatePrescaler = SX1278_SPI_BAUDRATE_PRESCALER;
    Sx1278SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
    Sx1278SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
    Sx1278SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
    Sx1278SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    Sx1278SpiHandle.Init.CRCPolynomial     = 7;
    Sx1278SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
    Sx1278SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    Sx1278SpiHandle.Init.NSS               = SPI_NSS_SOFT;
    Sx1278SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
    Sx1278SpiHandle.Init.Mode              = SPI_MODE_MASTER;
    HAL_SPI_Init(&Sx1278SpiHandle);
}

static void SX1278_RESET(void)
{
    HAL_GPIO_WritePin(SX1278_RESET_GPIO_PORT, SX1278_RESET_GPIO_PIN, GPIO_PIN_RESET);
    delay(200);
    HAL_GPIO_WritePin(SX1278_RESET_GPIO_PORT, SX1278_RESET_GPIO_PIN, GPIO_PIN_SET);
}

static void SX1278_CS_LOW(void)
{
    HAL_GPIO_WritePin(SX1278_SPI_CS_GPIO_PORT, SX1278_SPI_CS_GPIO_PIN, GPIO_PIN_RESET);
}

static void SX1278_CS_HIGH(void)
{
    HAL_GPIO_WritePin(SX1278_SPI_CS_GPIO_PORT, SX1278_SPI_CS_GPIO_PIN, GPIO_PIN_SET);
}

static uint8_t SX1278_SendByte(uint8_t byte)
{
    uint8_t c;

    HAL_SPI_TransmitReceive(&Sx1278SpiHandle, &byte, &c, 1, 5);
    return c;
}

void SX1278_Init(void)
{
    SX1278_SPI_Init();
}

uint8_t SX1278_GetVersion(void)
{
    uint8_t byte;

    /* Chip Select low */
    SX1278_CS_LOW();

    SX1278_SendByte(REG_LR_VERSION);

    byte = SX1278_SendByte(0);

    /* Chip Select high */
    SX1278_CS_HIGH();

    return byte;
}

bool SX1278_SelfCheck(void)
{
    SX1278_Init();

    uint8_t version = SX1278_GetVersion();
    //BOOT_DEBUG("version = 0x%x\r\n", version);
    if(0x12 == version) {
        return true;
    }
    return false;
}

