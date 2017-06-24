#include "intorobot_config.h"
#include "wiring_ex_lorawan.h"
#include "system_lorawan.h"
#include "system_event.h"
#include "wiring_system.h"
#include "service_debug.h"

#ifndef configNO_LORAWAN
static lorawan_data_t LoRaWanBuffer;
static lorawan_params_t LoRaWanParams;
static LoRaMacPrimitives_t LoRaMacPrimitives;
static LoRaMacCallback_t LoRaMacCallbacks;

/*!
 * \brief   MCPS-Confirm event function
 *
 * \param   [IN] mcpsConfirm - Pointer to the confirm structure,
 *               containing confirm attributes.
 */
static void McpsConfirm( McpsConfirm_t *mcpsConfirm )
{
    if( mcpsConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK )
    {
        switch( mcpsConfirm->McpsRequest )
        {
            case MCPS_UNCONFIRMED:
            {
                // Check Datarate
                // Check TxPower
                LoRaWanOnEvent(LORAWAN_EVENT_MCPS_UNCONFIRMED);
                break;
            }
            case MCPS_CONFIRMED:
            {
                // Check Datarate
                // Check TxPower
                // Check AckReceived
                // Check NbTrials
                LoRaWanOnEvent(LORAWAN_EVENT_MCPS_CONFIRMED);
                break;
            }
            case MCPS_PROPRIETARY:
            {
                LoRaWanOnEvent(LORAWAN_EVENT_MCPS_PROPRIETARY);
                break;
            }
            default:
                break;
        }
    }
}

/*!
 * \brief   MCPS-Indication event function
 *
 * \param   [IN] mcpsIndication - Pointer to the indication structure,
 *               containing indication attributes.
 */
static void McpsIndication( McpsIndication_t *mcpsIndication )
{
    if( mcpsIndication->Status != LORAMAC_EVENT_INFO_STATUS_OK )
    {
        return;
    }

    switch( mcpsIndication->McpsIndication )
    {
        case MCPS_UNCONFIRMED:
        {
            LoRaWanOnEvent(LORAWAN_EVENT_MCPS_UNCONFIRMED);
            break;
        }
        case MCPS_CONFIRMED:
        {
            LoRaWanOnEvent(LORAWAN_EVENT_MCPS_CONFIRMED);
            break;
        }
        case MCPS_PROPRIETARY:
        {
            LoRaWanOnEvent(LORAWAN_EVENT_MCPS_PROPRIETARY);
            break;
        }
        case MCPS_MULTICAST:
        {
            LoRaWanOnEvent(LORAWAN_EVENT_MCPS_MULTICAST);
            break;
        }
        default:
            break;
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

    if( mcpsIndication->RxData == true )
    {
        LoRaWanBuffer.available = true;
        LoRaWanBuffer.bufferSize = mcpsIndication->BufferSize;
        memcpy1(LoRaWanBuffer.buffer,mcpsIndication->Buffer,mcpsIndication->BufferSize);
        LoRaWanOnEvent(LORAWAN_EVENT_RX_COMPLETE);
    }
    else
    {
        LoRaWanBuffer.available = false;
    }

}

/*!
 * \brief   MLME-Confirm event function
 *
 * \param   [IN] mlmeConfirm - Pointer to the confirm structure,
 *               containing confirm attributes.
 */
static void MlmeConfirm( MlmeConfirm_t *mlmeConfirm )
{
    switch( mlmeConfirm->MlmeRequest )
    {
        case MLME_JOIN:
        {
            if( mlmeConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK )
            {
                // Status is OK, node has joined the network
                LoRaWanOnEvent(LORAWAN_EVENT_JOINED);
            }
            else
            {
                // Join was not successful. Try to join again
                LoRaWanOnEvent(LORAWAN_EVENT_JOIN_FAIL);
            }
            break;
        }
        case MLME_LINK_CHECK:
        {
            if( mlmeConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK )
            {
                // Check DemodMargin
                // Check NbGateways
                LoRaWanOnEvent(LORAWAN_EVENT_MLME_LINK_CHECK);
            }
            break;
        }
        default:
            break;
    }
}


//初始化
void LoRaWanInitialize(void)
{
    SX1276BoardInit(); //初始化1278
    LoRaMacPrimitives.MacMcpsConfirm = McpsConfirm;
    LoRaMacPrimitives.MacMcpsIndication = McpsIndication;
    LoRaMacPrimitives.MacMlmeConfirm = MlmeConfirm;
    LoRaMacCallbacks.GetBatteryLevel = BoardGetBatteryLevel;
    LoRaMacInitialization( &LoRaMacPrimitives, &LoRaMacCallbacks );

    DEBUG("sync data = 0x%x",SX1276Read(0x39));
    DEBUG("sx1278 version = 0x%x", SX1276GetVersion());
    DEBUG("sx1278 freq = %d",SX1276LoRaGetRFFrequency());
    DEBUG("sx1278 mode = 0x%x",SX1276Read(0x1));

    MibRequestConfirm_t mibReq;

    mibReq.Type = MIB_ADR;
    mibReq.Param.AdrEnable = false;
    LoRaMacMibSetRequestConfirm( &mibReq );

    mibReq.Type = MIB_PUBLIC_NETWORK;
    mibReq.Param.EnablePublicNetwork = LORAWAN_PUBLIC_NETWORK;
    LoRaMacMibSetRequestConfirm( &mibReq );

    //设置为C类
    mibReq.Type = MIB_DEVICE_CLASS;
    mibReq.Param.Class = CLASS_C;
    LoRaMacMibSetRequestConfirm( &mibReq );
}

//无线激活
void LoRaWanOTAAJoin(uint8_t *DevEui, uint8_t *AppEui, uint8_t *AppKey)
{
    MlmeReq_t mlmeReq;

    memcpy1(LoRaWanParams.devEui,DevEui,8);
    memcpy1(LoRaWanParams.appEui,AppEui,8);
    memcpy1(LoRaWanParams.appKey,AppKey,16);

    mlmeReq.Type = MLME_JOIN;
    mlmeReq.Req.Join.DevEui = LoRaWanParams.devEui;
    mlmeReq.Req.Join.AppEui = LoRaWanParams.appEui;
    mlmeReq.Req.Join.AppKey = LoRaWanParams.appKey;
    mlmeReq.Req.Join.NbTrials = 3;

    LoRaMacMlmeRequest( &mlmeReq );
}

//手动激活
void LoRaWanABPJoin(uint32_t DevAddr, uint8_t *NwkSKey, uint8_t *AppSKey)
{
    MibRequestConfirm_t mibReq;

    LoRaWanParams.devAddr = DevAddr;
    memcpy1(LoRaWanParams.nwkSkey,NwkSKey,16);
    memcpy1(LoRaWanParams.appSkey,AppSKey,16);


    mibReq.Type = MIB_NET_ID;
    mibReq.Param.NetID = LORAWAN_NETWORK_ID;
    LoRaMacMibSetRequestConfirm( &mibReq );

    mibReq.Type = MIB_DEV_ADDR;
    mibReq.Param.DevAddr = LoRaWanParams.devAddr;
    LoRaMacMibSetRequestConfirm( &mibReq );

    mibReq.Type = MIB_NWK_SKEY;
    mibReq.Param.NwkSKey = LoRaWanParams.nwkSkey;
    LoRaMacMibSetRequestConfirm( &mibReq );

    mibReq.Type = MIB_APP_SKEY;
    mibReq.Param.AppSKey = LoRaWanParams.appSkey;
    LoRaMacMibSetRequestConfirm( &mibReq );

    mibReq.Type = MIB_NETWORK_JOINED;
    mibReq.Param.IsNetworkJoined = true;
    LoRaMacMibSetRequestConfirm( &mibReq );
}

void LoRaWanGetABPParams(uint32_t &DevAddr, uint8_t *NwkSKey, uint8_t *AppSKey)
{
    MibRequestConfirm_t mibReq;
    LoRaMacStatus_t status;

    uint8_t i;
    mibReq.Type = MIB_DEV_ADDR;
    status = LoRaMacMibGetRequestConfirm( &mibReq );
    if(status == LORAMAC_STATUS_OK)
    {
        mibReq.Type = MIB_DEV_ADDR;
        DevAddr = mibReq.Param.DevAddr;
    }

    mibReq.Type = MIB_NWK_SKEY;
    status = LoRaMacMibGetRequestConfirm( &mibReq );
    if(status == LORAMAC_STATUS_OK)
    {
        memcpy1(NwkSKey,mibReq.Param.NwkSKey,16);
    }

    mibReq.Type = MIB_APP_SKEY;
    status = LoRaMacMibGetRequestConfirm( &mibReq );
    if(status == LORAMAC_STATUS_OK)
    {
        memcpy1(AppSKey,mibReq.Param.AppSKey,16);
    }
}

//发送数据 IsTxConfirmed 为false 发送不确认帧 true发送确认帧
bool LoRaWanSendFrame(uint8_t *buffer, uint16_t len, bool IsTxConfirmed)
{
    McpsReq_t mcpsReq;
    LoRaMacTxInfo_t txInfo;

    DEBUG("LoRaWan start send frame");
    DEBUG("LoRaWan data len = %d",len);
    LoRaMacStatus_t loramacStatus = LoRaMacQueryTxPossible( len, &txInfo ) ;
    DEBUG("LoRaMac Status = %d",loramacStatus);

    if(loramacStatus != LORAMAC_STATUS_OK)
    {
        DEBUG("LoRaWan send empty frame");
        // Send empty frame in order to flush MAC commands
        mcpsReq.Type = MCPS_UNCONFIRMED;
        mcpsReq.Req.Unconfirmed.fBuffer = NULL;
        mcpsReq.Req.Unconfirmed.fBufferSize = 0;
        mcpsReq.Req.Unconfirmed.Datarate = LORAWAN_DEFAULT_DATARATE;
    }
    else
    {
        if( IsTxConfirmed == false )
        {
            DEBUG("LoRaWan send unconfirmed frame");
            mcpsReq.Type = MCPS_UNCONFIRMED;
            mcpsReq.Req.Unconfirmed.fPort = 2;
            mcpsReq.Req.Unconfirmed.fBuffer = buffer;
            mcpsReq.Req.Unconfirmed.fBufferSize = len;
            mcpsReq.Req.Unconfirmed.Datarate = LORAWAN_DEFAULT_DATARATE;
        }
        else
        {
            DEBUG("LoRaWan send confirmed frame");
            mcpsReq.Type = MCPS_CONFIRMED;
            mcpsReq.Req.Confirmed.fPort = 2;
            mcpsReq.Req.Confirmed.fBuffer = buffer;
            mcpsReq.Req.Confirmed.fBufferSize = len;
            mcpsReq.Req.Confirmed.NbTrials = 8;
            mcpsReq.Req.Confirmed.Datarate = LORAWAN_DEFAULT_DATARATE;
        }
    }

    if( LoRaMacMcpsRequest( &mcpsReq ) == LORAMAC_STATUS_OK )
    {
        DEBUG("LoRaWan send frame OK!!!");
        LoRaWanOnEvent(LORAWAN_EVENT_TX_COMPLETE);
        return true;
    }

    return false;
}

//-1没有收到数据 否则返回数据长度
int LoRaWanReceiveFrame(uint8_t *buffer)
{
    if(!LoRaWanBuffer.available)
    {
        return -1;
    }
    else
    {
        memcpy1(buffer, LoRaWanBuffer.buffer, LoRaWanBuffer.bufferSize);
        return LoRaWanBuffer.bufferSize;
    }
}

//======loramac不运行========
static  RadioEvents_t loraRadioEvents;

static void OnLoRaRadioTxDone(void)
{
    system_notify_event(event_lora_radio_status,ep_lora_radio_tx_done);
}

static void OnLoRaRadioTxTimeout(void)
{

    system_notify_event(event_lora_radio_status,ep_lora_radio_tx_timeout);
}

static void OnLoRaRadioRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
{
    LoRaWan._rssi = rssi;
    LoRaWan._snr = snr;
    system_notify_event(event_lora_radio_status,ep_lora_radio_rx_done,payload,size);
}

static void OnLoRaRadioRxTimeout(void)
{

    system_notify_event(event_lora_radio_status,ep_lora_radio_rx_timeout);
}

static void OnLoRaRadioRxError(void)
{

    system_notify_event(event_lora_radio_status,ep_lora_radio_rx_error);
}

static void OnLoRaRadioCadDone(bool channelActivityDetected)
{
    uint8_t cadDetected;
    if(channelActivityDetected)
    {
        cadDetected = 1;
    }
    else
    {
        cadDetected = 0;
    }
    system_notify_event(event_lora_radio_status,ep_lora_radio_cad_done,&cadDetected,1);
}

void LoRaRadioInitialize(void)
{
    // Radio initialization
    SX1276BoardInit();
    loraRadioEvents.TxDone = OnLoRaRadioTxDone;
    loraRadioEvents.RxDone = OnLoRaRadioRxDone;
    loraRadioEvents.TxTimeout = OnLoRaRadioTxTimeout;
    loraRadioEvents.RxTimeout = OnLoRaRadioRxTimeout;
    loraRadioEvents.RxError = OnLoRaRadioRxError;
    loraRadioEvents.CadDone = OnLoRaRadioCadDone;
    Radio.Init( &loraRadioEvents );
    Radio.SetModem( MODEM_LORA );

    DEBUG("sync data = 0x%x",SX1276Read(0x39));
    DEBUG("sx1278 version = 0x%x", SX1276GetVersion());
    DEBUG("sx1278 freq = %d",SX1276LoRaGetRFFrequency());
    DEBUG("sx1278 mode = 0x%x",SX1276Read(0x1));
}

static void SystemWakeupCb(void)
{
    TimerStop( &LoRaWan.systemWakeupTimer );
    SX1276BoardInit();
    LoRaWan.radioSetModem(MODEM_LORA);
    if(LoRaWan.wakeupCb != NULL)
    {
        LoRaWan.wakeupCb();
    }
}

LoRaWanClass LoRaWan;

void LoRaWanClass::loramacSetClassType(DeviceClass_t classType)
{
    MibRequestConfirm_t mibReq;
    mibReq.Type = MIB_DEVICE_CLASS;
    mibReq.Param.Class = classType;
    LoRaMacMibSetRequestConfirm( &mibReq );
}

//暂停lorawan
void LoRaWanClass::loramacPause(void)
{
    System.disableFeature(SYSTEM_FEATURE_LORAMAC_ENABLED);
}

//恢复lorawan
void LoRaWanClass::loramacResume(void)
{
    System.enableFeature(SYSTEM_FEATURE_LORAMAC_ENABLED);
}

//系统休眠
void LoRaWanClass::systemSleep(void)
{
    radioSetSleep();
    TimerLowPowerHandler();
}

void LoRaWanClass::systemWakeupHandler(void)
{
    TimerStop( &systemWakeupTimer );

    SX1276BoardInit();
    if(wakeupCb != NULL)
    {
        wakeupCb();
    }
}

typedef void (*FUNC)(void);
void LoRaWanClass::setSystemWakeup(radioCb userHandler, uint32_t timeout) //单位s
{
    wakeupCb = userHandler;
    // TimerInit( &systemWakeupTimer, (FUNC)&LoRaWanClass::systemWakeupHandler);//TODO 此处回调无法运行
    TimerInit( &systemWakeupTimer, SystemWakeupCb);
    TimerSetValue( &systemWakeupTimer, timeout*1000 );
    TimerStart( &systemWakeupTimer );
}

//sx1278休眠
void LoRaWanClass::radioSetSleep(void)
{
    Radio.Sleep();
}

//设置频率
void LoRaWanClass::radioSetFreq(uint32_t freq)
{
    _freq = freq;
    Radio.SetChannel(_freq);
}

uint32_t LoRaWanClass::radioGetFreq(void)
{
    return _freq;
}

//设置模式 0:fsk 1:lora
void LoRaWanClass::radioSetModem(RadioModems_t modem)
{
    _modem = modem;
}

uint8_t LoRaWanClass::radioGetModem(void)
{
    return _modem;
}

//设置带宽
void LoRaWanClass::radioSetBandwidth(uint32_t bandwidth)
{
    _bandwidth = bandwidth;
}

uint32_t LoRaWanClass::radioGetBandwidth(void)
{
    return _bandwidth;
}

//设置扩频因子
void LoRaWanClass::radioSetSF(uint32_t sf)
{
    _datarate = sf;
}

uint8_t LoRaWanClass::radioGetSF(void)
{
    return _datarate;
}

//设置纠错编码率
void LoRaWanClass::radioSetCoderate(uint32_t coderate)
{
    _coderate = coderate;
}

uint8_t LoRaWanClass::radioGetCoderate(void)
{
    return _coderate;
}

//设置前导码超时
void LoRaWanClass::radioSetSymbTimeout(uint32_t symbTimeout)
{
    _symbTimeout = symbTimeout;
}

uint16_t LoRaWanClass::radioGetSymbTimeout(void)
{
    return _symbTimeout;
}

//设置crc校验
void LoRaWanClass::radioSetCrcOn(bool crcOn)
{
    _crcOn = crcOn;
}

bool LoRaWanClass::radioGetCrcOn(void)
{
    return _crcOn;
}

//设置前导码长度
void LoRaWanClass::radioSetPreambleLen(uint16_t preambleLen)
{
    _preambleLen = preambleLen;
}

uint16_t LoRaWanClass::radioGetPreambleLen(void)
{
    return _preambleLen;
}

void LoRaWanClass::radioSetTxTimeout(uint32_t timeout)
{
    _txTimeout = timeout;
}

void LoRaWanClass::radioSetRxContinuous(bool rxContinuous)
{
    _rxContinuous = rxContinuous;
}

bool LoRaWanClass::radioGetRxContinuous(void)
{
    return _rxContinuous;
}

//接收设置
void LoRaWanClass::radioSetRxConfig(void)
{
    Radio.SetRxConfig(_modem,_bandwidth,_datarate,_coderate,_bandwidthAfc,_preambleLen,_symbTimeout,_fixLen,_payloadLen,_crcOn,_freqHopOn,_hopPeriod,_iqInverted,_rxContinuous);
}

//发送设置
void LoRaWanClass::radioSetTxConfig(void)
{
    Radio.SetTxConfig(_modem,_power,_fdev,_bandwidth,_datarate,_coderate,_preambleLen,_fixLen,_crcOn,_freqHopOn,_hopPeriod,_iqInverted,_txTimeout);
}

//开始发送
void LoRaWanClass::radioSend( uint8_t *buffer, uint8_t size )
{
    radioSetTxConfig();
    Radio.Send(buffer,size);
}

//开始接收
void LoRaWanClass::radioRx(uint32_t timeout)
{
    radioSetRxConfig();
    Radio.Rx(timeout);
}

//读取寄存器值
uint8_t LoRaWanClass::radioRead(uint8_t addr)
{
    Radio.Read(addr);
}

//写寄存器值
void LoRaWanClass::radioWrite(uint8_t addr, uint8_t data)
{
    Radio.Write(addr,data);
}

//启动CAD
void LoRaWanClass::radioStartCad(void)
{
    Radio.StartCad();
}

//获取rssi
int16_t LoRaWanClass::radioReadRssi(void)
{
    return LoRaWan._rssi;
    // return Radio.Rssi(1);
}

int8_t LoRaWanClass::radioReadSnr(void)
{
    return LoRaWan._snr;
}
//设置负载最大长度
void LoRaWanClass::radioSetMaxPayloadLength(uint8_t max)
{
    Radio.SetMaxPayloadLength(_modem,max);
}

//设置同步字
void LoRaWanClass::radioSetSyncword(uint8_t syncword)
{
    Radio.Write(0x39,syncword);
}


#endif

// #define TEST_SX1276
#ifdef TEST_SX1276


#include "wiring.h"

#define USE_BAND_433
#define USE_MODEM_LORA

#define RF_FREQUENCY                               434665000 // Hz

#define TX_OUTPUT_POWER                            20        // dBm

#define LORA_BANDWIDTH                             0         // [0: 125 kHz,
                                                             //  1: 250 kHz,
                                                             //  2: 500 kHz,
                                                             //  3: Reserved]

#define LORA_SPREADING_FACTOR                      7         // [SF7..SF12]
#define LORA_CODINGRATE                            1         // [1: 4/5,
                                                             //  2: 4/6,
                                                             //  3: 4/7,
                                                             //  4: 4/8]

#define LORA_PREAMBLE_LENGTH                       8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                        1023      // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                 false
#define LORA_IQ_INVERSION_ON                       true


typedef enum
{
    LOWPOWER,
    RX_SX1276,
    RX_TIMEOUT,
    RX_ERROR,
    TX_SX1276,
    TX_TIMEOUT,
    TX_STATUS,
    RX_STATUS
}States_t;

#define RX_TIMEOUT_VALUE    1000
#define BUFFER_SIZE         64 // Define the payload size here

static uint16_t BufferSize = BUFFER_SIZE;

static States_t State = LOWPOWER;

static int8_t RssiValue = 0;
static int8_t SnrValue = 0;
static uint32_t currentTime = 0;
static uint8_t sx1278Version = 0;

static int8_t Buffer[BUFFER_SIZE] = {11,20,30,40,50,60,70,80};

static RadioEvents_t RadioEvents;
static void OnTxDone( void );
static void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );
static void OnTxTimeout( void );
static void OnRxTimeout( void );
static void OnRxError( void );
static void TestSX1276Init(void);
static uint8_t ProcessSX1276(void);

void TestSX1276Init(void)
{
    SX1276BoardInit();

    // Radio initialization
    RadioEvents.TxDone = OnTxDone;
    RadioEvents.RxDone = OnRxDone;
    RadioEvents.TxTimeout = OnTxTimeout;
    RadioEvents.RxTimeout = OnRxTimeout;
    RadioEvents.RxError = OnRxError;

    Radio.Init( &RadioEvents );

    Radio.SetChannel( RF_FREQUENCY );

    Radio.SetModem( MODEM_LORA );

    SX1276Write(0x39,0x12);
    sx1278Version = SX1276GetVersion();
    // DEBUG("sx1278 version = 0x%2x", sx1278Version);
    // DEBUG("sx1278 freq = %d",SX1276LoRaGetRFFrequency());
    // DEBUG("sync data = 0x%2x",SX1276Read(0x39));

    Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                       LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                       LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                       true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );

    Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                       LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                       LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                       0, true, 0, 0, LORA_IQ_INVERSION_ON, 1 );

    Radio.Rx( 0);

    State = TX_STATUS;
    currentTime = timerGetId();
}


uint8_t ProcessSX1276(void)
{
    switch(State)
    {
        case TX_STATUS:
                Radio.Send( Buffer, BufferSize );
                State = LOWPOWER;
                return 0;
            break;

        case TX_SX1276:
                Radio.Rx( 0 );
                State = LOWPOWER;
                return 1;
            break;

        case RX_SX1276:
            return 2;
            break;

        case LOWPOWER:
            if(timerIsEnd(currentTime,2000)) // 2s rx timeout
            {
                return 255;
            }
            else
            {
                return 1;
            }

            break;

        default:
            break;
    }
}

void OnTxDone( void )
{
    Radio.Sleep( );
    State = TX_SX1276;
    // DEBUG("tx done");
}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    Radio.Sleep( );
    BufferSize = size;
    memcpy( Buffer, payload, BufferSize );
    State = RX_SX1276;

    RssiValue = rssi;
    SnrValue = snr;

    #if 0
    DEBUG("rx done");
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

void OnTxTimeout( void )
{
    Radio.Sleep( );
    State = TX_TIMEOUT;
    // DEBUG("tx timeout");
}

void OnRxTimeout( void )
{
    Radio.Sleep( );
    State = RX_TIMEOUT;
    // DEBUG("rx timeout");
}

void OnRxError( void )
{
    Radio.Sleep( );
    State = RX_ERROR;
    // DEBUG("rx error");
}


uint32_t currTime = 0;

bool SX1276Test(int8_t &snr, int8_t &rssi, int8_t &txRssi)
{
    TestSX1276Init();
    if(sx1278Version != 18)
    {
        return false;
    }
    else
    {
        currTime = timerGetId();
        while(1)
        {
            switch(ProcessSX1276())
            {
                case 0:
                    if(timerIsEnd(currTime,2000)) // tx timeout
                    {
                        return false;
                    }
                    break;

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
                    return false; // rx timeout
                    break;
            }
        }
    }
}

#else

bool SX1276Test(int8_t &snr, int8_t &rssi, int8_t &txRssi)
{
    return false;
}
#endif

