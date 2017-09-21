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
 * Computes the mqtt connect cMac
 *
 * \param [IN]  buffer          - Data buffer
 * \param [IN]  size            - Data buffer size
 * \param [IN]  key             - AES key to be used
 * \param [OUT] cMac            - Computed cMac
 */
void MqttConnectComputeCmac( const uint8_t *buffer, uint16_t size, const uint8_t *key, uint8_t *cMac );

/*!
 * Computes the mqtt connect keys
 *
 * \param [IN]  key              - AES key to be used
 * \param [IN]  random           - random
 * \param [OUT] nwkSKey(16 byte) - Network session key
 * \param [OUT] appSKey(16 byte) - Application session key
 */
void MqttConnectComputeSKeys( const uint8_t *key, const int random, uint8_t *nwkSKey, uint8_t *appSKey );

/*!
 * Computes the mqtt frame MIC field
 *
 * \param [IN]  buffer          - Data buffer
 * \param [IN]  size            - Data buffer size
 * \param [IN]  key             - AES key to be used
 * \param [OUT] mic(4 byte)     - Computed MIC field
 */
void MqttComputeMic( const uint8_t *buffer, uint16_t size, const uint8_t *key, uint8_t *mic );

/*!
 * Computes the mqtt payload encryption
 *
 * \param [IN]  buffer          - Data buffer
 * \param [IN]  size            - Data buffer size
 * \param [IN]  key             - AES key to be used
 * \param [IN]  dir             - Frame direction [0: uplink, 1: downlink]
 * \param [IN]  seqId           - Frame sequence counter
 * \param [IN]  device_id       - device_id(last 10byte)
 * \param [OUT] encBuffer       - Encrypted buffer
 */
void MqttPayloadEncrypt( const uint8_t *buffer, uint16_t size, const uint8_t *key, uint8_t dir, uint16_t seqId, char *device_id, uint8_t *encBuffer );

/*!
 * Computes the mqtt payload decryption
 *
 * \param [IN]  buffer          - Data buffer
 * \param [IN]  size            - Data buffer size
 * \param [IN]  key             - AES key to be used
 * \param [IN]  dir             - Frame direction [0: uplink, 1: downlink]
 * \param [IN]  seqId           - Frame sequence counter
 * \param [IN]  device_id       - device_id(last 12byte)
 * \param [OUT] decBuffer       - Decrypted buffer
 */
void MqttPayloadDecrypt( const uint8_t *buffer, uint16_t size, const uint8_t *key, uint8_t dir, uint16_t seqId, char *device_id, uint8_t *encBuffer );

#ifdef __cplusplus
}
#endif

#endif // __MQTT_CRYPTO_H__
