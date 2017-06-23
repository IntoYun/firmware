#ifndef  WIRING_EX_LORAWAN_H_
#define  WIRING_EX_LORAWAN_H_

#include "lorawan/radio/inc/radio.h"
#include "lorawan/board/inc/sx1276-board.h"
#include "lorawan/radio/inc/sx1276.h"
#include "lorawan/board/inc/timer.h"
#include "lorawan/mac/inc/LoRaMac.h"
#include "lorawan/board/inc/utilities.h"

typedef enum event_t{
    LORAWAN_EVENT_JOINING, //入网中
    LORAWAN_EVENT_JOINED, //已入网
    LORAWAN_EVENT_JOIN_FAIL, //入网失败
    LORAWAN_EVENT_TX_COMPLETE,//发送完成
    LORAWAN_EVENT_RX_COMPLETE, //接收完成
    LORAWAN_EVENT_MLME_JOIN,
    LORAWAN_EVENT_MLME_LINK_CHECK,
    LORAWAN_EVENT_MCPS_UNCONFIRMED,
    LORAWAN_EVENT_MCPS_CONFIRMED,
    LORAWAN_EVENT_MCPS_PROPRIETARY,
    LORAWAN_EVENT_MCPS_MULTICAST,
}lorawan_event_t;


typedef struct {
    bool available; //接收到数据
    uint16_t bufferSize; //接收的数据长度
    uint8_t buffer[256];
}lorawan_data_t;

typedef struct sLoRaWanParams{
    uint32_t devAddr;
    uint8_t  nwkSkey[16];
    uint8_t  appSkey[16];
    uint8_t  devEui[8];
    uint8_t  appEui[8];
    uint8_t  appKey[16];
}lorawan_params_t;



#define LORAWAN_DEFAULT_DATARATE      DR_5
#define LORAWAN_ADR_ON                1
#define LORAWAN_PUBLIC_NETWORK        true
#define LORAWAN_NETWORK_ID            ( uint32_t )0




bool SX1276Test(int8_t &snr, int8_t &rssi, int8_t &txRssi);

void LoRaWanInitialize(void);
void LoRaWanOTAAJoin(uint8_t *DevEui, uint8_t *AppEui, uint8_t *AppKey);
void LoRaWanABPJoin(uint32_t DevAddr, uint8_t *NwkSKey, uint8_t *AppSKey);
void LoRaWanGetABPParams(uint32_t &DevAddr, uint8_t *NwkSKey, uint8_t *AppSKey);
bool LoRaWanSendFrame(uint8_t *buffer, uint16_t len, bool IsTxConfirmed);
int LoRaWanReceiveFrame(uint8_t *buffer);

void LoRaRadioInitialize(void);


enum _band_width_t {
    BW125 = 0,
    BW250,
    BW500
};

enum _spreading_factor_t {
    SF12 = 12,
    SF11 = 11,
    SF10 = 10,
    SF9 = 9,
    SF8 = 8,
    SF7 = 7,
    SF6 = 6
};

enum _coderate_t{
    CR4_5 = 1,
    CR4_6,
    CR4_7,
    CR4_8
};

typedef void (*radioCb)(void);

class LoRaWanClass
{
    public:
    uint8_t _modem = 1;//模式 0 fask 1 lora
    uint32_t _bandwidth  = 0;//带宽fsk 2600-250000Hz lora:0-125KHz 1-250K 2-500K
    uint32_t _datarate   = 7;//速率fsk:600-300000　lora:扩频因子 6-12
    uint8_t _coderate    = 0;//纠错码率 fsk:0 lora: 1(4/5) 2(4/6) 3(4/7) 4(4/8) 仅lora用
    uint32_t _bandwidthAfc = 0;//fsk:2600-250000 lora:0 仅fsk用
    uint16_t _preambleLen  = 8;//前导码长
    uint16_t _symbTimeout  = 8;//单次接收超时
    bool _fixLen = false;//固定包长 true:固定　false:无效
    uint8_t _payloadLen = 0;//当fixLen设为1时，设置包长度
    bool _crcOn=true;//crc 校验 1:on 0:off
    bool _freqHopOn = false;//内部跳频 fsk无效,设为0 lora:1-on 0-off
    uint8_t _hopPeriod = 0;//跳频周期 单位symbols fsk:0 lora: number of symbols
    bool _iqInverted = false;// IQ位翻转 1-翻转 0-不翻转
    bool _rxContinuous = true;//接收是否连续 1-连续　0-不连续
    int8_t _power = 20; //发射功率
    uint32_t _fdev = 0;//频率偏差　仅fsk有效 fsk:Hz lora:0
    uint32_t _txTimeout = 3000;//发射超时时间 单位ms
    uint32_t _freq = 434000000;
    int8_t _snr;
    int16_t _rssi;

    static TimerEvent_t systemWakeupTimer;
    static radioCb wakeupCb = 0;

    public:
        //切换class 类型
        void loramacSetClassType(DeviceClass_t classType);
        //暂停loramac
        void loramacPause(void);

        //恢复loramac
        void loramacResume(void);

        //休眠
        void systemSleep(void);

        //唤醒后处理
        static void systemWakeupHandler(void);

       //唤醒设置 userHanler为用户处理唤醒之后的外设和IO等
        void setSystemWakeup(radioCb userHandler, uint32_t timeout); //单位s

        //sx1278休眠
        void radioSetSleep(void);

        //设置频率
        void radioSetFreq(uint32_t freq);
        uint32_t radioGetFreq(void);

        //设置模式 0:fsk 1:lora
        void radioSetModem(RadioModems_t modem);
        uint8_t radioGetModem(void);

        //设置带宽
        void radioSetBanwidth(uint32_t bandwidth);
        uint32_t radioGetBanwidth(void);

        //设置扩频因子
        void radioSetSF(uint32_t sf);
        uint8_t radioGetSF(void);

        //设置纠错编码率
        void radioSetCoderate(uint32_t coderate);
        uint8_t radioGetCoderate(void);

        //设置前导码超时
        void radioSetSymbTimeout(uint32_t symbTimeout);
        uint16_t radioGetSymbTimeout(void);

        //设置crc校验
        void radioSetCrcOn(bool crcOn);
        bool radioGetCrcOn(void);

        //设置前导码长度
        void radioSetPreambleLen(uint16_t preambleLen);
        uint16_t radioGetPreambleLen(void);

        //设置发送超时时间 单位ms
        void radioSetTxTimeout(uint32_t timeout);

        //接收设置
        void radioSetRxConfig(void);

        //发送设置
        void radioSetTxConfig(void);

        //开始发送
        void radioSend( uint8_t *buffer, uint8_t size );

        //开始接收
        void radioRx(uint32_t timeout);

        //读取寄存器值
        uint8_t radioRead(uint8_t addr);

        //写寄存器值
        void radioWrite(uint8_t addr, uint8_t data);
        //启动CAD
        void radioStartCad(void);
        //获取rssi
        int16_t radioReadRssi(void);
        //获取rssi
        int8_t radioReadSnr(void);

        //设置负载最大长度
        void radioSetMaxPayloadLength(uint8_t max);
        //设置同步字
        void radioSetSyncword(uint8_t syncword);

    private:
    // TimerEvent_t systemWakeupTimer;
};

extern LoRaWanClass LoRaWan;

#endif /* WIRING_LORAWAN_H_ */

