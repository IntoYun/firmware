#if 0
/*该头文件引用由IntoRobot自动添加.*/
#include "application.h"

void setup()
{
    pinMode(D1, INPUT);
    pinMode(D0, OUTPUT);
}

void loop()
{
	noInterrupts();
    digitalWrite(D0, HIGH);     // 打开灯泡
    digitalWrite(D0, LOW);     // 打开灯泡
    digitalWrite(D0, HIGH);     // 打开灯泡
    digitalRead(D1);
	//delayMicroseconds(1);
    digitalWrite(D0, LOW);     // 打开灯泡
    digitalWrite(D0, HIGH);     // 打开灯泡
    digitalWrite(D0, LOW);     // 打开灯泡
	interrupts();
}

#endif

#if 0
/*该头文件引用由IntoRobot自动添加.*/
#include "Adafruit_DHT.h"

//温湿度计控件topic定义
#define HUMITURE_DATA_TEMPERATURE    "channel/humiture_0/data/temperature"   //空气温度
#define HUMITURE_DATA_HUMIDITY       "channel/humiture_0/data/humidity"      //空气湿度

//传感器通讯引脚
#define DHTPIN D0

//选择传感器类型
#define DHTTYPE DHT11		// DHT 11
//#define DHTTYPE DHT22		// DHT 22 (AM2302)
//#define DHTTYPE DHT21		// DHT 21 (AM2301)
float tmp,hum;
Adafruit_DHT dht(DHTPIN, DHTTYPE, 40);

void setup()
{
    //初始化
    dht.begin();
}

void loop()
{
    tmp=dht.getTempCelcius();
    hum=dht.getHumidity();
    SerialUSB.printf("tmp=%f hum=%f\r\n",tmp, hum);
	IntoRobot.publish(HUMITURE_DATA_TEMPERATURE, tmp); //上送温度值
	IntoRobot.publish(HUMITURE_DATA_HUMIDITY, hum);        //上送湿度值
	delay(2000);
	IntoRobot.printf("11111\r\n");
}
#endif
#if 1
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

#if 0
#include "dht11.h"

//温湿度计控件topic定义
#define HUMITURE_DATA_TEMPERATURE    "channel/humiture_0/data/temperature"   //空气温度
#define HUMITURE_DATA_HUMIDITY       "channel/humiture_0/data/humidity"      //空气湿度
#define DHT11_PIN   D0
char tmp;
u8 hum;
DHT11 dht11 = DHT11(DHT11_PIN);

void setup()
{
    //初始化
    dht11.begin();
}

void loop()
{
    tmp=dht11.ReadTemperature(0); //上送温度值
    hum=dht11.ReadHumidity();        //上送湿度值
    IntoRobot.publish(HUMITURE_DATA_TEMPERATURE, tmp); //上送温度值
    IntoRobot.publish(HUMITURE_DATA_HUMIDITY, hum);    //上送湿度值
	delay(1000);
}
#endif

