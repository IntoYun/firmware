/**
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
#include "lora_gateway_mgt.h"
#include "lora_gateway_log.h"

// ----------------------------------------------------------------------------
// Ths function gateway_mgt is called in the UDP Receive function after
// all well-known LoRa Gateway messages are scanned.
//
// As part of this function we will listen for another set of messages
// that is defined in loraModem.h.
// All opCodes start with 0x1y for at leaving opcodes 0x00 to 0x0F to the
// pure Gateway protocol
//
// Incoming mesage format:
// buf[0]-buf[2], These are 0x00 or dont care
// buf[3], contains opcode
// buf[4]-buf[7], contains parameter max. 4 bytes.
//
// Upstream Message format:
//
// ----------------------------------------------------------------------------
void gateway_mgt(uint8_t *buffer, uint8_t len)
{
    uint8_t opcode = buffer[3];
    switch (opcode) {
        case MGT_RESET:
            log_v("gateway_mgt:: RESET\r\n");
            break;

        case MGT_SET_SF:
            log_v("gateway_mgt:: SET SF\r\n");
            break;

        case MGT_SET_FREQ:
            log_v("gateway_mgt:: SET FREQ\r\n");
            break;

        default:
            log_v("gateway_mgt:: Unknow\r\n");
            break;
    }
}

