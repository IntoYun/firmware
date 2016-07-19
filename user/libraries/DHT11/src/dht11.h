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

#ifndef DHT_H_
#define DHT_H_

#include "application.h"

class DHT11
{
	
	public:
		
	DHT11(u16 pin);
	void begin(void);
	void StartSignal(void);

	char ReadTemperature(bool s);
	u8 ReadHumidity(void);

	char ConvertCtoF(char c); 
	char ConvertFtoC(char f);

	boolean Read(void);
	
	private:
		
	u16 _pin;
	u8  data[5];
	u32 lastReadTime;
	boolean firstReading; 
	
};

#endif 

