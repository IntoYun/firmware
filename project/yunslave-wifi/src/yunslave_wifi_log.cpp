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
#include "yunslave_wifi_log.h"


#define MAX_DEBUG_MESSAGE_LENGTH        512


const char * pathToFileName(const char * path)
{
    size_t i = 0;
    size_t pos = 0;
    char * p = (char *)path;
    while(*p){
        i++;
        if(*p == '/' || *p == '\\') {
            pos = i;
        }
        p++;
    }
    return path+pos;
}

void log_yunslave_int(void)
{
    Serial.begin(115200);   // As fast as possible for bus
}

void log_yunslave_print(const char *fmt, ...)
{
    char _buffer[MAX_DEBUG_MESSAGE_LENGTH];
    va_list args;

    va_start(args, fmt);
    int trunc = vsnprintf(_buffer, arraySize(_buffer), fmt, args);
    YUNSLAVE_DEBUG_STREAM.print(_buffer);
    if (trunc > arraySize(_buffer)) {
        YUNSLAVE_DEBUG_STREAM.print("...");
    }
    va_end(args);
}

void log_yunslave_print_dump(uint8_t *buf, uint16_t len)
{
    if(len > 0) {
        for(int i = 0; i < len-1; i++)
        {
            YUNSLAVE_DEBUG_STREAM.printf("%02x", buf[i]);
            YUNSLAVE_DEBUG_STREAM.print(':');
        }
        YUNSLAVE_DEBUG_STREAM.printf("%02x", buf[len-1]);
        YUNSLAVE_DEBUG_STREAM.print("\r\n");
    }
}

void log_yunslave_failed(const char *file, uint16_t line) {
    log_e("Program failed in file: %s, line: %d\r\n", file, line);
    while(1);
}

