/**
 ******************************************************************************
 * @file        : firmware_update.h
 * @author   : robin
 * @version  : V1.0.0
 * @date      : 2015-02-01
 * @brief      :
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
#ifndef   FIRMWARE_UPDATE_H_
#define   FIRMWARE_UPDATE_H_

#include "lib_process.h"

class FirmwareUpdateClass: public Process
{
    public:
        // Default constructor uses global Bridge instance
        FirmwareUpdateClass(){}
        ~FirmwareUpdateClass(){}

        bool st_firmware_down(const char *server_addr, const char * md5);
        bool st_firmware_update(void);
        bool st_firmware_isupdate(void);
        bool st_firmware_clearupdate(void);
        bool openwrt_firmware_version(char *p_sys_version);
        bool openwrt_firmware_down(const char *sys_ver, const char *server_addr);
        bool openwrt_firmware_update();
        bool openwrt_reboot(void);
        bool st_system_reset(void);
        bool st_system_isreset(void);
        bool st_system_clearreset(void);
};

extern FirmwareUpdateClass firmwareupdate;

#endif /*FIRMWARE_UPDATE_H_*/

