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

#ifndef LORA_GATEWAY_PARAMS_H_
#define LORA_GATEWAY_PARAMS_H_

#include "sx1278.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    uint32_t cp_nb_rx_rcv;
    uint32_t cp_nb_rx_ok;
    uint32_t cp_nb_rx_bad;
    uint32_t cp_nb_rx_nocrc;
    uint32_t cp_up_pkt_fwd;
}GatewayStatistics_t;

typedef struct {
    uint8_t ch;                 // index to freqs array, freqs[ifreq]=868100000 default
    uint16_t fcnt;              // =0 as init value
    uint8_t sf;                 // range from SF7 to SF12
    uint8_t debug;              // range 0 to 4
    bool cad;
    bool hop;
}GatewayConfig_t;

typedef struct {
    uint16_t lastToken;
    uint32_t lastTmst;
    uint32_t sendTmst;
}GatewayStatus_t;

typedef struct {
    uint32_t channel;
    RadioModems_t modem;
    uint32_t bandwidth;
    uint32_t datarate;
    uint8_t coderate;
    uint32_t bandwidthAfc;
    uint16_t preambleLen;
    uint16_t symbTimeout;
    bool fixLen;
    uint8_t payloadLen;
    bool crcOn; bool freqHopOn; uint8_t hopPeriod;
    bool iqInverted;
    bool rxContinuous;
}GatewayLoraRxParams_t;

typedef struct {
    uint32_t channel;
    RadioModems_t modem;
    int8_t power;
    uint32_t fdev;
    uint32_t bandwidth;
    uint32_t datarate;
    uint8_t coderate;
    uint16_t preambleLen;
    bool fixLen;
    bool crcOn;
    bool freqHopOn;
    uint8_t hopPeriod;
    bool iqInverted;
    uint32_t timeout;
}GatewayLoraTxParams_t;

typedef struct
{
    GatewayStatistics_t   Statistics;
    GatewayConfig_t       Config;
    GatewayStatus_t       Status;
    GatewayLoraRxParams_t RxParams;
    GatewayLoraTxParams_t TxParams;
}GatewayParams_t;

extern GatewayParams_t GatewayParams;

void loraParamsInit(void);
void loraParamsRxConfig(GatewayLoraRxParams_t *RxParams);
void loraParamsTxConfig(GatewayLoraTxParams_t *TxParams);
void loraRx(void);

#ifdef __cplusplus
}
#endif

#endif //LORA_GATEWAY_PARAMS_H_

