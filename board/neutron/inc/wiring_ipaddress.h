/**
 ******************************************************************************
 * @file     : wiring_ipadress.h
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

#ifndef   WIRING_IPADDRESS_H_
#define   WIRING_IPADDRESS_H_

//nclude "wiring.h"
#include "wiring_print.h"

// A class to make it easier to handle and pass around IP addresses

class IPAddress : public Printable 
{
    private:
        union 
        {
            uint8_t bytes[4];  // IPv4 address
            uint32_t dword;
        } _address;

        // Access the raw byte array containing the address.  Because this returns a pointer
        // to the internal structure rather than a copy of the address this function should only
        // be used when you know that the usage of the returned uint8_t* will be transient and not
        // stored.
        uint8_t* raw_address(void) { return _address.bytes; };

    public:
        // Constructors
        IPAddress();
        IPAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
        IPAddress(uint32_t address);
        IPAddress(const uint8_t *address);

        virtual ~IPAddress() {}

        // Overloaded cast operator to allow IPAddress objects to be used where a pointer
        // to a four-byte uint8_t array, uint32_t or another IPAddress object is expected.
        bool operator==(uint32_t address);
        operator uint32_t() const { return _address.dword; };
        bool operator==(const IPAddress& addr) const { return _address.dword == addr._address.dword; };
        bool operator==(const uint8_t* addr) const;

        // Overloaded index operator to allow getting and setting individual octets of the address
        uint8_t operator[](int index) const { return _address.bytes[index]; };
        uint8_t& operator[](int index) { return _address.bytes[index]; };

        // Overloaded copy operators to allow initialisation of IPAddress objects from other types
        IPAddress& operator=(const uint8_t *address);
        IPAddress& operator=(uint32_t address);

        virtual size_t printTo(Print& p) const;

        friend class UDP;
        friend class Client;
        friend class Server;
        friend class WiFiClass;
};

const IPAddress INADDR_NONE(0,0,0,0);

#endif /*WIRING_IPADDRESS_H_*/

