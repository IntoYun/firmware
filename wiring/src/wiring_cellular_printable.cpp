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

#include "firmware_config.h"
#ifdef FIRMWARE_CONFIG_WIRING_CELLULAR_ENABLE

#include "wiring_cellular_printable.h"
#include "wiring_print.h"
#include "string.h"

size_t CellularSignal::printTo(Print& p) const
{
    size_t n = 0;
    n += p.print((*this).rssi, DEC);
    n += p.print(',');
    n += p.print((*this).qual, DEC);
    return n;
}

#endif // Wiring_Cellular

