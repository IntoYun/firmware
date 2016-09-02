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
 * \file       sx1278-Hal.c
 * \brief      SX1278 Hardware Abstraction Layer
 *
 * \version    2.0.B2
 * \date       Nov 21 2012
 * \author     Miguel Luis
 *
 * Last modified by Miguel Luis on Jun 19 2013
 */
#include <stdint.h>
#include <stdbool.h>

#include "platform.h"
//#include "stm32l1xx.h"
#if defined( USE_SX1278_RADIO )

//#include "ioe.h"
#include "spi.h"
#include "sx1278-Hal.h"

/*!
 * SX1278 RESET I/O definitions
 */

#define RESET_IOPORT                                GPIOA
#define RESET_PIN                                   GPIO_PIN_8

/*!
 * SX1278 SPI NSS I/O definitions
 */
#define NSS_IOPORT                                  GPIOA
#define NSS_PIN                                     GPIO_PIN_15
/*!
 * SX1278 DIO pins  I/O definitions
 */
#define DIO0_IOPORT                                 GPIOC
#define DIO0_PIN                                    GPIO_PIN_13

#define DIO1_IOPORT                                 GPIOB
#define DIO1_PIN                                    GPIO_PIN_0

#define DIO2_IOPORT                                 GPIOB
#define DIO2_PIN                                    GPIO_PIN_14//GPIO_PIN_1

#define DIO3_IOPORT                                 GPIOB
#define DIO3_PIN                                    GPIO_PIN_7

#define DIO4_IOPORT                                 GPIOB
#define DIO4_PIN                                    GPIO_PIN_8

#define DIO5_IOPORT                                 GPIOB
#define DIO5_PIN                                    GPIO_PIN_6

//接收发生使能控制
#define RX_IOPORT                                   GPIOB
#define RX_PIN                                      GPIO_PIN_9

#define TX_IOPORT                                   GPIOB
#define TX_PIN                                      GPIO_PIN_2

//#if defined( STM32F4XX ) || defined( STM32F2XX ) || defined( STM32F429_439xx )
//#define RXTX_IOPORT
//#define RXTX_PIN                                    FEM_CTX_PIN
//#else
//#define RXTX_IOPORT
//#define RXTX_PIN                                    FEM_CTX_PIN
//#endif


void SX1278InitIo( void )
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull      = GPIO_NOPULL;
    GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
   // GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;

   // Configure SPI_NSS
    GPIO_InitStructure.Pin = NSS_PIN;
    HAL_GPIO_WritePin(NSS_IOPORT,NSS_PIN,GPIO_PIN_SET);
    HAL_GPIO_Init(NSS_IOPORT, &GPIO_InitStructure);

    GPIO_InitStructure.Mode  = GPIO_MODE_INPUT;
    // Configure DIO0
    GPIO_InitStructure.Pin =  DIO0_PIN;
    HAL_GPIO_Init( DIO0_IOPORT, &GPIO_InitStructure );

    // Configure DIO1
    GPIO_InitStructure.Pin =  DIO1_PIN;
    HAL_GPIO_Init( DIO1_IOPORT, &GPIO_InitStructure );

    // Configure DIO2
    GPIO_InitStructure.Pin =  DIO2_PIN;
    HAL_GPIO_Init( DIO2_IOPORT, &GPIO_InitStructure );

    // REAMARK: DIO3/4/5 configured are connected to IO expander

    // Configure DIO3 as input
    GPIO_InitStructure.Pin =  DIO3_PIN;
    HAL_GPIO_Init( DIO3_IOPORT, &GPIO_InitStructure );

    // Configure DIO4 as input
    GPIO_InitStructure.Pin =  DIO4_PIN;
    HAL_GPIO_Init( DIO4_IOPORT, &GPIO_InitStructure );

    // Configure DIO5 as input
    GPIO_InitStructure.Pin =  DIO5_PIN;
    HAL_GPIO_Init( DIO5_IOPORT, &GPIO_InitStructure );

    //接收发送脚初始化
    GPIO_InitStructure.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull      = GPIO_NOPULL;
    GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;

    GPIO_InitStructure.Pin =  RX_PIN;
    HAL_GPIO_Init( RX_IOPORT, &GPIO_InitStructure );

    GPIO_InitStructure.Pin =  TX_PIN;
    HAL_GPIO_Init( TX_IOPORT, &GPIO_InitStructure );
   //startup spi
    SpiInit();
}

void SX1278SetReset( uint8_t state )
{
    GPIO_InitTypeDef GPIO_InitStructure;

    if( state == RADIO_RESET_ON )
    {
        // Set RESET pin to 0
//        GPIO_WriteBit( RESET_IOPORT, RESET_PIN, Bit_RESET );
        HAL_GPIO_WritePin(RESET_IOPORT,RESET_PIN,GPIO_PIN_RESET);
        // Configure RESET as output
//#if defined( STM32F4XX ) || defined( STM32F2XX ) || defined( STM32F429_439xx )
//        GPIO_InitStructure.Mode      = GPIO_MODE_OUTPUT_PP;
//        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//#else

        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
//#endif
        GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStructure.Pin = RESET_PIN;
        HAL_GPIO_Init( RESET_IOPORT, &GPIO_InitStructure );
    }
    else
    {
//#if FPGA == 0
        // Configure RESET as input
//#if defined( STM32F4XX ) || defined( STM32F2XX ) || defined( STM32F429_439xx )
//        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//#else
      //  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
//#endif
    //    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    //    GPIO_InitStructure.Pin =  RESET_PIN;
    //    HAL_GPIO_Init( RESET_IOPORT, &GPIO_InitStructure );
///#else
       // GPIO_WriteBit( RESET_IOPORT, RESET_PIN, Bit_RESET );
        HAL_GPIO_WritePin(RESET_IOPORT,RESET_PIN,GPIO_PIN_SET);
        GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
        GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStructure.Pin =  RESET_PIN;
        HAL_GPIO_Init( RESET_IOPORT, &GPIO_InitStructure );

//#endif
    }
}

void SX1278Write( uint8_t addr, uint8_t data )
{
    SX1278WriteBuffer( addr, &data, 1 );
}

void SX1278Read( uint8_t addr, uint8_t *data )
{
    SX1278ReadBuffer( addr, data, 1 );
}

void SX1278WriteBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;

    //NSS = 0;
//    GPIO_WriteBit( NSS_IOPORT, NSS_PIN, Bit_RESET );
    HAL_GPIO_WritePin(NSS_IOPORT,NSS_PIN,GPIO_PIN_RESET);
    SpiInOut( addr | 0x80 );

    for( i = 0; i < size; i++ )
    {
        SpiInOut( buffer[i] );
    }

    //NSS = 1;
//    GPIO_WriteBit( NSS_IOPORT, NSS_PIN, Bit_SET );
    HAL_GPIO_WritePin(NSS_IOPORT,NSS_PIN,GPIO_PIN_SET);
}

void SX1278ReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;

    //NSS = 0;
   // GPIO_WriteBit( NSS_IOPORT, NSS_PIN, Bit_RESET );
    HAL_GPIO_WritePin(NSS_IOPORT,NSS_PIN,GPIO_PIN_RESET);
    SpiInOut( addr & 0x7F );

    for( i = 0; i < size; i++ )
    {
        buffer[i] = SpiInOut( 0 );
    }

    //NSS = 1;
   // GPIO_WriteBit( NSS_IOPORT, NSS_PIN, Bit_SET );
    HAL_GPIO_WritePin(NSS_IOPORT,NSS_PIN,GPIO_PIN_SET);
}

void SX1278WriteFifo( uint8_t *buffer, uint8_t size )
{
    SX1278WriteBuffer( 0, buffer, size );
}

void SX1278ReadFifo( uint8_t *buffer, uint8_t size )
{
    SX1278ReadBuffer( 0, buffer, size );
}

uint8_t SX1278ReadDio0( void )
{
   // return GPIO_ReadInputDataBit( DIO0_IOPORT, DIO0_PIN );
    return HAL_GPIO_ReadPin(DIO0_IOPORT, DIO0_PIN);
}

uint8_t SX1278ReadDio1( void )
{
   // return GPIO_ReadInputDataBit( DIO1_IOPORT, DIO1_PIN );
    return HAL_GPIO_ReadPin(DIO1_IOPORT, DIO1_PIN);
}

uint8_t SX1278ReadDio2( void )
{
   // return GPIO_ReadInputDataBit( DIO2_IOPORT, DIO2_PIN );
    return HAL_GPIO_ReadPin(DIO2_IOPORT, DIO2_PIN);
}

uint8_t SX1278ReadDio3( void )
{
//    return IoePinGet( RF_DIO3_PIN );
    return HAL_GPIO_ReadPin(DIO3_IOPORT, DIO3_PIN);
}

uint8_t SX1278ReadDio4( void )
{
   // return IoePinGet( RF_DIO4_PIN );
    return HAL_GPIO_ReadPin(DIO4_IOPORT, DIO4_PIN);
}

uint8_t SX1278ReadDio5( void )
{
   /// return IoePinGet( RF_DIO5_PIN );
    return HAL_GPIO_ReadPin(DIO5_IOPORT, DIO5_PIN);
}

void SX1278WriteRxTx( uint8_t txEnable )
{
    if( txEnable != 0 )
    {
        HAL_GPIO_WritePin( RX_IOPORT, RX_PIN, GPIO_PIN_RESET );
        HAL_GPIO_WritePin( TX_IOPORT, TX_PIN, GPIO_PIN_SET );
    }
    else
    {
        HAL_GPIO_WritePin( RX_IOPORT, RX_PIN, GPIO_PIN_SET );
        HAL_GPIO_WritePin( TX_IOPORT, TX_PIN, GPIO_PIN_RESET );
    }
}
#if 0
inline uint8_t SX1278ReadDio3( void )
{
    return IoePinGet( RF_DIO3_PIN );
}

inline uint8_t SX1278ReadDio4( void )
{
    return IoePinGet( RF_DIO4_PIN );
}

inline uint8_t SX1278ReadDio5( void )
{
    return IoePinGet( RF_DIO5_PIN );
}

inline void SX1278WriteRxTx( uint8_t txEnable )
{
    if( txEnable != 0 )
    {
        IoePinOn( FEM_CTX_PIN );
        IoePinOff( FEM_CPS_PIN );
    }
    else
    {
        IoePinOff( FEM_CTX_PIN );
        IoePinOn( FEM_CPS_PIN );
    }
}
#endif
#endif // USE_SX1278_RADIO
