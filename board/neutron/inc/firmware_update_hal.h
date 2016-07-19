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
#ifndef   FIRMWARE_UPDATE_HAL_H_
#define   FIRMWARE_UPDATE_HAL_H_

/*
成功返回0

*/

int mo_FirmwareUpdateClass_st_firmware_down_hal(const char *domain, const char *param, const char * md5);


/*
成功返回0

*/

int mo_FirmwareUpdateClass_st_firmware_update_hal(void);

/*
成功返回0

*/

int mo_FirmwareUpdateClass_st_firmware_isupdate_hal(void);
/*
成功返回0

*/

int mo_FirmwareUpdateClass_st_firmware_clearupdate_hal(void);

/*
成功返回0

*/

int mo_FirmwareUpdateClass_net_firmware_version_hal(char *p_version);



int mo_FirmwareUpdateClass_net_firmware_down(const char *domain, const char *param);



int mo_FirmwareUpdateClass_net_firmware_update_hal(void);


int mo_FirmwareUpdateClass_net_firmware_isupdate(void);


int mo_FirmwareUpdateClass_net_firmware_clearupdate(void);


int mo_FirmwareUpdateClass_st_system_reset_hal(void);

int  mo_FirmwareUpdateClass_st_system_isreset_hal(void);

int mo_FirmwareUpdateClass_st_system_clearreset_hal(void);


#endif /*FIRMWARE_UPDATE_H_*/

