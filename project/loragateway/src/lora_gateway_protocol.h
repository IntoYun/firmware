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

#ifndef LORA_GATEWAY_UDP_PROTOCOL_H__
#define LORA_GATEWAY_UDP_PROTOCOL_H__

#ifdef __cplusplus
extern "C"
{
#endif

// ----------------------------------------
// Definitions for UDP message arriving from server
#define PROTOCOL_VERSION         0x01
#define PKT_PUSH_DATA            0x00
#define PKT_PUSH_ACK             0x01
#define PKT_PULL_DATA            0x02
#define PKT_PULL_RESP            0x03
#define PKT_PULL_ACK             0x04


void initCommunication(void);
void waitServerConnect(void);
void manageServerConnect(void);
void manageServerCommunication(void);
void sendPacketToServer(uint8_t * msg, int length);

int gatewayPullData(void);
int gatewaySendStat(void);
int gatewayBuildUpstreamPacket(uint32_t tmst, uint8_t *bufferUp, uint8_t *message, uint16_t messageLength);
int gatewayParseDownStreamPacket(uint8_t *message, uint8_t *bufferDown, uint8_t length);

#ifdef __cplusplus
}
#endif

#endif /* LORA_GATEWAY_LOG_H__ */

