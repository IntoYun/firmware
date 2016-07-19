/**
 ******************************************************************************
 * @file        : firmware_update.cpp
 * @author   : robin
 * @version  : V1.0.0
 * @date      : 6-December-2014
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
#include "firmware_update.h"
#include "firmware_update_hal.h"
#include "variant.h"

volatile uint8_t  online_downfile_flag = 0xff;   //0 下载成功  1 下载失败  2 正在下载
volatile uint8_t  net_down_flag = 0xff;
/*********************************************************************************
  *Function		:   bool st_firmware_down(char *app_id)
  *Description	:   down firmware to openwrt
  *Input              :   app_id(char *): application id
  *Output		:   none
  *Return		:   true: success   false: fail
  *author		:   robot
  *date			:   2015-02-01
  *Others		:
**********************************************************************************/
bool FirmwareUpdateClass::st_firmware_down(const char *domain, const char *param, const char * md5)
{
    int res = mo_FirmwareUpdateClass_st_firmware_down_hal(domain, param, md5);
    if(res == 0)
    {
        return true;
    }
    return false;
}

/*********************************************************************************
  *Function		:     bool st_firmware_update(void)
  *Description	:     update the stm32 firmware
  *Input              :     none
  *Output		:     none
  *Return		:     true: success   false: fail
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
bool FirmwareUpdateClass::st_firmware_update(void)
{/*
    begin("stm32_update_online");
    addParameter("UPDATE");
    int res = run();*/

    int res=mo_FirmwareUpdateClass_st_firmware_update_hal();
    if(res == 0)
    {
        return true;
    }
    return false;

}

/*********************************************************************************
  *Function		:     bool st_firmware_isupdate(void)
  *Description	:     check the firmware is updated
  *Input              :     none
  *Output		:     none
  *Return		:     true: success   false: fail
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
bool FirmwareUpdateClass::st_firmware_isupdate(void)
{
/*
    begin("stm32_update_online");
    addParameter("ISUPDATE");
    int res = run();*/

    int res=mo_FirmwareUpdateClass_st_firmware_isupdate_hal();

    if(res == 0)
    {
        return true;
    }
    return false;
}

/*********************************************************************************
  *Function		:     bool st_firmware_clearupdate(void)
  *Description	:     remove the updated flag
  *Input              :     none
  *Output		:     none
  *Return		:     true: success   false: fail
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
bool FirmwareUpdateClass::st_firmware_clearupdate(void)
{
/*
    begin("stm32_update_online");
    addParameter("CRUPDATE");
    int res = run();*/
    int res=mo_FirmwareUpdateClass_st_firmware_clearupdate_hal();
    if(res == 0)
    {
        return true;
    }
    return false;
}

/*********************************************************************************
  *Function		:   bool openwrt_firmware_down(char *app_id)
  *Description	:   down firmware to openwrt
  *Input              :   none
  *Output		:   p_version: the firmware version of openwrt
  *Return		:   true: success   false: fail
  *author		:   robot
  *date			:   2015-02-01
  *Others		:
**********************************************************************************/
bool FirmwareUpdateClass::net_firmware_version(char *p_version)
{
/*
    String tmp;

    begin("openwrt_update_online");
    addParameter("VERSION");
    int res = run();
*/
    int res=mo_FirmwareUpdateClass_net_firmware_version_hal(p_version);

    if(res == 0)
    {/*
        while (available())
        {
            tmp+=(char)read();
        }
        strcpy(p_version,tmp.c_str());    */
        return true;
    }
    return false;
}

/*********************************************************************************
  *Function		:   bool net_firmware_down(char *app_id)
  *Description	:   down firmware to openwrt
  *Input              :   pserver(char *): you can get the firmware through the server
  *Output		:   none
  *Return		:   true: success   false: fail
  *author		:   robot
  *date			:   2015-02-01
  *Others		:
**********************************************************************************/
bool FirmwareUpdateClass::net_firmware_down(const char *app_id, const char *server_addr)
{
    int res=mo_FirmwareUpdateClass_net_firmware_down(app_id, server_addr);
    if(res == 0)
    {
        return true;
    }
    return false;
}

/*********************************************************************************
  *Function		:     bool net_firmware_update(void)
  *Description	:     update the stm32 firmware
  *Input              :     none
  *Output		:     none
  *Return		:     true: success   false: fail
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
bool FirmwareUpdateClass::net_firmware_update(void)
{/*
    begin("openwrt_update_online");
    addParameter("UPDATE");
    int res = run();*/

    int res=mo_FirmwareUpdateClass_net_firmware_update_hal();
    if(res == 0)
    {
        return true;
    }
    return false;
}

/*********************************************************************************
  *Function		:     bool openwrt_firmware_isupdate(void)
  *Description	:     check the firmware is updated
  *Input              :     none
  *Output		:     none
  *Return		:     true: success   false: fail
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
bool FirmwareUpdateClass::net_firmware_isupdate(void)
{/*
    begin("openwrt_update_online");
    addParameter("ISUPDATE");
    int res = run();*/

    int res= mo_FirmwareUpdateClass_net_firmware_isupdate();
    if(res == 0)
    {
        return true;
    }
    return false;
}

/*********************************************************************************
  *Function		:     bool openwrt_firmware_clearupdate(void)
  *Description	:     remove the updated flag
  *Input              :     none
  *Output		:     none
  *Return		:     true: success   false: fail
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
bool FirmwareUpdateClass::net_firmware_clearupdate(void)
{
/*
    begin("openwrt_update_online");
    addParameter("CRUPDATE");
    int res = run();*/
    int res= mo_FirmwareUpdateClass_net_firmware_clearupdate();
    if(res == 0)
    {
        return true;
    }
    return false;
}

/*********************************************************************************
  *Function		:     bool st_system_reset(void)
  *Description	:     reset the stm32 system
  *Input              :     none
  *Output		:     none
  *Return		:     true: success   false: fail
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
bool FirmwareUpdateClass::st_system_reset(void)
{/*
    begin("stm32_system_reset");
    addParameter("SRESET");
    int res = run();*/
    int res= mo_FirmwareUpdateClass_st_system_reset_hal();
    if(res == 0)
    {
        return true;
    }
    return false;
}

/*********************************************************************************
  *Function		:     bool st_system_isreset(void)
  *Description	:     check the system is resetted
  *Input              :     none
  *Output		:     none
  *Return		:     true: success   false: fail
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
bool FirmwareUpdateClass::st_system_isreset(void)
{/*
    begin("stm32_system_reset");
    addParameter("ISRESET");
    int res = run();*/
    int res=  mo_FirmwareUpdateClass_st_system_isreset_hal();
    if(res == 0)
    {
        return true;
    }
    return false;
}

/*********************************************************************************
  *Function		:     bool st_system_clearreset(void)
  *Description	:     remove the system reset flag
  *Input              :     none
  *Output		:     none
  *Return		:     true: success   false: fail
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
bool FirmwareUpdateClass::st_system_clearreset(void)
{/*
    begin("stm32_system_reset");
    addParameter("CRRESET");
    int res = run();*/
    int res= mo_FirmwareUpdateClass_st_system_clearreset_hal();
    if(res == 0)
    {
        return true;
    }
    return false;
}

FirmwareUpdateClass firmwareupdate;
