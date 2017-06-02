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
#include "yunslave_wifi_log.h"


at_stateType  at_state = at_statIdle;
bool AtSendDataCmd = false;
bool echoFlag = true;
uint8_t at_cmdLine[at_cmdLenMax];
uint8_t at_dataLine[at_dataLenMax];


static int16_t at_cmdSearch(int8_t cmdLen, uint8_t *pCmd)
{
    int16_t i;

    if(cmdLen == 0) {
        return 0;
    } else if(cmdLen > 0) {
        for(i=1; i<at_cmdNum; i++)
        {
            if(cmdLen == at_cmd[i].at_cmdLen) {
                if(memcmp(pCmd, at_cmd[i].at_cmdName, cmdLen) == 0) {
                    return i;
                }
            }
        }
    }
    return -1;
}

static int8_t at_getCmdLen(uint8_t *pCmd)
{
    uint8_t n,i;

    n = 0;
    i = 128;

    while(i--)
    {
        if((*pCmd == '\r') || (*pCmd == '=') || (*pCmd == '?') || ((*pCmd >= '0')&&(*pCmd <= '9'))) {
            return n;
        } else {
            pCmd++;
            n++;
        }
    }
    return -1;
}

static void at_cmdProcess(uint8_t *pAtRcvData)
{
    char tempStr[32];

    int16_t cmdId;
    int8_t cmdLen;
    uint16_t i;

    cmdLen = at_getCmdLen(pAtRcvData);
    if(cmdLen != -1) {
        cmdId = at_cmdSearch(cmdLen, pAtRcvData);
    } else {
        cmdId = -1;
    }
    if(cmdId != -1) {
        pAtRcvData += cmdLen;
        if(*pAtRcvData == '\r') {
            if(at_cmd[cmdId].at_exeCmd) {
                at_cmd[cmdId].at_exeCmd(cmdId);
            } else {
                at_response_error();
            }
        }
        else if(*pAtRcvData == '?' && (pAtRcvData[1] == '\r'))
        {
            if(at_cmd[cmdId].at_queryCmd) {
                at_cmd[cmdId].at_queryCmd(cmdId);
            } else {
                at_response_error();
            }
        }
        else if((*pAtRcvData == '=') && (pAtRcvData[1] == '?') && (pAtRcvData[2] == '\r'))
        {
            if(at_cmd[cmdId].at_testCmd) {
                at_cmd[cmdId].at_testCmd(cmdId);
            } else {
                at_response_error();
            }
        }
        else if((*pAtRcvData >= '0') && (*pAtRcvData <= '9') || (*pAtRcvData == '='))
        {
            if(at_cmd[cmdId].at_setupCmd) {
                at_cmd[cmdId].at_setupCmd(cmdId, pAtRcvData);
            } else {
                at_response_error();
            }
        } else {
            at_response_error();
        }
    } else {
        at_response_error();
    }
}

void at_parse(void)
{
    static uint8_t atHead[2];
    static uint8_t *pCmdLine = NULL;
    static uint8_t *pDataLine = NULL;
    uint8_t temp;

    while(Serial.available())
    {
        temp = Serial.read();
        if((temp != '\n') && (echoFlag)) {
            Serial.print((char)temp);
        }

        switch(at_state)
        {
            case at_statIdle: //serch "AT" head
                //log_v("at_statIdle\r\n");
                atHead[0] = atHead[1];
                atHead[1] = temp;
                if((memcmp(atHead, "AT", 2) == 0) || (memcmp(atHead, "at", 2) == 0)) {
                    at_state = at_statRecving;
                    pCmdLine = at_cmdLine;
                    atHead[1] = 0x00;
                } else if(temp == '\n') {
                    at_response_error();
                }
                break;

            case at_statRecving: //push receive data to cmd line
                //log_v("at_statRecving\r\n");
                *pCmdLine = temp;
                if(temp == '\n') {
                    pCmdLine++;
                    *pCmdLine = '\0';
                    // deal at command
                    AtSendDataCmd = false;
                    at_cmdProcess(at_cmdLine);
                    if(false == AtSendDataCmd) {
                        at_state = at_statIdle;
                    } else {
                        pDataLine = at_dataLine;
                        at_state = at_statDataSending;
                    }
                } else if(pCmdLine >= &at_cmdLine[at_cmdLenMax - 1]) {
                    at_state = at_statIdle;
                }
                pCmdLine++;
                break;

            case at_statDataSending:
                //log_v("at_statDataSending\r\n");
                *pDataLine = temp;
                if((pDataLine >= &at_dataLine[at_dataSendLen - 1]) || (pDataLine >= &at_dataLine[at_dataLenMax - 1]))
                {
                    at_DataSending(at_dataLine);
                    at_state = at_statIdle;
                } else {
                    pDataLine++;
                }
                break;

            default:
                if(temp == '\n')
                {
                }
                break;
        }
    }
}

/**
  * @brief  get digit form at cmd line.the maybe alter pSrc
  * @param  p_src: at cmd line string
  *         result:the buffer to be placed result
  *         err : err num
  * @retval TRUE:
  *         FALSE:
  */
bool at_get_next_int_dec(char **p_src, int *result, int *err)
{
    char temp[16];
    char i = 0, value = 0;

    memset(temp, 0, sizeof(temp));
    for(i = 0; i < 10; i++) {
        value = (*p_src)[i];
        if((0 == i) && ((value >= '0') && (value <= '9') || (value == '-'))) {
            temp[i] = value;
        } else if((value >= '0') && (value <= '9')) {
            temp[i] = value;
        } else {
            break;
        }
    }
    if((0 == i)) {
        *err = 1;
        return false;
    } else if (10 == i) {
        *err = 2;
        return false;
    } else if((1 == i) && (temp[0] == '-')){
        *err = 3;
        return false;
    }
    *result = atoi(temp);
    *p_src += i;
    return true;
}

/**
  * @brief  get string form at cmd line.the maybe alter pSrc
  * @param  p_dest: the buffer to be placed result
  *         p_src: at cmd line string
  *         max_len :max len of string excepted to get
  * @retval None
  */
int32_t at_data_str_copy(char *p_dest, char **p_src, int32_t max_len)
{
    int n = 0, i = 0;
    char value = 0, start = false;

    for(i = 0, n = 0; n < max_len; i++) {
        value = (*p_src)[i];
        if(value == '\"') {
            if(!start) {
                start = true;
                continue;
            } else {
                i++;
                break;
            }
        }
        if(start) {
            p_dest[n++] = (*p_src)[i];
        }
    }
    *p_src += i;
    return n;
}

/**
  * @brief  Response "OK" to uart.
  * @param  None
  * @retval None
  */
void at_response_ok(void)
{
    Serial.print("\r\nOK\r\n");
}

/**
  * @brief  Response "ERROR" to uart.
  * @param  None
  * @retval None
  */
void at_response_error(void)
{
    Serial.print("\r\nERROR\r\n");
}

/**
  * @brief  Response string.
  *  It is equivalent to at_port_print,if not call at_register_response_func or call at_register_response_func(NULL);
  *  It will run custom response function,if call at_register_response_func and parameter is not NULL.
  * @param  string
  * @retval None
  */
void at_response(const char *str)
{
    Serial.print(str);
}

void at_response_data(const uint8_t *data, uint16_t dataLen)
{
    if(NULL == data) {
        return;
    }

    for(int i = 0; i < dataLen; i++) {
        Serial.write(data[i]);
    }
}

