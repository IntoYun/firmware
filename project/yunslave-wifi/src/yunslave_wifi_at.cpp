/**
******************************************************************************
Copyright (c) 2013-2014 IntoRobot Team.  All right reserved.

robin  2017-05-03

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

#include "yunslave_wifi_config.h"
#include "yunslave_wifi_at_parse.h"
#include "yunslave_wifi_at_cmd.h"
#include "yunslave_wifi_at.h"
#include "yunslave_wifi_log.h"

YunSlaveParams_t yunSlaveParams;

static void initYunSlaveParams(YunSlaveParams_t *params) {
    memset((uint8_t *)params, 0, sizeof(YunSlaveParams_t));
    params->header = YUNSLAVE_PARAMS_HEADER;
}

static void readYunSlaveParams(YunSlaveParams_t *params) {
    uint8_t *point = (uint8_t *)params;

    for(int i = 0; i < sizeof(YunSlaveParams_t); i++)
    {
        point[i] = EEPROM.read(i);
    }

    if( YUNSLAVE_PARAMS_HEADER != params->header ) {
        initYunSlaveParams(params);
    }
}

static void writeYunSlaveParams(YunSlaveParams_t *params) {
    uint8_t *point = (uint8_t *)params;

    for(int i = 0; i < sizeof(YunSlaveParams_t); i++)
    {
        EEPROM.write(i, point[i]);
    }
}

void loadYunSlaveParams(void)
{
    memset(&yunSlaveParams, 0, sizeof(yunSlaveParams));
    //readYunSlaveParams(&yunSlaveParams);
}

void saveYunSlaveParams(void)
{
    //writeYunSlaveParams(&yunSlaveParams);
}

void at_init(void)
{
    loadYunSlaveParams();
}

void at_ready(void)
{
    at_response("\r\nREADY\r\n");
}

void at_uart_init(uint32_t comm_baud, uint32_t debug_baud)
{
    Serial.begin(comm_baud);   //communication serial
    Serial1.begin(debug_baud); //debug serial
}

void at_loop(void)
{
    at_parse();
}

