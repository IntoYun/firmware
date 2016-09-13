/**
 ******************************************************************************
 * @file    spark.cpp
 * @authors Matthew McGowan
 * @date    13 January 2015
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

test(api_spark_variable) {

    int valueInt = 0;
    int32_t valueInt32 = 0;
    uint32_t valueUint32 = 0;
    double valueDouble = 0;
    const char* constValueString = "oh no!";
    char valueString[] = "oh yeah!";

    String valueSmartString(valueString);
    bool boolValue = true;
    uint8_t uint8Value = 1;

    // API_COMPILE(IntoRobot.variable("mybool", boolValue));
    // API_COMPILE(IntoRobot.variable("mybool", uint8Value));

    // API_COMPILE(IntoRobot.variable("myint", &valueInt, INT));

    // API_COMPILE(IntoRobot.variable("mydouble", &valueDouble, DOUBLE));

    // API_COMPILE(IntoRobot.variable("mystring", valueString, STRING));
    // This doesn't compile and shouldn't
    // API_COMPILE(IntoRobot.variable("mystring", &valueString, STRING));

    // API_NO_COMPILE(IntoRobot.variable("mystring", constValueString, STRING));

    // API_COMPILE(IntoRobot.variable("mystring", &valueSmartString, STRING));

    // API_COMPILE(IntoRobot.variable("mystring", &valueInt32, INT));
    // API_COMPILE(IntoRobot.variable("mystring", &valueUint32, INT));

    // API_NO_COMPILE(IntoRobot.variable("mystring", valueUint32));
    // API_COMPILE(IntoRobot.variable("mystring", valueInt));
    // API_COMPILE(IntoRobot.variable("mystring", valueInt32));
    // API_COMPILE(IntoRobot.variable("mystring", valueUint32));

    // API_COMPILE(IntoRobot.variable("mystring", valueDouble));

    // API_COMPILE(IntoRobot.variable("mystring", valueString));
    // API_COMPILE(IntoRobot.variable("mystring", constValueString));
    // API_COMPILE(IntoRobot.variable("mystring", valueSmartString));
    
    // This should gives a compiler error about too long name
    // API_COMPILE(IntoRobot.variable("mystring123456789", valueString));

}

test(api_spark_function) {
    int (*handler)(String) = NULL;

    API_COMPILE(IntoRobot.function("name", handler));

    // This should gives a compiler error about too long name
    //API_COMPILE(IntoRobot.function("superlongfunctionname", handler));

    // Length not checked until run time
    API_COMPILE(IntoRobot.function(String("name"), handler));
    const char *longname = "superlongfunctionname";
    API_COMPILE(IntoRobot.function(longname, handler));

    class MyClass {
      public:
        int handler(String arg) { return 0; }
    } myObj;
    API_COMPILE(IntoRobot.function("name", &MyClass::handler, &myObj));
}

test(api_spark_publish) {

    API_COMPILE(IntoRobot.publish("public event name"));

    API_COMPILE(IntoRobot.publish("public event name", "event data"));

    API_COMPILE(IntoRobot.publish("public event name", "event data"));

    API_COMPILE(IntoRobot.publish("public event name", "event data", 60));

    API_COMPILE(IntoRobot.publish("public event name", "event data", 60, PUBLIC));

    API_COMPILE(IntoRobot.publish("private event name", "event data", 60, PRIVATE));

    API_COMPILE(IntoRobot.publish("public event name", PRIVATE));

    API_COMPILE(IntoRobot.publish("public event name", "event data", PRIVATE));

    API_COMPILE(IntoRobot.publish("public event name", PUBLIC));


    API_COMPILE(IntoRobot.publish(String("public event name")));

    API_COMPILE(IntoRobot.publish(String("public event name"), String("event data")));

    API_COMPILE(IntoRobot.publish(String("public event name"), String("event data")));

    API_COMPILE(IntoRobot.publish(String("public event name"), String("event data"), 60));

    API_COMPILE(IntoRobot.publish(String("public event name"), String("event data"), 60, PUBLIC));

    API_COMPILE(IntoRobot.publish(String("public event name"), String("event data"), 60, PRIVATE));

    API_COMPILE(IntoRobot.publish(String("public event name"), PRIVATE));

    API_COMPILE(IntoRobot.publish(String("public event name"), String("event data"), PRIVATE));

    API_COMPILE(IntoRobot.publish(String("public event name"), PUBLIC));

}

test(api_spark_subscribe) {

    void (*handler)(const char *event_name, const char *data) = NULL;

    API_COMPILE(IntoRobot.subscribe("name", handler));

    API_COMPILE(IntoRobot.subscribe("name", handler, MY_DEVICES));

    API_COMPILE(IntoRobot.subscribe("name", handler, "1234"));


    API_COMPILE(IntoRobot.subscribe(String("name"), handler));

    API_COMPILE(IntoRobot.subscribe(String("name"), handler, MY_DEVICES));

    API_COMPILE(IntoRobot.subscribe(String("name"), handler, "1234"));

    class MyClass {
      public:
        void handler(const char *event_name, const char *data) { }
    } myObj;

    API_COMPILE(IntoRobot.subscribe("name", &MyClass::handler, &myObj));

    API_COMPILE(IntoRobot.subscribe("name", &MyClass::handler, &myObj, MY_DEVICES));

    API_COMPILE(IntoRobot.subscribe("name", &MyClass::handler, &myObj, "1234"));

}

test(api_spark_sleep) {

    API_COMPILE(System.sleep(60));

    API_COMPILE(System.sleep(SLEEP_MODE_WLAN, 60));

    API_COMPILE(System.sleep(SLEEP_MODE_DEEP, 60));

    API_COMPILE(System.sleep(A0, CHANGE));
    API_COMPILE(System.sleep(A0, RISING));
    API_COMPILE(System.sleep(A0, FALLING));
    API_COMPILE(System.sleep(A0, FALLING, 20));

    API_COMPILE(System.sleep(SLEEP_MODE_DEEP));

}

test(api_spark_connection) {
    bool connected = false;
    API_COMPILE(connected=Particle.connected());
    connected++;
    API_COMPILE(IntoRobot.connect());
    API_COMPILE(IntoRobot.disconnect());
    API_COMPILE(IntoRobot.process());

#if HAL_PLATFORM_CLOUD_UDP
    API_COMPILE(IntoRobot.keepAlive(20 * 60));
#endif
}

test(api_spark_deviceID) {
    String id;
    API_COMPILE(id = IntoRobot.deviceID());
}

test(api_spark_syncTime) {
    API_COMPILE(IntoRobot.syncTime());
}
