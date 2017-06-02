#if   0
//lora透传测试 收发转换
SerialDebugOutput debugOutput(115200, ALL_LEVEL);

#include "wiring_ex_lorawan.h"
#include "Adafruit_SSD1306.h"

// #define MASTER_SX1276
#define OLED_DISPLAY


#define LED           D6

#define OLED_RESET  A3
#define OLED_DC     A4
#define OLED_CS     A5

uint32_t currentTime;
uint32_t txPacketCnt;
uint32_t rxPacketCnt;
uint32_t errorPacketCnt;


Adafruit_SSD1306 display(OLED_RESET);  // Hareware I2C

// void OLEDDisplay(const char *itemName, const char *result)
void OLEDDisplay(const char *result)
{
     display.clearDisplay();
     display.setTextSize(1);
     display.setCursor(0,0);
     // display.setCursor(row,column);
     // display.println(itemName);
     // display.setTextSize(1);
     // display.setCursor(0,32);
     // display.setCursor(row,column);
     display.println(result);
     display.display();
}

void RFParameterDispaly(int8_t snr ,int8_t rssi)
{
    String p;
    char tmp[16];
    p = p+"snr:";
    sprintf(tmp,"%d",snr);
    p = p+tmp;
    p = p+ "\r\n";
    p = p + "rssi:";
    sprintf(tmp,"%d",rssi);
    p = p+tmp;

    // p = p+ "\r\n";
    // p = p + "tx packets:";
    // sprintf(tmp,"%d",txPacketCnt);
    // p = p+tmp;

    p = p+ "\r\n";
    p = p + "error packets:";
    sprintf(tmp,"%d",errorPacketCnt);
    p = p+tmp;


    // p = p+ "\r\n";
    // p = p + "rx packets:";
    // sprintf(tmp,"%d",rxPacketCnt);

    // p = p + "tx rssi:";
    // sprintf(tmp,"%d",txRssiVal);

    // p = p+tmp;
    // OLEDDisplay("RX",p);
    OLEDDisplay(p);
}

void OLEDInit(void)
{
    display.begin(SSD1306_SWITCHCAPVCC,0x78>>1);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.clearDisplay();
    display.println("SX1276 Test");
    display.display();
}


#define USE_BAND_433
#define USE_MODEM_LORA

// #define RF_FREQUENCY                                434665000 // Hz
// #define RF_FREQUENCY                                   470000000 // Hz
#define RF_FREQUENCY                                433175000 // Hz

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
#define LORA_IQ_INVERSION_ON                       false


typedef enum
{
    LOWPOWER,
    RX_SX1276,
    RX_TIMEOUT,
    RX_ERROR,
    TX_SX1276,
    TX_TIMEOUT,
    TX_START
}States_t;

#define RX_TIMEOUT_VALUE    1000
#define BUFFER_SIZE         64 // Define the payload size here

uint16_t BufferSize = BUFFER_SIZE;

States_t State = LOWPOWER;

int8_t RssiValue = 0;
int8_t SnrValue = 0;

uint8_t Buffer[BUFFER_SIZE] = {1,2,3,4,5,6,7,8};

/*!
 * Radio events function pointer
 */
static RadioEvents_t RadioEvents;

/*!
 * \brief Function to be executed on Radio Tx Done event
 */
void OnTxDone( void );

/*!
 * \brief Function to be executed on Radio Rx Done event
 */
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );

/*!
 * \brief Function executed on Radio Tx Timeout event
 */
void OnTxTimeout( void );

/*!
 * \brief Function executed on Radio Rx Timeout event
 */
void OnRxTimeout( void );

/*!
 * \brief Function executed on Radio Rx Error event
 */
void OnRxError( void );

TimerEvent_t RxWindowTimer1;

void OnRxWindow1TimerEvent()
{
    TimerStop( &RxWindowTimer1 );

    if( Radio.GetStatus( ) == RF_IDLE )
        {
            Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                               LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                               LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                               0, true, 0, 0, LORA_IQ_INVERSION_ON, 0 );

            Radio.Rx( 3000 );
            // delay(1);
            // for(uint8_t i=1; i<0x4f;i++)
            //     {
            //         DEBUG("reg address = %x",i);
            //         DEBUG("reg dat = %x",SX1276Read(i));
            //     }
            DEBUG("startup rx");
        }
}

void setup()
{
    #ifdef OLED_DISPLAY
    OLEDInit();
    #endif

    pinMode(LED,OUTPUT);

    delay(100);
    DEBUG("***********sx1278-lora test******************");
    DEBUG("system clock = %d",SystemCoreClock);

    // Target board initialisation
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
    // Change LoRa modem SyncWord
    // Radio.Write( REG_LR_SYNCWORD, 0x34 );

    DEBUG("sx1278 version = 0x%2x", SX1276GetVersion());
    DEBUG("sx1278 freq = %d",SX1276LoRaGetRFFrequency());

    Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                       LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                       LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                       true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );


    Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                       LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                       LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                       0, true, 0, 0, LORA_IQ_INVERSION_ON, 1 );

    TimerInit( &RxWindowTimer1, OnRxWindow1TimerEvent );
    TimerSetValue( &RxWindowTimer1, 1000 ); // 1s后调用OnRxWindow1TimerEvent
    // TimerStart( &RxWindowTimer1 );

    // Radio.Rx( RX_TIMEOUT_VALUE );


    DEBUG("sync data = 0x%2x",SX1276Read(0x39));

    DEBUG("sx1278 init success ");
    // for(uint8_t i = 0x01; i < 0x40; i++)
    // {
    //    DEBUG("register address = %x",i);
    //    DEBUG("register data = %x",SX1276Read(i));
    // }

    delay(100);
    #ifdef MASTER_SX1276
    State = TX_START;
    DEBUG("SX1276 Start TX");
    #else
    Radio.Rx( 0 );
    DEBUG("SX1276 Start RX");
    #endif

}


void loop()
{
    #ifdef MASTER_SX1276
    switch(State)
    {
    case TX_START:
        DEBUG("master start tx");
        // OLEDDisplay("Start TX..","");
        Radio.Send( Buffer, BufferSize );
        txPacketCnt++;
        State = LOWPOWER;
        break;

    case TX_SX1276:
        DEBUG("master tx done");
        Radio.Rx( 3000 );
        State = LOWPOWER;
        break;

    case RX_SX1276:
        DEBUG("master rx done");
        rxPacketCnt++;
        #ifdef OLED_DISPLAY
        RFParameterDispaly(SnrValue,RssiValue);
        #endif
        delay(1000);
        State = TX_START;
        break;

    case TX_TIMEOUT:
        DEBUG("master tx timeout");
        // State = TX_START;
        Radio.Rx( 3000 );
        State = LOWPOWER;
        break;

    case RX_TIMEOUT:
    case RX_ERROR:
        DEBUG("master rx timeout");
        State = TX_START;
        break;

    case LOWPOWER:
        break;

    default:
        break;
    }
    #else
    //接收方
    switch(State)
    {
        case TX_SX1276:
            DEBUG("slave tx done");
            Radio.Rx( 0 );
            State = LOWPOWER;
            break;

        case RX_SX1276:
            DEBUG("slave rx done");
            #ifdef OLED_DISPLAY
            RFParameterDispaly(SnrValue,RssiValue);
            #endif

            Radio.Send( Buffer, BufferSize );
            State = LOWPOWER;
            break;

        case RX_ERROR:
        case RX_TIMEOUT:
            DEBUG("slave rx timeout");
            Radio.Rx( 0 );
            State = LOWPOWER;
            break;

        case TX_TIMEOUT:
            DEBUG("salve tx timeout");
            Radio.Rx( 0 );
            State = LOWPOWER;
            break;

        case LOWPOWER:
            break;

        default:
            break;
    }

    #endif
}

static bool ledFlag = false;

void OnTxDone( void )
{
    Radio.Sleep( );
    State = TX_SX1276;
    DEBUG("tx done");

#ifdef OLED_DISPLAY
    // OLEDDisplay("DONE!!!");
#endif

    ledFlag = !ledFlag;
    digitalWrite(LED,ledFlag);
}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    Radio.Sleep( );
    BufferSize = size;
    memcpy( Buffer, payload, BufferSize );
    RssiValue = rssi;
    DEBUG("rssi = %d",RssiValue);
    SnrValue = snr;
    DEBUG("snr = %d",SnrValue);
    State = RX_SX1276;

    DEBUG("rx done");

    ledFlag = !ledFlag;
    digitalWrite(LED,ledFlag);

    for(uint8_t i=0; i<BufferSize;i++)
    {
        if(Buffer[i] != 0)
        {
            DEBUG("receive data = %d",Buffer[i]);
        }
    }

    // Radio.Rx( 0 );
    // TimerStart( &RxWindowTimer1 );
}

void OnTxTimeout( void )
{
    Radio.Sleep( );
    State = TX_TIMEOUT;
    DEBUG("tx timeout");
}

void OnRxTimeout( void )
{
    Radio.Sleep( );
    State = RX_TIMEOUT;
    DEBUG("rx timeout");
    // Radio.Rx( 0 );
    // TimerStart( &RxWindowTimer1 );
}

void OnRxError( void )
{
    Radio.Sleep( );
    State = RX_ERROR;
    errorPacketCnt++;
    DEBUG("rx error");
    // Radio.Rx( 0 );
    // TimerStart( &RxWindowTimer1 );
}

#endif

#if  0
//lora　一方只发　一方只收  透传测试

#if (PLATFORM_ID == PLATFORM_L6) || (PLATFORM_ID == PLATFORM_ANT)

SerialDebugOutput debugOutput(115200, ALL_LEVEL);

#include "wiring_ex_lorawan.h"
#define LED           D6
#define RED_LED       A0
#define GRREN_LED     A1
#define BLUE_LED      A2

#define OLED_RESET  A3
#define OLED_DC     A4
#define OLED_CS     A5

#elif PLATFORM_ID == PLATFORM_ANYTEST
//anytest board Serial: A7-TX A8-RX
SerialDebugOutput debugOutput(115200, ALL_LEVEL);
#include "lora.h" //用于测试板

#define OLED_RESET  D16
#define OLED_DC     D17
#define OLED_CS     D21

#define LED          A15
#define RED_LED       A0
#define GRREN_LED     A1
#define BLUE_LED      A2

#endif

#define USE_BAND_433
#define USE_MODEM_LORA

// #define SX1276_TX_EN
// #define OLED_DISPLAY

// #define RF_FREQUENCY                                434665000 // Hz
#define RF_FREQUENCY                                   470000000 // Hz
// #define RF_FREQUENCY                                433175000 // Hz

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
#define LORA_IQ_INVERSION_ON                       false//true


typedef enum
{
    LOWPOWER,
    RX_SX1276,
    RX_TIMEOUT,
    RX_ERROR,
    TX_SX1276,
    TX_TIMEOUT,
}States_t;

#define RX_TIMEOUT_VALUE    1000
#define BUFFER_SIZE         4 // Define the payload size here

const uint8_t PingMsg[] = "PING";
const uint8_t PongMsg[] = "PONG";

uint16_t BufferSize = BUFFER_SIZE;

States_t State = LOWPOWER;

int8_t RssiValue = 0;
int8_t SnrValue = 0;

uint8_t Buffer[BUFFER_SIZE] = {1,2,3,4,};//5,6,7,8};

/*!
 * Radio events function pointer
 */
static RadioEvents_t RadioEvents;

/*!
 * \brief Function to be executed on Radio Tx Done event
 */
void OnTxDone( void );

/*!
 * \brief Function to be executed on Radio Rx Done event
 */
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );

/*!
 * \brief Function executed on Radio Tx Timeout event
 */
void OnTxTimeout( void );

/*!
 * \brief Function executed on Radio Rx Timeout event
 */
void OnRxTimeout( void );

/*!
 * \brief Function executed on Radio Rx Error event
 */
void OnRxError( void );

#include "Adafruit_SSD1306.h"

#if PLATFORM_ID == PLATFORM_ANYTEST
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);  // Hareware SPI
#else
Adafruit_SSD1306 display(OLED_RESET);  // Hareware I2C
#endif


void OLEDDisplay(const char *itemName, const char *result)
{
     display.clearDisplay();
     display.setTextSize(2);
     display.setCursor(0,0);
     // display.setCursor(row,column);
     display.println(itemName);
     display.setTextSize(1);
     display.setCursor(0,32);
     // display.setCursor(row,column);
     display.println(result);
     display.display();
}

void RFParameterDispaly(int8_t snr ,int8_t rssi)
{
    String p;
    char tmp[16];

    p = p+"snr:";
    sprintf(tmp,"%d",snr);
    p = p+tmp;
    p = p+ "\r\n";
    p = p + "rssi:";
    sprintf(tmp,"%d",rssi);
    p = p+tmp;
    p = p+ "\r\n";

    // p = p + "tx rssi:";
    // sprintf(tmp,"%d",txRssiVal);
    p = p + "data:";
    for(uint8_t i=0; i<4; i++)
    {
        sprintf(tmp,"%d",Buffer[i]);
        p = p+tmp;
        p = p+ " ";
    }

    OLEDDisplay("RX",p);
}

void OLEDInit(void)
{
    #if PLATFORM_ID == PLATFORM_ANYTEST
    display.begin(SSD1306_SWITCHCAPVCC);
    #else
    display.begin(SSD1306_SWITCHCAPVCC,0x78>>1);
    #endif
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.clearDisplay();
    display.println("SX1276 Test");
    display.display();
}

static uint8_t regDat[2];
static uint8_t len;

void ReadSX1276RegData(void)
{
    while(Serial.available())
    {
        DEBUG("****************lora register data***************");
        regDat[len] = (unsigned char)Serial.read();
        if(++len == 2)
        {
            len = 0;
            if((regDat[0] > 0x70) || (regDat[0] < 0x1) )
            {
                DEBUG("register address error");
            }
            else
            {
                // regDat[0] =  HexToAsc(regDat[0]);
                // regDat[1] =  HexToAsc(regDat[1]);
                for(uint8_t i = 0; i < regDat[1]; i++)
                {
                    DEBUG("register address = %x",regDat[0]+i);
                    DEBUG("register data = %x",SX1276Read(regDat[0]+i));
                }
                len = 0;
            }
        }
        DEBUG("**************************************");
    }
}

TimerEvent_t RxWindowTimer1;

void OnRxWindow1TimerEvent()
{
    TimerStop( &RxWindowTimer1 );

    if( Radio.GetStatus( ) == RF_IDLE )
        {
            Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                               LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                               LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                               0, true, 0, 0, LORA_IQ_INVERSION_ON, 0 );

            Radio.Rx( 3000 );
            // delay(1);
            // for(uint8_t i=1; i<0x4f;i++)
            //     {
            //         DEBUG("reg address = %x",i);
            //         DEBUG("reg dat = %x",SX1276Read(i));
            //     }
            DEBUG("startup rx windows");
        }
}

void setup()
{
    #ifdef OLED_DISPLAY
    OLEDInit();
    #endif
    pinMode(LED,OUTPUT);
    pinMode(RED_LED,OUTPUT);
    pinMode(GRREN_LED,OUTPUT);
    pinMode(BLUE_LED,OUTPUT);
    digitalWrite(RED_LED,1);
    digitalWrite(GRREN_LED,1);
    digitalWrite(BLUE_LED,1);

    DEBUG("lora board test");
    delay(100);
    DEBUG("***********sx1278-lora test******************");
    DEBUG("system clock = %d",SystemCoreClock);

    // Target board initialisation
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
    // Change LoRa modem SyncWord
    Radio.Write( REG_LR_SYNCWORD, 0x34 );

    DEBUG("sx1278 version = %d", SX1276GetVersion());
    DEBUG("sx1278 freq = %d",SX1276LoRaGetRFFrequency());

    Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                       LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                       LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                       true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );

    DEBUG("tx config success");

    Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                       LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                       LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                       0, true, 0, 0, LORA_IQ_INVERSION_ON, 1 );

    // Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
    //                    LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
    //                    LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
    //                    0, false, 0, 0, false, false );

    TimerInit( &RxWindowTimer1, OnRxWindow1TimerEvent );
    TimerSetValue( &RxWindowTimer1, 1000 ); // 1s后调用OnRxWindow1TimerEvent
    // TimerStart( &RxWindowTimer1 );

    // DEBUG("rx config success");

    // Radio.Rx( RX_TIMEOUT_VALUE );

    Radio.Rx( 0);

    // Radio.Write( 0x01, 0x8d );

    DEBUG("sync data = %x",SX1276Read(0x39));
    DEBUG("sx1278 init success ");
//    for(uint8_t i = 0x01; i <= 0x40; i++)
//         {
//             DEBUG("register address = %x",i);
//             DEBUG("register data = %x",SX1276Read(i));
//         }
//     while(1);
}




void loop()
{
    // ReadSX1276RegData();

#ifdef  SX1276_TX_EN

    #ifdef OLED_DISPLAY
    OLEDDisplay("Start TX..","");
    #endif

    Radio.Send( Buffer, BufferSize );

    if(Buffer[3] < 255)
    {
        Buffer[0]++;
        Buffer[1]++;
        Buffer[2]++;
        Buffer[3]++;
    }
    else
    {
        Buffer[0] = 1;
        Buffer[1] = 2;
        Buffer[2] = 3;
        Buffer[3] = 4;
    }
    delay(1000);
#endif

#if  0

    // Radio.Rx( RX_TIMEOUT_VALUE );
    Radio.Rx( 0 );
    delay(500);
    digitalWrite(D7,0);   //从机一直接收

#endif
}

static bool ledFlag = false;

void OnTxDone( void )
{
    Radio.Sleep( );
    State = TX_SX1276;
    DEBUG("tx done");

#ifdef OLED_DISPLAY
    OLEDDisplay("TX","DONE!!!");
#endif
    ledFlag = !ledFlag;
    digitalWrite(LED,ledFlag);
}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    Radio.Sleep( );
    BufferSize = size;
    memcpy( Buffer, payload, BufferSize );
    RssiValue = rssi;

    DEBUG("rssi = %d",RssiValue);
    SnrValue = snr;
    DEBUG("snr = %d",SnrValue);
    State = RX_SX1276;

#ifdef OLED_DISPLAY
   RFParameterDispaly(SnrValue,RssiValue);
#endif

    for(uint8_t i=0; i<BUFFER_SIZE;i++)
    {
        DEBUG("reveive data = %d",Buffer[i]);
    }

    ledFlag = !ledFlag;
    digitalWrite(LED,ledFlag);

    // DEBUG("radio status = %d",Radio.GetStatus( ) );
    Radio.Rx( 0 );
    // TimerStart( &RxWindowTimer1 );
}

void OnTxTimeout( void )
{
    Radio.Sleep( );
    State = TX_TIMEOUT;
}

void OnRxTimeout( void )
{
    Radio.Sleep( );
    State = RX_TIMEOUT;
    Radio.Rx( 0 );
    DEBUG("rx timeout");
    // DEBUG("radio status = %d",Radio.GetStatus( ) );
    // TimerStart( &RxWindowTimer1 );
}

void OnRxError( void )
{
    Radio.Sleep( );
    State = RX_ERROR;
    Radio.Rx( 0 );

    DEBUG("rx error");
    // TimerStart( &RxWindowTimer1 );
    digitalWrite(RED_LED,0);
}

#endif


#if  0
//lora sx1276 基站 用于测试

#include "wiring_ex_lorawan.h"

SerialDebugOutput debugOutput(115200, ALL_LEVEL);

#define USE_BAND_433
#define USE_MODEM_LORA

#define RF_FREQUENCY                                434665000 // Hz

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
}States_t;

#define RX_TIMEOUT_VALUE    1000
#define BUFFER_SIZE         64 // Define the payload size here

static uint16_t BufferSize = BUFFER_SIZE;

static States_t State = LOWPOWER;

static int8_t RssiValue = 0;
static int8_t SnrValue = 0;

static RadioEvents_t RadioEvents;
static void OnTxDone( void );
static void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );
static void OnTxTimeout( void );
static void OnRxTimeout( void );
static void OnRxError( void );

static TimerEvent_t RxWindowTimer1;


#define LED          D7

void setup()
{
    // System.configEnd();

    pinMode(LED,OUTPUT);
    digitalWrite(LED,0);
    delay(100);
    DEBUG("***********SX1276 LoRa Station******************");
    DEBUG("system clock = %d",SystemCoreClock);

    // Target board initialisation
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

    DEBUG("sx1278 version = 0x%2x", SX1276GetVersion());
    DEBUG("sx1278 freq = %d",SX1276LoRaGetRFFrequency());
    DEBUG("sync data = 0x%2x",SX1276Read(0x39));

    Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                       LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                       LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                       true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );

    Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                       LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                       LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                       0, true, 0, 0, LORA_IQ_INVERSION_ON, 1 );

    // TimerInit( &RxWindowTimer1, OnRxWindow1TimerEvent );
    // TimerSetValue( &RxWindowTimer1, 1000 ); // 1s后调用OnRxWindow1TimerEvent
    // TimerStart( &RxWindowTimer1 );

    DEBUG("sx1278 init success ");

    Radio.Rx( 0);
    State = LOWPOWER;
    // System.configEnd();
}


static uint8_t Buffer[BUFFER_SIZE] = {0};
static int8_t Buffer1[BUFFER_SIZE] = {22,20,32,41,53,61,74,89};


void loop()
{
    switch(State)
    {
        case TX_SX1276:
                Radio.Rx( 0 );
                State = LOWPOWER;
            break;

        case RX_SX1276:
                DEBUG("sx1278 tx data");
                delay(500);
                Radio.Send( Buffer1, BufferSize );
                State = LOWPOWER;
            break;

         case RX_ERROR:
         case RX_TIMEOUT:
             Radio.Rx( 0 );
             DEBUG("1278 rx timeout");
             State = LOWPOWER;
             break;

         case TX_TIMEOUT:
             delay(500);
             Radio.Send( Buffer1, BufferSize );
             DEBUG("1278 tx again");
             // State = LOWPOWER;
             break;

         case LOWPOWER:
             break;

        default:
            break;
    }
}

static bool ledFlag = false;

void OnTxDone( void )
{
    Radio.Sleep( );
    State = TX_SX1276;

    DEBUG("tx done");
}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    Radio.Sleep( );
    BufferSize = size;
    memcpy( Buffer, payload, BufferSize );
    State = RX_SX1276;

    RssiValue = rssi;
    // DEBUG("rssi = %d",RssiValue);
    Buffer1[1] = RssiValue;
    SnrValue = snr;
    // DEBUG("snr = %d",SnrValue);
    DEBUG("rx done");

    ledFlag = !ledFlag;
    digitalWrite(LED,ledFlag);

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

    // Radio.Rx( 0 );
    // TimerStart( &RxWindowTimer1 );
}

void OnTxTimeout( void )
{
    Radio.Sleep( );
    State = TX_TIMEOUT;
    DEBUG("tx timeout");
}

void OnRxTimeout( void )
{
    Radio.Sleep( );
    State = RX_TIMEOUT;

    DEBUG("rx timeout");
    // DEBUG("radio status = %d",Radio.GetStatus( ) );
    // TimerStart( &RxWindowTimer1 );
}

void OnRxError( void )
{
    Radio.Sleep( );
    State = RX_ERROR;
    DEBUG("rx error");
    // TimerStart( &RxWindowTimer1 );
}

#endif
