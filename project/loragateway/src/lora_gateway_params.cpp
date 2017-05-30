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

#include "lora_gateway_config.h"
#include "lora_gateway_params.h"
#include "lora_gateway_log.h"
#include "sx1278.h"

GatewayParams_t GatewayParams;

void loraParamsInit(void)
{
    memset(&GatewayParams, 0, sizeof(GatewayParams_t));

    GatewayParams.RxParams.channel = CONFIG_FREQUENCY;
    GatewayParams.RxParams.modem = MODEM_LORA;
    GatewayParams.RxParams.bandwidth = 0;
    GatewayParams.RxParams.datarate = CONFIG_SPREADING_FACTOR;
    GatewayParams.RxParams.coderate = 1;
    GatewayParams.RxParams.bandwidthAfc = 0;
    GatewayParams.RxParams.preambleLen = 8;
    GatewayParams.RxParams.symbTimeout = 5;
    GatewayParams.RxParams.fixLen = false;
    GatewayParams.RxParams.payloadLen = 0;
    GatewayParams.RxParams.crcOn = true;
    GatewayParams.RxParams.freqHopOn = 0;
    GatewayParams.RxParams.hopPeriod = 0;
    GatewayParams.RxParams.iqInverted = false;
    GatewayParams.RxParams.rxContinuous = true;

    memset(&GatewayParams.TxParams, 0, sizeof(GatewayLoraTxParams_t));
    GatewayParams.TxParams.channel = CONFIG_FREQUENCY;
    GatewayParams.TxParams.modem = MODEM_LORA;
    GatewayParams.TxParams.power = 14;
    GatewayParams.TxParams.fdev = 0;
    GatewayParams.TxParams.bandwidth = 0;
    GatewayParams.TxParams.datarate = CONFIG_SPREADING_FACTOR;
    GatewayParams.TxParams.coderate = 1;
    GatewayParams.TxParams.preambleLen = 8;
    GatewayParams.TxParams.fixLen = false;
    GatewayParams.TxParams.crcOn = false;
    GatewayParams.TxParams.freqHopOn = 0;
    GatewayParams.TxParams.hopPeriod = 0;
    GatewayParams.TxParams.iqInverted = false;
    GatewayParams.TxParams.timeout = 3000;

    Radio.SetModem( MODEM_LORA );
    Radio.Write( REG_LR_SYNCWORD, 0x34 ); //必须是LORA模式
    Radio.Write( REG_LNA, 0x23 );
}

void loraParamsRxConfig(GatewayLoraRxParams_t *RxParams)
{
#if 0
    log_v("loraParamsRxConfig\r\n");
    log_v("channel = %d\r\n", RxParams->channel);
    log_v("modem = %d\r\n", RxParams->modem);
    log_v("bandwidth = %d\r\n", RxParams->bandwidth);
    log_v("datarate = %d\r\n", RxParams->datarate);
    log_v("coderate = %d\r\n", RxParams->coderate);
    log_v("bandwidthafc = %d\r\n", RxParams->bandwidthAfc);
    log_v("preamblelen = %d\r\n", RxParams->preambleLen);
    log_v("symbtimeout = %d\r\n", RxParams->symbTimeout);
    log_v("fixlen = %d\r\n", RxParams->fixLen);
    log_v("payloadlen = %d\r\n", RxParams->payloadLen);
    log_v("crcon = %d\r\n", RxParams->crcOn);
    log_v("freqhopon = %d\r\n", RxParams->freqHopOn);
    log_v("hopperiod = %d\r\n", RxParams->hopPeriod);
    log_v("iqinverted = %d\r\n", RxParams->iqInverted);
    log_v("rxcontinuous = %d\r\n", RxParams->rxContinuous);
#endif
    Radio.Sleep();
    Radio.SetRxConfig(
            RxParams->modem,
            RxParams->bandwidth,
            RxParams->datarate,
            RxParams->coderate,
            RxParams->bandwidthAfc,
            RxParams->preambleLen,
            RxParams->symbTimeout,
            RxParams->fixLen,
            RxParams->payloadLen,
            RxParams->crcOn,
            RxParams->freqHopOn,
            RxParams->hopPeriod,
            RxParams->iqInverted,
            RxParams->rxContinuous);
    Radio.SetChannel( RxParams->channel );
}

void loraParamsTxConfig(GatewayLoraTxParams_t *TxParams)
{
#if 0
    //该调试信息打开，因为占用时间，会导致数据落不到note的时间窗口上
    log_v("loraParamsTxConfig\r\n");
    log_v("channel = %d\r\n", TxParams->channel);
    log_v("modem = %d\r\n", TxParams->modem);
    log_v("power = %d\r\n", TxParams->power);
    log_v("fdev = %d\r\n", TxParams->fdev);
    log_v("bandwidth = %d\r\n", TxParams->bandwidth);
    log_v("datarate = %d\r\n", TxParams->datarate);
    log_v("coderate = %d\r\n", TxParams->coderate);
    log_v("preamblelen = %d\r\n", TxParams->preambleLen);
    log_v("fixlen = %d\r\n", TxParams->fixLen);
    log_v("crcon = %d\r\n", TxParams->crcOn);
    log_v("freqhopon = %d\r\n", TxParams->freqHopOn);
    log_v("hopperiod = %d\r\n", TxParams->hopPeriod);
    log_v("iqinverted = %d\r\n", TxParams->iqInverted);
    log_v("timeout = %d\r\n", TxParams->timeout);
#endif
    Radio.Sleep();
    Radio.SetTxConfig(
            TxParams->modem,
            TxParams->power,
            TxParams->fdev,
            TxParams->bandwidth,
            TxParams->datarate,
            TxParams->coderate,
            TxParams->preambleLen,
            TxParams->fixLen,
            TxParams->crcOn,
            TxParams->freqHopOn,
            TxParams->hopPeriod,
            TxParams->iqInverted,
            TxParams->timeout);
    Radio.SetChannel( TxParams->channel );
}

void loraRx(void)
{
    loraParamsRxConfig(&GatewayParams.RxParams);
    Radio.Rx(0);
}

