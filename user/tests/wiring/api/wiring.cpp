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

test(api_i2c) {
    int buffer;

    API_COMPILE(buffer=I2C_BUFFER_LENGTH);
    (void)buffer;
}

test(api_wiring_pinMode) {
    PinMode mode = PIN_MODE_NONE;

    API_COMPILE(mode=getPinMode(INTOROBOT_TEST_PIN_DIGITAL));
    API_COMPILE(pinMode(INTOROBOT_TEST_PIN_DIGITAL, mode));
    (void)mode;
}

test(api_wiring_analogWrite) {
    API_COMPILE(analogWrite(INTOROBOT_TEST_PIN_ADC, 50));
    API_COMPILE(analogWrite(INTOROBOT_TEST_PIN_ADC, 50, 10000));
}

test(api_wiring_wire_setSpeed) {
    API_COMPILE(Wire.setSpeed(CLOCK_SPEED_100KHZ));
}

void interrupt_callback() {
}

test(api_wiring_interrupt) {
    API_COMPILE(interrupts());
    API_COMPILE(noInterrupts());

    API_COMPILE(attachInterrupt(INTOROBOT_TEST_PIN_DIGITAL, interrupt_callback, RISING));
    API_COMPILE(detachInterrupt(INTOROBOT_TEST_PIN_DIGITAL));

    class MyClass {
        public:
            void handler() { }
    } myObj;

    API_COMPILE(attachInterrupt(INTOROBOT_TEST_PIN_DIGITAL, &MyClass::handler, &myObj, RISING));
    API_COMPILE(attachInterrupt(INTOROBOT_TEST_PIN_DIGITAL, interrupt_callback, RISING, 14));
    API_COMPILE(attachInterrupt(INTOROBOT_TEST_PIN_DIGITAL, interrupt_callback, RISING, 14, 0));
    API_COMPILE(attachInterrupt(INTOROBOT_TEST_PIN_DIGITAL, &MyClass::handler, &myObj, RISING, 14));
    API_COMPILE(attachInterrupt(INTOROBOT_TEST_PIN_DIGITAL, &MyClass::handler, &myObj, RISING, 14, 0));

    //API_COMPILE(attachSystemInterrupt(SysInterrupt_TIM1_CC_IRQ, D0_callback));
}

test(api_wiring_usartserial) {
    API_COMPILE(Serial.halfduplex(true));
    API_COMPILE(Serial.halfduplex(false));

    API_COMPILE(Serial.blockOnOverrun(false));
    API_COMPILE(Serial.blockOnOverrun(true));

    API_COMPILE(Serial.availableForWrite());

    API_COMPILE(Serial.begin(9600, SERIAL_8N1));
    API_COMPILE(Serial.end());
    API_COMPILE(Serial.begin(9600, SERIAL_8N2));
    API_COMPILE(Serial.end());

    API_COMPILE(Serial.begin(9600, SERIAL_8E1));
    API_COMPILE(Serial.end());
    API_COMPILE(Serial.begin(9600, SERIAL_8E2));
    API_COMPILE(Serial.end());

    API_COMPILE(Serial.begin(9600, SERIAL_8O1));
    API_COMPILE(Serial.end());
    API_COMPILE(Serial.begin(9600, SERIAL_8O2));
    API_COMPILE(Serial.end());


    API_COMPILE(Serial.begin(9600, SERIAL_9N1));
    API_COMPILE(Serial.end());
    API_COMPILE(Serial.begin(9600, SERIAL_9N2));
    API_COMPILE(Serial.end());
}

#if FIRMWARE_CONFIG_WIRING_USB_USART
test(api_wiring_usbserial) {
    API_COMPILE(SerialUSB.blockOnOverrun(false));
    API_COMPILE(SerialUSB.blockOnOverrun(true));
    API_COMPILE(SerialUSB.availableForWrite());
}
#endif

void TIM3_callback()
{
}

test(api_wiring_system_interrupt) {
    //API_COMPILE(attachSystemInterrupt(SysInterrupt_TIM3_IRQ, TIM3_callback));
    //API_COMPILE(detachSystemInterrupt(SysInterrupt_TIM3_IRQ));
}

void externalLEDHandler(uint8_t r, uint8_t g, uint8_t b) {
}

class ExternalLed {
    public:
        void handler(uint8_t r, uint8_t g, uint8_t b) {
        }
} externalLed;

test(api_rgb) {
    bool flag; uint8_t value;
    API_COMPILE(flag=RGB.controlled());
    API_COMPILE(RGB.control(true));
    API_COMPILE(RGB.color(255,255,255));
    API_COMPILE(RGB.color(RGB_COLOR_WHITE));
    API_COMPILE(RGB.blink(255,255,255,1000));
    API_COMPILE(RGB.blink(RGB_COLOR_WHITE, 1000));
    API_COMPILE(RGB.breath(255,255,255,1000));
    API_COMPILE(RGB.breath(RGB_COLOR_WHITE, 1000));
    (void)flag; (void)value; // unused
}

test(api_servo_trim) {
    Servo servo;
    servo.setTrim(234);
}

test(api_wire) {
    API_COMPILE(Wire.begin());
    API_COMPILE(Wire.reset());
    API_COMPILE(Wire.end());
}

test(api_map) {
    map(0x01,0x00,0xFF,0,255);
}

/**
 * Ensures that we can stil take the address of the global instances.
 *
 */
test(api_wiring_globals) {
    void* ptrs[] = {
        &SPI,
#if FIRMWARE_CONFIG_WIRING_SPI > 1
        &SPI1,
#endif
#if FIRMWARE_CONFIG_WIRING_SPI > 2
        &SPI2,
#endif
        &Serial,
        &Wire,
#if FIRMWARE_CONFIG_WIRING_WIRE > 1
        &Wire1,
#endif
#if FIRMWARE_CONFIG_WIRING_WIRE > 2
        &Wire2,
#endif
#if FIRMWARE_CONFIG_WIRING_WIRE > 3
        &Wire3,
#endif
#if FIRMWARE_CONFIG_WIRING_USART > 1
        &Serial1,
#endif
#if FIRMWARE_CONFIG_WIRING_USART > 2
        &Serial2,
#endif
#if FIRMWARE_CONFIG_WIRING_USART > 3
        &Serial3,
#endif
#if FIRMWARE_CONFIG_WIRING_USART > 4
        &Serial4,
#endif
#if FIRMWARE_CONFIG_WIRING_USART > 5
        &Serial5,
#endif
        &EEPROM,
    };
    (void)ptrs;
}

