/**
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
*/

#include <stdlib.h>
#include <stdint.h>

#include "aes.h"
#include "cmac.h"

#include "mqttcrypto.h"

/*!
 * Contains the computed MIC field.
 *
 * \remark Only the 4 first bytes are used
 */
static uint8_t Mic[16];

/*!
 * AES computation context variable
 */
static aes_context AesContext;

/*!
 * CMAC computation context variable
 */
static AES_CMAC_CTX AesCmacCtx[1];

/*!
 * \brief Computes the LoRaMAC frame MIC field
 *
 * \param [IN]  buffer          Data buffer
 * \param [IN]  size            Data buffer size
 * \param [IN]  key             AES key to be used
 * \param [IN]  seqId           sequence counter
 * \param [OUT] mic Computed MIC field
 */
void MqttComputeMic( const uint8_t *buffer, uint16_t size, const uint8_t *key, uint32_t seqId, uint32_t *mic )
{
    /*
    AES_CMAC_Init( AesCmacCtx );
    AES_CMAC_SetKey( AesCmacCtx, key );
    AES_CMAC_Update( AesCmacCtx, MicBlockB0, LORAMAC_MIC_BLOCK_B0_SIZE );
    AES_CMAC_Update( AesCmacCtx, buffer, size & 0xFF );
    AES_CMAC_Final( Mic, AesCmacCtx );
    */
    *mic = ( uint32_t )( ( uint32_t )Mic[3] << 24 | ( uint32_t )Mic[2] << 16 | ( uint32_t )Mic[1] << 8 | ( uint32_t )Mic[0] );
}

void MqttPayloadEncrypt( const uint8_t *buffer, uint16_t size, const uint8_t *key, uint8_t *encBuffer )
{
    /*
    uint16_t i;
    uint8_t bufferIndex = 0;
    uint16_t ctr = 1;

    memset( AesContext.ksch, '\0', 240 );
    aes_set_key( key, 16, &AesContext );

    aBlock[5] = dir;

    aBlock[6] = ( address ) & 0xFF;
    aBlock[7] = ( address >> 8 ) & 0xFF;
    aBlock[8] = ( address >> 16 ) & 0xFF;
    aBlock[9] = ( address >> 24 ) & 0xFF;

    aBlock[10] = ( sequenceCounter ) & 0xFF;
    aBlock[11] = ( sequenceCounter >> 8 ) & 0xFF;
    aBlock[12] = ( sequenceCounter >> 16 ) & 0xFF;
    aBlock[13] = ( sequenceCounter >> 24 ) & 0xFF;

    while( size >= 16 )
    {
        aBlock[15] = ( ( ctr ) & 0xFF );
        ctr++;
        aes_encrypt( aBlock, sBlock, &AesContext );
        for( i = 0; i < 16; i++ )
        {
            encBuffer[bufferIndex + i] = buffer[bufferIndex + i] ^ sBlock[i];
        }
        size -= 16;
        bufferIndex += 16;
    }

    if( size > 0 )
    {
        aBlock[15] = ( ( ctr ) & 0xFF );
        aes_encrypt( aBlock, sBlock, &AesContext );
        for( i = 0; i < size; i++ )
        {
            encBuffer[bufferIndex + i] = buffer[bufferIndex + i] ^ sBlock[i];
        }
    }
    */
}

void MqttPayloadDecrypt( const uint8_t *buffer, uint16_t size, const uint8_t *key, uint8_t *decBuffer )
{
    /*
    LoRaMacPayloadEncrypt( buffer, size, key, address, dir, sequenceCounter, decBuffer );
    */
}

void MqttConnectComputeMic( const uint8_t *buffer, uint16_t size, const uint8_t *key, uint32_t *mic )
{
    /*
    AES_CMAC_Init( AesCmacCtx );
    AES_CMAC_SetKey( AesCmacCtx, key );
    AES_CMAC_Update( AesCmacCtx, buffer, size & 0xFF );
    AES_CMAC_Final( Mic, AesCmacCtx );
    */
    *mic = ( uint32_t )( ( uint32_t )Mic[3] << 24 | ( uint32_t )Mic[2] << 16 | ( uint32_t )Mic[1] << 8 | ( uint32_t )Mic[0] );
}

void MqttConnectComputeSKeys( const uint8_t *key, const uint8_t *random, uint8_t *nwkSKey, uint8_t *appSKey )
{
    /*
    uint8_t nonce[16];
    uint8_t *pDevNonce = ( uint8_t * )&devNonce;

    memset( AesContext.ksch, '\0', 240 );
    aes_set_key( key, 16, &AesContext );

    memset( nonce, 0, sizeof( nonce ) );
    nonce[0] = 0x01;
    memcpy( nonce + 1, appNonce, 6 );
    memcpy( nonce + 7, pDevNonce, 2 );
    aes_encrypt( nonce, nwkSKey, &AesContext );

    memset( nonce, 0, sizeof( nonce ) );
    nonce[0] = 0x02;
    memcpy( nonce + 1, appNonce, 6 );
    memcpy( nonce + 7, pDevNonce, 2 );
    aes_encrypt( nonce, appSKey, &AesContext );
    */
}
