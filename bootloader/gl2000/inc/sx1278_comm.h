/**
  ******************************************************************************
  * @file    spi_flash.h
  * @author  Satish Nair
  * @version V1.0.0
  * @date    10-Dec-2014
  * @brief   Header for spi flash driver
  ******************************************************************************
  Copyright (c) 2013-2015 Particle Industries, Inc.  All rights reserved.

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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SX1278_COMM_H
#define __SX1278_COMM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define REG_LR_VERSION                              0x42

//SPI FLASH Interface pins
#define SX1278_RESET_GPIO_PIN               GPIO_PIN_9                  /* PB.9 */
#define SX1278_RESET_GPIO_PORT              GPIOB                       /* GPIOB */
#define SX1278_SPI                          SPI1
#define SX1278_SPI_CS_GPIO_PIN              GPIO_PIN_4                  /* PA.4 */
#define SX1278_SPI_CS_GPIO_PORT             GPIOA                       /* GPIOA */
#define SX1278_SPI_SCK_GPIO_PIN             GPIO_PIN_5                  /* PA.5 */
#define SX1278_SPI_SCK_GPIO_PORT            GPIOA                       /* GPIOA */
#define SX1278_SPI_SCK_AF                   GPIO_AF5_SPI1
#define SX1278_SPI_MISO_GPIO_PIN            GPIO_PIN_5                  /* PA.5 */
#define SX1278_SPI_MISO_GPIO_PORT           GPIOA                       /* GPIOA */
#define SX1278_SPI_MISO_AF                  GPIO_AF5_SPI1
#define SX1278_SPI_MOSI_GPIO_PIN            GPIO_PIN_6                  /* PA.6 */
#define SX1278_SPI_MOSI_GPIO_PORT           GPIOA                       /* GPIOA */
#define SX1278_SPI_MOSI_AF                  GPIO_AF5_SPI1

#define SX1278_SPI_BAUDRATE_PRESCALER       SPI_BAUDRATEPRESCALER_16

bool SX1278_SelfCheck(void);

#ifdef __cplusplus
}
#endif

#endif

