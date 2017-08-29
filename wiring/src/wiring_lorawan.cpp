#include "intorobot_config.h"
#include "wiring_lorawan.h"
#include "system_lorawan.h"
#include "system_event.h"
#include "wiring_system.h"
#include "string_convert.h"
#include "service_debug.h"
#include "wiring_ext.h"

#ifndef configNO_LORAWAN

LoRaWanClass LoRaWan;
LoRaClass LoRa;
static LoRaMacPrimitives_t LoRaMacPrimitives;
static LoRaMacCallback_t LoRaMacCallbacks;
static  RadioEvents_t loraRadioEvents;

//======loramac不运行========
static void OnLoRaRadioTxDone(void)
{
    LoRa._radioRunStatus = ep_lora_radio_tx_done;
    system_notify_event(event_lora_radio_status,ep_lora_radio_tx_done);
}

static void OnLoRaRadioTxTimeout(void)
{
    LoRa._radioRunStatus = ep_lora_radio_tx_timeout;
    system_notify_event(event_lora_radio_status,ep_lora_radio_tx_timeout);
}

static void OnLoRaRadioRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
{
    LoRa._rssi = rssi;
    LoRa._snr = snr;
    LoRa._length = size;
    LoRa._radioAvailable = true;
    memcpy( LoRa._buffer, payload, LoRa._length);
    LoRa._radioRunStatus = ep_lora_radio_rx_done;
    system_notify_event(event_lora_radio_status,ep_lora_radio_rx_done,payload,size);
}

static void OnLoRaRadioRxTimeout(void)
{
    LoRa._radioRunStatus = ep_lora_radio_rx_timeout;
    system_notify_event(event_lora_radio_status,ep_lora_radio_rx_timeout);
}

static void OnLoRaRadioRxError(void)
{
    LoRa._radioRunStatus = ep_lora_radio_rx_error;
    system_notify_event(event_lora_radio_status,ep_lora_radio_rx_error);
}

static void OnLoRaRadioCadDone(bool channelActivityDetected)
{
    uint8_t cadDetected;
    if(channelActivityDetected){
        cadDetected = 1;
        LoRa._cadDetected = true;
    }else{
        cadDetected = 0;
        LoRa._cadDetected = false;
    }
    system_notify_event(event_lora_radio_status,ep_lora_radio_cad_done,&cadDetected,1);
}

//======loramac不运行 end ========

//loramac运行回调函数
static void McpsConfirm( McpsConfirm_t *mcpsConfirm )
{
    if( mcpsConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK )
    {
        LoRaWan._macSendStatus = 1;
        switch( mcpsConfirm->McpsRequest )
        {
            case MCPS_UNCONFIRMED:
            {
                // Check Datarate
                // Check TxPower
                LoRaWan._uplinkDatarate = mcpsConfirm->Datarate;
                LoRaWan._txPower = mcpsConfirm->TxPower;
                LoRaWan._macRunStatus = ep_lorawan_mcpsconfirm_unconfirmed;
                system_notify_event(event_lorawan_status,ep_lorawan_mcpsconfirm_unconfirmed);
                break;
            }
            case MCPS_CONFIRMED:
            {
                // Check Datarate
                // Check TxPower
                // Check AckReceived
                // Check NbTrials
                LoRaWan._uplinkDatarate = mcpsConfirm->Datarate;
                LoRaWan._txPower = mcpsConfirm->TxPower;
                LoRaWan._ackReceived = mcpsConfirm->AckReceived;
                if(!LoRaWan._ackReceived){
                    LoRaWan._macSendStatus = 2;
                }
                LoRaWan._macRunStatus = ep_lorawan_mcpsconfirm_confirmed;
                system_notify_event(event_lorawan_status,ep_lorawan_mcpsconfirm_confirmed_ackreceived);
                system_notify_event(event_lorawan_status,ep_lorawan_mcpsconfirm_confirmed);
                break;
            }
            case MCPS_PROPRIETARY:
            {
                system_notify_event(event_lorawan_status,ep_lorawan_mcpsconfirm_proprietary);
                break;
            }
            default:
                break;
        }
    }
    else
    {
        LoRaWan._macSendStatus = 2;
    }
}

static void McpsIndication( McpsIndication_t *mcpsIndication )
{
    if( mcpsIndication->Status != LORAMAC_EVENT_INFO_STATUS_OK )
    {
        return;
    }

    // Check Multicast
    // Check Port
    // Check Datarate
    // Check FramePending
    // Check Buffer
    // Check BufferSize
    // Check Rssi
    // Check Snr
    // Check RxSlot
    LoRaWan._downlinkDatarate = mcpsIndication->RxDatarate;
    LoRaWan._macRssi = mcpsIndication->Rssi;
    LoRaWan._macSnr = mcpsIndication->Snr;
    LoRaWan._framePending = mcpsIndication->FramePending;

    if( mcpsIndication->RxData == true )
    {
        LoRaWan.macBuffer.available = true;
        LoRaWan.macBuffer.bufferSize = mcpsIndication->BufferSize;
        memcpy1(LoRaWan.macBuffer.buffer,mcpsIndication->Buffer,mcpsIndication->BufferSize);

        LoRaWanOnEvent(LORAWAN_EVENT_RX_COMPLETE);
        system_notify_event(event_lorawan_status,ep_lorawan_mcpsindication_receive_data);
    }
    else
    {
        LoRaWan.macBuffer.available = false;
    }

    switch( mcpsIndication->McpsIndication )
    {
        case MCPS_UNCONFIRMED:
        {
            system_notify_event(event_lorawan_status,ep_lorawan_mcpsindication_unconfirmed);
            break;
        }
        case MCPS_CONFIRMED:
        {
            LoRaWanOnEvent(LORAWAN_EVENT_MCPSINDICATION_CONFIRMED);
            system_notify_event(event_lorawan_status,ep_lorawan_mcpsindication_confirmed);
            break;
        }
        case MCPS_PROPRIETARY:
        {
            system_notify_event(event_lorawan_status,ep_lorawan_mcpsindication_proprietary);
            break;
        }
        case MCPS_MULTICAST:
        {
            system_notify_event(event_lorawan_status,ep_lorawan_mcpsindication_multicast);
            break;
        }
        default:
            break;
    }

}

static void MlmeConfirm( MlmeConfirm_t *mlmeConfirm )
{
    switch( mlmeConfirm->MlmeRequest )
    {
        case MLME_JOIN:
        {
            if( mlmeConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK )
            {
                // Status is OK, node has joined the network
                LoRaWan._macRunStatus = ep_lorawan_mlmeconfirm_join_success;
                LoRaWanOnEvent(LORAWAN_EVENT_JOINED);
                // system_notify_event(event_lorawan_status,ep_lorawan_mlmeconfirm_join_success);
            }
            else
            {
                // Join was not successful. Try to join again
                LoRaWan._macRunStatus = ep_lorawan_mlmeconfirm_join_fail;
                LoRaWanOnEvent(LORAWAN_EVENT_JOIN_FAIL);
                system_notify_event(event_lorawan_status,ep_lorawan_mlmeconfirm_join_fail);
            }
            break;
        }
        case MLME_LINK_CHECK:
        {
            if( mlmeConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK )
            {
                // Check DemodMargin
                // Check NbGateways
                LoRaWan._demodMargin = mlmeConfirm->DemodMargin;
                LoRaWan._nbGateways = mlmeConfirm->NbGateways;
                system_notify_event(event_lorawan_status,ep_lorawan_mlmeconfirm_link_check);
            }
            break;
        }
        default:
            break;
    }
}

//lorawan 接口
bool LoRaWanClass::connect(join_mode_t mode, uint16_t timeout)
{
    if(mode == JOIN_ABP){
        return joinABP();
    }else{
        return joinOTAA(timeout);
    }
}

void LoRaWanClass::disconnect(void)
{
    LoRaWanDisconnect();
    LoRaMacAbortRun();
    setMacClassType(CLASS_A);
}

bool LoRaWanClass::connected(void)
{
    if(LoRaWanActiveStatus() == 1){
        return true;
    }else{
        return false;
    }
}

void LoRaWanClass::macPause(void)
{
    System.disableFeature(SYSTEM_FEATURE_LORAMAC_RUN_ENABLED);
    LoRaWanJoinEnable(false);
    // Radio initialization
    // SX1276BoardInit();
    disable_irq( );
    SX1276IoIrqDeInit();
    loraRadioEvents.TxDone = OnLoRaRadioTxDone;
    loraRadioEvents.RxDone = OnLoRaRadioRxDone;
    loraRadioEvents.TxTimeout = OnLoRaRadioTxTimeout;
    loraRadioEvents.RxTimeout = OnLoRaRadioRxTimeout;
    loraRadioEvents.RxError = OnLoRaRadioRxError;
    loraRadioEvents.CadDone = OnLoRaRadioCadDone;
    Radio.Init( &loraRadioEvents );
    enable_irq( );
    Radio.SetModem( MODEM_LORA );

    DEBUG("lora radio init!!!");
    DEBUG("sync data = 0x%x",SX1276Read(0x39));
    DEBUG("sx1278 version = 0x%x", SX1276GetVersion());
    DEBUG("sx1278 freq = %d",SX1276LoRaGetRFFrequency());
}

void LoRaWanClass::macResume(void)
{
    System.enableFeature(SYSTEM_FEATURE_LORAMAC_RUN_ENABLED);
    SX1276BoardInit(); //初始化1278
    LoRaMacPrimitives.MacMcpsConfirm = McpsConfirm;
    LoRaMacPrimitives.MacMcpsIndication = McpsIndication;
    LoRaMacPrimitives.MacMlmeConfirm = MlmeConfirm;
    LoRaMacCallbacks.GetBatteryLevel = BoardGetBatteryLevel;
    LoRaMacInitialization( &LoRaMacPrimitives, &LoRaMacCallbacks );

    DEBUG("loramac init!!!");
    DEBUG("sync data = 0x%x",SX1276Read(0x39));
    DEBUG("sx1278 version = 0x%x", SX1276GetVersion());
    DEBUG("sx1278 freq = %d",SX1276LoRaGetRFFrequency());

    MibRequestConfirm_t mibReq;

    mibReq.Type = MIB_ADR;
    mibReq.Param.AdrEnable = _adrOn;
    LoRaMacMibSetRequestConfirm( &mibReq );

    mibReq.Type = MIB_PUBLIC_NETWORK;
    mibReq.Param.EnablePublicNetwork = LORAWAN_PUBLIC_NETWORK;
    LoRaMacMibSetRequestConfirm( &mibReq );
}

bool LoRaWanClass::joinABP(void)
{
    return LoRaWanJoinABP();
}

bool LoRaWanClass::joinOTAA(uint16_t timeout)
{
    uint32_t _timeout = timeout;
    LoRaWanJoinEnable(true);

    if(_timeout == 0){
        return true;
    }
    if(_timeout < 180){
        _timeout = 180;
    }

    _macRunStatus = 0;
    uint32_t prevTime = millis();
    while(1){
        if(_macRunStatus == ep_lorawan_mlmeconfirm_join_success){
            return true;
        }else if(_macRunStatus == ep_lorawan_mlmeconfirm_join_fail){
            return false;
        }

        if(millis() - prevTime >= _timeout * 1000){
            disconnect();
            return false;
        }
        intorobot_process();
    }
}

bool LoRaWanClass::sendConfirmed(uint8_t port, uint8_t *buffer, uint16_t len, uint16_t timeout)
{
    McpsReq_t mcpsReq;
    LoRaMacTxInfo_t txInfo;
    uint32_t _timeout = timeout;

    LoRaMacStatus_t loramacStatus = LoRaMacQueryTxPossible( len, &txInfo ) ;

    if(loramacStatus != LORAMAC_STATUS_OK) {
        DEBUG("LoRaWan send empty frame");
        // Send empty frame in order to flush MAC commands
        mcpsReq.Type = MCPS_UNCONFIRMED;
        mcpsReq.Req.Unconfirmed.fBuffer = NULL;
        mcpsReq.Req.Unconfirmed.fBufferSize = 0;
        mcpsReq.Req.Unconfirmed.Datarate = _macDatarate;
    } else {
        DEBUG("LoRaWan send confirmed frame");
        mcpsReq.Type = MCPS_CONFIRMED;
        mcpsReq.Req.Confirmed.fPort = port;
        mcpsReq.Req.Confirmed.fBuffer = buffer;
        mcpsReq.Req.Confirmed.fBufferSize = len;
        mcpsReq.Req.Confirmed.NbTrials = _confirmedNbTrials;
        mcpsReq.Req.Confirmed.Datarate = _macDatarate;
    }

    if( LoRaMacMcpsRequest( &mcpsReq ) == LORAMAC_STATUS_OK ) {
        DEBUG("LoRaWan send confirm frame status OK!!!");
        _macSendStatus = 0;
        if(_timeout == 0){
            return true;
        }
    }else{
        return false;
    }

    if(_timeout < 120){
        _timeout = 120;
    }
    _macRunStatus = 0;
    uint32_t prevTime = millis();
    while(1){
        if(_macRunStatus == ep_lorawan_mcpsconfirm_confirmed){
            if(_ackReceived == true){
                return true;
            }else{
                return false;
            }
        }

        if(millis() - prevTime >= _timeout * 1000){
            return false;
        }
        intorobot_process();
    }
}

bool LoRaWanClass::sendUnconfirmed(uint8_t port, uint8_t *buffer, uint16_t len, uint16_t timeout)
{
    McpsReq_t mcpsReq;
    LoRaMacTxInfo_t txInfo;
    uint32_t _timeout = timeout;

    LoRaMacStatus_t loramacStatus = LoRaMacQueryTxPossible( len, &txInfo ) ;

    if(loramacStatus != LORAMAC_STATUS_OK) {
        DEBUG("LoRaWan send empty frame");
        // Send empty frame in order to flush MAC commands
        mcpsReq.Type = MCPS_UNCONFIRMED;
        mcpsReq.Req.Unconfirmed.fBuffer = NULL;
        mcpsReq.Req.Unconfirmed.fBufferSize = 0;
        mcpsReq.Req.Unconfirmed.Datarate = _macDatarate;
    } else {
        DEBUG("LoRaWan send unconfirmed frame");
        mcpsReq.Type = MCPS_UNCONFIRMED;
        mcpsReq.Req.Unconfirmed.fPort = port;
        mcpsReq.Req.Unconfirmed.fBuffer = buffer;
        mcpsReq.Req.Unconfirmed.fBufferSize = len;
        mcpsReq.Req.Unconfirmed.Datarate = _macDatarate;
    }

    if( LoRaMacMcpsRequest( &mcpsReq ) == LORAMAC_STATUS_OK ) {
        DEBUG("LoRaWan send unnconfirm frame status OK!!!");
        _macSendStatus = 0;
        if(_timeout == 0){
            return true;
        }
    }else{
        return false;
    }

    if(_timeout < 60){
        _timeout = 60;
    }

    DEBUG("_timeout = %d",_timeout);
    _macRunStatus = 0;
    uint32_t prevTime = millis();
    while(1){
        if(_macRunStatus == ep_lorawan_mcpsconfirm_unconfirmed){
            return true;
        }

        if(millis() - prevTime >= _timeout * 1000){
            return false;
        }
        intorobot_process();
    }
}

uint16_t LoRaWanClass::receive(uint8_t *buffer, uint16_t length, int *rssi)
{
    if(macBuffer.available) {
        macBuffer.available = false; //数据已读取
        *rssi = _macRssi;
        memcpy1(buffer, macBuffer.buffer, macBuffer.bufferSize);
        return macBuffer.bufferSize;
    }
    return 0;
}

void LoRaWanClass::setDeviceEUI(char *devEui)
{
    HAL_PARAMS_Set_System_device_id(devEui);
    HAL_PARAMS_Save_Params();
}

void LoRaWanClass::getDeviceEUI(char *devEui, uint16_t len)
{
    char deveui[24]={0};
    HAL_PARAMS_Get_System_device_id(deveui, sizeof(deveui));
    strncpy(devEui,deveui,len);
}

void LoRaWanClass::setDeviceAddr(char *devAddr)
{
    HAL_PARAMS_Set_System_devaddr(devAddr);
    HAL_PARAMS_Save_Params();
}

void LoRaWanClass::getDeviceAddr(char *devAddr, uint16_t len)
{
    char devaddr[16] = {0};
    HAL_PARAMS_Get_System_devaddr(devaddr, sizeof(devaddr));
    strncpy(devAddr,devaddr,len);
}

void LoRaWanClass::setAppEUI(char *appEui)
{
    HAL_PARAMS_Set_System_appeui(appEui);
    HAL_PARAMS_Save_Params();
}

void LoRaWanClass::getAppEUI(char *appEui, uint16_t len)
{
    char appeui[24]={0};
    HAL_PARAMS_Get_System_appeui(appeui, sizeof(appeui));
    strncpy(appEui,appeui,len);
}

void LoRaWanClass::setAppKey(char *appKey)
{
    HAL_PARAMS_Set_System_appkey(appKey);
    HAL_PARAMS_Save_Params();
}

void LoRaWanClass::setNwkSessionKey(uint8_t *nwkSkey)
{
    char nwkskey[36] = "";
    memcpy1(macParams.nwkSkey,nwkSkey,16);
    hex2string(macParams.nwkSkey, 16, nwkskey, false);
    HAL_PARAMS_Set_System_nwkskey(nwkskey);
    HAL_PARAMS_Save_Params();
}

void LoRaWanClass::setAppSessionKey(uint8_t *appSkey)
{
    char appskey[36] = "";
    memcpy1(macParams.appSkey,appSkey,16);
    hex2string(macParams.appSkey, 16, appskey, false);
    HAL_PARAMS_Set_System_appskey(appskey);
    HAL_PARAMS_Save_Params();
}

void LoRaWanClass::setMacClassType(DeviceClass_t classType)
{
    MibRequestConfirm_t mibReq;
    mibReq.Type = MIB_DEVICE_CLASS;
    mibReq.Param.Class = classType;
    LoRaMacMibSetRequestConfirm( &mibReq );
    _classType = classType;
}

DeviceClass_t LoRaWanClass::getMacClassType(void)
{
    MibRequestConfirm_t mibReq;
    mibReq.Type = MIB_DEVICE_CLASS;
    LoRaMacMibGetRequestConfirm( &mibReq );
    _classType = mibReq.Param.Class;
    return _classType;
}

void LoRaWanClass::setTxPower(uint8_t index)
{
    if(index > 5){
        return;
    }
    MibRequestConfirm_t mibReq;
    mibReq.Type = MIB_CHANNELS_DEFAULT_TX_POWER;
    mibReq.Param.ChannelsDefaultTxPower = index;
    LoRaMacMibSetRequestConfirm( &mibReq );

    mibReq.Type = MIB_CHANNELS_TX_POWER;
    mibReq.Param.ChannelsTxPower = index;
    LoRaMacMibSetRequestConfirm( &mibReq );
}

uint8_t LoRaWanClass::getTxPower(void)
{
    MibRequestConfirm_t mibReq;
    mibReq.Type = MIB_CHANNELS_TX_POWER;
    if(LoRaMacMibSetRequestConfirm( &mibReq ) != LORAMAC_STATUS_OK){
        return;
    }
    _txPower = mibReq.Param.ChannelsTxPower;
    return _txPower;
}

void LoRaWanClass::setDataRate(uint8_t datarate)
{
    if(datarate > DR_5){
        return;
    }
    _macDatarate = datarate;

    MibRequestConfirm_t mibReq;
    mibReq.Type = MIB_CHANNELS_DEFAULT_DATARATE;
    mibReq.Param.ChannelsDefaultDatarate = _macDatarate;
    LoRaMacMibSetRequestConfirm( &mibReq );

    mibReq.Type = MIB_CHANNELS_DATARATE;
    mibReq.Param.ChannelsDatarate = _macDatarate;
    LoRaMacMibSetRequestConfirm( &mibReq );
}

uint8_t LoRaWanClass::getDataRate(void)
{
    return _macDatarate;
}

void LoRaWanClass::setAdrOn(bool AdrOn)
{
    MibRequestConfirm_t mibReq;
    _adrOn = AdrOn;
    mibReq.Type = MIB_ADR;
    mibReq.Param.AdrEnable = _adrOn;
    LoRaMacMibSetRequestConfirm( &mibReq );
}

bool LoRaWanClass::getAdrOn(void)
{
    return _adrOn;
}

uint8_t LoRaWanClass::getDutyCyclePrescaler(void)
{
    return;
}

void LoRaWanClass::setChannelFreq(uint8_t channel, uint32_t freq)
{
    if(channel < 3 || channel > 15){
        return;
    }
    ChannelParams_t channelParams = {freq, { ( ( DR_5 << 4 ) | DR_0 ) }, 0};
    LoRaMacChannelAdd(channel,channelParams);
}

uint32_t LoRaWanClass::getChannelFreq(uint8_t channel)
{
    if(channel > 15){
        return;
    }
    return LoRaMacGetChannelFreq(channel);
}

void LoRaWanClass::setChannelDutyCycle(uint8_t channel, uint16_t dcycle)
{
    return;
}

void LoRaWanClass::getChannelDutyCycle(uint8_t channel)
{
    return;
}

void LoRaWanClass::setChannelDRRange(uint8_t channel, uint8_t minDR, uint8_t maxDR)
{
    if(channel > 15 || maxDR > DR_5){
        return;
    }
    if(minDR > maxDR){
        return;
    }
    uint32_t tmpFreq = LoRaMacGetChannelFreq(channel);
    ChannelParams_t channelParams = {tmpFreq, { ( ( maxDR << 4 ) | minDR ) }, 0};
    LoRaMacChannelAdd(channel,channelParams);
}

uint8_t LoRaWanClass::getChannelDRRange(uint8_t channel)
{

}

void LoRaWanClass::setChannelStatus(uint8_t channel, bool enable)
{
    if(channel > 15){
        return;
    }

    MibRequestConfirm_t mibReq;
    uint16_t channelMask = 0;
    mibReq.Type = MIB_CHANNELS_MASK;
    if(LoRaMacMibGetRequestConfirm( &mibReq ) != LORAMAC_STATUS_OK){
        return;
    }

    channelMask = *mibReq.Param.ChannelsMask;
    if(enable){
        channelMask = channelMask | (1<<channel);
    }else{
        channelMask = channelMask & (~(1<<channel));
    }

    mibReq.Type = MIB_CHANNELS_DEFAULT_MASK;
    mibReq.Param.ChannelsDefaultMask = &channelMask;
    LoRaMacMibSetRequestConfirm( &mibReq );

    mibReq.Type = MIB_CHANNELS_MASK;
    mibReq.Param.ChannelsMask = &channelMask;
    LoRaMacMibSetRequestConfirm( &mibReq );
}

bool LoRaWanClass::getChannelStatus(uint8_t channel)
{
    if(channel > 15){
        return;
    }

    MibRequestConfirm_t mibReq;
    uint16_t channelMask = 0;
    mibReq.Type = MIB_CHANNELS_MASK;
    if(LoRaMacMibGetRequestConfirm( &mibReq ) != LORAMAC_STATUS_OK){
        return;
    }
    channelMask = *mibReq.Param.ChannelsMask;
    if(channelMask & (1<<channel)){
        return true;
    }else{
        return false;
    }
}

void LoRaWanClass::setConfirmedNbTrials(uint8_t trials)
{
    _confirmedNbTrials = trials;
}

uint8_t LoRaWanClass::getConfirmedNbTrials(void)
{
    return _confirmedNbTrials;
}

void LoRaWanClass::setUnconfirmedNbTrials(uint8_t trials)
{
    MibRequestConfirm_t mibReq;

    _unConfirmedNbTrials = trials;
    if(trials >= 1 && trials <= 15) {
        mibReq.Type = MIB_CHANNELS_NB_REP;
        mibReq.Param.ChannelNbRep = trials;
        LoRaMacMibSetRequestConfirm( &mibReq );
    }
}

uint8_t LoRaWanClass::getUnconfirmedNbTrials(void)
{
    return _unConfirmedNbTrials;
}

void LoRaWanClass::setJoinNbTrials(uint8_t trials)
{
    _joinNbTrials = trials;
}

uint8_t LoRaWanClass::getJoinNbTrials(void)
{
    return _joinNbTrials;
}

void LoRaWanClass::setUpCounter(uint32_t counter)
{
    MibRequestConfirm_t mibReq;
    mibReq.Type = MIB_UPLINK_COUNTER;
    mibReq.Param.UpLinkCounter = counter;
    LoRaMacMibSetRequestConfirm(&mibReq);
}

uint32_t LoRaWanClass::getUpCounter(void)
{
    MibRequestConfirm_t mibReq;
    LoRaMacStatus_t status;

    mibReq.Type = MIB_UPLINK_COUNTER;
    status = LoRaMacMibGetRequestConfirm( &mibReq );
    if(status == LORAMAC_STATUS_OK) {
        return mibReq.Param.UpLinkCounter;
    }
}

void LoRaWanClass::setDownCounter(uint32_t counter)
{
    MibRequestConfirm_t mibReq;
    mibReq.Type = MIB_DOWNLINK_COUNTER;
    mibReq.Param.DownLinkCounter = counter;
    LoRaMacMibSetRequestConfirm(&mibReq);
}

uint32_t LoRaWanClass::getDownCounter(void)
{
    MibRequestConfirm_t mibReq;
    LoRaMacStatus_t status;

    mibReq.Type = MIB_DOWNLINK_COUNTER;
    status = LoRaMacMibGetRequestConfirm( &mibReq );
    if(status == LORAMAC_STATUS_OK) {
        return mibReq.Param.DownLinkCounter;
    }
}

uint8_t LoRaWanClass::setRX2Parameters(uint8_t datarate, uint32_t frequency)
{
    if(datarate > DR_5){
        return 0;
    }
    MibRequestConfirm_t mibReq;
    mibReq.Type = MIB_RX2_DEFAULT_CHANNEL;
    mibReq.Param.Rx2DefaultChannel = ( Rx2ChannelParams_t ){ frequency,datarate };
    LoRaMacMibSetRequestConfirm( &mibReq );

    mibReq.Type = MIB_RX2_CHANNEL;
    mibReq.Param.Rx2DefaultChannel = ( Rx2ChannelParams_t ){ frequency,datarate };
    LoRaMacMibSetRequestConfirm( &mibReq );
}

uint8_t LoRaWanClass::setRX1Delay(uint16_t delay)
{
    MibRequestConfirm_t mibReq;
    mibReq.Type = MIB_RECEIVE_DELAY_1;
    mibReq.Param.ReceiveDelay1 = delay;
    LoRaMacMibSetRequestConfirm( &mibReq );
}

uint8_t LoRaWanClass::getMargin(void)
{
    return _demodMargin;
}

uint8_t LoRaWanClass::getGatewayNumber(void)
{
    return _nbGateways;
}

uint8_t LoRaWanClass::getSnr(void)
{
    return _macSnr;
}

int LoRaWanClass::getRssi(void)
{
    return _macRssi;
}
//发送状态查询 0-发送中 1-成功 2-失败
uint8_t LoRaWanClass::sendStatus(void)
{
    return _macSendStatus;
}

//P2P透传接口
bool LoRaClass::radioSend(uint8_t *buffer, uint16_t length, uint32_t timeout)
{
    Radio.SetTxConfig(_modem, _power, _fdev, _bandwidth, _datarate, _coderate, _preambleLen, _fixLenOn, _crcOn, _freqHopOn, _hopPeriod, _iqInverted, timeout);
    Radio.Send(buffer, length);
    uint32_t _timeout = timeout;
    if(_timeout == 0){
        return true;
    }

    if(_timeout < 3000){
        _timeout = 3000;
    }

    _radioRunStatus = 0;
    uint32_t prevTime = millis();
    while(1){
        if(_radioRunStatus == ep_lora_radio_tx_done){
            return true;
        }
        if(_radioRunStatus == ep_lora_radio_tx_timeout){
            return false;
        }

        if(millis() - prevTime >= _timeout){
            return false;
        }
        intorobot_process();
    }
}

void LoRaClass::radioStartRx(uint32_t timeout)
{
    Radio.SetRxConfig(_modem, _bandwidth, _datarate, _coderate, _bandwidthAfc, _preambleLen, _symbTimeout, _fixLenOn, _payloadLen, _crcOn, _freqHopOn, _hopPeriod, _iqInverted, _rxContinuous);
    Radio.Rx(timeout);
}

uint16_t LoRaClass::radioRx(uint8_t *buffer, uint16_t length, int16_t *rssi)
{
    uint16_t size = 0;
    if(_radioAvailable){
        _radioAvailable = false;
        if(length < _length){
            size = length;
        }else{
            size = _length;
        }
        memcpy( buffer, _buffer, size);
        *rssi = _rssi;
        return _length;
    }else{
        return 0;
    }
}

void LoRaClass::radioStartCad(void)
{
    Radio.StartCad();
}

bool LoRaClass::radioCad(void)
{
    return _cadDetected;
}

int16_t LoRaClass::radioReadRssi(void)
{
    return _rssi;
}

int8_t LoRaClass::radioReadSnr(void)
{
    return _snr;
}

void LoRaClass::radioSetSleep(void)
{
    Radio.Sleep();
}

void LoRaClass::radioSetFreq(uint32_t freq)
{
    _freq = freq;
    Radio.SetChannel(_freq);
}

uint32_t LoRaClass::radioGetFreq(void)
{
    return _freq;
}

void LoRaClass::radioSetMaxPayloadLength(uint8_t maxPayloadLen)
{
    uint8_t _maxPayloadLen = maxPayloadLen;
    Radio.SetMaxPayloadLength(_modem, _maxPayloadLen);
}

uint8_t LoRaClass::radioGetMaxPayloadLength(void)
{
    return _maxPayloadLen;
}

void LoRaClass::radioSetModem(RadioModems_t modem)
{
    _modem = modem;
    Radio.SetModem(_modem);
}

RadioModems_t LoRaClass::radioGetModem(void)
{
    return _modem;
}

void LoRaClass::radioSetBandwidth(uint8_t bandwidth)
{
    _bandwidth = bandwidth;
}

uint8_t LoRaClass::radioGetBandwidth(void)
{
    return _bandwidth;
}

void LoRaClass::radioSetSF(uint8_t sf)
{
    _datarate = sf;
}

uint8_t LoRaClass::radioGetSF(void)
{
    return _datarate;
}

void LoRaClass::radioSetCR(uint8_t coderate)
{
    _coderate = coderate;
}

uint8_t LoRaClass::radioGetCR(void)
{
    return _coderate;
}

void LoRaClass::radioSetPreambleLen(uint16_t preambleLen)
{
    _preambleLen = preambleLen;
}

uint16_t LoRaClass::radioGetPreambleLen(void)
{
    return _preambleLen;
}

void LoRaClass::radioSetFixLenOn(bool fixLenOn)
{
    _fixLenOn = fixLenOn;
}

bool LoRaClass::radioGetFixLenOn(void)
{
    return _fixLenOn;
}

void LoRaClass::radioSetCrcOn(bool crcOn)
{
    _crcOn = crcOn;
}

bool LoRaClass::radioGetCrcOn(void)
{
    return _crcOn;
}

void LoRaClass::radioSetFreqHopOn(bool FreqHopOn)
{
    _freqHopOn = FreqHopOn;
}

bool LoRaClass::radioGetFreqHopOn(void)
{
    return _freqHopOn;
}

void LoRaClass::radioSetHopPeriod(uint8_t hopPeriod)
{
    _hopPeriod = hopPeriod;
}

uint8_t LoRaClass::radioGetHopPeriod(void)
{
    return _hopPeriod;
}

void LoRaClass::radioSetIqInverted(bool iqInverted)
{
    _iqInverted = iqInverted;
}

bool LoRaClass::radioGetIqInverted(void)
{
    return _iqInverted;
}

void LoRaClass::radioSetRxContinuous(bool rxContinuous)
{
    _rxContinuous = rxContinuous;
}

bool LoRaClass::radioGetRxContinuous(void)
{
    return _rxContinuous;
}

//发送射频相关参数
void LoRaClass::radioSetTxPower(uint8_t txPower)
{
    _power = txPower;
}

uint8_t LoRaClass::radioGetTxPower(void)
{
    return _power;
}

//接收射频相关参数
void LoRaClass::radioSetFixPayloadLen(uint8_t payloadLen)
{
    _payloadLen = payloadLen;
}

void LoRaClass::radioSetSymbTimeout(uint32_t symbTimeout)
{
    if(symbTimeout >= 4 && symbTimeout <= 1023) {
        _symbTimeout = symbTimeout;
    }
}

uint32_t LoRaClass::radioGetSymbTimeout(void)
{
    return _symbTimeout;
}

uint8_t LoRaClass::radioRead(uint8_t addr)
{
    return Radio.Read(addr);
}

void LoRaClass::radioWrite(uint8_t addr, uint8_t data)
{
    Radio.Write(addr,data);
}


//生产时板子测试代码
#define RF_FREQUENCY        434665000 // Hz
#define RF_IQ_INVERTED      true
#define RX_TIMEOUT_VALUE    2000
#define BUFFER_SIZE         8 // Define the payload size here

typedef enum
{
    LOWPOWER,
    RX_DONE,
    RX_TIMEOUT,
    RX_ERROR,
    TX_DONE,
    TX_TIMEOUT,
    TX_START,
}States_t;

static uint16_t BufferSize = BUFFER_SIZE;
static int8_t Buffer[BUFFER_SIZE] = {11,20,30,40,50,60,70,80};
static States_t State = LOWPOWER;
static int8_t RssiValue = 0;
static int8_t SnrValue = 0;
static uint8_t sx1278Version = 0;
static RadioEvents_t testRadioEvents;

static void TestOnTxDone( void );
static void TestOnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );
static void TestOnTxTimeout( void );
static void TestOnRxTimeout( void );
static void TestOnRxError( void );
static void TestSX1278Init(void);
static int8_t TestSX1278Run(void);

void TestSX1278Init(void)
{
    SX1276BoardInit();

    // Radio initialization
    testRadioEvents.TxDone = TestOnTxDone;
    testRadioEvents.RxDone = TestOnRxDone;
    testRadioEvents.TxTimeout = TestOnTxTimeout;
    testRadioEvents.RxTimeout = TestOnRxTimeout;
    testRadioEvents.RxError = TestOnRxError;
    Radio.Init( &testRadioEvents );

    LoRa.radioSetFreq(RF_FREQUENCY);
    LoRa.radioSetModem(MODEM_LORA);
    LoRa.radioSetIqInverted(RF_IQ_INVERTED);

    sx1278Version = LoRa.radioRead(0x42);
    // DEBUG("sx1278 version = 0x%2x", sx1278Version);
    // DEBUG("sx1278 freq = %d",SX1276LoRaGetRFFrequency());
    // DEBUG("sync data = 0x%2x",LoRaWan.radioRead(0x39));

    State = TX_START;
}


int8_t TestSX1278Run(void)
{
    switch(State)
    {
        case TX_START:
                LoRa.radioSend(Buffer, BufferSize, 5000);
                State = LOWPOWER;
                return 0;
            break;

        case TX_DONE:
                LoRa.radioStartRx(RX_TIMEOUT_VALUE);
                State = LOWPOWER;
                return 1;
            break;

        case RX_DONE:
            return 2;
            break;

        case LOWPOWER:
            break;

         case TX_TIMEOUT:
         case RX_TIMEOUT:
         case RX_ERROR:
             return -1;
             break;

        default:
            break;
    }
}

bool SX1278Test(int8_t &snr, int8_t &rssi, int8_t &txRssi)
{
    TestSX1278Init();
    if(sx1278Version != 18)
    {
        return false;
    }
    else
    {
        while(1)
        {
            switch(TestSX1278Run())
            {
                case 0:
                case 1:
                    break;

                case 2:
                    if(Buffer[0] == 22)
                    {
                        snr = SnrValue;
                        rssi = RssiValue;
                        txRssi = Buffer[1];
                        // DEBUG("snr = %d",SnrValue);
                        // DEBUG("rssi = %d",RssiValue);
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                    break;

                default:
                    return false; // tx or rx timeout
                    break;
            }
        }
    }
}

void TestOnTxDone( void )
{
    LoRa.radioSetSleep();
    State = TX_DONE;
    // DEBUG("tx done");
}

void TestOnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    LoRa.radioSetSleep();
    BufferSize = size;
    memcpy( Buffer, payload, BufferSize );
    State = RX_DONE;

    RssiValue = rssi;
    SnrValue = snr;
    // DEBUG("rx done");

    #if 0
    DEBUG_D("rx data:");
    for(uint8_t i=0; i<BufferSize; i++)
    {
        if(Buffer[i] != 0)
        {
            DEBUG_D("%d ",Buffer[i]);
        }
    }
    DEBUG_D("\r\n");
    #endif
}

void TestOnTxTimeout( void )
{
    LoRa.radioSetSleep();
    State = TX_TIMEOUT;
    // DEBUG("tx timeout");
}

void TestOnRxTimeout( void )
{
    LoRa.radioSetSleep();
    State = RX_TIMEOUT;
    // DEBUG("rx timeout");
}

void TestOnRxError( void )
{
    LoRa.radioSetSleep();
    State = RX_ERROR;
    // DEBUG("rx error");
}

#endif
