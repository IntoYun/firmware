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

#ifndef WIRING_HTTPDOWNLOAD_H_
#define WIRING_HTTPDOWNLOAD_H_

#include "intorobot_config.h"

#ifndef configNO_NETWORK

#include "md5_builder.h"
#include "system_update.h"
#include "wiring_ticks.h"
#include "wiring_httpclient.h"

// note we use HTTP client errors too so we start at 100
#define HTTP_DE_TOO_LESS_SPACE              (-100)
#define HTTP_DE_SERVER_NOT_REPORT_SIZE      (-101)
#define HTTP_DE_SERVER_FILE_NOT_FOUND       (-102)
#define HTTP_DE_SERVER_FORBIDDEN            (-103)
#define HTTP_DE_SERVER_WRONG_HTTP_CODE      (-104)
#define HTTP_DE_SERVER_FAULTY_MD5           (-105)
#define HTTP_DE_BIN_VERIFY_HEADER_FAILED    (-106)
#define HTTP_DE_BIN_FOR_WRONG_FLASH         (-107)

typedef enum {
    HTTP_DOWNLOAD_FAILED,
    HTTP_DOWNLOAD_NO_UPDATES,
    HTTP_DOWNLOAD_OK
} http_download_return_t;

class HTTPDownload
{
public:
    typedef void (*THandlerFunction_Progress)(uint8_t *data, size_t len, uint32_t currentSize, uint32_t totalSize);

    HTTPDownload(void);
    ~HTTPDownload(void);

    http_download_return_t update(const String& url);
    http_download_return_t update(const String& url, String md5);
    http_download_return_t update(const String& host, uint16_t port, const String& uri, String md5);

    http_download_return_t download(const String& url);
    http_download_return_t download(const String& url, String md5);
    http_download_return_t download(const String& host, uint16_t port, const String& uri, String md5);

    size_t size();
    int getLastError(void);
    String getLastErrorString(void);
    void onProgress(THandlerFunction_Progress fn);

protected:
    http_download_return_t handleUpdate(HTTPClient& http, String md5, updater_mode_t mode);
    bool runUpdate(Stream& in, uint32_t size, String md5, updater_mode_t mode);

    uint32_t _size;
    int _lastError;
    THandlerFunction_Progress _progress_callback;
};

#endif
#endif /* WIRING_HTTPDOWNLOAD_H_ */
