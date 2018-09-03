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

test(SPI_clock)
{
    API_COMPILE(SPI.setClockDivider(SPI_CLOCK_DIV2));
    API_COMPILE(SPI.setClockDivider(SPI_CLOCK_DIV256));
    API_COMPILE(SPI.setClockSpeed(100, MHZ));
    API_COMPILE(SPI.setClockSpeed(100, KHZ));
    API_COMPILE(SPI.setClockSpeed(SPI_CLK_ARDUINO));
    API_COMPILE(SPI.setClockSpeed(SPI_CLK_ARDUINO/4));
    API_COMPILE(SPI.setClockDividerReference(SPI_CLK_ARDUINO));
}

#if FIRMWARE_CONFIG_WIRING_SPI > 1
test(SPI1_begin)
{
    API_COMPILE(SPI1.begin());
}

test(SPI1_clock)
{
    API_COMPILE(SPI1.setClockDivider(SPI_CLOCK_DIV2));
    API_COMPILE(SPI1.setClockDivider(SPI_CLOCK_DIV256));
    API_COMPILE(SPI1.setClockSpeed(100, MHZ));
    API_COMPILE(SPI1.setClockSpeed(100, KHZ));
    API_COMPILE(SPI1.setClockSpeed(SPI_CLK_ARDUINO));
    API_COMPILE(SPI1.setClockSpeed(SPI_CLK_ARDUINO/4));
    API_COMPILE(SPI1.setClockDividerReference(SPI_CLK_ARDUINO));
}
#endif
