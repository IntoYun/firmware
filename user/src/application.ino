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
Serial1DebugOutput debugOutput(115200, ALL_LEVEL);


#define LED_PIN D7

void setup()
{
    Serial.begin(115200);
    // initialize digital pin 13 as an output.
    pinMode(LED_PIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop()
{
    char _buffer[200];
    //int trunc = snprintf(_buffer, arraySize(_buffer), "%010u:%s: %s %s(%d):", (unsigned)HAL_Timer_Get_Milli_Seconds(), "111", "222", "333", 333);
    //int trunc = sprintf(_buffer, "%d:%s: %s %s(%d):", 22, "111", "222", "333", 333);
    //int trunc = snprintf(_buffer, arraySize(_buffer), "1111111111111111111\r\n");
    //DEBUG(_buffer);
    Serial.print("intorobot-intorobot-intobot");
    //Serial.print("i");
    digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);              // wait for a second
}
#endif

