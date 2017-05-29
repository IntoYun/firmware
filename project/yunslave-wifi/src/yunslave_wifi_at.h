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

#ifndef YUNSLAVE_WIFI_AT_H__
#define YUNSLAVE_WIFI_AT_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define  YUNSLAVE_PARAMS_HEADER    0x5aa5f66f

typedef struct
{
    uint32_t header;          // 参数区标志  固定为0x5AA5F66F
    char product_id[33];
    char hardware_version[33];
    char soft_version[33];
}YunSlaveParams_t;

extern YunSlaveParams_t yunSlaveParams;

void loadYunSlaveParams(void);
void saveYunSlaveParams(void);

void at_init(void);
void at_ready(void);
void at_uart_init(uint32_t comm_baud, uint32_t debug_baud);
void at_loop(void);

#ifdef __cplusplus
}
#endif

#endif /* YUNSLAVE_WIFI_AT_H__*/

