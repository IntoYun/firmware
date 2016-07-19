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
#include "intorobot_api.h"

/*********************************************************************************
  *Function		:   bool st_firmware_down(char *app_id)
  *Description	:   down firmware to openwrt
  *Input		      :   app_id(char *): application id
  *Output		:   none
  *Return		:   true: success   false: fail
  *author		:   robot
  *date			:   2015-02-01
  *Others		:
**********************************************************************************/
bool FirmwareUpdateClass::st_firmware_down(const char *server_addr, const char * md5)
{
    begin("stm32_update_online");
    addParameter("DOWN");
    addParameter(server_addr);
    addParameter(md5);
    int res = run();
    if(res == 0)
    {
        return true;
    }
    return false;
}

/*********************************************************************************
  *Function		:     bool st_firmware_update(void)
  *Description	:     update the stm32 firmware
  *Input		      :     none
  *Output		:     none
  *Return		:     true: success   false: fail
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
bool FirmwareUpdateClass::st_firmware_update(void)
{
    begin("stm32_update_online");
    addParameter("UPDATE");
    int res = run();
    if(res == 0)
    {
        return true;
    }
    return false;
}

/*********************************************************************************
  *Function		:     bool st_firmware_isupdate(void)
  *Description	:     check the firmware is updated
  *Input		      :     none
  *Output		:     none
  *Return		:     true: success   false: fail
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
bool FirmwareUpdateClass::st_firmware_isupdate(void)
{
    begin("stm32_update_online");
    addParameter("ISUPDATE");
    int res = run();
    if(res == 0)
    {
        return true;
    }
    return false;
}

/*********************************************************************************
  *Function		:     bool st_firmware_clearupdate(void)
  *Description	:     remove the updated flag
  *Input		      :     none
  *Output		:     none
  *Return		:     true: success   false: fail
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
bool FirmwareUpdateClass::st_firmware_clearupdate(void)
{
    begin("stm32_update_online");
    addParameter("CRUPDATE");
    int res = run();
    if(res == 0)
    {
        return true;
    }
    return false;
}

/*********************************************************************************
  *Function		:   bool openwrt_firmware_down(char *app_id)
  *Description	:   down firmware to openwrt
  *Input		      :   none
  *Output		:   p_version: the firmware version of openwrt
  *Return		:   true: success   false: fail
  *author		:   robot
  *date			:   2015-02-01
  *Others		:
**********************************************************************************/
bool FirmwareUpdateClass::openwrt_firmware_version(char *p_sys_version)
{
    String tmp="";

    begin("openwrt_update_online");
    addParameter("GETVERSION");
    int res = run();

    if(res == 0)
    {
        while (available())
        {
            tmp+=(char)read();
        }
        if(strlen(tmp.c_str()))
        {
            strcpy(p_sys_version,tmp.c_str());
        }
        else
        {
            return false;
        }
    }
    return false;
}

/*********************************************************************************
  *Function		:   bool openwrt_firmware_down(const char *com_ver, const char *mcu_ver, const char *server_addr)
  *Description	:   down firmware to openwrt
  *Input		      :   pserver(char *): you can get the firmware through the server
  *Output		:   none
  *Return		:   true: success   false: fail
  *author		:   robot
  *date			:   2015-02-01
  *Others		:
**********************************************************************************/
bool FirmwareUpdateClass::openwrt_firmware_down(const char *sys_ver, const char *server_addr)
{
    char temp[64],c;
    String process;

    begin("openwrt_update_online");
    addParameter("DOWN");
    addParameter(sys_ver);
    addParameter(server_addr);

    runAsynchronously();
    while (running())
    {
        process="";
        if(available()&&('<'==(char)read()))
        {
            while (available())//获取进度
            {
                c=(char)read();
                if('>'==c)
                {break;}
                process+=c;
            }
            if(strlen(process.c_str()))
            {
                memset(temp,0,sizeof(temp));
                sprintf(temp,"{\"status\":\"%s\",\"progress\": %s}", INTOROBOT_MQTT_RESMES_OW_FWUPREADY, process.c_str());
                IntoRobot.publish(INTOROBOT_MQTT_RESPONSE_JSON_TOPIC, (uint8_t*)temp, strlen(temp),false);
            }
        }
    }
    if(exitValue() == 0)
    {
        return true;
    }
    return false;
}

/*********************************************************************************
  *Function		:     bool openwrt_firmware_update(void)
  *Description	:     update the stm32 firmware
  *Input		      :     none
  *Output		:     none
  *Return		:     true: success   false: fail
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
bool FirmwareUpdateClass::openwrt_firmware_update(void)
{
    begin("openwrt_update_online");
    addParameter("UPDATE");
    int res = run();
    if(res == 0)
    {
        return true;
    }
    return false;
}

/*********************************************************************************
  *Function		:     bool st_system_reset(void)
  *Description	:     reset the stm32 system
  *Input		      :     none
  *Output		:     none
  *Return		:     true: success   false: fail
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
bool FirmwareUpdateClass::openwrt_reboot(void)
{
    begin("reboot");
    runAsynchronously();
    return true;
}

/*********************************************************************************
  *Function		:     bool st_system_reset(void)
  *Description	:     reset the stm32 system
  *Input		      :     none
  *Output		:     none
  *Return		:     true: success   false: fail
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
bool FirmwareUpdateClass::st_system_reset(void)
{
    begin("stm32_system_reset");
    addParameter("SRESET");
    int res = run();
    if(res == 0)
    {
        return true;
    }
    return false;
}


/*********************************************************************************
  *Function		:     bool st_system_isreset(void)
  *Description	:     check the system is resetted
  *Input		      :     none
  *Output		:     none
  *Return		:     true: success   false: fail
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
bool FirmwareUpdateClass::st_system_isreset(void)
{
    begin("stm32_system_reset");
    addParameter("ISRESET");
    int res = run();
    if(res == 0)
    {
        return true;
    }
    return false;
}

/*********************************************************************************
  *Function		:     bool st_system_clearreset(void)
  *Description	:     remove the system reset flag
  *Input		      :     none
  *Output		:     none
  *Return		:     true: success   false: fail
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
bool FirmwareUpdateClass::st_system_clearreset(void)
{
    begin("stm32_system_reset");
    addParameter("CRRESET");
    int res = run();
    if(res == 0)
    {
        return true;
    }
    return false;
}

FirmwareUpdateClass firmwareupdate;

