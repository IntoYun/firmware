#include <Adafruit_DHT/Adafruit_DHT.h>

//传感器通讯引脚
#define DHTPIN 2     

//选择传感器类型
//#define DHTTYPE DHT11		// DHT 11 
#define DHTTYPE DHT22		// DHT 22 (AM2302)
//#define DHTTYPE DHT21		// DHT 21 (AM2301)

Adafruit_DHT dht(DHTPIN, DHTTYPE);

void setup() 
{
    SerialUSB.println("DHTxx test!");
    dht.begin();
}

void loop() 
{
	delay(2000);
    
	float h = dht.getHumidity();     //获取湿度
	float t = dht.getTempCelcius();  //获取摄氏温度
	float f = dht.getTempFarenheit();//获取华氏温度
  
    //检查是否获取数据失败(失败重新开始) 
	if (isnan(h) || isnan(t) || isnan(f)) 
    {
        SerialUSB.println("Failed to read from DHT sensor!");
        return;
    }

	float hi = dht.getHeatIndex();  //获取热指数
	float dp = dht.getDewPoint();   //获取露点
	float k = dht.getTempKelvin();  //获取绝对温度

	SerialUSB.print("Humid: "); 
	SerialUSB.print(h);
	SerialUSB.print("% - ");
	SerialUSB.print("Temp: "); 
	SerialUSB.print(t);
	SerialUSB.print("*C ");
	SerialUSB.print(f);
	SerialUSB.print("*F ");
	SerialUSB.print(k);
	SerialUSB.print("*K - ");
	SerialUSB.print("DewP: ");
	SerialUSB.print(dp);
	SerialUSB.print("*C - ");
	SerialUSB.print("HeatI: ");
	SerialUSB.print(hi);
	SerialUSB.println("*C");
	SerialUSB.println(Time.timeStr());
}

