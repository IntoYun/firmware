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

#include "ota_flash_hal.h"
#include "core_hal.h"
#include "parser.h"


down_status_t HAL_OTA_Download_App(const char *host, const char *param, const char * md5)
{
    return (down_status_t)esp8266MDM.downOtaFile(host, param, md5);
}

down_status_t HAL_OTA_Get_App_Download_Status(void)
{
    return (down_status_t)esp8266MDM.getDownOtafileStatus();
}

void HAL_OTA_Update_App(void)
{
    HAL_Core_Enter_Ota_Update_Mode();
}

down_status_t HAL_OTA_Download_Subsys(const char *host, const char *param)
{
    return (down_status_t)esp8266MDM.downNetFile(host, param);
}

down_status_t HAL_OTA_Get_Subsys_Download_Status(void)
{
    return (down_status_t)esp8266MDM.getDownNetfileStatus();
}

void HAL_OTA_Upadate_Subsys(void)
{
    esp8266MDM.updateNet();
}

uint8_t HAL_OTA_Get_Download_Progress()
{
    return (uint8_t)esp8266MDM.getDownFileProgress();
}

