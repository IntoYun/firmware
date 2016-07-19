#include <DHT11/dht11.h>

DHT11 dht11 = DHT11(D15);

char 	airTemperature; 
u8 		airHumidity;  

void setup()
{
	dht11.begin();
	SerialUSB.begin(115200);
	delay(500);
}


void loop()
{
		airTemperature = dht11.ReadTemperature(0); 
		
		SerialUSB.print(airTemperature); 
		SerialUSB.print("\t");
		
		airHumidity = dht11.ReadHumidity();  
		SerialUSB.print(airHumidity); 
		SerialUSB.print("\t");

		SerialUSB.print("\r\n");
		
}




