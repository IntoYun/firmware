#define ANYTEST_TEST_ALL_BOARD
// #define ANYTEST_TEST_L7_MODULE
// #define ANYTEST_TEST_SX1276

#ifdef ANYTEST_TEST_ALL_BOARD
// 测试板程序
#include "pinmap_hal.h"
#include "Adafruit_SSD1306.h"
#include "ajson.h"

#define OLED_RESET  D16
#define OLED_DC     D17
#define OLED_CS     D21

#define NONE_PIN  254

#define W323_TOTAL_TEST_PIN   34
#define FIG_TOTAL_TEST_PIN    40
#define W67_TOTAL_TEST_PIN    11
#define L6_TOTAL_TEST_PIN     19
#define ANT_TOTAL_TEST_PIN    36
#define FOX_TOTAL_TEST_PIN    37

//测试板与 被测板 引脚接法 前面是测试板引脚，后面是被测板引脚
/****W323
D1-GPIO21    A6-GPIO32
D4-GPIO22    A7-GPIO33
D5-GPIO19    A8-GPIO25
D6-GPIO23    A9-GPIO26
D7-GPIO18    A10-GPIO27
D8-GPIO5     A11-GPIO14
D9-GPIO17    A12-GPIO12
D11-GPIO16   A13-GPIO13
D12-GPIO4    A14-GPIO15
D13-GPIO0
D14-GPIO2
****/
const uint8_t W323PinMapping[W323_TOTAL_TEST_PIN] = {D13,0xff,D14,0xff,D12,D8,0xff,0xff,0xff,0xff,0xff,0xff,A12,A13,A11,A14,D11,D9,D7,D5,0xff,D1,D4,D6,0xff,A8,A9,A10,0xff,0xff,0xff,0xff,A6,A7};
const char *W323PinName[] = {"GPIO0","NONE","GPIO2","NONE","GPIO4","GPIO5","NONE","NONE","NONE","NONE","NONE","NONE","GPIO12","GPIO13","GPIO14","GPIO15","GPIO16","GPIO17","GPIO18","GPIO19","NONE","GPIO21","GPIO22","GPIO23","NONE","GPIO25","GPIO26","GPIO27","NONE","NONE","NONE","NONE","NONE","GPIO32","GPIO33"};

//测试板    Fig
//D4-D10接D0-D6
//A5-A11接A3-A9 一一对应
const uint8_t figPinMapping[FIG_TOTAL_TEST_PIN] = {D4,D5,D6,D7,D8,D9,D10,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,A5,A6,A7,A8,A9,A10,A11};
const char *figPinName[] = {"D0","D1","D2","D3","D4","D5","D6","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","A3","A4","A5","A6","A7","A8","A9"};

//w67 pin
//D4-GPIO5  A5-GPIO16
//D5-GPIO4  A6-GPIO14
//D6-GPIO0  A7-GPIO12
//D7-GPIO2  A8-GPIO13
//D8-GPIO15 *GPIO0 不读取
const uint8_t W67PinMapping[W67_TOTAL_TEST_PIN] = {0xff,0xff,D7,0xff,D5,D4,A7,A8,A6,D8,A5};
const char *W67PinName[] = {"NONE","NONE","GPIO2","NONE","GPIO4","GPIO5","GPIO12","GPIO13","GPIO14","GPIO15","GPIO16"};

//L6 PA2 PA3 is serial PA13 PA14 is SWDIO SWCLK
const uint8_t L6PinMapping[L6_TOTAL_TEST_PIN] = {A13,A14,A15,D15,D14,D13,D12,A2,A3,0xff,0xff,A6,A7,A8,A9,D11,D10,D9,D8};
const char *L6PinName[] = {"PB2","PB10","PB11","PB12","PB13","PB14","PB15","PA0","PA1","NONE","NONE","PA4","PA5","PA6","PA7","PA9","PA10","PA11","PA12"};

//ANT PA9(TXD) PA10(RXD) is serial PA13 PA14 做为烧录 SWDIO SWCLK
const uint8_t antPinMapping[ANT_TOTAL_TEST_PIN] = {D4,D5,D6,D7,D8,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,A2,A3,A4,A5,A6,A7};
const char *antPinName[] = {"D0","D1","D2","D3","D4","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","A0","A1","A2","A3","A4","A5"};

//FOX PA9(TXD) PA10(RXD) is serial PA13 PA14(D5 D6) 做为烧录 SWDIO SWCLK
const uint8_t foxPinMapping[FOX_TOTAL_TEST_PIN] = {D4,D5,D6,D7,D8,0xff,0xff,D11,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,A2,A3,A4,A5,A6,A7,A8};
const char *foxPinName[] = {"D0","D1","D2","D3","D4","NONE","NONE","D7","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","A0","A1","A2","A3","A4","A5","A6"};


Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);  // Hareware SPI

aJsonClass aJson;
uint8_t boardType;
uint8_t step;
bool testResult = true;
uint32_t currentTime;
uint8_t helloCnt;
uint8_t pinNo;
uint8_t errorType;
uint8_t errorPin;
int8_t snrVal = 0;
int8_t rssiVal = 0;
int8_t txRssiVal = 0;

enum ErrorType
{
    ERROR_PIN_HIGH = 0,
    ERROR_PIN_LOW,
    ERROR_ANALOG_READ,
    ERROR_SELF_TEST,
    ERROR_RF_CHECK,
    ERROR_SENSOR_DATA
};


enum BoardType
{
    ATOM = 1,
    NEUTRON,
    NUT,
    W67,
    FIG,
    W323,
    ANT, //L6+底板
    L6, //LORA 模块
    FOX,//GPRS模块
    UNKOWN_BOARD_TYPE = 0xfe,
    ERROR_COMMAND = 0xff
};

typedef enum TestItem
{
    TEST_DIGITAL_WRITE_HIGH = 0,
    TEST_DIGITAL_WRITE_LOW,
    TEST_ANALOG_READ,
    TEST_SELF_TEST,
    TEST_RF_CHECK,
    TEST_SENSOR_DATA
}testItem_t;

typedef enum TestStep
{
    STEP_GET_BOARD_TYPE = 0,
    STEP_CONFIRM_GET_BOARD_TYPE,
    STEP_DIGITAL_WRITE_HIGH,
    STEP_CONFIRM_DIGITAL_WRITE_HIGH_RESULT,
    STEP_DIGITAL_WRITE_LOW,
    STEP_CONFIRM_DIGITAL_WRITE_LOW_RESULT,

    STEP_SINGLE_DIGITAL_WRITE_HIGH,
    STEP_SINGLE_CONFIRM_DIGITAL_WRITE_HIGH_RESULT,
    STEP_SINGLE_DIGITAL_WRITE_LOW,
    STEP_SINGLE_CONFIRM_DIGITAL_WRITE_LOW_RESULT,

    STEP_ANALOG_READ,
    STEP_CONFIRM_ANALOG_READ_RESULT,
    STEP_SELF_TEST,
    STEP_CONFIRM_SELF_TEST_RESULT,
    STEP_RF_CHECK,
    STEP_CONFIRM_RF_CHECK_RESULT,
    STEP_SENSOR_DATA,
    STEP_CONFIRM_SENSOR_DATA_RESULT,
    STEP_TEST_END
}testStep_t;

String readString(void)
{
    return Serial1.readString();
}

int SerialAvailable(void)
{
    return Serial1.available();
}

void SerialPrint(const char *c)
{
    Serial1.println(c);
}

void queryBoard(void)
{
    aJsonObject* root = aJson.createObject();
    aJson.addStringToObject(root, "command", "hello");
    char *boardPtr = aJson.print(root);
    SerialPrint(boardPtr);
    free(boardPtr);
    aJson.deleteItem(root);
    currentTime = timerGetId();
}

uint8_t getBoardType(void)
{
    char *boardPtr = nullptr;
    if(timerIsEnd(currentTime,1000)) // 5s timeout
    {
        display.clearDisplay();
        display.setCursor(0,0);
        display.println("Board is not in the config mode");
        display.display();
        return ERROR_COMMAND;
    }

    while(SerialAvailable())
    {
            String tmp = readString();

            aJsonObject *root = NULL;
            root = aJson.parse((char *)tmp.c_str());
            if (root == NULL)
            {
                aJson.deleteItem(root);
                return ERROR_COMMAND;
            }

            aJsonObject* statusObject = aJson.getObjectItem(root, "status");
            if(statusObject == NULL)
            {
                aJson.deleteItem(root);
                return ERROR_COMMAND;
            }
            aJsonObject* versionObject = aJson.getObjectItem(root, "version");
            if(versionObject == NULL)
            {
                aJson.deleteItem(root);
                return ERROR_COMMAND;
            }
            aJsonObject* boardObject = aJson.getObjectItem(root, "board");
            if(boardObject == NULL)
            {
                aJson.deleteItem(root);
                return ERROR_COMMAND;
            }

            boardPtr = boardObject->valuestring;
            if(strcmp(boardPtr,"888004") == 0) {aJson.deleteItem(root);return ATOM;}         //atom
            else if(strcmp(boardPtr,"888002") == 0) {aJson.deleteItem(root);return NEUTRON;} //neutron
            else if(strcmp(boardPtr,"888003") == 0) {aJson.deleteItem(root);return NUT;}     //nut
            else if(strcmp(boardPtr,"888005") == 0) {aJson.deleteItem(root);return FIG;}     //fig
            else if(strcmp(boardPtr,"888006") == 0) {aJson.deleteItem(root);return W67;}     //w6/7
            else if(strcmp(boardPtr,"888007") == 0) {aJson.deleteItem(root);return W323;}    //w32/33
            else if(strcmp(boardPtr,"878008") == 0) {aJson.deleteItem(root);return FOX;}     //fox
            else if(strcmp(boardPtr,"868009") == 0) {aJson.deleteItem(root);return ANT;}     //ant
            else if(strcmp(boardPtr,"868010") == 0) {aJson.deleteItem(root);return L6;}      //l6
            else
            {
                boardType = UNKOWN_BOARD_TYPE;
                aJson.deleteItem(root);
                return UNKOWN_BOARD_TYPE;
            }

            aJson.deleteItem(root);
    }
}

void TestInit(void)
{
    display.begin(SSD1306_SWITCHCAPVCC);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.clearDisplay();
    display.println("check board type ...");
    display.display();
}

void OLEDDisplay(uint8_t textSize, const char *itemName, const char *result)
{
    display.clearDisplay();
    display.setTextSize(textSize);
    display.setCursor(0,0);
    // display.setCursor(row,column);
    display.println(itemName);
    display.setTextSize(1);
    display.setCursor(0,32);
    // display.setCursor(row,column);
    display.println(result);
    display.display();
}

void JudgeBoardType(void)
{
    switch(boardType)
        {
        case UNKOWN_BOARD_TYPE:
            step = STEP_TEST_END;
            break;

        case ERROR_COMMAND:
            step = STEP_GET_BOARD_TYPE;
            break;

        case ATOM:
            display.clearDisplay();
            display.setCursor(0,0);
            display.println("Atom Test");
            display.display();
            delay(500);
            step = STEP_DIGITAL_WRITE_HIGH;
            break;

        case NEUTRON:
            display.clearDisplay();
            display.setCursor(0,0);
            display.println("Neutron Test");
            display.display();
            delay(500);
            step = STEP_DIGITAL_WRITE_HIGH;
            break;

        case NUT:
            display.clearDisplay();
            display.setCursor(0,0);
            display.println("Nut Test");
            display.display();
            delay(500);
            step = STEP_DIGITAL_WRITE_HIGH;
            break;

        case W67:
            display.clearDisplay();
            display.setCursor(0,0);
            display.println("W6/7 Test");
            display.display();
            delay(500);
            step = STEP_DIGITAL_WRITE_HIGH;
            break;

        case FIG:
            display.clearDisplay();
            display.setCursor(0,0);
            display.println("Fig Test");
            display.display();
            delay(500);
            step = STEP_DIGITAL_WRITE_HIGH;
            break;

        case W323:
            display.clearDisplay();
            display.setCursor(0,0);
            display.println("W32/33 Test");
            display.display();
            delay(500);
            step = STEP_DIGITAL_WRITE_HIGH;
            break;

        case ANT:
            display.clearDisplay();
            display.setCursor(0,0);
            display.println("Ant Test");
            display.display();
            delay(500);
            step = STEP_DIGITAL_WRITE_HIGH;
            break;

        case L6:
            display.clearDisplay();
            display.setCursor(0,0);
            display.println("L6 Test");
            display.display();
            delay(500);
            step = STEP_DIGITAL_WRITE_HIGH;
            break;

        case FOX:
            display.clearDisplay();
            display.setCursor(0,0);
            display.println("Fox Test");
            display.display();
            delay(500);
            step = STEP_DIGITAL_WRITE_HIGH;
            break;


        default:
            break;
        }
}

bool JudgePinLevel(uint8_t pin,uint8_t val)
{
    if(val == 1)
    {
        if(digitalRead(pin) == 0)
        {
            errorPin = pin;
            errorType = ERROR_PIN_HIGH;
            return false;
        }
    }
    else
    {
        if(digitalRead(pin) == 1)
        {
            errorPin = pin;
            errorType = ERROR_PIN_LOW;
            return false;
        }
    }
    return true;
}

bool readAllPinLevel(uint8_t pinsCnt, const uint8_t *pinMap, uint8_t level)
{
    uint8_t pin;
    for(pin = 0;  pin < pinsCnt; pin++)
    {
        if(pinMap[pin] != 0xff)
        {
            if(digitalRead(pinMap[pin]) == level)
            {
                if(level == 1)
                    errorType = ERROR_PIN_LOW;
                else
                    errorType = ERROR_PIN_HIGH;
                errorPin = pinMap[pin];

                return false;
            }
        }
    }
    return true;
}

bool readSinglePinLevel(uint8_t singlePin, uint8_t pinsCnt, const uint8_t *pinMap,uint8_t level)
{
    uint8_t pin;
    if(digitalRead(pinMap[singlePin]) == level)
    {
        if(level == 1)
            errorType = ERROR_PIN_HIGH;
        else
            errorType = ERROR_PIN_LOW;
        errorPin = pinMap[singlePin];
        return false;
    }

    for(pin = pinsCnt-1; pin > singlePin; pin--)
    {
        if(pinMap[pin] != 0xff)
        {
            return JudgePinLevel(pinMap[pin],level);
        }
    }

    for(pin = 0; pin < singlePin;pin++)
    {
        if(pinMap[pin] != 0xff)
        {
            return JudgePinLevel(pinMap[pin],level);
        }
    }
    return true;
}



bool ReadBoardPinLevel(uint16_t gpio,uint8_t val)
{
    uint8_t pin;
    switch(boardType)
    {
        case ATOM:
            break;

        case NEUTRON:
            break;

        case NUT:
            break;

        case W67:
            for(pin = 0; pin < W67_TOTAL_TEST_PIN; pin++)
            {
                pinMode(W67PinMapping[pin],INPUT);
            }
            delay(1);
            if(gpio == 255)
            {
                return readAllPinLevel(W67_TOTAL_TEST_PIN,W67PinMapping,val);
            }
            else
            {
                return readSinglePinLevel(gpio,W67_TOTAL_TEST_PIN,W67PinMapping,val);
            }
            break;

        case FIG:
            for(pin = 0; pin < FIG_TOTAL_TEST_PIN; pin++)
            {
                pinMode(figPinMapping[pin],INPUT);
            }
            delay(1);
            if(gpio == 255)
            {
                return readAllPinLevel(FIG_TOTAL_TEST_PIN,figPinMapping,val);
            }
            else
            {
                return readSinglePinLevel(gpio,FIG_TOTAL_TEST_PIN,figPinMapping,val);
            }

            break;

        case W323:
            for(pin = 0; pin < W323_TOTAL_TEST_PIN; pin++)
            {
                pinMode(W323PinMapping[pin],INPUT);
            }
            delay(1);
            if(gpio == 255)
            {
                return readAllPinLevel(W323_TOTAL_TEST_PIN,W323PinMapping,val);
            }
            else
            {
                return readSinglePinLevel(gpio,W323_TOTAL_TEST_PIN,W323PinMapping,val);
            }
            break;

        case ANT:
            for(pin = 0;  pin < ANT_TOTAL_TEST_PIN; pin++)
            {
                pinMode(antPinMapping[pin],INPUT_PULLUP);
            }
            delay(1);
            if(gpio == 255)
            {
                return readAllPinLevel(ANT_TOTAL_TEST_PIN,antPinMapping,val);
            }
            else
            {
                return readSinglePinLevel(gpio,ANT_TOTAL_TEST_PIN,antPinMapping,val);
            }

            break;

        case L6:
            for(pin = 0;  pin < L6_TOTAL_TEST_PIN; pin++)
            {
                pinMode(L6PinMapping[pin],INPUT_PULLUP);
            }
            delay(1);
            if(gpio == 255)
            {
                return readAllPinLevel(L6_TOTAL_TEST_PIN,L6PinMapping,val);
            }
            else
            {
                return readSinglePinLevel(gpio,L6_TOTAL_TEST_PIN,L6PinMapping,val);
            }
            break;

        case FOX:
            for(pin = 0;  pin < FOX_TOTAL_TEST_PIN; pin++)
            {
                pinMode(foxPinMapping[pin],INPUT_PULLUP);
            }
            delay(1);
            if(gpio == 255)
            {
                return readAllPinLevel(FOX_TOTAL_TEST_PIN,foxPinMapping,val);
            }
            else
            {
                return readSinglePinLevel(gpio,FOX_TOTAL_TEST_PIN,foxPinMapping,val);
            }
            break;

        default:
            break;
    }
    return true;
}

bool ReceiveTestResult(testItem_t testItem, uint16_t gpio)
{
        while(SerialAvailable())
        {
            String tmp = readString();

            aJsonObject *root = NULL;
            root = aJson.parse((char *)tmp.c_str());
            if (root == NULL)
            {
                aJson.deleteItem(root);
                return false;
            }

            aJsonObject* statusObject = aJson.getObjectItem(root, "status");
            if(statusObject == NULL)
            {
                aJson.deleteItem(root);
                return false;
            }

            switch(testItem)
            {
                case TEST_DIGITAL_WRITE_HIGH:
                    if((statusObject->valueint == 200) && ReadBoardPinLevel(gpio,0))
                    {
                        OLEDDisplay(1,"digital high:","OK");
                    }
                    else
                    {
                        OLEDDisplay(1,"digital high:","NG");
                        testResult = false;
                    }
                    break;

                case TEST_DIGITAL_WRITE_LOW:
                    if((statusObject->valueint == 200) && ReadBoardPinLevel(gpio,1))
                    {
                        OLEDDisplay(1,"digital Low:","OK");
                    }
                    else
                    {
                        OLEDDisplay(1,"digital Low:","NG");
                        testResult = false;
                    }
                    break;

                case TEST_ANALOG_READ:
                    if(statusObject->valueint == 200)
                    {
                        aJsonObject* valueObject = aJson.getObjectItem(root, "value");
                        int16_t adVal = valueObject->valueint;
                        if((adVal >= 0) && (adVal <= 4095))
                        {
                            OLEDDisplay(1,"analogRead:","OK");
                        }
                        else
                        {
                            OLEDDisplay(1,"analogRead:","NG");
                            errorType = ERROR_ANALOG_READ;
                            testResult = false;
                        }
                    }
                    else
                    {
                        OLEDDisplay(1,"analogRead:","NG");
                        errorType = ERROR_ANALOG_READ;
                        testResult = false;
                    }

                    break;

                case TEST_SELF_TEST:
                    switch(boardType)
                    {
                        case ATOM:
                            break;

                        case NEUTRON:
                            break;

                        case W67:
                        case W323:
                        case FIG:
                            if(statusObject->valueint == 200)
                             {
                                 OLEDDisplay(1,"selfTest:","OK");
                             }
                            else
                            {
                                OLEDDisplay(1,"selfTest:","NG");
                                errorType = ERROR_SELF_TEST;
                                testResult = false;
                            }
                            break;

                        case ANT:
                        case L6:
                        case FOX:
                            if(statusObject->valueint == 200)
                            {
                                OLEDDisplay(1,"selfTest:","OK");
                            }
                            else
                            {
                                OLEDDisplay(1,"selfTest:","NG");
                                errorType = ERROR_SELF_TEST;
                                testResult = false;
                            }

                            break;

                        default:
                            break;
                    }
                    break;

                case TEST_RF_CHECK:
                if((boardType == ATOM) || (boardType == NEUTRON)||(boardType == NUT)||(boardType == W67)||(boardType == FIG)||(boardType == W323))
                {
                    if(statusObject->valueint == 200)
                    {
                        aJsonObject* listnumtObject = aJson.getObjectItem(root, "listnum");
                        if(listnumtObject == NULL)
                        {
                           aJson.deleteItem(root);
                           return false;
                        }

                        aJsonObject* ssidlistArrayObject = aJson.getObjectItem(root, "ssidlist");
                        if(ssidlistArrayObject == NULL)
                        {
                            aJson.deleteItem(root);
                            return false;
                        }

                        uint8_t arrayNumber = aJson.getArraySize(ssidlistArrayObject);

                        WiFiAccessPoint ap[20];

                        for(uint8_t num = 0; num < arrayNumber; num++)
                        {
                            aJsonObject* ssidObject = aJson.getArrayItem(ssidlistArrayObject,num);
                            if(ssidObject == NULL)
                            {
                                aJson.deleteItem(root);
                                return false;
                            }

                            aJsonObject* ssidNameObject = aJson.getObjectItem(ssidObject,"ssid");
                            if(ssidNameObject == NULL)
                            {
                                aJson.deleteItem(root);
                                return false;
                            }

                            strcpy(ap[num].ssid,ssidNameObject->valuestring);

                            aJsonObject* entypeObject = aJson.getObjectItem(ssidObject,"entype");
                            if(entypeObject == NULL)
                            {
                                aJson.deleteItem(root);
                                return false;
                            }
                            // ap[num].security = (WLanSecurityCipher)entypeObject->valueint;

                            aJsonObject* signalObject = aJson.getObjectItem(ssidObject,"signal");
                            if(signalObject == NULL)
                            {
                                aJson.deleteItem(root);
                                return false;
                            }
                            ap[num].rssi = signalObject->valueint;
                        }

                        bool assignWifi = false;
                        for(uint8_t i = 0; i < arrayNumber; i++)
                        {
                            if(strcmp(ap[i].ssid,"IntoRobot-NETGEAR") == 0)
                            {
                                assignWifi = true;
                                if(ap[i].rssi >= (-65))
                                {
                                    OLEDDisplay(1,"wifiCheck:","OK");
                                    break;
                                }
                                else
                                {
                                    OLEDDisplay(1,"wifiCheck:","NG");
                                    errorType = ERROR_RF_CHECK;
                                    testResult = false;
                                    break;
                                }
                            }
                        }

                        if(!assignWifi)
                        {
                            OLEDDisplay(1,"Not Open WiFi Network!!!","");
                            while(1);
                        }
                    }
                    else
                    {
                        OLEDDisplay(1,"wifiCheck:","NG");
                        errorType = ERROR_RF_CHECK;
                        testResult = false;
                    }
                }
                else if((boardType == ANT) || (boardType == L6))
                {
                    uint8_t statusVal = statusObject->valueint;
                    aJsonObject* snrObject = aJson.getObjectItem(root, "snr");
                    snrVal = snrObject->valueint;
                    aJsonObject* rssiObject = aJson.getObjectItem(root, "rssi");
                    rssiVal = rssiObject->valueint;
                    aJsonObject* txrssiObject = aJson.getObjectItem(root, "txrssi");
                    txRssiVal = txrssiObject->valueint;


                    if(statusVal == 200)
                    {
                        if((rssiVal > -30) && (txRssiVal > -30))
                        {
                            OLEDDisplay(1,"RFCheck:","OK");
                        }
                        else
                        {
                            OLEDDisplay(1,"RFCheck:","NG");
                            errorType = ERROR_RF_CHECK;
                            testResult = false;
                        }
                    }
                    else
                    {
                        OLEDDisplay(1,"RFCheck:","NG");
                        errorType = ERROR_RF_CHECK;
                        testResult = false;
                    }
                }
                else if(boardType == FOX)
                {
                    uint8_t statusVal = statusObject->valueint;
                    aJsonObject* rssiObject = aJson.getObjectItem(root, "rssi");
                    rssiVal = rssiObject->valueint;
                    if(statusVal == 200)
                    {
                        if(rssiVal > -30)
                        {
                            OLEDDisplay(1,"RFCheck:","OK");
                        }
                        else
                        {
                            OLEDDisplay(1,"RFCheck:","NG");
                            errorType = ERROR_RF_CHECK;
                            testResult = false;
                        }
                    }
                    else
                    {
                        OLEDDisplay(1,"RFCheck:","NG");
                        errorType = ERROR_RF_CHECK;
                        testResult = false;
                    }


                }
                break;

                case TEST_SENSOR_DATA:
                    break;

                default:
                    break;
            }

            aJson.deleteItem(root);
            return true;
        }
}

void SendTestCommand(testItem_t itemCommand, uint16_t gpio)
{
    aJsonObject* root = aJson.createObject();
    aJson.addStringToObject(root, "command", "test");
    aJsonObject* valueObject = aJson.createObject();
    aJson.addItemToObject(root, "value", valueObject);

    char* strPtr = nullptr;

    switch(itemCommand)
    {
        case TEST_DIGITAL_WRITE_HIGH:
            aJson.addStringToObject(valueObject, "item", "digitalWrite");
            aJson.addNumberToObject(valueObject, "pin", gpio);
            aJson.addStringToObject(valueObject, "val", "HIGH");
            strPtr = aJson.print(root);
            SerialPrint(strPtr);
            aJson.deleteItem(root);

            break;

        case TEST_DIGITAL_WRITE_LOW:
            aJson.addStringToObject(valueObject, "item", "digitalWrite");
            aJson.addNumberToObject(valueObject, "pin", gpio);
            aJson.addStringToObject(valueObject, "val", "LOW");
            strPtr = aJson.print(root);
            SerialPrint(strPtr);
            aJson.deleteItem(root);

            break;


        case TEST_ANALOG_READ:
            aJson.addStringToObject(valueObject, "item", "analogRead");
            aJson.addNumberToObject(valueObject, "pin", gpio);
            strPtr = aJson.print(root);
            SerialPrint(strPtr);
            aJson.deleteItem(root);

            break;

        case TEST_SELF_TEST:
            aJson.addStringToObject(valueObject, "item", "selfTest");
            strPtr = aJson.print(root);
            SerialPrint(strPtr);
            aJson.deleteItem(root);

            break;

        case TEST_RF_CHECK:
            aJson.addStringToObject(valueObject, "item", "rfCheck");
            strPtr = aJson.print(root);
            SerialPrint(strPtr);
            aJson.deleteItem(root);
            break;

        case TEST_SENSOR_DATA:
            aJson.addStringToObject(valueObject, "item", "sensorData");
            strPtr = aJson.print(root);
            SerialPrint(strPtr);
            aJson.deleteItem(root);

            break;
        default:
            break;
    }

    free(strPtr);
    aJson.deleteItem(root);
}

//testHighLevel true:test high false:test low
//testEnd   true:test end  flase: no
void FigPinLoopTestHandle(testItem_t itemCommand, bool testHighLevel,bool testEnd)
{
    if(pinNo < FIG_TOTAL_TEST_PIN)
    {
        if(ReceiveTestResult(itemCommand,pinNo))
        {
            // delay(10);
            pinNo++;
            if(pinNo == FIG_TOTAL_TEST_PIN)
            {
                pinNo = 0;
                if(testEnd)
                {
                    step = STEP_TEST_END;
                }
                else
                {
                    if(testHighLevel)
                        step = STEP_SINGLE_DIGITAL_WRITE_LOW;
                    else
                        step = STEP_RF_CHECK;
                }
            }
            else
            {
                if(pinNo == 7) pinNo = 33;
                if(testHighLevel)
                {
                    step = STEP_SINGLE_DIGITAL_WRITE_HIGH;
                }
                else
                {
                    step = STEP_SINGLE_DIGITAL_WRITE_LOW;
                }
            }

        }
    }
}

void W323PinLoopTestHandle(testItem_t itemCommand, bool testHighLevel,bool testEnd)
{
    if(pinNo < W323_TOTAL_TEST_PIN)
    {
        if(ReceiveTestResult(itemCommand,pinNo))
        {
            // delay(10);
            pinNo++;
            if(pinNo == W323_TOTAL_TEST_PIN)
            {
                pinNo = 0;
                if(testEnd)
                {
                    step = STEP_TEST_END;
                }
                else
                {
                    if(testHighLevel)
                        step = STEP_SINGLE_DIGITAL_WRITE_LOW;
                    else
                        step = STEP_ANALOG_READ;
                }
            }
            else
            {
                if(pinNo == 1)
                {
                    pinNo = 2;
                }
                else if(pinNo == 3)
                {
                    pinNo = 4;
                }
                else if(pinNo == 6)
                {
                    pinNo = 12;
                }
                else if(pinNo == 20)
                {
                    pinNo = 21;
                }
                else if(pinNo == 24)
                {
                    pinNo = 25;
                }
                else if(pinNo == 28)
                {
                    pinNo = 32;
                }

                if(testHighLevel)
                {
                    step = STEP_SINGLE_DIGITAL_WRITE_HIGH;
                }
                else
                {
                    step = STEP_SINGLE_DIGITAL_WRITE_LOW;
                }
            }

        }
    }
}

void L6PinLoopTestHandle(testItem_t itemCommand, bool testHighLevel,bool testEnd)
{
    if(pinNo < L6_TOTAL_TEST_PIN)
    {
        if(ReceiveTestResult(itemCommand,pinNo))
        {
            // delay(10);
            pinNo++;
            if(pinNo == L6_TOTAL_TEST_PIN)
            {
                pinNo = 0;
                if(testEnd)
                {
                    step = STEP_TEST_END;
                }
                else
                {
                    if(testHighLevel)
                        step = STEP_SINGLE_DIGITAL_WRITE_LOW;
                    else
                        step = STEP_ANALOG_READ;
                }
            }
            else
            {
                if(pinNo == 9) pinNo = 11;
                if(testHighLevel)
                {
                    step = STEP_SINGLE_DIGITAL_WRITE_HIGH;
                }
                else
                {
                    step = STEP_SINGLE_DIGITAL_WRITE_LOW;
                }
            }
        }
    }
}

void W67PinLoopTestHandle(testItem_t itemCommand, bool testHighLevel,bool testEnd)
{
    if(pinNo < W67_TOTAL_TEST_PIN)
    {
        if(ReceiveTestResult(itemCommand,pinNo))
        {
            // delay(10);
            pinNo++;
            if(pinNo == W67_TOTAL_TEST_PIN)
            {
                pinNo = 0;
                if(testEnd)
                {
                    step = STEP_TEST_END;
                }
                else
                {
                    if(testHighLevel)
                    {
                        pinNo = 2;
                        step = STEP_SINGLE_DIGITAL_WRITE_LOW;
                    }
                    else
                    {
                        step = STEP_ANALOG_READ;
                    }
                }
            }
            else
            {
                if(pinNo == 3) pinNo = 4;
                if(testHighLevel)
                {
                    step = STEP_SINGLE_DIGITAL_WRITE_HIGH;
                }
                else
                {
                    step = STEP_SINGLE_DIGITAL_WRITE_LOW;
                }
            }
        }
    }
}

void AntPinLoopTestHandle(testItem_t itemCommand, bool testHighLevel, bool testEnd)
{
    if(pinNo < ANT_TOTAL_TEST_PIN)
    {
        if(ReceiveTestResult(itemCommand,pinNo))
        {
            // delay(10);
            pinNo++;
            if(pinNo == ANT_TOTAL_TEST_PIN)
            {
                pinNo = 0;
                if(testEnd)
                {
                    step = STEP_TEST_END;
                }
                else
                {
                    if(testHighLevel)
                        step = STEP_SINGLE_DIGITAL_WRITE_LOW;
                    else
                        step = STEP_ANALOG_READ;
                }
            }
            else
            {
                if(pinNo == 5) pinNo = 30;
                if(testHighLevel)
                {
                    step = STEP_SINGLE_DIGITAL_WRITE_HIGH;
                }
                else
                {
                    step = STEP_SINGLE_DIGITAL_WRITE_LOW;
                }
            }
        }
    }
}

void FoxPinLoopTestHandle(testItem_t itemCommand, bool testHighLevel, bool testEnd)
{
    if(pinNo < FOX_TOTAL_TEST_PIN)
    {
        if(ReceiveTestResult(itemCommand,pinNo))
        {
            // delay(10);
            pinNo++;
            if(pinNo == FOX_TOTAL_TEST_PIN)
            {
                pinNo = 0;
                if(testEnd)
                {
                    step = STEP_TEST_END;
                }
                else
                {
                    if(testHighLevel)
                        step = STEP_SINGLE_DIGITAL_WRITE_LOW;
                    else
                        step = STEP_ANALOG_READ;
                }
            }
            else
            {
                if(pinNo == 5) pinNo = 7;
                else if(pinNo == 8) pinNo = 30;
                if(testHighLevel)
                {
                    step = STEP_SINGLE_DIGITAL_WRITE_HIGH;
                }
                else
                {
                    step = STEP_SINGLE_DIGITAL_WRITE_LOW;
                }
            }
        }
    }
}



String  strError = "GPIO Error:";

void SearchErrorPin(uint8_t boardPins, char *pinMap, const char *pinName[])
{
    uint8_t index;
    for(index = 0; index < boardPins; index++)
    {
        if(errorPin == pinMap[index])
        {
            strError = strError + pinName[index];
            return;
        }
    }
}

void setup()
{
    Serial1.begin(115200);
    Serial1.setTimeout(50);
    delay(100);
    TestInit();
    step = STEP_GET_BOARD_TYPE;
}

uint32_t ctimeout;

void loop()
{
    switch(step)
    {
        case STEP_GET_BOARD_TYPE:
            queryBoard();
            step = STEP_CONFIRM_GET_BOARD_TYPE;
            break;

        case STEP_CONFIRM_GET_BOARD_TYPE:
            boardType = getBoardType();
            JudgeBoardType();
            break;

        case STEP_DIGITAL_WRITE_HIGH:
            OLEDDisplay(1,"test 1:","digitalWrite High");
            SendTestCommand(TEST_DIGITAL_WRITE_HIGH,255);
            step = STEP_CONFIRM_DIGITAL_WRITE_HIGH_RESULT;
            break;

        case STEP_CONFIRM_DIGITAL_WRITE_HIGH_RESULT:
            if(ReceiveTestResult(TEST_DIGITAL_WRITE_HIGH,255))
            {
                delay(100);
                step = STEP_DIGITAL_WRITE_LOW;
            }
            break;
        case STEP_DIGITAL_WRITE_LOW:
            OLEDDisplay(1,"test 2:","digitalWrite Low");
            SendTestCommand(TEST_DIGITAL_WRITE_LOW,255);
            step = STEP_CONFIRM_DIGITAL_WRITE_LOW_RESULT;
            break;

        case STEP_CONFIRM_DIGITAL_WRITE_LOW_RESULT:
            if(ReceiveTestResult(TEST_DIGITAL_WRITE_LOW,255))
            {
                delay(100);
                step = STEP_SINGLE_DIGITAL_WRITE_HIGH;
                if(boardType == W67)
                    pinNo = 2;
                else
                    pinNo = 0;
            }
            break;

        case STEP_SINGLE_DIGITAL_WRITE_HIGH:
            {
             OLEDDisplay(1,"test 3:","single digital high");
             SendTestCommand(TEST_DIGITAL_WRITE_LOW,255);
             ctimeout = timerGetId();
             while(1)
             {
                 if(ReceiveTestResult(TEST_DIGITAL_WRITE_LOW,255))
                 {
                     break;
                 }
                 if(timerIsEnd(ctimeout,1000))
                 {
                     break;
                 }
             }
             SendTestCommand(TEST_DIGITAL_WRITE_HIGH,pinNo);
             step = STEP_SINGLE_CONFIRM_DIGITAL_WRITE_HIGH_RESULT;
             }
            break;

        case STEP_SINGLE_CONFIRM_DIGITAL_WRITE_HIGH_RESULT:
            if(boardType == W67)
            {
                W67PinLoopTestHandle(TEST_DIGITAL_WRITE_HIGH,true,false);
            }
            else if(boardType == FIG)
            {
                FigPinLoopTestHandle(TEST_DIGITAL_WRITE_HIGH,true,false);
            }
            else if(boardType == W323)
            {
                W323PinLoopTestHandle(TEST_DIGITAL_WRITE_HIGH,true,false);
            }
            else if(boardType == L6)
            {
                L6PinLoopTestHandle(TEST_DIGITAL_WRITE_HIGH,true,false);
            }
            else if(boardType == ANT)
            {
                AntPinLoopTestHandle(TEST_DIGITAL_WRITE_HIGH,true,false);
            }
            else if(boardType == FOX)
            {
                FoxPinLoopTestHandle(TEST_DIGITAL_WRITE_HIGH,true,false);
            }

            break;

        case STEP_SINGLE_DIGITAL_WRITE_LOW:
            {
            OLEDDisplay(1,"test 4:","single digital low");
            SendTestCommand(TEST_DIGITAL_WRITE_HIGH,255);
            ctimeout = timerGetId();
            while(1)
            {
                if(ReceiveTestResult(TEST_DIGITAL_WRITE_HIGH,255))
                {
                    break;
                }
                if(timerIsEnd(ctimeout,1000)) 
                {
                    break;
                }
            }
            SendTestCommand(TEST_DIGITAL_WRITE_LOW,pinNo);
            step = STEP_SINGLE_CONFIRM_DIGITAL_WRITE_LOW_RESULT;
            }
            break;

        case STEP_SINGLE_CONFIRM_DIGITAL_WRITE_LOW_RESULT:
            if(boardType == W67)
            {
                W67PinLoopTestHandle(TEST_DIGITAL_WRITE_LOW,false,false);
            }
            else if(boardType == FIG)
            {
                FigPinLoopTestHandle(TEST_DIGITAL_WRITE_LOW,false,false);
            }
            else if(boardType == W323)
            {
                W323PinLoopTestHandle(TEST_DIGITAL_WRITE_LOW,false,true);
            }
            else if(boardType == L6)
            {
                L6PinLoopTestHandle(TEST_DIGITAL_WRITE_LOW,false,false);
            }
            else if(boardType == ANT)
            {
                AntPinLoopTestHandle(TEST_DIGITAL_WRITE_LOW,false,false);
            }
            else if(boardType == FOX)
            {
                FoxPinLoopTestHandle(TEST_DIGITAL_WRITE_LOW,false,false);
            }

            break;

        case STEP_ANALOG_READ:
            OLEDDisplay(1,"test 5:","analogRead");
            if(boardType == L6)
            {
                SendTestCommand(TEST_ANALOG_READ,14);
            }
            else
            {
                SendTestCommand(TEST_ANALOG_READ,30);
            }
            step = STEP_CONFIRM_ANALOG_READ_RESULT;
            break;

        case STEP_CONFIRM_ANALOG_READ_RESULT:
            {
                uint8_t analogPin = 30;
                if(boardType == L6)
                {
                    analogPin = 14;
                }

                if(ReceiveTestResult(TEST_ANALOG_READ,analogPin))
                {
                    delay(100);
                    step = STEP_SELF_TEST;
                }
            }
            break;

        case STEP_SELF_TEST:
            OLEDDisplay(1,"test 6:","selfTest");
            SendTestCommand(TEST_SELF_TEST,NONE_PIN);
            step = STEP_CONFIRM_SELF_TEST_RESULT;

            break;

        case STEP_CONFIRM_SELF_TEST_RESULT:
            if(ReceiveTestResult(TEST_SELF_TEST,NONE_PIN))
            {
                delay(100);
                if(boardType == FOX)
                {
                    step = STEP_TEST_END;
                }
                else
                {
                    step = STEP_RF_CHECK;
                }
            }

            break;

        case STEP_RF_CHECK:
            OLEDDisplay(1,"test 7:","RFCheck");
            SendTestCommand(TEST_RF_CHECK,NONE_PIN);
            step = STEP_CONFIRM_RF_CHECK_RESULT;
            break;

        case STEP_CONFIRM_RF_CHECK_RESULT:
            if(ReceiveTestResult(TEST_RF_CHECK,NONE_PIN))
            {
                if(boardType == NEUTRON || boardType == NUT )//|| boardType == FIG)
                {
                    step = STEP_SENSOR_DATA;
                }
                else
                {
                    step = STEP_TEST_END;
                }
            }

            break;

        case STEP_SENSOR_DATA:
            SendTestCommand(TEST_SENSOR_DATA,NONE_PIN);
            break;

        case STEP_CONFIRM_SENSOR_DATA_RESULT:
            if(ReceiveTestResult(TEST_SENSOR_DATA,NONE_PIN))
            {
                step = STEP_TEST_END;
            }
            break;

        case STEP_TEST_END:

            delay(100);
            OLEDDisplay(1,"Test End!!","");
            delay(500);
            if(boardType == UNKOWN_BOARD_TYPE)
            {
                OLEDDisplay(1,"unkown board type","");
            }
            else
            {
                if(testResult)
                {
                    if(boardType == L6 || boardType == ANT)
                    {
                        String p;
                        p = p+"snr:";
                        char tmp[16];
                        sprintf(tmp,"%d",snrVal);
                        p = p+tmp;
                        p = p+ "\r\n";
                        p = p + "rssi:";
                        sprintf(tmp,"%d",rssiVal);
                        p = p+tmp;
                        p = p+ "\r\n";
                        p = p + "tx rssi:";
                        sprintf(tmp,"%d",txRssiVal);
                        p = p+tmp;
                        OLEDDisplay(2,"PASS",p);
                    }
                    else if(boardType == FOX)
                    {
                        String p;
                        char tmp[16];
                        p = p + "rssi:";
                        sprintf(tmp,"%d",rssiVal);
                        p = p+tmp;
                        // OLEDDisplay(2,"PASS",p);
                        OLEDDisplay(2,"PASS","");
                    }
                    else
                    {
                        OLEDDisplay(2,"PASS","");
                    }

                }
                else
                {
                    switch(errorType)
                    {
                        case ERROR_PIN_HIGH:
                        case ERROR_PIN_LOW:
                            {
                                if(boardType == W67)
                                {
                                    SearchErrorPin(W67_TOTAL_TEST_PIN,W67PinMapping,W67PinName);
                                }
                                else if(boardType == FIG)
                                {
                                    SearchErrorPin(FIG_TOTAL_TEST_PIN,figPinMapping,figPinName);
                                }
                                else if(boardType == W323)
                                {
                                    SearchErrorPin(W323_TOTAL_TEST_PIN,W323PinMapping,W323PinName);
                                }
                                else if(boardType == L6)
                                {
                                    SearchErrorPin(L6_TOTAL_TEST_PIN,L6PinMapping,L6PinName);
                                }
                                else if(boardType == ANT)
                                {
                                    SearchErrorPin(ANT_TOTAL_TEST_PIN,antPinMapping,antPinName);
                                }
                                else if(boardType == FOX)
                                {
                                    SearchErrorPin(FOX_TOTAL_TEST_PIN,foxPinMapping,foxPinName);
                                }

                                OLEDDisplay(2,"FAIL", strError);
                            }
                            break;

                        case ERROR_ANALOG_READ:
                            OLEDDisplay(2,"FAIL","analogRead Fail");
                            break;

                        case ERROR_SELF_TEST:
                            if(boardType == L6 || boardType == ANT || boardType == FOX)
                            {
                                OLEDDisplay(2,"FAIL","selfTest Fail:32.768KHz Fail");
                            }
                            else
                            {
                                OLEDDisplay(2,"FAIL","selfTest Fail");
                            }
                            break;

                        case ERROR_RF_CHECK:
                            if(boardType == L6 || boardType == ANT)
                            {
                                String p = "RFCheck Fail";
                                p = p+"\r\n";
                                p = p+"snr:";
                                char tmp[16];
                                sprintf(tmp,"%d",snrVal);
                                p = p+tmp;
                                p = p+ "\r\n";
                                p = p + "rssi:";
                                sprintf(tmp,"%d",rssiVal);
                                p = p+tmp;
                                p = p+ "\r\n";
                                p = p + "tx rssi:";
                                sprintf(tmp,"%d",txRssiVal);
                                p = p+tmp;
                                OLEDDisplay(2,"FAIL",p);
                            }
                            else if(boardType == FOX)
                            {
                                String p;
                                char tmp[16];
                                p = p + "rssi:";
                                sprintf(tmp,"%d",rssiVal);
                                p = p+tmp;
                                OLEDDisplay(2,"FAIL",p);
                            }
                            else
                            {
                                OLEDDisplay(2,"FAIL","RFCheck Fail");
                            }
                            break;

                        case ERROR_SENSOR_DATA:
                            OLEDDisplay(2,"FAIL","sensor Fail");
                            break;
                    }

                }
            }
            while(1);
            break;

        default:
            break;
    }
}

#elif defined ANYTEST_TEST_L7_MODULE

//L7 test code
#include "Adafruit_SSD1306.h"
#include "lora.h"

Serial1DebugOutput debugOutput(115200, ALL_LEVEL);

#define OLED_RESET  D16
#define OLED_DC     D17
#define OLED_CS     D21

Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);  // Hareware SPI

int8_t loraData[64];
int8_t rssiVal;
int8_t snrVal;
int8_t txRssiVal;
uint32_t currTime;

void OLEDDisplay(uint8_t textSize, const char *itemName, const char *result)
{
    display.clearDisplay();
    display.setTextSize(textSize);
    display.setCursor(0,0);
    display.println(itemName);
    display.setTextSize(1);
    display.setCursor(0,32);
    display.println(result);
    display.display();
}

void setup()
{
    Serial1.begin(115200);
    delay(100);

    display.begin(SSD1306_SWITCHCAPVCC);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.clearDisplay();
    display.println("test L7");
    display.display();
    delay(500);

    TestSX1276Init();
    if(SX1276GetVersion() != 18)
    {
        OLEDDisplay(2,"FAIL","SPI FAIL");
        while(1);
    }
    currTime = timerGetId();
}


void loop()
{
    switch(ProcessSX1276(rssiVal,snrVal,loraData))
    {
        case 0:
                OLEDDisplay(1,"sx1278 send data...","");
                if(timerIsEnd(currTime,2000)) // 5s timeout
                {
                    OLEDDisplay(2,"FAIL","sx1278 send timeout");
                }
            break;

        case 1:
            OLEDDisplay(1,"sx1278 receive data...","");
            break;

        case 2:
            if(loraData[0] == 22)
            {
                txRssiVal = loraData[1];
                Serial1.println("data is ok");
                String p = "snr:";
                char tmp[16];
                sprintf(tmp,"%d",snrVal);
                p = p+tmp;
                p = p+ "\r\n";
                p = p + "rssi:";
                sprintf(tmp,"%d",rssiVal);
                p = p+tmp;
                p = p+ "\r\n";
                p = p + "tx rssi:";
                sprintf(tmp,"%d",txRssiVal);
                p = p+tmp;

                if(rssiVal > -40)
                    OLEDDisplay(2,"PASS",p);
                else
                {
                    p = p + "\r\n";
                    p = p + "error: rssi < -40";
                    OLEDDisplay(2,"FAIL",p);
                }

                while(1);
            }
            else
            {
                OLEDDisplay(2,"FAIL","receive data is error");
                while(1);
            }

            break;

        default:
            OLEDDisplay(2,"FAIL                 sx1278 receive timeout","please again or station is not open");
            // OLEDDisplay("sx1278 receive timeout","please again or station is not open");
            while(1);
            break;
    }
}


#elif defined ANYTEST_TEST_SX1276

#if 0
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

#define SX1276_TX_EN
#define OLED_DISPLAY

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

#if 0
//lora透传测试 收发转换

//anytest board Serial: A7-TX A8-RX
SerialDebugOutput debugOutput(115200, ALL_LEVEL);
#include "Adafruit_SSD1306.h"
#include "lora.h" //用于测试板

#define MASTER_SX1276
#define OLED_DISPLAY

#define OLED_RESET  D16
#define OLED_DC     D17
#define OLED_CS     D21

#define LED          A15
#define RED_LED       A0
#define GRREN_LED     A1
#define BLUE_LED      A2

uint32_t currentTime;
uint32_t txPacketCnt;
uint32_t rxPacketCnt;
uint32_t errorPacketCnt;

Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);  // Hareware SPI

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
    display.begin(SSD1306_SWITCHCAPVCC);
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
    //     {
    //         DEBUG("register address = %x",i);
    //         DEBUG("register data = %x",SX1276Read(i));
    //     }

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

static bool ledFlag=false;

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



#endif
