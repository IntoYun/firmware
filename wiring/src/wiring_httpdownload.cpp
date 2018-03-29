/**
 * ESP8266HTTPClient.cpp
 *
 * Created on: 02.11.2015
 *
 * Copyright (c) 2015 Markus Sattler. All rights reserved.
 * This file is part of the ESP8266HTTPClient for Arduino.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "intorobot_config.h"

#ifndef configNO_NETWORK

#include <stdlib.h>
#include "base64.h"
#include "stream_string.h"
#include "ota_flash_hal.h"
#include "wiring_ticks.h"
#include "wiring_httpclient.h"
#include "wiring_httpdownload.h"

/*debug switch*/
//#define WIRING_HTTPDOWNLOAD_DEBUG

#ifdef WIRING_HTTPDOWNLOAD_DEBUG
#define WHTTPDOWNLOAD_DEBUG(...)    do {DEBUG(__VA_ARGS__);}while(0)
#define WHTTPDOWNLOAD_DEBUG_D(...)  do {DEBUG_D(__VA_ARGS__);}while(0)
#define WHTTPDOWNLOAD_DEBUG_DUMP    DEBUG_DUMP
#else
#define WHTTPDOWNLOAD_DEBUG(...)
#define WHTTPDOWNLOAD_DEBUG_D(...)
#define WHTTPDOWNLOAD_DEBUG_DUMP
#endif

HTTPDownload::HTTPDownload(void)
    : _size(0)
    , _lastError(0)
    , _progress_callback(NULL)
{
}

HTTPDownload::~HTTPDownload(void)
{
}

http_download_return_t HTTPDownload::update(const String& url)
{
    HTTPClient http;
    http.begin(url);
    return handleUpdate(http, "", UPDATER_MODE_UPDATE);
}

http_download_return_t HTTPDownload::update(const String& url, String md5)
{
    HTTPClient http;
    http.begin(url);
    return handleUpdate(http, md5, UPDATER_MODE_UPDATE);
}

http_download_return_t HTTPDownload::update(const String& host, uint16_t port, const String& uri, String md5)
{
    HTTPClient http;
    http.begin(host, port, uri);
    return handleUpdate(http, md5, UPDATER_MODE_UPDATE);
}

http_download_return_t HTTPDownload::download(const String& url)
{
    HTTPClient http;
    http.begin(url);
    return handleUpdate(http, "", UPDATER_MODE_DOWNLOAD);
}

http_download_return_t HTTPDownload::download(const String& url, String md5)
{
    HTTPClient http;
    http.begin(url);
    return handleUpdate(http, md5, UPDATER_MODE_DOWNLOAD);
}

http_download_return_t HTTPDownload::download(const String& host, uint16_t port, const String& uri, String md5)
{
    HTTPClient http;
    http.begin(host, port, uri);
    return handleUpdate(http, md5, UPDATER_MODE_DOWNLOAD);
}

size_t HTTPDownload::size()
{
    return _size;
}

/**
 * return error code as int
 * @return int error code
 */
int HTTPDownload::getLastError(void)
{
    return _lastError;
}

/**
 * return error code as String
 * @return String error
 */
String HTTPDownload::getLastErrorString(void)
{
    if(_lastError == 0) {
        return String(); // no error
    }

    // error from Update class
    if(_lastError > 0) {
        StreamString error;
        Update.printError(error);
        error.trim(); // remove line ending
        return String(F("Update error: ")) + error;
    }

    // error from http client
    if(_lastError > -100) {
        return String(F("HTTP error: ")) + HTTPClient::errorToString(_lastError);
    }

    switch(_lastError) {
        case HTTP_DE_TOO_LESS_SPACE:
            return F("To less space");
        case HTTP_DE_SERVER_NOT_REPORT_SIZE:
            return F("Server not Report Size");
        case HTTP_DE_SERVER_FILE_NOT_FOUND:
            return F("File not Found (404)");
        case HTTP_DE_SERVER_FORBIDDEN:
            return F("Forbidden (403)");
        case HTTP_DE_SERVER_WRONG_HTTP_CODE:
            return F("Wrong HTTP code");
        case HTTP_DE_SERVER_FAULTY_MD5:
            return F("Faulty MD5");
        case HTTP_DE_BIN_VERIFY_HEADER_FAILED:
            return F("Verify bin header failed");
        case HTTP_DE_BIN_FOR_WRONG_FLASH:
            return F("bin for wrong flash size");
    }

    return String();
}

void HTTPDownload::onProgress(THandlerFunction_Progress fn)
{
    _progress_callback = fn;
}

/**
 *
 * @param http HTTPClient *
 * @param currentVersion const char *
 * @return http_download_return_t
 */
http_download_return_t HTTPDownload::handleUpdate(HTTPClient& http, String md5, updater_mode_t mode)
{
    http_download_return_t ret = HTTP_DOWNLOAD_FAILED;

    // use HTTP/1.0 for update since the update handler not support any transfer Encoding
    http.useHTTP10(true);
    http.setReuse(true); /// keep-alive
    http.setTimeout(4000);
    http.setUserAgent(F("Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.101 Safari/537.36"));
    http.addHeader(F("Cache-Control"), F("no-cache"));
    http.addHeader(F("Accept"), F("*/*"));
    http.addHeader(F("Accept-Encoding"), F("gzip,deflate"));
    http.addHeader(F("Accept-Language"), F("zh-CN,eb-US;q=0.8"));

    const char * headerkeys[] = { "X-Md5" };
    size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);

    // track these headers
    http.collectHeaders(headerkeys, headerkeyssize);

    int code = http.GET();
    int len = http.getSize();

    if(code <= 0) {
        WHTTPDOWNLOAD_DEBUG("[httpDownload] HTTP error: %s\r\n", http.errorToString(code).c_str());
        _lastError = code;
        http.end();
        return HTTP_DOWNLOAD_FAILED;
    }

    WHTTPDOWNLOAD_DEBUG("[httpDownload] Header read fin.\r\n");
    WHTTPDOWNLOAD_DEBUG("[httpDownload] Server header:\r\n");
    WHTTPDOWNLOAD_DEBUG("[httpDownload] - code: %d\r\n", code);
    WHTTPDOWNLOAD_DEBUG("[httpDownload] - len: %d\r\n", len);

    if(http.hasHeader("X-Md5")) {
        WHTTPDOWNLOAD_DEBUG("[httpDownload] - MD5: %s\r\n", http.header("X-Md5").c_str());
    }

    switch(code) {
        case HTTP_CODE_OK:  ///< OK (Start Update)
            if(len > 0) {
                bool startUpdate = true;
                if((UPDATER_MODE_UPDATE == mode) && (len > (int)HAL_OTA_FlashLength())) {
                    WHTTPDOWNLOAD_DEBUG("[httpDownload] FreeSketchSpace to low (%d) needed: %d\r\n", HAL_OTA_FlashLength(), len);
                    startUpdate = false;
                }

                if(!startUpdate) {
                    _lastError = HTTP_DE_TOO_LESS_SPACE;
                    ret = HTTP_DOWNLOAD_FAILED;
                } else {
                    TCPClient * tcp = http.getStreamPtr();

                    delay(100);
                    WHTTPDOWNLOAD_DEBUG("[httpDownload] runUpdate flash...\r\n");

                    if(runUpdate(*tcp, len, http.header("X-Md5"), mode)) {
                        ret = HTTP_DOWNLOAD_OK;
                        WHTTPDOWNLOAD_DEBUG("[httpDownload] Update ok\r\n");
                        http.end();
                    } else {
                        ret = HTTP_DOWNLOAD_FAILED;
                        WHTTPDOWNLOAD_DEBUG("[httpDownload] Update failed\r\n");
                    }
                }
            } else {
                _lastError = HTTP_DE_SERVER_NOT_REPORT_SIZE;
                ret = HTTP_DOWNLOAD_FAILED;
                WHTTPDOWNLOAD_DEBUG("[httpDownload] Content-Length is 0 or not set by Server?!\r\n");
            }
            break;
        case HTTP_CODE_NOT_MODIFIED:
            ///< Not Modified (No updates)
            ret = HTTP_DOWNLOAD_NO_UPDATES;
            break;
        case HTTP_CODE_NOT_FOUND:
            _lastError = HTTP_DE_SERVER_FILE_NOT_FOUND;
            ret = HTTP_DOWNLOAD_FAILED;
            break;
        case HTTP_CODE_FORBIDDEN:
            _lastError = HTTP_DE_SERVER_FORBIDDEN;
            ret = HTTP_DOWNLOAD_FAILED;
            break;
        default:
            _lastError = HTTP_DE_SERVER_WRONG_HTTP_CODE;
            ret = HTTP_DOWNLOAD_FAILED;
            WHTTPDOWNLOAD_DEBUG("[httpDownload] HTTP Code is (%d)\r\n", code);
            break;
    }

    http.end();
    return ret;
}

/**
 * write Update to flash
 * @param in Stream&
 * @param size uint32_t
 * @param md5 String
 * @return true if Update ok
 */
bool HTTPDownload::runUpdate(Stream& in, uint32_t size, String md5, updater_mode_t mode)
{
    StreamString error;

    if(!Update.begin(size, mode)) {
        _lastError = Update.getError();
        Update.printError(error);
        error.trim(); // remove line ending
        WHTTPDOWNLOAD_DEBUG("[httpDownload] Update.begin failed! (%s)\r\n", error.c_str());
        return false;
    }

    if(NULL != _progress_callback) {
        Update.onProgress(_progress_callback);
    }

    if(md5.length()) {
        if(!Update.setMD5(md5.c_str())) {
            _lastError = HTTP_DE_SERVER_FAULTY_MD5;
            WHTTPDOWNLOAD_DEBUG("[httpDownload] Update.setMD5 failed! (%s)\r\n", md5.c_str());
            return false;
        }
    }

    if(Update.writeStream(in) != size) {
        _lastError = Update.getError();
        Update.printError(error);
        error.trim(); // remove line ending
        WHTTPDOWNLOAD_DEBUG("[httpDownload] Update.writeStream failed! (%s)\r\n", error.c_str());
        return false;
    }

    _size = Update.size();

    if(!Update.end()) {
        _size = 0;
        _lastError = Update.getError();
        Update.printError(error);
        error.trim(); // remove line ending
        WHTTPDOWNLOAD_DEBUG("[httpDownload] Update.end failed! (%s)\r\n", error.c_str());
        return false;
    }

    return true;
}

#endif
