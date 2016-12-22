/*
 * anytest 默认程序
 */
// 测试板程序

#include "Adafruit_SSD1306.h"
#include "ajson.h"

#define OLED_RESET  D16
#define OLED_DC     D17
#define OLED_CS     D21

Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);  // Hareware SPI

aJsonClass aJson;
uint8_t boardType;
uint8_t step;
bool testResult = true;
uint32_t currentTime;
uint8_t helloCnt;

enum BoardType
{
    ATOM = 1,
    NEUTRON,
    NUT,
    W67,
    FIG,
    W323,
    LORA,
    L6,
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
    // TEST_WIFI_CHECK,
    // TEST_LORA_CHECK,
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
    STEP_ANALOG_READ,
    STEP_CONFIRM_ANALOG_READ_RESULT,
    STEP_SELF_TEST,
    STEP_CONFIRM_SELF_TEST_RESULT,
    STEP_RF_CHECK,
    STEP_CONFIRM_RF_CHECK_RESULT,
    // STEP_WIFI_CHECK,
    // STEP_CONFIRM_WIFI_CHECK_RESULT,
    // STEP_LORA_CHECK,
    // STEP_CONFIRM_LORA_CHECK_RESULT,
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
            if(strcmp(boardPtr,"888004") == 0) return ATOM;         // atom
            else if(strcmp(boardPtr,"888002") == 0) return NEUTRON;//neutron
            else if(strcmp(boardPtr,"888003") == 0) return NUT;   //nut
            else if((strcmp(boardPtr,"888101") == 0) || (strcmp(boardPtr,"887101")) == 0) return W67;   //w6/7
            else if(strcmp(boardPtr,"888005") == 0) return FIG;   //fig
            else if(strcmp(boardPtr,"888102") == 0) return W323; //w32/33
            else if(strcmp(boardPtr,"888006") == 0) return LORA; //lora
            else if(strcmp(boardPtr,"888103") == 0) return L6;   //l6
            else
            {
                boardType = UNKOWN_BOARD_TYPE;
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

// void OLEDDisplay(uint8_t row, uint8_t column, const char *itemName, const char *result)
void OLEDDisplay(const char *itemName, const char *result)
{
    display.clearDisplay();
    display.setCursor(0,0);
    // display.setCursor(row,column);
    display.println(itemName);
    display.setCursor(0,32);
    // display.setCursor(row,column);
    display.println(result);
    display.display();
}

bool ReadBoardPinLevel(uint8_t val)
{
    uint8_t pin;
    switch(boardType)
        {
        case ATOM:
            break;

        case NEUTRON:
            break;

        case NUT:
        case W67:
            for(pin = D4;  pin <= D8; pin++)
            {
                pinMode(pin,INPUT);
            }

            for(pin = A5;  pin <= A8; pin++)
            {
                pinMode(pin,INPUT);
            }
            delay(10);
            for(pin = D4;  pin <= D8; pin++)
            {
                if(digitalRead(pin) == val && pin != D6)
                {
                    return false;
                }
            }

            for(pin = A5;  pin <= A8; pin++)
            {
                if(digitalRead(pin) == val)
                {
                    return false;
                }
            }
            break;

        case FIG:
        case W323:
            for(pin = D0;  pin < D7; pin++)
            {
                pinMode(pin,INPUT);
            }
            for(pin = A0;  pin < A9; pin++)
            {
                pinMode(pin,INPUT);
            }
            delay(10);
            for(pin = D0;  pin < D7; pin++)
            {
                if(digitalRead(pin) == val)
                {
                    return false;
                }
            }
            for(pin = A0; pin < A9; pin++)
            {
                if(digitalRead(pin) == val)
                {
                    return false;
                }
            }

            break;

        case LORA:
            break;

        case L6:
            break;

        default:
            break;
        }

    return true;
}

bool ReceiveTestResult(testItem_t testItem)
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
                    if((statusObject->valueint == 200) && ReadBoardPinLevel(0))
                    {
                        OLEDDisplay("digitalWrite High:","OK");
                    }
                    else
                    {
                        OLEDDisplay("digitalWrite High:","NG");
                        testResult = false;
                    }
                    break;

                case TEST_DIGITAL_WRITE_LOW:
                    if((statusObject->valueint == 200) && ReadBoardPinLevel(1))
                    {
                        OLEDDisplay("digitalWrite Low:","OK");
                    }
                    else
                    {
                        OLEDDisplay("digitalWrite Low:","NG");
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
                            OLEDDisplay("analogRead:","OK");
                        }
                        else
                        {
                            OLEDDisplay("analogRead:","NG");
                            testResult = false;
                        }
                    }
                    else
                    {
                        OLEDDisplay("analogRead:","NG");
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
                            if(statusObject->valueint == 200)
                             {
                                 OLEDDisplay("selfTest:","OK");
                             }
                            else
                            {
                                OLEDDisplay("selfTest:","NG");
                                testResult = false;
                            }
                            break;

                        case LORA:
                        case L6:
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
                            // if(strcmp(ap[i].ssid,"IntoRobot-NETGEAR") == 0)
                            if(strcmp(ap[i].ssid,"TP-LINK_3816") == 0)
                            {
                                assignWifi = true;
                                if(ap[i].rssi >= (-65))
                                {
                                    OLEDDisplay("wifiCheck:","OK");
                                    break;
                                }
                                else
                                {
                                    OLEDDisplay("wifiCheck:","NG");
                                    testResult = false;
                                    break;
                                }
                            }
                        }

                        if(!assignWifi)
                        {
                            OLEDDisplay("Not Open WiFi Network!!!","");
                            while(1);
                        }
                    }
                    else
                    {
                        OLEDDisplay("wifiCheck:","NG");
                        testResult = false;
                    }
                }
                else if((boardType == LORA)||(boardType == L6))
                {
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

void SendTestCommand(testItem_t itemCommand)
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
            aJson.addNumberToObject(valueObject, "pin", 255);
            aJson.addStringToObject(valueObject, "val", "HIGH");
            strPtr = aJson.print(root);
            SerialPrint(strPtr);

            break;

        case TEST_DIGITAL_WRITE_LOW:
            aJson.addStringToObject(valueObject, "item", "digitalWrite");
            aJson.addNumberToObject(valueObject, "pin", 255);
            aJson.addStringToObject(valueObject, "val", "LOW");
            strPtr = aJson.print(root);
            SerialPrint(strPtr);

            break;


        case TEST_ANALOG_READ:
            aJson.addStringToObject(valueObject, "item", "analogRead");
            aJson.addNumberToObject(valueObject, "pin", 30);
            strPtr = aJson.print(root);
            SerialPrint(strPtr);

            break;

        case TEST_SELF_TEST:
            aJson.addStringToObject(valueObject, "item", "selfTest");
            strPtr = aJson.print(root);
            SerialPrint(strPtr);

            break;

        case TEST_RF_CHECK:
            aJson.addStringToObject(valueObject, "item", "rfCheck");
            strPtr = aJson.print(root);
            SerialPrint(strPtr);
            break;

        case TEST_SENSOR_DATA:
            aJson.addStringToObject(valueObject, "item", "sensorData");
            strPtr = aJson.print(root);
            SerialPrint(strPtr);

            break;
        default:
            break;
    }

    free(strPtr);
    aJson.deleteItem(root);
}

void setup()
{
    Serial1.begin(115200);
    delay(100);
    TestInit();
    step = STEP_GET_BOARD_TYPE;
}

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

                case LORA:
                    display.clearDisplay();
                    display.setCursor(0,0);
                    display.println("Lora Test");
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

                default:
                    break;
            }
            break;

        case STEP_DIGITAL_WRITE_HIGH:
            OLEDDisplay("test 1:","digitalWrite High");
            SendTestCommand(TEST_DIGITAL_WRITE_HIGH);
            step = STEP_CONFIRM_DIGITAL_WRITE_HIGH_RESULT;
            break;

        case STEP_CONFIRM_DIGITAL_WRITE_HIGH_RESULT:
            if(ReceiveTestResult(TEST_DIGITAL_WRITE_HIGH))
            {
                delay(500);
                step = STEP_DIGITAL_WRITE_LOW;
            }
            break;
        case STEP_DIGITAL_WRITE_LOW:
            OLEDDisplay("test 2:","digitalWrite Low");
            SendTestCommand(TEST_DIGITAL_WRITE_LOW);
            step = STEP_CONFIRM_DIGITAL_WRITE_LOW_RESULT;
            break;

        case STEP_CONFIRM_DIGITAL_WRITE_LOW_RESULT:
            if(ReceiveTestResult(TEST_DIGITAL_WRITE_LOW))
            {
                delay(500);
                step = STEP_ANALOG_READ;
            }
            break;

        case STEP_ANALOG_READ:
            OLEDDisplay("test 3:","analogRead");
            SendTestCommand(TEST_ANALOG_READ);
            step = STEP_CONFIRM_ANALOG_READ_RESULT;
            break;

        case STEP_CONFIRM_ANALOG_READ_RESULT:
            if(ReceiveTestResult(TEST_ANALOG_READ))
            {
                delay(500);
                step = STEP_SELF_TEST;
            }
            break;

        case STEP_SELF_TEST:
            OLEDDisplay("test 4:","selfTest");
            SendTestCommand(TEST_SELF_TEST);
            step = STEP_CONFIRM_SELF_TEST_RESULT;

            break;

        case STEP_CONFIRM_SELF_TEST_RESULT:
            if(ReceiveTestResult(TEST_SELF_TEST))
            {
                delay(500);
                step = STEP_RF_CHECK;
            }

            break;

        case STEP_RF_CHECK:
            OLEDDisplay("test 5:","RFCheck");
            SendTestCommand(TEST_RF_CHECK);
            step = STEP_CONFIRM_RF_CHECK_RESULT;
            break;

        case STEP_CONFIRM_RF_CHECK_RESULT:
            if(ReceiveTestResult(TEST_RF_CHECK))
            {
                if(boardType == NEUTRON || boardType == NUT || boardType == FIG)
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
            SendTestCommand(TEST_SENSOR_DATA);
            break;

        case STEP_CONFIRM_SENSOR_DATA_RESULT:
            if(ReceiveTestResult(TEST_RF_CHECK))
            {

                step = STEP_TEST_END;
            }
            break;

        case STEP_TEST_END:

            delay(500);
            OLEDDisplay("Test End!!","");
            delay(500);
            if(boardType == UNKOWN_BOARD_TYPE)
            {
                OLEDDisplay("unkown board type","");
            }
            else
            {
                if(testResult)
                {
                    OLEDDisplay("PASS","");
                }
                else
                {
                    OLEDDisplay("FAIL","");
                }
            }
            while(1);
            break;

        default:
            break;
    }
}


#if 0
#define LED_PIN D7

void setup()
{
    pinMode(LED_PIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop()
{
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    delay(1000);
}
#endif
