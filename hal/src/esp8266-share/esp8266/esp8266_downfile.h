/*
 ESP8266WiFiGeneric.h - esp8266 Wifi support.
 Based on WiFi.h from Ardiono WiFi shield library.
 Copyright (c) 2011-2014 Arduino.  All right reserved.
 Modified by Ivan Grokhotkov, December 2014
 Reworked by Markus Sattler, December 2015

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef ESP8266_DOWNFILE_H_
#define ESP8266_DOWNFILE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OTA_APP_FILE,            //ota应用文件
    DOWNLOAD_FILE,           //其他下载文件
} file_type_t;

typedef enum {
    DOWNSTATUS_SUCCESS = 0,
    DOWNSTATUS_FAIL    = 1,
    DOWNSTATUS_DOWNING = 2,
} down_status_t;

typedef void (*esp8266_downfile_handle_t)(uint8_t *data, size_t len, uint32_t currentSize, uint32_t totalSize);

down_status_t esp8266_downfile(const char *host, const char *uri, const char * md5, file_type_t type);
down_status_t esp8266_getDownfileStatus(void);
void esp8266_set_downfile_handle(esp8266_downfile_handle_t fn);

#ifdef __cplusplus
}
#endif

#endif /* ESP8266_WIFI_GENERIC_H_ */
