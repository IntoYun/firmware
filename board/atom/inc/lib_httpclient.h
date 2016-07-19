/**
 ******************************************************************************
 * @file     : lib_httpclient.h
 * @author   : robin
 * @version  : V1.0.0
 * @date     : 6-December-2014
 * @brief    :     
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

#ifndef   HTTPCLIENT_H_
#define   HTTPCLIENT_H_

#include "lib_process.h"

class HttpClient : public Process 
{
    public:
        HttpClient();

        unsigned int get(String &url);
        unsigned int get(const char * url);
        void getAsynchronously(String &url);
        void getAsynchronously(const char * url);
        unsigned int post(String &url, String &data);
        unsigned int post(const char * url, const char * data);
        void postAsynchronously(String &url, String &data);
        void postAsynchronously(const char * url, const char * data);
        void setHeader(String &header);
        void setHeader(const char * header);
        boolean ready(void);
        unsigned int getResult(void);
        void noCheckSSL(void);
        void checkSSL(void);

    private:
        boolean insecure;

    private:
        void addHeader(void);
        String header;
};

#endif /* HTTPCLIENT_H_ */
