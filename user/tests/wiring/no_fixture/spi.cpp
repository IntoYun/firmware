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

void assertClockDivider(unsigned reference, unsigned desired, uint8_t expected_divider, unsigned expected_clock)
{
    unsigned clock;
    uint8_t divider;

    SPI.computeClockDivider(reference, desired, divider, clock);
    assertEqual(expected_divider, divider);
    assertEqual(expected_clock, clock);
}

test(spi_computeClockSpeed)
{
    assertClockDivider(60*MHZ, 120*MHZ, SPI_CLOCK_DIV2, 30*MHZ);
    assertClockDivider(60*MHZ, 30*MHZ, SPI_CLOCK_DIV2, 30*MHZ);
    assertClockDivider(60*MHZ, 20*MHZ, SPI_CLOCK_DIV4, 15*MHZ);
    assertClockDivider(60*MHZ, 8*MHZ, SPI_CLOCK_DIV8, 7500*KHZ);
    assertClockDivider(60*MHZ, 7*MHZ, SPI_CLOCK_DIV16, 3750*KHZ);
    assertClockDivider(60*MHZ, 300*KHZ, SPI_CLOCK_DIV256, 234375*HZ);
    assertClockDivider(60*MHZ, 1*KHZ, SPI_CLOCK_DIV256, 234375*HZ);
}