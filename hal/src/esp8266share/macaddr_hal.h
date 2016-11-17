
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

#ifndef MACADDR_HAL_H_
#define MACADDR_HAL_H_

#define FLASH_MAC_HEADER      0x5aa5f88f

typedef struct __attribute__((__packed__))  _mac_param_t {
    uint32_t header;
    uint8_t stamac_addrs[6];
    uint8_t apmac_addrs[6];
} mac_param_t;

int wlan_set_macaddr_from_flash(uint8_t *stamacaddr, uint8_t *apmacaddr);
int wlan_set_macaddr_when_init(void);

#endif /*MACADDR_HAL_H_*/
