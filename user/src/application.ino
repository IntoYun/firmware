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


// ALL_LEVEL, TRACE_LEVEL, DEBUG_LEVEL, WARN_LEVEL, ERROR_LEVEL, PANIC_LEVEL, NO_LOG_LEVEL
Serial1DebugOutput debugOutput(115200, ALL_LEVEL);
#if 0
#define LED_PIN D7

void setup()
{
    // initialize digital pin 13 as an output.
    pinMode(LED_PIN, OUTPUT);
    pinMode(A0, OUTPUT);
    DEBUG("GPIO Test\r\n");
}

// the loop function runs over and over again forever
void loop()
{
    digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);              // wait for a second
    DEBUG("digital Read: %d\r\n", digitalRead(A0));
}
#endif

#if 0
#define testA A0
#define LED_PIN D7

void setup()
{
   pinMode(testA, INPUT);
   pinMode(LED_PIN, OUTPUT);
   DEBUG("AnalogRead Test\r\n");
}

// the loop function runs over and over again forever
void loop()
{
    uint16_t value = analogRead(testA);
    DEBUG("analogRead value: %d", value);
    digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);              // wait for a second

}
#endif

#if 0
#include "gy30.h"

#define LED_PIN D7

GY30 gy30;
void setup()
{
    DEBUG("I2C Test\r\n");
    pinMode(LED_PIN, OUTPUT);
    gy30.begin();
}

// the loop function runs over and over again forever
void loop()
{
    digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                   // wait for a second
    digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                   // wait for a second
    DEBUG("GY30 value: %f", gy30.Read());
    //DEBUG("Runing !");
}
#endif

#if 1
#define LED_PIN D7
#define testPinPWM A0
#define INTERRUPT_PIN D0

uint8_t flag = 0;
void Blink(void)
{
	if(flag == 0)
	{
		flag = 1;
		digitalWrite(LED_PIN,HIGH);
	}
	else
	{
		flag = 0;
		digitalWrite(LED_PIN,LOW);
	}
}

void setup()
{
    DEBUG("Interrupt Test\r\n");
    pinMode(LED_PIN, OUTPUT);

	pinMode(INTERRUPT_PIN, INPUT_PULLUP);
	pinMode(testPinPWM, OUTPUT);
	attachInterrupt(INTERRUPT_PIN, Blink, FALLING);
    DEBUG("After setup\r\n");
}

// the loop function runs over and over again forever
void loop()
{
    DEBUG("Enter loop\r\n");
    attachInterrupt(INTERRUPT_PIN, Blink, FALLING);

	for(uint8_t i = 0; i < 5; i++)
	{
		digitalWrite(testPinPWM, HIGH);
		delay(500);
		digitalWrite(testPinPWM, LOW);
		delay(500);
	}

	detachInterrupt(INTERRUPT_PIN);
	delay(1000);
	attachInterrupt(INTERRUPT_PIN,Blink, FALLING);

	for(uint8_t i = 0; i < 5; i++) // ledÉÁË¸
	{
		digitalWrite(testPinPWM, HIGH);
		delay(500);
		digitalWrite(testPinPWM, LOW);
		delay(500);
	}
	noInterrupts();
	delay(1000);
	interrupts();
}

#endif












