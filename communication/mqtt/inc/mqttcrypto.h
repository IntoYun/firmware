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

#ifndef __MQTT_CRYPTO_H__
#define __MQTT_CRYPTO_H__

#ifdef __cplusplus
extern "C" {
#endif
/*!
 * Computes the mqtt frame MIC field
 *
 * \param [IN]  buffer          - Data buffer
 * \param [IN]  size            - Data buffer size
 * \param [IN]  key             - AES key to be used
 * \param [IN]  seqId           - sequence counter
 * \param [OUT] mic             - Computed MIC field
 */
void MqttComputeMic( const uint8_t *buffer, uint16_t size, const uint8_t *key, uint32_t seqId, uint32_t *mic );

/*!
 * Computes the mqtt payload encryption
 *
 * \param [IN]  buffer          - Data buffer
 * \param [IN]  size            - Data buffer size
 * \param [IN]  key             - AES key to be used
 * \param [OUT] encBuffer       - Encrypted buffer
 */
void MqttPayloadEncrypt( const uint8_t *buffer, uint16_t size, const uint8_t *key, uint8_t *encBuffer );

/*!
 * Computes the mqtt payload decryption
 *
 * \param [IN]  buffer          - Data buffer
 * \param [IN]  size            - Data buffer size
 * \param [IN]  key             - AES key to be used
 * \param [OUT] decBuffer       - Decrypted buffer
 */
void MqttPayloadDecrypt( const uint8_t *buffer, uint16_t size, const uint8_t *key, uint8_t *decBuffer );

/*!
 * Computes the mqtt connect MIC
 *
 * \param [IN]  buffer          - Data buffer
 * \param [IN]  size            - Data buffer size
 * \param [IN]  key             - AES key to be used
 * \param [OUT] mic             - Computed MIC field
 */
void MqttConnectComputeMic( const uint8_t *buffer, uint16_t size, const uint8_t *key, uint32_t *mic );

/*!
 * Computes the mqtt connect keys
 *
 * \param [IN]  key             - AES key to be used
 * \param [IN]  random          - random
 * \param [OUT] nwkSKey         - Network session key
 * \param [OUT] appSKey         - Application session key
 */
void MqttConnectComputeSKeys( const uint8_t *key, const uint8_t *random, uint8_t *nwkSKey, uint8_t *appSKey );

#ifdef __cplusplus
}
#endif

#endif // __MQTT_CRYPTO_H__
