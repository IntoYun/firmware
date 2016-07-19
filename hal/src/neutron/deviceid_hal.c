/**
 ******************************************************************************
  Copyright (c) 2013-2014 IntoRobot Team.  All right reserved.

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


#include "deviceid_hal.h"
#include <stddef.h>

//board type
#define INTOROBOT_BOARD_TYPE1    "888002"
#define INTOROBOT_BOARD_TYPE2    "887002"


unsigned HAL_device_ID(uint8_t* dest, unsigned destLen)
{
    if (dest!=NULL && destLen>0)
        *dest = 0;
    return 0;
}

unsigned HAL_Board_TYPE(uint8_t* dest, unsigned destLen)
{
    if (dest!=NULL && destLen>0)
        *dest = 0;
    return 0;
}

unsigned HAL_Platform_ID()
{
    return PLATFORM_ID;
}
