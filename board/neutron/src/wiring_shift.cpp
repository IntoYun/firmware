/**
 ******************************************************************************
 * @file     : wiring.cpp
 * @author   : robin
 * @version  : V1.0.0
 * @date     : 6-December-2014
 * @brief    :
 ******************************************************************************
  Copyright (c) 2013-2014 IntoRobot Team.  All right reserved.

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
  ******************************************************************************
 */
#include "wiring_shift.h"
#include "application.h"
/*********************************************************************************
  *Function      : uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder)
  *Description  : Data shift receiving
  *Input          : dataPin:Data input port number
  				  clockPin:The clock port number
  				  bitOrder: 1 or 0
  *Output        : none
  *Return        : return data
  *author        : lz
  *date           : 6-December-2014
  *Others        : Can be used for communication
**********************************************************************************/
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder)
{
    uint8_t value = 0;
    uint8_t i;

    for (i = 0; i < 8; ++i)
    {
        digitalWrite(clockPin, HIGH);
        if (bitOrder == LSBFIRST)
        value |= digitalRead(dataPin) << i;
        else
        value |= digitalRead(dataPin) << (7 - i);
        digitalWrite(clockPin, LOW);
    }
    return value;
}

/*********************************************************************************
  *Function     : void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
  *Description  : Data shift receiving
  *Input        : dataPin:Data input port number
  				  clockPin:The clock port number
  				  bitOrder: 1 or 0
  				  val:To send data
  *Output       : none
  *Return       : none
  *author       : lz
  *date         : 6-December-2014
  *Others       : Can be used for communication
**********************************************************************************/
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        if (bitOrder == LSBFIRST)
        {digitalWrite(dataPin, !!(val & (1 << i)));}
        else
        {digitalWrite(dataPin, !!(val & (1 << (7 - i))));}

        digitalWrite(clockPin, HIGH);
        digitalWrite(clockPin, LOW);
    }

}
