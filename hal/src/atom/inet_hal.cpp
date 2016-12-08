/**
 ******************************************************************************
 * @file    inet_hal.c
 * @author  Matthew McGowan
 * @version V1.0.0
 * @date    27-Sept-2014
 * @brief
 ******************************************************************************
  Copyright (c) 2013-2015 IntoRobot Industries, Inc.  All rights reserved.

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

#include <stdio.h>
#include "inet_hal.h"
#include "enums_hal.h"
#include "wiring_ex_process.h"


int inet_gethostbyname(const char* hostname, uint16_t hostnameLen, HAL_IPAddress* out_ip_addr,
        network_interface_t nif, void* reserved)
{
    String tmp;
    uint8_t addr_data[4];
    Process Proc;

    Proc.begin("wifi_get_hostip");
    Proc.addParameter(hostname);
    int res = Proc.run();
    if(res == 0)
    {
        while (Proc.available())
        {
            tmp+=(char)Proc.read();
        }
        sscanf(tmp.c_str(),"%d.%d.%d.%d",&addr_data[0],&addr_data[1],&addr_data[2],&addr_data[3]);
        out_ip_addr->ipv4 = IPADR(addr_data[0], addr_data[1], addr_data[2], addr_data[3]);
        return 0;
    }
    return -1;
}

int inet_ping(const HAL_IPAddress* address, network_interface_t nif, uint8_t nTries,
        void* reserved)
{
    return 0;
}
