/**
 ******************************************************************************
 Copyright (c) 2015 IntoRobot Industries, Inc.  All rights reserved.

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

#include "testapi.h"

test(time_format) {
    const char* format;

    API_COMPILE(Time.timeStr());
    API_COMPILE(Time.timeStr(1234));
    API_COMPILE(Time.format(TIME_FORMAT_DEFAULT));
    API_COMPILE(Time.format(TIME_FORMAT_ISO8601_FULL));
    API_COMPILE(Time.format(1234, TIME_FORMAT_ISO8601_FULL));
    API_COMPILE(Time.setFormat(TIME_FORMAT_ISO8601_FULL));
    API_COMPILE((void)(format=Time.getFormat()));
    API_COMPILE(Time.format(1234, "abcd"))
}

test(time_zone) {
    float zone;
    time_t time;

    API_COMPILE(Time.zone(1.0f));
    API_COMPILE(Time.zone(-5));
    API_COMPILE(zone=Time.zone());
    API_COMPILE(time=Time.now());
    API_COMPILE(time=Time.local());
    zone+=1.0; // avoid unused warning
    (void)time++; // avoid unused warning
}

