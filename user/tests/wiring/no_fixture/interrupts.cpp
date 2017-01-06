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

test(interrupts_atomic_section)
{
    // using SysTick for convenience - we could use a stm32 timer interrupt for a more generic test
    // millis() will not increment while interrupts are disabled.
    // micros() does increment

    uint32_t start_micros = micros();
    uint32_t start_millis, end_millis;
    {
        ATOMIC_BLOCK() {
            start_millis = millis();
            while (micros()-start_micros<2000)
            {
                // validates that atomic sections can be nested and interrupts restored when the outermost one exits
                ATOMIC_BLOCK();
            }
            end_millis = millis();
        }
    }
    assertEqual(start_millis, end_millis);

    // now do the same without an atomic block
    {
        start_micros = micros();
        start_millis = millis();
        while (micros()-start_micros<2000)
        {
            // busy wait
        }
        end_millis = millis();
    }

    assertMore(end_millis, start_millis);
}
