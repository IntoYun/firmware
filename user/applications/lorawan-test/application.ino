/*
 * InotYun 测试程序
 */

//SerialUSBDebugOutput debugOutput(115200, ALL_LEVEL);
SerialDebugOutput debugOutput(115200, ALL_LEVEL);

PRODUCT_ID(PctOaj94F4KyHdkC3f4xrvWpEFRfj18f)     //产品ID
PRODUCT_SECRET(efacbc991dbdc66cbb327bcf04a1c119) //产品密钥
PRODUCT_VERSION(2)     //产品版本号

#define DPID_ENUM_LIGHT_MODE             1        //颜色模式
#define DPID_NUMBER_TEMPERATURE          2        //温度
#define DPID_BOOL_SWITCH                 3        //灯泡开关
#define DPID_BOOL_LIGHT_STATUS           4        //灯泡亮灭状态
#define DPID_NUMBER_RHEOSTAT             5        //速度
#define DPID_STRING_LCD_DISPLAY          6        //字符显示

#define LEDPIN    LED_USER    //例子灯

void setup()
{
    Serial.begin(115200);
    pinMode(LEDPIN, OUTPUT);
    IntoRobot.defineDataPointEnum(DPID_ENUM_LIGHT_MODE, UP_DOWN, 0);                     //颜色模式
    IntoRobot.defineDataPointNumber(DPID_NUMBER_TEMPERATURE, UP_ONLY, 0, 100, 1, 0);     //温度
    IntoRobot.defineDataPointBool(DPID_BOOL_SWITCH, UP_DOWN, false);                     //灯泡开关
    IntoRobot.defineDataPointBool(DPID_BOOL_LIGHT_STATUS, UP_ONLY, false);               //灯泡亮灭状态
    IntoRobot.defineDataPointNumber(DPID_NUMBER_RHEOSTAT, UP_DOWN, 0, 100, 0, 0);        //速度
    IntoRobot.defineDataPointString(DPID_STRING_LCD_DISPLAY, UP_DOWN, "oh yeah!");       //字符显示
}

void loop()
{
    delay(10000);
    IntoRobot.writeDataPoint(DPID_ENUM_LIGHT_MODE, 1);
    IntoRobot.writeDataPoint(DPID_NUMBER_TEMPERATURE, 11.11);
    IntoRobot.writeDataPoint(DPID_BOOL_SWITCH, true);
    IntoRobot.writeDataPoint(DPID_BOOL_LIGHT_STATUS, true);
    IntoRobot.writeDataPoint(DPID_NUMBER_RHEOSTAT, 343);
    IntoRobot.writeDataPoint(DPID_STRING_LCD_DISPLAY, "adfb");
    IntoRobot.sendDataPointAll();
}

