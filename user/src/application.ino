#if 0
/*
 * Atom 默认程序
 */
#include "application.h"

#define SMARTLIGHT_CMD_SWITCH    "channel/smartLight_0/cmd/switch"   //开关命令
#define SMARTLIGHT_DATA_STATUS   "channel/smartLight_0/data/status"  //开关状态

#define LEDPIN    D7    //定义灯泡控制引脚

void smartLightSwitchCb(uint8_t *payload, uint32_t len)
{
    if(payload[0] == '1')
    {
        digitalWrite(LEDPIN, HIGH);     // 打开灯泡
        IntoRobot.publish(SMARTLIGHT_DATA_STATUS,"1");
    }
    else
    {
        digitalWrite(LEDPIN, LOW);      //关闭灯泡
        IntoRobot.publish(SMARTLIGHT_DATA_STATUS,"0");
    }
}

void setup()
{
    //初始化
    pinMode(LEDPIN, OUTPUT);
    //接收灯开关命令
    IntoRobot.subscribe(SMARTLIGHT_CMD_SWITCH, NULL, smartLightSwitchCb);
}

void loop()
{
}
#endif

#if 1

// ALL_LEVEL, TRACE_LEVEL, DEBUG_LEVEL, WARN_LEVEL, ERROR_LEVEL, PANIC_LEVEL, NO_LOG_LEVEL
SerialDebugOutput debugOutput(115200, ALL_LEVEL);
/*
static void log_output(const char* msg)
{
    Serial.print(msg);
}
*/
#define LED_PIN D7

void setup()
{
    //Serial.begin(115200);
    //set_logger_output(log_output, ALL_LEVEL);
    //char version[32];
    // initialize digital pin 13 as an output.
    pinMode(LED_PIN, OUTPUT);
    esp8266MDM.init();
    //esp8266MDM.getNetVersion(version);
    //DEBUG_D("%s\r\n", version);
}

// the loop function runs over and over again forever
void loop()
{
    //DEBUG("%10.3f AT send    ", (HAL_Timer_Get_Milli_Seconds())*0.001);
    //DEBUG("1111");
    digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                   // wait for a second
    digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                   // wait for a second
}
#endif

