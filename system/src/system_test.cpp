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

#include "intorobot_config.h"
#ifdef configSETUP_ENABLE

#include "system_test.h"
#include "ajson.h"
#include "wiring.h"
#include "system_config.h"
#include "platforms.h"
#include "rtc_hal.h"

#if PLATFORM_ID == PLATFORM_ANT || PLATFORM_ID == PLATFORM_L6
#include "wiring_lorawan.h"
bool SX1278Test(int8_t &snr, int8_t &rssi, int8_t &txRssi);
#endif

/*debug switch*/
#define SYSTEM_TEST_DEBUG

#ifdef SYSTEM_TEST_DEBUG
#define STEST_DEBUG(...)    do {DEBUG(__VA_ARGS__);}while(0)
#define STEST_DEBUG_D(...)  do {DEBUG_D(__VA_ARGS__);}while(0)
#define STEST_DEBUG_DUMP    DEBUG_DUMP
#else
#define STEST_DEBUG(...)
#define STEST_DEBUG_D(...)
#define STEST_DEBUG_DUMP
#endif

#ifndef configNO_NETWORK
using namespace intorobot;
#endif

void testDigitalWrite(uint16_t pin, uint16_t value, void* cookie)
{
    aJsonClass aJson;

    ((DeviceConfig*)cookie)->sendComfirm(200);
#if PLATFORM_ID == PLATFORM_ATOM
#elif PLATFORM_ID == PLATFORM_NEUTRON
#elif PLATFORM_ID == PLATFORM_NUT
#elif PLATFORM_ID == PLATFORM_W67
    aJsonObject* root = aJson.createObject();
    char* strPtr = nullptr;

    if(pin == 0xff) {
        uint8_t pinNumber;
        for(pinNumber = 1; pinNumber <= TOTAL_DIGITAL_PINS; pinNumber++) {
            pinMode(pinNumber,OUTPUT);
            digitalWrite(pinNumber,value);
        }
    } else {
        pinMode(pin,OUTPUT);
        digitalWrite(pin,value);
    }
    aJson.addNumberToObject(root, "status", 200);
    strPtr = aJson.print(root);
    ((DeviceConfig*)cookie)->write((unsigned char *)strPtr, strlen(strPtr));
    free(strPtr);
    aJson.deleteItem(root);

#elif PLATFORM_ID == PLATFORM_FIG
    aJsonObject* root = aJson.createObject();
    char* strPtr = nullptr;

    if(pin == 0xff) {
        uint8_t pinNumber;
        for(pinNumber = D0; pinNumber <= D6; pinNumber++) {
            pinMode(pinNumber,OUTPUT);
            digitalWrite(pinNumber,value);
        }
        for(pinNumber = A3; pinNumber <= A9; pinNumber++) {
            pinMode(pinNumber,OUTPUT);
            digitalWrite(pinNumber,value);
        }
    } else {
        pinMode(pin,OUTPUT);
        digitalWrite(pin,value);
    }
    aJson.addNumberToObject(root, "status", 200);
    strPtr = aJson.print(root);
    ((DeviceConfig*)cookie)->write((unsigned char *)strPtr, strlen(strPtr));
    free(strPtr);
    aJson.deleteItem(root);

#elif PLATFORM_ID == PLATFORM_W323
    aJsonObject* root = aJson.createObject();
    char* strPtr = nullptr;
    uint8_t pinNumber;

    if(pin == 0xff) {
        uint8_t pinNumber;
        pinMode(0,OUTPUT);
        pinMode(2,OUTPUT);
        digitalWrite(0,value);
        digitalWrite(2,value);
        for(pinNumber = 4; pinNumber <= 35; pinNumber++) {
            pinMode(pinNumber,OUTPUT);
            digitalWrite(pinNumber,value);
        }
    } else {
        pinMode(pin,OUTPUT);
        digitalWrite(pin,value);
    }
    aJson.addNumberToObject(root, "status", 200);
    strPtr = aJson.print(root);
    ((DeviceConfig*)cookie)->write((unsigned char *)strPtr, strlen(strPtr));
    free(strPtr);
    aJson.deleteItem(root);

#elif PLATFORM_ID == PLATFORM_ANT
    aJsonObject* root = aJson.createObject();
    char* strPtr = nullptr;
    if(pin == 0xff) {
        uint8_t pinNumber;
        for(pinNumber = D0; pinNumber <= D4; pinNumber++) {
            pinMode(pinNumber,OUTPUT);
            digitalWrite(pinNumber,value);
        }
        for(pinNumber = A0; pinNumber <= A5; pinNumber++) {
            pinMode(pinNumber,OUTPUT);
            digitalWrite(pinNumber,value);
        }
    } else {
        pinMode(pin,OUTPUT);
        digitalWrite(pin,value);
    }

    aJson.addNumberToObject(root, "status", 200);
    strPtr = aJson.print(root);
    ((DeviceConfig*)cookie)->write((unsigned char *)strPtr, strlen(strPtr));
    free(strPtr);
    aJson.deleteItem(root);

#elif PLATFORM_ID == PLATFORM_L6
    aJsonObject* root = aJson.createObject();
    char* strPtr = nullptr;
    if(pin == 0xff) {
        uint8_t pinNumber;
        for(pinNumber = 0; pinNumber < 9; pinNumber++) {
            pinMode(pinNumber,OUTPUT);
            digitalWrite(pinNumber,value);
        }
        for(pinNumber = 11; pinNumber < 19; pinNumber++) {
            pinMode(pinNumber,OUTPUT);
            digitalWrite(pinNumber,value);
        }
    } else {
        pinMode(pin,OUTPUT);
        digitalWrite(pin,value);
    }

    aJson.addNumberToObject(root, "status", 200);
    strPtr = aJson.print(root);
    ((DeviceConfig*)cookie)->write((unsigned char *)strPtr, strlen(strPtr));
    free(strPtr);
    aJson.deleteItem(root);

#elif PLATFORM_ID == PLATFORM_FOX
    aJsonObject* root = aJson.createObject();
    char* strPtr = nullptr;
    if(pin == 0xff) {
        uint8_t pinNumber;
        for(pinNumber = 0; pinNumber < 5; pinNumber++) {
            pinMode(pinNumber,OUTPUT);
            digitalWrite(pinNumber,value);
        }
        pinMode(7,OUTPUT);
        digitalWrite(7,value);
        for(pinNumber = 30; pinNumber < 37; pinNumber++) {
            pinMode(pinNumber,OUTPUT);
            digitalWrite(pinNumber,value);
        }
    } else {
        pinMode(pin,OUTPUT);
        digitalWrite(pin,value);
    }

    aJson.addNumberToObject(root, "status", 200);
    strPtr = aJson.print(root);
    ((DeviceConfig*)cookie)->write((unsigned char *)strPtr, strlen(strPtr));
    free(strPtr);
    aJson.deleteItem(root);
#endif
}

void testAnalogRead(uint16_t pin, void* cookie)
{
    aJsonClass aJson;

    ((DeviceConfig*)cookie)->sendComfirm(200);
#if PLATFORM_ID == PLATFORM_ATOM
#elif PLATFORM_ID == PLATFORM_NEUTRON
#elif PLATFORM_ID == PLATFORM_W67 || PLATFORM_ID == PLATFORM_NUT
    aJsonObject* root = aJson.createObject();
    char* strPtr = nullptr;
    aJson.addNumberToObject(root, "status", 200);
    aJson.addNumberToObject(root, "value", analogRead(pin));
    strPtr = aJson.print(root);
    ((DeviceConfig*)cookie)->write((unsigned char *)strPtr, strlen(strPtr));
    free(strPtr);
    aJson.deleteItem(root);

#elif PLATFORM_ID == PLATFORM_FIG
#elif PLATFORM_ID == PLATFORM_W323
#elif PLATFORM_ID == PLATFORM_ANT
    aJsonObject* root = aJson.createObject();
    char* strPtr = nullptr;
    aJson.addNumberToObject(root, "status", 200);
    aJson.addNumberToObject(root, "value", (int)analogRead(pin));
    strPtr = aJson.print(root);
    ((DeviceConfig*)cookie)->write((unsigned char *)strPtr, strlen(strPtr));
    free(strPtr);
    aJson.deleteItem(root);

#elif PLATFORM_ID == PLATFORM_L6
    aJsonObject* root = aJson.createObject();
    char* strPtr = nullptr;
    aJson.addNumberToObject(root, "status", 200);
    aJson.addNumberToObject(root, "value", (int)analogRead(pin));
    strPtr = aJson.print(root);
    ((DeviceConfig*)cookie)->write((unsigned char *)strPtr, strlen(strPtr));
    free(strPtr);
    aJson.deleteItem(root);

#elif PLATFORM_ID == PLATFORM_FOX
    aJsonObject* root = aJson.createObject();
    char* strPtr = nullptr;
    aJson.addNumberToObject(root, "status", 200);
    aJson.addNumberToObject(root, "value", (int)analogRead(pin));
    strPtr = aJson.print(root);
    ((DeviceConfig*)cookie)->write((unsigned char *)strPtr, strlen(strPtr));
    free(strPtr);
    aJson.deleteItem(root);

#endif
}

void testSelfTest(void* cookie)
{
    aJsonClass aJson;

    ((DeviceConfig*)cookie)->sendComfirm(200);
#if PLATFORM_ID == PLATFORM_ATOM
#elif PLATFORM_ID == PLATFORM_NEUTRON
#elif PLATFORM_ID == PLATFORM_W67 || PLATFORM_ID == PLATFORM_NUT
    aJsonObject* root = aJson.createObject();
    char* strPtr = nullptr;
    aJson.addNumberToObject(root, "status", 200);
    strPtr = aJson.print(root);
    ((DeviceConfig*)cookie)->write((unsigned char *)strPtr, strlen(strPtr));
    aJson.deleteItem(root);
    free(strPtr);

#elif PLATFORM_ID == PLATFORM_FIG
#elif PLATFORM_ID == PLATFORM_W323
#elif PLATFORM_ID == PLATFORM_ANT
    aJsonObject* root = aJson.createObject();
    char* strPtr = nullptr;
    if(GetRTCSatus()) {
        aJson.addNumberToObject(root, "status", 200);
    } else {
        aJson.addNumberToObject(root, "status", 201);
    }
    strPtr = aJson.print(root);
    ((DeviceConfig*)cookie)->write((unsigned char *)strPtr, strlen(strPtr));
    free(strPtr);
    aJson.deleteItem(root);

#elif PLATFORM_ID == PLATFORM_L6
    aJsonObject* root = aJson.createObject();
    char* strPtr = nullptr;
    if(GetRTCSatus()) {
        aJson.addNumberToObject(root, "status", 200);
    } else {
        aJson.addNumberToObject(root, "status", 201);
    }
    strPtr = aJson.print(root);
    ((DeviceConfig*)cookie)->write((unsigned char *)strPtr, strlen(strPtr));
    free(strPtr);
    aJson.deleteItem(root);

#elif PLATFORM_ID == PLATFORM_FOX
    aJsonObject* root = aJson.createObject();
    char* strPtr = nullptr;
    if(GetRTCSatus()) {
        aJson.addNumberToObject(root, "status", 200);
    } else {
        aJson.addNumberToObject(root, "status", 201);
    }
    strPtr = aJson.print(root);
    ((DeviceConfig*)cookie)->write((unsigned char *)strPtr, strlen(strPtr));
    free(strPtr);
    aJson.deleteItem(root);

#endif
}

void testRfCheck(void* cookie)
{
    ((DeviceConfig*)cookie)->sendComfirm(200);
#if PLATFORM_ID == PLATFORM_ATOM || PLATFORM_ID == PLATFORM_NEUTRON || PLATFORM_ID == PLATFORM_NUT || PLATFORM_ID == PLATFORM_W67 || PLATFORM_ID == PLATFORM_FIG || PLATFORM_ID == PLATFORM_W323
    ((DeviceConfig*)cookie)->dealGetWifiList();

#elif (PLATFORM_ID == PLATFORM_ANT) || (PLATFORM_ID == PLATFORM_L6)
    aJsonClass aJson;
    aJsonObject* root = aJson.createObject();
    char* strPtr = nullptr;
    int8_t snrVal = 0;
    int8_t rssiVal = 0;
    int8_t txRssiVal = 0;

    if(SX1278Test(snrVal,rssiVal,txRssiVal)) {
        aJson.addNumberToObject(root, "status", 200);
    } else {
        aJson.addNumberToObject(root, "status", 201);
    }

    aJson.addNumberToObject(root, "snr", snrVal);
    aJson.addNumberToObject(root, "rssi", rssiVal);
    aJson.addNumberToObject(root, "txrssi", txRssiVal);

    strPtr = aJson.print(root);
    ((DeviceConfig*)cookie)->write((unsigned char *)strPtr, strlen(strPtr));
    free(strPtr);
    aJson.deleteItem(root);

#elif (PLATFORM_ID == PLATFORM_FOX)
    aJsonClass aJson;
    aJsonObject* root = aJson.createObject();
    char* strPtr = nullptr;

    CellularSignal sig;
    sig = Cellular.RSSI();

    aJson.addNumberToObject(root, "status", 200);
    aJson.addNumberToObject(root, "rssi", sig.rssi);

    strPtr = aJson.print(root);
    ((DeviceConfig*)cookie)->write((unsigned char *)strPtr, strlen(strPtr));
    free(strPtr);
    aJson.deleteItem(root);

#endif
}

void testSensorData(void* cookie)
{
    ((DeviceConfig*)cookie)->sendComfirm(200);
}

#if PLATFORM_ID == PLATFORM_ANT || PLATFORM_ID == PLATFORM_L6
//ant l6生产时板子测试代码
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

    sx1278Version = LoRa.radioReadReg(0x42);
    // DEBUG("sx1278 version = 0x%2x\r\n", sx1278Version);
    // DEBUG("sx1278 freq = %d\r\n",SX1276LoRaGetRFFrequency());
    // DEBUG("sync data = 0x%2x\r\n",LoRa.radioRead(0x39));

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
                        // DEBUG("snr = %d\r\n",SnrValue);
                        // DEBUG("rssi = %d\r\n",RssiValue);
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
    // DEBUG("tx done\r\n");
}

void TestOnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    LoRa.radioSetSleep();
    BufferSize = size;
    memcpy( Buffer, payload, BufferSize );
    State = RX_DONE;

    RssiValue = rssi;
    SnrValue = snr;
    // DEBUG("rx done\r\n");

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
    // DEBUG("tx timeout\r\n");
}

void TestOnRxTimeout( void )
{
    LoRa.radioSetSleep();
    State = RX_TIMEOUT;
    // DEBUG("rx timeout\r\n");
}

void TestOnRxError( void )
{
    LoRa.radioSetSleep();
    State = RX_ERROR;
    // DEBUG("rx error\r\n");
}
#endif
#endif
