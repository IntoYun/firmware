/*
   / _____)             _              | |
   ( (____  _____ ____ _| |_ _____  ____| |__
   \____ \| ___ |    (_   _) ___ |/ ___)  _ \
   _____) ) ____| | | || |_| ____( (___| | | |
   (______/|_____)_|_|_| \__)_____)\____)_| |_|
   (C)2013 Semtech

Description: SX1278 driver specific target board functions implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/

#include "lora_gateway_config.h"
#include "lora_gateway_log.h"
#include "radio.h"
#include "sx1278-board.h"

#define         ID1                                 ( 0x1FF80050 )
#define         ID2                                 ( 0x1FF80054 )
#define         ID3                                 ( 0x1FF80064 )

/*!
 * Flag used to set the RF switch control pins in low power mode when the radio is not active.
 */
static bool RadioIsActive = false;

/*!
 * Radio driver structure initialization
 */
const struct Radio_s Radio =
{
    SX1278Init,
    SX1278GetStatus,
    SX1278SetModem,
    SX1278SetChannel,
    SX1278IsChannelFree,
    SX1278Random,
    SX1278SetRxConfig,
    SX1278SetTxConfig,
    SX1278CheckRfFrequency,
    SX1278GetTimeOnAir,
    SX1278Send,
    SX1278SetSleep,
    SX1278SetStby,
    SX1278SetRx,
    SX1278StartCad,
    SX1278ReadRssi,
    SX1278Write,
    SX1278Read,
    SX1278WriteBuffer,
    SX1278ReadBuffer,
    SX1278SetMaxPayloadLength
};

/*!
 * Antenna switch GPIO pins objects
 */
void SX1278IoInit( void )
{
    //初始化管脚
    SX1278.Dio0  = lora_pins.dio[0];
    SX1278.Dio1  = lora_pins.dio[1];
    SX1278.Dio2  = lora_pins.dio[2];
    SX1278.Dio3  = lora_pins.dio[3];
    SX1278.Dio4  = lora_pins.dio[4];
    SX1278.Dio5  = lora_pins.dio[5];
    SX1278.Nss   = lora_pins.nss;
    SX1278.Reset = lora_pins.rst;
    SX1278.RxTx  = lora_pins.rxtx;

    ASSERT(SX1278.Nss != LORA_UNUSED_PIN);
    ASSERT(SX1278.Dio0 != LORA_UNUSED_PIN);
    ASSERT(SX1278.Dio1 != LORA_UNUSED_PIN || SX1278.Dio2 != LORA_UNUSED_PIN);

    pinMode(SX1278.Nss,OUTPUT);
    digitalWrite(SX1278.Nss,1);

    pinMode(SX1278.Dio0,INPUT_PULLUP);
    if (SX1278.Dio1 != LORA_UNUSED_PIN) {
        pinMode(SX1278.Dio1,INPUT_PULLUP);
    }
    if (SX1278.Dio2 != LORA_UNUSED_PIN) {
        pinMode(SX1278.Dio2,INPUT_PULLUP);
    }
    if (SX1278.Dio3 != LORA_UNUSED_PIN) {
        pinMode(SX1278.Dio3,INPUT_PULLUP);
    }
    if (SX1278.Dio4 != LORA_UNUSED_PIN) {
        pinMode(SX1278.Dio4,INPUT_PULLUP);
    }
    if (SX1278.Dio5 != LORA_UNUSED_PIN) {
        pinMode(SX1278.Dio5,INPUT_PULLUP);
    }
}

void SX1278IoIrqInit( DioIrqHandler **irqHandlers )
{
    /*
    attachInterrupt(SX1278.Dio0, irqHandlers[0], RISING, 1, 0);
    if (SX1278.Dio1 != LORA_UNUSED_PIN) {
        attachInterrupt(SX1278.Dio1, irqHandlers[1], RISING, 1, 0);
    }
    if (SX1278.Dio2 != LORA_UNUSED_PIN) {
        attachInterrupt(SX1278.Dio2, irqHandlers[2], RISING, 1, 0);
    }
    if (SX1278.Dio3 != LORA_UNUSED_PIN) {
        attachInterrupt(SX1278.Dio3, irqHandlers[3], RISING, 1, 0);
    }
    if (SX1278.Dio4 != LORA_UNUSED_PIN) {
        attachInterrupt(SX1278.Dio4, irqHandlers[4], RISING, 1, 0);
    }
    if (SX1278.Dio5 != LORA_UNUSED_PIN) {
        attachInterrupt(SX1278.Dio5, irqHandlers[5], RISING, 1, 0);
    }
    */
}

void SX1278IoDeInit( void )
{
    ASSERT(SX1278.Nss != LORA_UNUSED_PIN);
    ASSERT(SX1278.Dio0 != LORA_UNUSED_PIN);
    ASSERT(SX1278.Dio1 != LORA_UNUSED_PIN || SX1278.Dio2 != LORA_UNUSED_PIN);

    pinMode(SX1278.Nss,OUTPUT);
    digitalWrite(SX1278.Nss,1);

    pinMode(SX1278.Dio0,INPUT);
    if (SX1278.Dio1 != LORA_UNUSED_PIN) {
        pinMode(SX1278.Dio1,INPUT);
    }
    if (SX1278.Dio2 != LORA_UNUSED_PIN) {
        pinMode(SX1278.Dio2,INPUT);
    }
    if (SX1278.Dio3 != LORA_UNUSED_PIN) {
        pinMode(SX1278.Dio3,INPUT);
    }
    if (SX1278.Dio4 != LORA_UNUSED_PIN) {
        pinMode(SX1278.Dio4,INPUT);
    }
    if (SX1278.Dio5 != LORA_UNUSED_PIN) {
        pinMode(SX1278.Dio5,INPUT);
    }
}

static bool dio_states[NUM_DIO] = {0};

void SX1278IoCheck( void ){
    uint8_t i;
    for (i = 0; i < NUM_DIO; ++i) {
        if (lora_pins.dio[i] == LORA_UNUSED_PIN)
            continue;

        if (dio_states[i] != digitalRead(lora_pins.dio[i])) {
            dio_states[i] = !dio_states[i];
            if (dio_states[i]) {
                DioIrq[i]();
            }
        }
    }
}

uint8_t SX1278GetPaSelect( uint32_t channel )
{
    if( channel < RF_MID_BAND_THRESH )
    {
        return RF_PACONFIG_PASELECT_PABOOST;
    }
    else
    {
        return RF_PACONFIG_PASELECT_RFO;
    }
}

void SX1278SetAntSwLowPower( bool status )
{
    if( RadioIsActive != status )
    {
        RadioIsActive = status;

        if( status == false )
        {
            SX1278AntSwInit( );
        }
        else
        {
            SX1278AntSwDeInit( );
        }
    }
}

void SX1278AntSwInit( void )
{
    if (SX1278.RxTx == LORA_UNUSED_PIN) {
        return;
    }

    pinMode(SX1278.RxTx,OUTPUT);
    digitalWrite(SX1278.RxTx,1);
}

void SX1278AntSwDeInit( void )
{
    if (SX1278.RxTx == LORA_UNUSED_PIN) {
        return;
    }

    pinMode(SX1278.RxTx,INPUT);
}

void SX1278SetAntSw( uint8_t rxTx )
{
    if( rxTx != 0 ) // 1: TX, 0: RX
    {
        digitalWrite(SX1278.RxTx,0);
    }
    else
    {
        digitalWrite(SX1278.RxTx,1);
    }
}

bool SX1278CheckRfFrequency( uint32_t frequency )
{
    // Implement check. Currently all frequencies are supported
    return true;
}

void SX1278SetReset(void)
{
    if (SX1278.Reset == LORA_UNUSED_PIN) {
        return;
    }

    pinMode(SX1278.Reset,OUTPUT);
    digitalWrite(SX1278.Reset,0);
    delay(1);
    pinMode(SX1278.Reset,INPUT);
    delay(6);
}

void SpiSetNss(uint8_t val)
{
    digitalWrite(SX1278.Nss,val);
}

uint8_t SpiInOut( uint8_t outData )
{
    SPI.transfer(outData);
}

void SX1278BoardInit(void)
{
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV16);
    SPI.setDataMode(0);
    SPI.begin();
    SX1278IoInit();
}

void DelayMs(uint32_t ms)
{
    delay(ms);
}

uint8_t GetBoardPowerSource( void )
{
    return 0;
}

uint8_t BoardGetBatteryLevel(void)
{
    return 200;
}

uint32_t BoardGetRandomSeed( void )
{
    return ( ( *( uint32_t* )ID1 ) ^ ( *( uint32_t* )ID2 ) ^ ( *( uint32_t* )ID3 ) );
}

void BoardGetUniqueId( uint8_t *id )
{
    id[7] = ( ( *( uint32_t* )ID1 )+ ( *( uint32_t* )ID3 ) ) >> 24;
    id[6] = ( ( *( uint32_t* )ID1 )+ ( *( uint32_t* )ID3 ) ) >> 16;
    id[5] = ( ( *( uint32_t* )ID1 )+ ( *( uint32_t* )ID3 ) ) >> 8;
    id[4] = ( ( *( uint32_t* )ID1 )+ ( *( uint32_t* )ID3 ) );
    id[3] = ( ( *( uint32_t* )ID2 ) ) >> 24;
    id[2] = ( ( *( uint32_t* )ID2 ) ) >> 16;
    id[1] = ( ( *( uint32_t* )ID2 ) ) >> 8;
    id[0] = ( ( *( uint32_t* )ID2 ) );
}

