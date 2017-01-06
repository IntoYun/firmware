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

test(spi_clock)
{
    API_COMPILE(SPI.setClockSpeed(24, MHZ));
    API_COMPILE(SPI.setClockSpeed(24000000));
    API_COMPILE(SPI.setClockDivider(SPI_CLOCK_DIV2));
    API_COMPILE(SPI.setClockDividerReference(SPI_CLK_ARDUINO));

}

test(spi_transfer)
{
    API_COMPILE(SPI.transfer(0));
    API_COMPILE(SPI.transfer(NULL, NULL, 1, NULL));
}