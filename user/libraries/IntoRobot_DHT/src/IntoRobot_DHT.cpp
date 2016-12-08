/*
 ******************************************************************************
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
  
  This library is used to DHT temperature and humidity sensor module series
********************************************************************
*/

#include "IntoRobot_DHT.h"

IntoRobot_DHT::IntoRobot_DHT(uint8_t pin, uint8_t type) 
{
	_pin = pin;
	_type = type;
	firstreading = true;
}

void IntoRobot_DHT::begin(void) 
{
    // 设置引脚
	pinMode(_pin, INPUT);
	digitalWrite(_pin, HIGH);
	_lastreadtime = 0;
}

float IntoRobot_DHT::readTemperature() 
{
	float f;

	if (read()) 
	{
		switch (_type) 
		{
			case DHT11:
				f = data[2];
				return f;
			case DHT22:
			case DHT21:
				f = data[2] & 0x7F;
				f *= 256;
				f += data[3];
				f /= 10;
				if (data[2] & 0x80)
					f *= -1;
				return f;
		}
	}
	return NAN;
}

float IntoRobot_DHT::getHumidity() 
{
	return readHumidity();
}

float IntoRobot_DHT::getTempCelcius() 
{
	return readTemperature();
}

float IntoRobot_DHT::getTempFarenheit() 
{
	return convertCtoF(readTemperature());
}

float IntoRobot_DHT::getTempKelvin() 
{
	return convertCtoK(readTemperature());
}

float IntoRobot_DHT::getHeatIndex() 
{
	return convertFtoC(computeHeatIndex(convertCtoF(readTemperature()), readHumidity()));
}

float IntoRobot_DHT::getDewPoint() 
{
	return computeDewPoint(readTemperature(), readHumidity());
}

float IntoRobot_DHT::convertFtoC(float f) 
{
	return (f - 32) * 5 / 9;
}

float IntoRobot_DHT::convertCtoF(float c) 
{
	return c * 9 / 5 + 32;
}

float IntoRobot_DHT::convertCtoK(float c) 
{
	return c + 273.15;
}

float IntoRobot_DHT::readHumidity(void) 
{
	float f;

	if (read()) 
	{
		switch (_type) 
		{
			case DHT11:
				f = data[0];
				return f;
			case DHT22:
			case DHT21:
				f = data[0];
				f *= 256;
				f += data[1];
				f /= 10;
				return f;
		}
	}
	return NAN;
}

float IntoRobot_DHT::computeHeatIndex(float tempFahrenheit, float percentHumidity) 
{
// Wikipedia: http://en.wikipedia.org/wiki/Heat_index
	return -42.379 + 
		 2.04901523 * tempFahrenheit + 
		10.14333127 * percentHumidity +
		-0.22475541 * tempFahrenheit * percentHumidity +
		-0.00683783 * pow(tempFahrenheit, 2) +
		-0.05481717 * pow(percentHumidity, 2) + 
		 0.00122874 * pow(tempFahrenheit, 2) * percentHumidity + 
		 0.00085282 * tempFahrenheit * pow(percentHumidity, 2) +
		-0.00000199 * pow(tempFahrenheit, 2) * pow(percentHumidity, 2);
}

float IntoRobot_DHT::computeDewPoint(float tempCelcius, float percentHumidity) 
{
	double a = 17.271;
	double b = 237.7;
	double tC = (a * (float) tempCelcius) / (b + (float) tempCelcius) + log( (float) percentHumidity / 100);
	double Td = (b * tC) / (a - tC);
	return Td;
}


boolean IntoRobot_DHT::read(void) 
{
	uint8_t laststate = HIGH;
	uint8_t counter = 0;
	uint8_t j = 0, i;
	unsigned long currenttime;
    // 2s后才开始读取传感器数据
	currenttime = millis();
	if (currenttime < _lastreadtime) 
	{
		_lastreadtime = 0;
	}
	
	if (!firstreading && ((currenttime - _lastreadtime) < 2000)) 
	{
		return true; //延时(2000 - (currenttime - _lastreadtime));
	}
	firstreading = false;
	_lastreadtime = millis();

	data[0] = data[1] = data[2] = data[3] = data[4] = 0;
  
	digitalWrite(_pin, HIGH); //拉高电平 等待250ms
	delay(250);
	pinMode(_pin, OUTPUT);
	digitalWrite(_pin, LOW);
	delay(20); //开始信号 20ms
	noInterrupts();
	digitalWrite(_pin, HIGH);
	//delayMicroseconds(40);
	pinMode(_pin, INPUT_PULLUP);
    unsigned long lastMicros = 0;
    unsigned long currentMicros = 0;

	for ( i = 0; i < MAXTIMINGS; i++) 
    {
		counter = 0;
        lastMicros = micros();
        while (digitalRead(_pin) == laststate)
        {
            currentMicros = micros();
            counter = currentMicros - lastMicros;
            if(counter >= 250) break;
        }
		laststate = digitalRead(_pin);
		if (counter == 255) break;
        //忽略前3个电平
		if ((i >= 4) && (i%2 == 0))
		{
			data[j/8] <<= 1;
			if (counter > TIME_US_BIT_1)
				data[j/8] |= 1;
			j++;
		}

	}

	interrupts();
    //校验数据
	if ((j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) ) 
    {
		return true;
	}

	return false;
}