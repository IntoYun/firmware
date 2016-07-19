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
  Adapted for IntoRobot by Robin, Sept 19, 2015
********************************************************************
*/

#ifndef ADAFRUIT_DHT_H
#define ADAFRUIT_DHT_H

#include "application.h"
#include "math.h"

// how many timing transitions we need to keep track of. 2 * number bits + extra
#define MAXTIMINGS 85

#define DHT11 11
#define DHT22 22
#define DHT21 21
#define AM2301 21

class Adafruit_DHT
{
    private:
		uint8_t data[6];
		uint8_t _pin, _type, _count;
		unsigned long _lastreadtime;
		boolean firstreading;
		float readTemperature();
		float convertFtoC(float);
		float convertCtoF(float);
		float convertCtoK(float);
		float computeHeatIndex(float tempFahrenheit, float percentHumidity);
		float computeDewPoint(float tempCelcius, float percentHumidity);
		float readHumidity(void);
		boolean read(void);

	public:
		Adafruit_DHT(uint8_t pin, uint8_t type, uint8_t count=6);
		void begin(void);
		float getHumidity();
		float getTempCelcius();
		float getTempFarenheit();
		float getTempKelvin();
		float getHeatIndex();
        float getDewPoint();

};
#endif

