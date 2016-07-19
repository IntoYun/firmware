/**
 ******************************************************************************
 * @file     : wiring_ipaddress.cpp
 * @author   : robin
 * @version	 : V1.0.0
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

#include "wiring_ipaddress.h"

/*********************************************************************************
  *Function		: IPAddress::IPAddress()
  *Description	: ip address
  *Input		: none
  *Output		: none
  *Return		: none
  *author		: lz
  *date			: 2015-2-1
  *Others		: none
**********************************************************************************/
IPAddress::IPAddress()
{
    _address.dword = 0;
}

/*********************************************************************************
  *Function		: IPAddress::IPAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
  *Description	: set ip address
  *Input		: 4 bytes of the IP
  *Output		: none
  *Return		: none
  *author		: lz
  *date			: 2015-2-1
  *Others		: none
**********************************************************************************/
IPAddress::IPAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    _address.bytes[0] = first_octet;
    _address.bytes[1] = second_octet;
    _address.bytes[2] = third_octet;
    _address.bytes[3] = fourth_octet;
}

IPAddress::IPAddress(uint32_t address)
{
    _address.dword = address;
}

/*********************************************************************************
  *Function		: IPAddress::IPAddress(const uint8_t* address)
  *Description	: set ip address
  *Input		: address: ip
  *Output		: none
  *Return		: none
  *author		: lz
  *date			: 2015-2-1
  *Others		: none
**********************************************************************************/
IPAddress::IPAddress(const uint8_t* address)
{
    memcpy(_address.bytes, address, sizeof(_address.bytes));
}

IPAddress& IPAddress::operator=(const uint8_t* address)
{
    memcpy(_address.bytes, address, sizeof(_address.bytes));
    return *this;
}

IPAddress& IPAddress::operator=(uint32_t address)
{
    _address.dword = address;
    return *this;
}

bool IPAddress::operator==(uint32_t address)
{
    return (_address.dword == address);
}

bool IPAddress::operator==(const uint8_t* addr) const
{
    return memcmp(addr, _address.bytes, sizeof(_address.bytes)) == 0;
}

/*********************************************************************************
  *Function		: size_t IPAddress::printTo(Print& p) const
  *Description	: Send the IP address
  *Input		: p:ip data
  *Output		: none
  *Return		: none
  *author		: lz
  *date			: 2015-2-1
  *Others		: none
**********************************************************************************/
size_t IPAddress::printTo(Print& p) const
{
    size_t n = 0;
    for (int i =0; i < 3; i++)
    {
        n += p.print(_address.bytes[i], DEC);
        n += p.print('.');
    }
    n += p.print(_address.bytes[3], DEC);
    return n;
}

