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
#include "params_hal.h"
#include <stddef.h>

//board type
#define INTOROBOT_BOARD_TYPE    "888002"
#define INTOROBOT_BOARD_TYPE1    "887002"


uint32_t HAL_device_ID(uint8_t* dest, uint32_t destLen)
{
    uint32_t len = strlen(HAL_System_Param().device_id);

    if (dest!=NULL && destLen>0) {
        if(len >= destlen) {
            len = destlen-1;
        }
        memcpy(dest, HAL_System_Param().device_id, len);
        return len;
    }
    return 0;
}

uint32_t HAL_Board_Type(uint8_t* dest, uint32_t destLen, uint8_t type)
{
    uint32_t len = strlen(INTOROBOT_BOARD_TYPE1);

    if (dest!=NULL && destLen>0) {
        if(len >= destlen) {
            len = destlen-1;
        }
        if(0==type) {
            memcpy(dest, INTOROBOT_BOARD_TYPE, len);
        }
        else{
            memcpy(dest, INTOROBOT_BOARD_TYPE1, len);
        }
        return len;
    }
    return 0;
}

uint32_t HAL_Platform_ID(void)
{
    return PLATFORM_ID;
}
