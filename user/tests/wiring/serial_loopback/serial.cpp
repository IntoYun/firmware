/**
  ******************************************************************************
 Copyright (c) 2013-2015 IntoRobot Industries, Inc.  All rights reserved.

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

#include "application.h"
#include "unit-test/unit-test.h"

/*
 * Serial Test requires TX to be jumpered to RX as follows:
 * valid for both Core and Photon
 *           WIRE
 * (TX) --==========-- (RX)
 *
 */

#ifdef FIRMWARE_CONFIG_WIRING_USBSERIAL
test(SERIALUSB_ReadWriteSucceedsWithUserIntervention) {
    //The following code will test all the important USB Serial routines
    char test[] = "hello";
    char message[10];
    // when
    SerialUSB.print("Type the following message and press Enter : ");
    SerialUSB.println(test);
    serialReadLine(&SerialUSB, message, 9, 10000);//10 sec timeout
    SerialUSB.println("");
    // then
    assertTrue(strncmp(test, message, 5)==0);
}
#endif

test(SERIAL_ReadWriteSucceedsInLoopbackWithTxRxShorted) {
    //The following code will test all the important USART Serial1 routines
    char test[] = "hello";
    char message[10];
    // when
    Serial.begin(9600);
    Serial.println(test);
    serialReadLine(&Serial, message, 9, 1000);//1 sec timeout
    Serial.end();
    // then
    assertTrue(strncmp(test, message, 5)==0);
}

test(SERIAL_ReadWriteParity8N1SucceedsInLoopbackWithTxRxShorted) {
    //The following code will test all the important USART Serial1 routines
    char test[] = "hello";
    char message[10];
    // when
    Serial.begin(9600, SERIAL_8N1);
    Serial.println(test);
    serialReadLine(&Serial, message, 9, 1000);//1 sec timeout
    Serial.end();
    // then
    assertTrue(strncmp(test, message, 5)==0);
}

test(SERIAL_ReadWriteParity8E1SucceedsInLoopbackWithTxRxShorted) {
    //The following code will test all the important USART Serial1 routines
    char test[] = "hello";
    char message[10];
    // when
    Serial.begin(9600, SERIAL_8E1);
    Serial.println(test);
    serialReadLine(&Serial, message, 9, 1000);//1 sec timeout
    Serial.end();
    // then
    assertTrue(strncmp(test, message, 5)==0);
}

test(SERIAL_ReadWriteParity8O1SucceedsInLoopbackWithTxRxShorted) {
    //The following code will test all the important USART Serial1 routines
    char test[] = "hello";
    char message[10];
    // when
    Serial.begin(9600, SERIAL_8O1);
    Serial.println(test);
    serialReadLine(&Serial, message, 9, 1000);//1 sec timeout
    Serial.end();
    // then
    assertTrue(strncmp(test, message, 5)==0);
}

test(SERIAL_ReadWriteParity8N2SucceedsInLoopbackWithTxRxShorted) {
    //The following code will test all the important USART Serial1 routines
    char test[] = "hello";
    char message[10];
    // when
    Serial.begin(9600, SERIAL_8N2);
    Serial.println(test);
    serialReadLine(&Serial, message, 9, 1000);//1 sec timeout
    Serial.end();
    // then
    assertTrue(strncmp(test, message, 5)==0);
}

test(SERIAL_ReadWriteParity8E2SucceedsInLoopbackWithTxRxShorted) {
    //The following code will test all the important USART Serial1 routines
    char test[] = "hello";
    char message[10];
    // when
    Serial.begin(9600, SERIAL_8E2);
    Serial.println(test);
    serialReadLine(&Serial, message, 9, 1000);//1 sec timeout
    Serial.end();
    // then
    assertTrue(strncmp(test, message, 5)==0);
}

test(SERIAL_ReadWriteParity8O2SucceedsInLoopbackWithTxRxShorted) {
    //The following code will test all the important USART Serial1 routines
    char test[] = "hello";
    char message[10];
    // when
    Serial.begin(9600, SERIAL_8O2);
    Serial.println(test);
    serialReadLine(&Serial, message, 9, 1000);//1 sec timeout
    Serial.end();
    // then
    assertTrue(strncmp(test, message, 5)==0);
}

test(SERIAL_ReadWriteParity9N1SucceedsInLoopbackWithTxRxShorted) {
    //The following code will test all the important USART Serial1 routines
    char test[] = "hello";
    char message[10];
    // when
    Serial.begin(9600, SERIAL_9N1);
    Serial.println(test);
    serialReadLine(&Serial, message, 9, 1000);//1 sec timeout
    Serial.end();
    // then
    assertTrue(strncmp(test, message, 5)==0);
}

test(SERIAL_ReadWriteParity9N2SucceedsInLoopbackWithTxRxShorted) {
    //The following code will test all the important USART Serial1 routines
    char test[] = "hello";
    char message[10];
    // when
    Serial.begin(9600, SERIAL_9N2);
    Serial.println(test);
    serialReadLine(&Serial, message, 9, 1000);//1 sec timeout
    Serial.end();
    // then
    assertTrue(strncmp(test, message, 5)==0);
}

