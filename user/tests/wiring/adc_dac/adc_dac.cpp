/**
 ******************************************************************************
  Copyright (c) 2015 IntoRobot Industries, Inc.  All rights reserved.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

#include "application.h"
#include "unit-test/unit-test.h"

/*
 * ADC Test requires two 100k ohm resistors hooked up to the unit under test as follows:
 *           R1 100k           R2 100k
 * (3V3*) ---/\/\/\--- (adc_pin) ---/\/\/\--- (GND)
 *
 * connect DAC Output to ADC input as follows:
 *
 *           WIRE
 * (dac_pin) --==========-- (adc_pin)
 *
 */

test(ADC_NoAnalogReadWhenPinSelectedIsOutOfRange) {
    pin_t pin = 100;//pin under test (not a valid user pin)
    // when
    int32_t ADCValue = analogRead(pin);
    // then
    assertFalse(ADCValue!=0);
    //To Do : Add test for remaining pins if required
}

test(ADC_AnalogReadOnPinWithVoltageDividerResultsInCorrectValue) {
    // when
    int32_t ADCValue = analogRead(INTOROBOT_TEST_PIN_ADC); //pin under test (Voltage divider with equal resistor values)
    // then
    assertTrue((ADCValue>2000)&&(ADCValue<2100));//ADCValue should be around 2048
    //To Do : Add test for remaining pins if required
}

#ifdef INTOROBOT_TEST_PIN_DAC
test(ADC_AnalogReadOnPinWithDACOutputResultsInCorrectValue) {
    // when
    analogWrite(INTOROBOT_TEST_PIN_DAC, 2048);
    int32_t ADCValue = analogRead(INTOROBOT_TEST_PIN_ADC);
    // then
    assertTrue((ADCValue>2000)&&(ADCValue<2100));//ADCValue should be around 2048
    //To Do : Add test for remaining pins if required
}
#endif

