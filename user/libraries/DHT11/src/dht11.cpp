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

  This library is DHT11 temperature and humidity sensor module
  
  ******************************************************************************
 */

#include "dht11.h"

// Define the data pins
DHT11::DHT11(u16 pin)
{
    _pin = pin;
    firstReading = true;
}

//You need to call before using
void DHT11::begin(void)
{
    pinMode(_pin,OUTPUT);
    digitalWrite(_pin, HIGH);
    lastReadTime = 0;
}


void DHT11::StartSignal(void)
{
    digitalWrite(_pin,LOW);
    delay(20);
    digitalWrite(_pin,HIGH);
}

char DHT11::ConvertCtoF(char c)
{
    return (char)c * 9 / 5 + 32;
    
}

char DHT11::ConvertFtoC(char f)
{
    return (char)(f - 32) * 5 / 9;
}

//Temperature data
char DHT11::ReadTemperature(bool s)
{
    char temp = 0;
    
    if(Read())
    {
        temp = data[2];
        
        if(s)
        {
          	temp = ConvertCtoF(temp);
        }
        
    }

    return temp;
}

//Get humidity data
u8 DHT11::ReadHumidity(void)
{
    u8 temp = 0;

    if(Read())
    {
        temp = data[0];
    }

    return temp;
}



// Data Acquisition
boolean DHT11::Read(void)
{
     uint8_t i,timeOut = 0;
     uint8_t laststate = HIGH;
     uint8_t j = 0;

     u32 currentTime;

     currentTime = millis();

	if (currentTime < lastReadTime) 
	{
		// ie there was a rollover
		lastReadTime = 0;
	}
	if (!firstReading && ((currentTime - lastReadTime) < 2000)) 
	{
		return true; // return last correct measurement
		//delay(2000 - (currenttime - _lastreadtime));
	}
  
  	firstReading = false;

  	lastReadTime = millis();

    data[0] = data[1] = data[2] = data[3] = data[4] = 0;

    noInterrupts();
     
    StartSignal();

    pinMode(_pin,INPUT_PULLUP);

    // read in timings
    for(i = 0; i < 83; i++) 
    {
        timeOut = 0;
        
        while (digitalRead(_pin) == laststate) 
        {
          timeOut++;
          delayMicroseconds(1);
          if (timeOut == 255) 
          {
             break;
          }
        }
        
        laststate = digitalRead(_pin);

        if (timeOut == 255) break;

        // ignore first 3 transitions
        if ((i >= 4) && (i%2 == 0)) // The fourth level as well as the high level to store data
        {
              // shove each bit into the storage bytes
              data[j/8] <<= 1;
              
              if(timeOut > 40) // data 1
              {
                data[j/8] |= 1;
              }
              
              j++;
        }

    }

    pinMode(_pin,OUTPUT);
    digitalWrite(_pin,1);

	interrupts();
	
    if((j >= 40) && ((data[0] + data[1] + data[2] + data[3]) == data[4]))
    {
        return true;
    }
    else
    {
        return false;
    }
    
}






