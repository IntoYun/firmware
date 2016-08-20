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

  This library is GY30 light sensor module, this module IIC communication

  ******************************************************************************
 */

#include "gy30.h"

GY30::GY30()
{
    devAddress = GY30_ADDRESS;
}

//You need to call this function before using this module,
//but before you need to call 'Wire.begin()' function.
void GY30::begin(void)
{
	Wire.begin();
	delay(10);
  	Write(0x01);
}

//Get light intensity value
float GY30::Read(void)
{
	uint8_t i;
	Write(0x01);
	Write(0x10);
	delay(180);

    Wire.beginTransmission(devAddress); // ID
    Wire.requestFrom((uint8_t)devAddress, (uint8_t)2);

    for(i = 0; i < 2; i++)
    {
        dat[i] = Wire.read();
    }

 	lxData = dat[0];
    lxData = (lxData << 8)+dat[1];
    return (float)lxData/1.2;
}

//Write register address
void GY30::Write(uint8_t regAddress)
{
    Wire.beginTransmission(devAddress);
    Wire.write(regAddress); // send address
    Wire.endTransmission();
}

