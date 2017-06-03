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

#ifndef YUNSLAVE_WIFI_AT_PARSE_H__
#define YUNSLAVE_WIFI_AT_PARSE_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum{
    at_statIdle,
    at_statRecving,
    at_statDataSending,
}at_stateType;

extern bool echoFlag;
extern bool AtSendDataCmd;

void at_parse(void);
bool at_get_next_int_dec(char **p_src, int *result, int *err);
int32_t at_data_str_copy(char *p_dest, char **p_src, int32_t max_len);
void at_response_ok(void);
void at_response_error(void);
void at_response(const char *str);
void at_response_data(const uint8_t *data, uint16_t dataLen);

#define at_cmdLenMax      128
#define at_dataLenMax     2048

#ifdef __cplusplus
}
#endif

#endif /* YUNSLAVE_WIFI_AT_PARSE_H__*/

