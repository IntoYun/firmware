/**
 ******************************************************************************
  Copyright (c) 2016 IntoRobot Industries, Inc.  All rights reserved.

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

#if FIRMWARE_CONFIG_WIRING_CAN

test(CAN_INTERFACE1_ReceivesTransmittedMessage) {
    CANChannel can(HAL_CAN_INTERFACE1);
    can.begin(500000, CAN_TEST_MODE);

    CANMessage tx;
    tx.id = 0x101;
    tx.len = 1;
    tx.data[0] = 10;
    can.transmit(tx);
    delay(1);
    CANMessage rx;
    bool hasMessage = can.receive(rx);
    can.end();

    assertEqual(hasMessage, true);
    assertEqual(rx.id, 0x101);
    assertEqual(rx.len, 1);
    assertEqual(rx.data[0], 10);
}

test(CAN_INTERFACE1_ReceivesAcceptedMessage) {
    CANChannel can(HAL_CAN_INTERFACE1);
    can.begin(500000, CAN_TEST_MODE);
    // Accept only standard message 0x100
    can.addFilter(0x100, 0x7FF);

    CANMessage tx;
    tx.id = 0x100;
    tx.len = 1;
    tx.data[0] = 10;
    can.transmit(tx);
    delay(1);
    CANMessage rx;
    bool hasMessage = can.receive(rx);
    can.end();

    assertEqual(hasMessage, true);
}

test(CAN_INTERFACE1_DoesntReceiveFilteredMessage) {
    CANChannel can(HAL_CAN_INTERFACE1);
    can.begin(500000, CAN_TEST_MODE);
    // Accept only standard message 0x100
    can.addFilter(0x100, 0x7FF);

    CANMessage tx;
    tx.id = 0x101;
    tx.len = 1;
    tx.data[0] = 10;
    can.transmit(tx);
    delay(1);
    CANMessage rx;
    bool hasMessage = can.receive(rx);
    can.end();

    assertEqual(hasMessage, false);
}

test(CAN_INTERFACE1_ReceivesTransmittedExtendedMessage) {
    CANChannel can(HAL_CAN_INTERFACE1);
    can.begin(500000, CAN_TEST_MODE);

    CANMessage tx;
    tx.extended = true;
    tx.id = 0x3000;
    tx.len = 1;
    tx.data[0] = 10;
    can.transmit(tx);
    delay(1);
    CANMessage rx;
    bool hasMessage = can.receive(rx);
    can.end();

    assertEqual(hasMessage, true);
    assertEqual(rx.extended, true);
    assertEqual(rx.id, 0x3000);
    assertEqual(rx.len, 1);
    assertEqual(rx.data[0], 10);
}

test(CAN_INTERFACE1_ReceivesAcceptedExtendedMessage) {
    CANChannel can(HAL_CAN_INTERFACE1);
    can.begin(500000, CAN_TEST_MODE);
    // Accept only extended message 0x3000
    can.addFilter(0x3000, 0x7FF, CAN_FILTER_EXTENDED);

    CANMessage tx;
    tx.extended = true;
    tx.id = 0x3000;
    tx.len = 1;
    tx.data[0] = 10;
    can.transmit(tx);
    delay(1);
    CANMessage rx;
    bool hasMessage = can.receive(rx);
    can.end();

    assertEqual(hasMessage, true);
}

test(CAN_INTERFACE1_DoesntReceiveFilteredExtendedMessage) {
    CANChannel can(HAL_CAN_INTERFACE1);
    can.begin(500000, CAN_TEST_MODE);
    // Accept only extended message 0x3000
    can.addFilter(0x3000, 0x7FF, CAN_FILTER_EXTENDED);

    CANMessage tx;
    tx.extended = true;
    tx.id = 0x3001;
    tx.len = 1;
    tx.data[0] = 10;
    can.transmit(tx);
    delay(1);
    CANMessage rx;
    bool hasMessage = can.receive(rx);
    can.end();

    assertEqual(hasMessage, false);
}

#endif

#if FIRMWARE_CONFIG_WIRING_CAN1

test(CAN_INTERFACE2_ReceivesTransmittedMessage) {
    CANChannel can(HAL_CAN_INTERFACE2);
    can.begin(500000, CAN_TEST_MODE);

    CANMessage tx;
    tx.id = 0x101;
    tx.len = 1;
    tx.data[0] = 10;
    can.transmit(tx);
    delay(1);
    CANMessage rx;
    bool hasMessage = can.receive(rx);
    can.end();

    assertEqual(hasMessage, true);
    assertEqual(rx.id, 0x101);
    assertEqual(rx.len, 1);
    assertEqual(rx.data[0], 10);
}

test(CAN_INTERFACE2_ReceivesAcceptedMessage) {
    CANChannel can(HAL_CAN_INTERFACE2);
    can.begin(500000, CAN_TEST_MODE);
    // Accept only standard message 0x100
    can.addFilter(0x100, 0x7FF);

    CANMessage tx;
    tx.id = 0x100;
    tx.len = 1;
    tx.data[0] = 10;
    can.transmit(tx);
    delay(1);
    CANMessage rx;
    bool hasMessage = can.receive(rx);
    can.end();

    assertEqual(hasMessage, true);
}

test(CAN_INTERFACE2_DoesntReceiveFilteredMessage) {
    CANChannel can(HAL_CAN_INTERFACE2);
    can.begin(500000, CAN_TEST_MODE);
    // Accept only standard message 0x100
    can.addFilter(0x100, 0x7FF);

    CANMessage tx;
    tx.id = 0x101;
    tx.len = 1;
    tx.data[0] = 10;
    can.transmit(tx);
    delay(1);
    CANMessage rx;
    bool hasMessage = can.receive(rx);
    can.end();

    assertEqual(hasMessage, false);
}

test(CAN_INTERFACE2_ReceivesTransmittedExtendedMessage) {
    CANChannel can(HAL_CAN_INTERFACE2);
    can.begin(500000, CAN_TEST_MODE);

    CANMessage tx;
    tx.extended = true;
    tx.id = 0x3000;
    tx.len = 1;
    tx.data[0] = 10;
    can.transmit(tx);
    delay(1);
    CANMessage rx;
    bool hasMessage = can.receive(rx);
    can.end();

    assertEqual(hasMessage, true);
    assertEqual(rx.extended, true);
    assertEqual(rx.id, 0x3000);
    assertEqual(rx.len, 1);
    assertEqual(rx.data[0], 10);
}

test(CAN_INTERFACE2_ReceivesAcceptedExtendedMessage) {
    CANChannel can(HAL_CAN_INTERFACE2);
    can.begin(500000, CAN_TEST_MODE);
    // Accept only extended message 0x3000
    can.addFilter(0x3000, 0x7FF, CAN_FILTER_EXTENDED);

    CANMessage tx;
    tx.extended = true;
    tx.id = 0x3000;
    tx.len = 1;
    tx.data[0] = 10;
    can.transmit(tx);
    delay(1);
    CANMessage rx;
    bool hasMessage = can.receive(rx);
    can.end();

    assertEqual(hasMessage, true);
}

test(CAN_INTERFACE2_DoesntReceiveFilteredExtendedMessage) {
    CANChannel can(HAL_CAN_INTERFACE2);
    can.begin(500000, CAN_TEST_MODE);
    // Accept only extended message 0x3000
    can.addFilter(0x3000, 0x7FF, CAN_FILTER_EXTENDED);

    CANMessage tx;
    tx.extended = true;
    tx.id = 0x3001;
    tx.len = 1;
    tx.data[0] = 10;
    can.transmit(tx);
    delay(1);
    CANMessage rx;
    bool hasMessage = can.receive(rx);
    can.end();

    assertEqual(hasMessage, false);
}

#endif
