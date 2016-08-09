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

#ifndef GY30_H_
#define GY30_H_

#include "application.h"


#define   GY30_ADDRESS 	0x46 >> 1


class GY30
{
	public:
	GY30();
	void begin(void);
	float Read(void);

	void Write(uint8_t regAddress);

	private:
	uint8_t devAddress;
	uint8_t dat[2];
	uint16_t lxData;
};


#endif 

