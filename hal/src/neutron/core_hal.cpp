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

/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"
#include "core_hal.h"
#include "flash_map.h"
#include "parser.h"
#include "intorobot_macros.h"


uint16_t HAL_Core_Get_Subsys_Version(char* buffer, uint16_t len)
{
    char temp[32];
    uint16_t templen;

    esp8266MDM.init();  // 由于此时esp866通讯可能没有开始，需要添加初始化，否则版本号读不出来
    if (buffer!=NULL && len>0) {
        if(esp8266MDM.getNetVersion(temp))
        {
            templen = MIN(strlen(temp), len-1);
            memset(buffer, 0, len);
            memcpy(buffer, temp, templen);
            return templen;
        }
    }
    return 0;
}

