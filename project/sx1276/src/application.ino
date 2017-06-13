#if  0
//lora透传测试 收发转换
SerialDebugOutput debugOutput(115200, ALL_LEVEL);

#include "wiring_ex_lorawan.h"
#include "Adafruit_SSD1306.h"

#define MASTER_SX1276
#define OLED_DISPLAY


#define LED           D6
#define LED1          D5

#define OLED_RESET  A3
#define OLED_DC     A4
#define OLED_CS     A5

uint32_t currentTime;
uint32_t txPacketCnt;
uint32_t rxPacketCnt;
uint32_t errorPacketCnt;


Adafruit_SSD1306 display(OLED_RESET);  // Hareware I2C

void OLEDDisplay(const char *itemName, const char *result)
{
     display.clearDisplay();
     display.setTextSize(1);
     display.setCursor(0,0);
     display.println(itemName);
     display.setTextSize(1);
     display.setCursor(0,16);
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

    #ifdef MASTER_SX1276
    OLEDDisplay("tx",p);
    #else
    OLEDDisplay("rx",p);
    #endif
}

void OLEDInit(void)
{
    display.begin(SSD1306_SWITCHCAPVCC,0x78>>1);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.clearDisplay();
    #ifdef MASTER_SX1276
    display.println("SX1278 TX");
    #else
    display.println("SX1278 RX");
    #endif
    display.display();
}


#define USE_BAND_433
#define USE_MODEM_LORA

// #define RF_FREQUENCY                                434665000 // Hz
// #define RF_FREQUENCY                                   470000000 // Hz
#define RF_FREQUENCY                               433175000 // Hz

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

#if 0
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
#endif

void setup()
{
    #ifdef OLED_DISPLAY
    OLEDInit();
    #endif

    pinMode(LED,OUTPUT);
    pinMode(LED1,OUTPUT);

    delay(100);
    DEBUG("********sx1278 test**********");
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
    DEBUG("sync word = 0x%2x",SX1276Read(0x39));

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

    // Radio.Rx( RX_TIMEOUT_VALUE );

    DEBUG("sx1278 init success ");
    #if 0
    //读取寄存器值
    for(uint8_t i = 0x01; i < 0x40; i++)
    {
       DEBUG("register address = %x",i);
       DEBUG("register data = %x",SX1276Read(i));
    }
    #endif

    delay(100);
    #ifdef MASTER_SX1276
    State = TX_START;
    DEBUG("SX1278 Start TX");
    #else
    Radio.Rx( 0 );
    DEBUG("SX1278 Start RX");
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
        delay(1000);
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
        State = TX_START;
        break;

    case TX_TIMEOUT:
        DEBUG("master tx timeout");
        State = TX_START;
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
            // DEBUG("slave rx done");
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

   // ledFlag = !ledFlag;
   // digitalWrite(LED,ledFlag);
}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    Radio.Sleep( );
    BufferSize = size;
    memcpy( Buffer, payload, BufferSize );
    RssiValue = rssi;
    SnrValue = snr;

    // DEBUG("snr = %d",SnrValue);
    // DEBUG("rssi = %d",RssiValue);
    State = RX_SX1276;

    DEBUG("rx done");

    ledFlag = !ledFlag;
    digitalWrite(LED,ledFlag);
    digitalWrite(LED1,ledFlag);

    #if 0
    for(uint8_t i=0; i<BufferSize;i++)
    {
        if(Buffer[i] != 0)
        {
            DEBUG("receive data = %d",Buffer[i]);
        }
    }
    #endif

    // Radio.Rx( 0 );
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
}

void OnRxError( void )
{
    Radio.Sleep( );
    State = RX_ERROR;
    errorPacketCnt++;
    DEBUG("rx error");
    // Radio.Rx( 0 );
}

#endif

#if 1 
//lora　一方只发　一方只收  透传测试

#include "wiring_ex_lorawan.h"
#include "Adafruit_SSD1306.h"

SerialDebugOutput debugOutput(115200, ALL_LEVEL);

// #define SX1276_TX_EN
#define OLED_DISPLAY


#define LED           D5
#define LED1          D6

#define OLED_RESET    A3
#define OLED_DC       A4
#define OLED_CS       A5

#define USE_BAND_433
#define USE_MODEM_LORA

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
}States_t;

#define RX_TIMEOUT_VALUE    1000
#define BUFFER_SIZE         4 // Define the payload size here

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


Adafruit_SSD1306 display(OLED_RESET);  // Hareware I2C

void OLEDDisplay(const char *itemName, const char *result)
{
     display.clearDisplay();
     display.setTextSize(1);
     display.setCursor(0,0);
     display.println(itemName);
     display.setTextSize(1);
     display.setCursor(0,32);
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
    display.begin(SSD1306_SWITCHCAPVCC,0x78>>1);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.clearDisplay();
    display.println("SX1278 test");
    display.display();
}

#if 0
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
        DEBUG("startup rx windows");
    }
}
#endif

void setup()
{
    #ifdef OLED_DISPLAY
    OLEDInit();
    delay(500);
    #ifdef SX1276_TX_EN
    OLEDDisplay("TX","");
    #else
    OLEDDisplay("RX","");
    #endif
    #endif
    pinMode(LED,OUTPUT);
    pinMode(LED1,OUTPUT);
    delay(100);
    DEBUG("***********sx1278 test******************");
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

    DEBUG("sx1278 version = %d", SX1276GetVersion());
    DEBUG("sx1278 freq = %d",SX1276LoRaGetRFFrequency());
    DEBUG("sync word = 0x%x",SX1276Read(0x39));

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

    // Radio.Rx( RX_TIMEOUT_VALUE );
    Radio.Rx( 0);

    DEBUG("sx1278 init ok!!! ");
    #if 0
    for(uint8_t i = 0x01; i <= 0x40; i++)
    {
        DEBUG("register address = %x",i);
        DEBUG("register data = %x",SX1276Read(i));
    }
    while(1);
    #endif
}




void loop()
{
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
    digitalWrite(LED1,ledFlag);
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

#ifdef OLED_DISPLAY
   RFParameterDispaly(SnrValue,RssiValue);
#endif

    for(uint8_t i=0; i<BUFFER_SIZE;i++)
    {
        DEBUG("reveive data = %d",Buffer[i]);
    }

    ledFlag = !ledFlag;
    digitalWrite(LED,ledFlag);
    digitalWrite(LED1,ledFlag);

    Radio.Rx( 0 );
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
    Radio.Rx( 0 );
    DEBUG("rx timeout");
}

void OnRxError( void )
{
    Radio.Sleep( );
    State = RX_ERROR;
    Radio.Rx( 0 );
    DEBUG("rx error");
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
