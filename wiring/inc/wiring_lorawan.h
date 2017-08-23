#ifndef  WIRING_LORAWAN_H_
#define  WIRING_LORAWAN_H_

#ifndef configNO_LORAWAN

#include "radio/inc/radio.h"
#include "radio/inc/sx1276.h"
#include "board/inc/sx1276-board.h"
#include "board/inc/utilities.h"
#include "board/inc/timer.h"
#include "mac/inc/LoRaMac.h"
#include "mac/inc/LoRaMacTest.h"
#include "rtc_hal_lora.h"

#define LORAWAN_DEFAULT_DATARATE      DR_3
#define LORAWAN_ADR_ON                1
#define LORAWAN_PUBLIC_NETWORK        true
#define LORAWAN_NETWORK_ID            ( uint32_t )0

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
    bool available;      //是否接收到数据 true有效
    uint16_t bufferSize; //接收的数据长度
    uint8_t buffer[256]; //缓存大小
}lorawan_data_t;

typedef struct sLoRaWanParams{
    uint32_t devAddr;
    uint8_t  nwkSkey[16];
    uint8_t  appSkey[16];
    uint8_t  devEui[8];
    uint8_t  appEui[8];
    uint8_t  appKey[16];
}lorawan_params_t;

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


class LoRaWanClass
{
    public:
    //loramc
    lorawan_data_t macBuffer;
    lorawan_params_t macParams;
    DeviceClass_t _classType = CLASS_C; //class 类型
    uint8_t _port = 2; //端口号
    bool _adrEnable = false; //ADR使能
    uint8_t _confirmedFrameNbTrials = 3; //确认帧重发次数
    uint8_t _joinNbTrials = 3; //入网包重发次数
    uint8_t _demodMargin = 0;
    uint8_t _nbGateways = 0;
    uint8_t _uplinkDatarate = 0; //上行速率
    uint8_t _downlinkDatarate = 0; //下行速率
    uint8_t _txPower = 0; //发射功率
    bool _ackReceived = false; //是否收到ack
    uint8_t _framePending = 0;
    uint8_t _macSnr;
    int16_t _macRssi;
    bool _frameType = false; //帧类型 true确认帧

    public:
    //切换class 类型
    void setMacClassType(DeviceClass_t classType);
    //获取 class类型
    DeviceClass_t getMacClassType(void);
    //暂停loramac 不运行lorawan协议
    void macPause(void);
    //恢复loramac
    void macResume(void);
    //不固定频率
    void setMacFixedFreq(bool enabled);
    //不固定扩频因子
    void setMacFixedSF(bool enabled);
    //OTAA入网激活
    void joinOTAA(void);
    //ABP入网
    void joinABP(void);

    //设置deviceeui
    void setDeviceEUI(uint8_t *devEui);
    void getDeviceEUI(uint8_t *devEui);
    //设置appeui
    void setAppEUI(uint8_t *appEui);
    void getAppEUI(uint8_t *appEui);
    //设置appkey
    void setAppKey(uint8_t *appKey);
    //设置device addr
    void setDeviceAddr(uint32_t devAddr);//设置device addr
    uint32_t getDeviceAddr(void);
    //设置nwkSkey
    void setNwkSessionKey(uint8_t *nwkSkey);
    //设置appSkey
    void setAppSessionKey(uint8_t *appSkey);
    //设置端口号
    void setPort(uint8_t port);//设置端口号
    uint8_t getPort(void);
    //发送数据帧
    bool sendFrame(uint8_t *buffer,uint16_t len, bool IsTxConfirmed);//发送帧
    void sendConfirmedFrame(uint8_t *payload,uint16_t len);//发送确认帧
    void sendUnconfirmedFrame(uint8_t *payload,uint16_t len);//发送无需确认帧
    //接收数据帧 返回-1没有数据 否则返回数据长度
    int receiveFrame(uint8_t *buffer);

    //使能ADR自适应速率 true使能 false不使能
    void setADR(bool enabled);
    //设置入网重发次数 默认最大48次
    void setJoinNbTrials(uint8_t trials);
    //设置确认帧重发次数 1-8
    void setConfirmedFrameNbTrials(uint8_t trials);
    //设置无需确认帧重发次数 1-15
    void setUnconfirmedFrameNbTrials(uint8_t trials);
    //获取上行帧个数
    uint16_t  getUpLinkCounter(void);
    //获取下行帧个数
    uint16_t  getDownLinkCounter(void);
    //获取收到数据后的snr
    uint8_t getSnr(void);
    //获取收到数据后的rssi
    int16_t getRssi(void);
    //发送确认帧后是否收到了ack true收到
    bool getAckReceived(void);
    //获取发射功率
    uint8_t getTxPower(void);
    //设置帧类型 true确认帧 默认不确认
    void setFrameType(bool frameType);
    bool getFrameType(void);
    //获取激活状态 1-激活 0-激活中 -1-激活失败
    int8_t getActiveStatus(void);
    //true使能占空比,false关闭
    void setDutyCycleOn(bool enable);
    //增加通道 默认只有3个最多16个 起始id从3开始
    void addChannel(uint8_t id, uint32_t freq);
    //设置rx2默认参数datarate 范围0-5
    void setRx2DefaultChannel(uint32_t freq, uint8_t datarate);
    //设置rx2参数 datarate 范围0-5
    void setRx2Channel(uint32_t freq, uint8_t datarate);

    private:
};

class LoRaClass
{
    public:
    uint8_t _modem = 1;              //模式 0:fsk 1:lora
    uint32_t _bandwidth  = 0;        //带宽fsk:2600-250000Hz lora:0-125KHz 1-250K 2-500K
    uint32_t _datarate   = 7;        //速率fsk:600-300000　lora:扩频因子 6-12
    uint8_t _coderate    = 1;        //纠错码率 fsk:0 lora: 1(4/5) 2(4/6) 3(4/7) 4(4/8) 仅lora用
    uint32_t _bandwidthAfc = 0;      //fsk:2600-250000 lora:0 仅fsk用
    uint16_t _preambleLen  = 8;      //前导码长度
    uint16_t _symbTimeout  = 8;      //单次接收超时
    bool _fixLen = false;            //固定包长 true:固定　false:无效
    uint8_t _payloadLen = 0;         //当fixLen设为1时才有效,设置包长度
    bool _crcOn = true;              //crc 校验 1:on 0:off
    bool _freqHopOn = false;         //内部跳频 fsk无效,设为0 lora:1-on 0-off
    uint8_t _hopPeriod = 0;          //跳频周期 单位symbols fsk:0 lora: number of symbols
    bool _iqInverted = false;        //IQ位翻转 1:翻转 0:不翻转
    bool _rxContinuous = true;       //接收是否连续 1-连续　0-不连续
    int8_t _power = 20;              //发射功率
    uint32_t _fdev = 0;              //频率偏差　仅fsk有效 fsk:Hz lora:0
    uint32_t _txTimeout = 3000;      //发射超时时间 单位ms
    uint32_t _freq = 434000000;      //工作频率
    int8_t _snr;                     //接收完数据收snr值
    int16_t _rssi;                   //接收完数据收rssi值

    public:
    //sx1278休眠
    void radioSetSleep(void);
    //设置频率
    void radioSetFreq(uint32_t freq);
    //设置模式 0:fsk 1:lora
    void radioSetModem(RadioModems_t modem);
    //设置带宽
    void radioSetBandwidth(uint32_t bandwidth);
    //设置扩频因子
    void radioSetSF(uint32_t sf);
    //设置纠错编码率
    void radioSetCoderate(uint32_t coderate);
    //设置发射功率
    void radioSetTxPower(uint8_t txPower);
    //设置前导码超时
    void radioSetSymbTimeout(uint32_t symbTimeout);
    //设置crc校验
    void radioSetCrcOn(bool crcOn);
    //设置前导码长度
    void radioSetPreambleLen(uint16_t preambleLen);
    //设置发送超时时间 单位ms
    void radioSetTxTimeout(uint32_t timeout);
    //设置iq翻转
    void radioSetIqInverted(bool iqInverted);
    //设置接收模式
    void radioSetRxContinuous(bool rxContinuous);
    //设置负载最大长度
    void radioSetMaxPayloadLength(uint8_t max);
    //SF=6时包长度需固定
    void radioSetFixLen(bool fixLen);
    //SF=6时包长度需固定,设置包长度
    void radioSetFixPayloadLen(uint8_t payloadLen);
    //调频使能
    void radioSetFreqHopOn(bool enabled);
    //调频周期
    void radioSetHopPeriod(uint16_t period);
    //接收设置
    void radioSetRxConfig(void);
    //发送设置
    void radioSetTxConfig(void);
    //开始发送
    void radioSend( uint8_t *buffer, uint8_t size );
    //开始接收
    void radioRx(uint32_t timeout);

    //获取频率
    uint32_t radioGetFreq(void);
    //获取模式　0:fsk 1:lora
    uint8_t radioGetModem(void);
    //获取带宽
    uint32_t radioGetBandwidth(void);
    //获取扩频因子
    uint8_t radioGetSF(void);
    //获取纠错码率
    uint8_t radioGetCoderate(void);
    //获取发射功率
    uint8_t radioGetTxPower(void);
    //获取前导码超时值
    uint16_t radioGetSymbTimeout(void);
    //获取Iq是否翻转
    bool radioGetIqInverted(void);
    //获取接收模式值
    bool radioGetRxContinuous(void);
    //获取crc是否开启 0:关闭1:开启
    bool radioGetCrcOn(void);
    //获取前导码长度
    uint16_t radioGetPreambleLen(void);
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

    private:
};

extern LoRaWanClass LoRaWan;
extern LoRaClass LoRa;

bool SX1278Test(int8_t &snr, int8_t &rssi, int8_t &txRssi);

#endif
#endif /* WIRING_LORAWAN_H_ */

