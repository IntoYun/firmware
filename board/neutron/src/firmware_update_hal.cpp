//=================================================================================================================
//input
/****************************************************************************
 *Private Included Files
 ****************************************************************************/

#include <stdint.h>
#include "lib_system_all.h"
#include "lib_wifi_drv.h"


extern uint8_t  online_downfile_flag;   //0 下载成功  1 下载失败  2 正在下载
extern uint8_t  net_down_flag;

extern void mo_system_reboot_hal();

 //=================================================================================================================
//come true hidden
/************************************************************************************
* Private Types
************************************************************************************/


/************************************************************************************
* Private Variables
************************************************************************************/


/************************************************************************************
* Private Functions
************************************************************************************/


//=================================================================================================================
//come true export
/************************************************************************************
* Public Type
************************************************************************************/



/************************************************************************************
*  Public Functions
************************************************************************************/

/*
成功返回0

*/
//cmd:AT+MDSET="md5:32bytes"
//cmd:AT+DOWNFW="http://www.intorobot.com/v1/bin?dwn_token=32bytes"
int mo_FirmwareUpdateClass_st_firmware_down_hal(const char *domain, const char *param, const char * md5)
{
    String server_down_cmd = "AT+IR_DOWNFILE=";
    server_down_cmd += "\"";
    server_down_cmd += domain;
    server_down_cmd += "\",\"";
    server_down_cmd += param;
    server_down_cmd += "\",\"";
    server_down_cmd += md5;
    server_down_cmd += "\"";

    MO_DEBUG(( server_down_cmd.c_str() ));

    char *server_download_cmd = const_cast<char *>(server_down_cmd.c_str());

    if (mo_drv_wifi_run_cmd( server_download_cmd, "+IR_DOWNFILE:0", 20) )
    {
        intorobot_system_param.ota_flag = 1;
        saveSystemParams( &intorobot_system_param );

        MO_DEBUG(("Download Succeed"));
        return 0;
    }
    else
    {
        MO_ERROR(("Download Failed"));
        return 1;
    }
}


/*
成功返回0

*/

int mo_FirmwareUpdateClass_st_firmware_update_hal(void)
{
    MO_ERROR(("not support"));
    return 0;
}

/*
成功返回0

*/

int mo_FirmwareUpdateClass_st_firmware_isupdate_hal(void)
{
    MO_DEBUG(("ota_flag = %d\r\n",intorobot_system_param.ota_flag));
    if (1 == intorobot_system_param.ota_flag)
    {
        return 0;
    }
    return 1;
}

/*
成功返回0

*/

int mo_FirmwareUpdateClass_st_firmware_clearupdate_hal(void)
{
    // clear the ota flag
    intorobot_system_param.ota_flag = 0;
    saveSystemParams( &intorobot_system_param );
    return 0;

}

/*
成功返回0

*/

int mo_FirmwareUpdateClass_net_firmware_version_hal(char *p_version)
{
    char cmdRun[128], bufRet[128], temp[64];
    char *ptr0 = NULL;
    char *ptr1 = NULL;

    memset(cmdRun, 0, sizeof(cmdRun));
    memset(bufRet, 0, sizeof(bufRet));
    sprintf(cmdRun, "AT+IR_GETVERSION?");
    if(mo_drv_wifi_cmd_transfer(cmdRun, strlen(cmdRun), bufRet, sizeof(bufRet), 5, 0x03)>0)
    {
        MO_DEBUG(("bufRet: %s", bufRet));
        if((char *)strstr(bufRet, "+IR_GETVERSION:") != NULL )
        {
            ptr0 = strstr(bufRet, ":");
            ptr1 = strstr(bufRet, "\r\n");
            memcpy(p_version, ptr0+1, ptr1-ptr0-1);
            return 0;
        }
    }
    return 1;
}

int mo_FirmwareUpdateClass_net_firmware_down(const char *domain, const char *param)
{
    char cmdRun[128], bufRet[128], temp[64], progress[16];
    char *ptr0 = NULL;
    char *ptr1 = NULL;

    memset(cmdRun, 0, sizeof(cmdRun));
    memset(bufRet, 0, sizeof(bufRet));
    sprintf(cmdRun, "AT+IR_NETDOWN=\"%s\",\"%s\"", domain, param);
    if (mo_drv_wifi_run_cmd( cmdRun, "OK", 10) )
    {
        if(0 == net_down_flag)
        {
            MO_DEBUG(("Download Succeed"));
            return 0;
        }
        else if(1 == net_down_flag)
        {
            MO_ERROR(("Download Failed"));
            return 1;
        }
        else if(2 == net_down_flag)
        {
        }
    }
    else
    {
        MO_ERROR(("Download Failed"));
        return 1;
    }

    //下载进度
    net_down_flag = 0xff;
    while(1)
    {
        delay(1000);
        sprintf(cmdRun, "AT+IR_DOWNPROGRESS?");
        memset(bufRet, 0, sizeof(bufRet));
        if(mo_drv_wifi_cmd_transfer(cmdRun, strlen(cmdRun), bufRet, sizeof(bufRet), 5, 0x03)>0)
        {
            MO_DEBUG(("bufRet: %s", bufRet));
            if((char *)strstr(bufRet, "+IR_DOWNPROGRESS:") != NULL )
            {
                ptr0 = strstr(bufRet, ":");
                ptr1 = strstr(bufRet, "\r\n");
                memset(progress, 0, sizeof(progress));
                memcpy(progress, ptr0+1, ptr1-ptr0-1);
                memset(temp,0,sizeof(temp));
                sprintf(temp,"{\"status\":\"%s\",\"progress\": %s}", INTOROBOT_MQTT_RESMES_OW_FWUPREADY, progress);
                IntoRobot.publish(INTOROBOT_MQTT_RESPONSE_JSON_TOPIC, (uint8_t*)temp, strlen(temp),false);
            }
        }
        if(0 == net_down_flag)
        {
            return 0;
        }
        else if(1 == net_down_flag)
        {
            return 1;
        }
    }
    return 0;
}

int mo_FirmwareUpdateClass_net_firmware_update_hal(void)
{
    String server_down_cmd = "AT+IR_NETUPDATE";
    char *server_download_cmd = const_cast<char *>(server_down_cmd.c_str());

    if (mo_drv_wifi_run_cmd( server_download_cmd, "OK", 3) )
    {
        MO_DEBUG(("Download Succeed"));
        return 0;
    }
    else
    {
        MO_ERROR(("Download Failed"));
        return 1;
    }

    MO_ERROR(("not support"));
    return 0;

    MO_ERROR(("not support"));
    return 0;
}


int mo_FirmwareUpdateClass_net_firmware_isupdate(void)
{
    MO_ERROR(("not support"));
    return 0;

}


int mo_FirmwareUpdateClass_net_firmware_clearupdate(void)
{
    MO_ERROR(("not support"));
    return 0;
}


int mo_FirmwareUpdateClass_st_system_reset_hal(void)
{
    intorobot_system_param.reset_flag = 1;
    saveSystemParams( &intorobot_system_param );
    mo_system_reboot_hal();
    MO_ERROR(("not support"));
    return 0;
}

// 0 for succeed
// 1 for failed
int  mo_FirmwareUpdateClass_st_system_isreset_hal(void)
{
    MO_DEBUG(("reset_flag = %d\r\n",intorobot_system_param.reset_flag));
    if (1 == intorobot_system_param.reset_flag)
    {
        return 0;
    }
    return 1;

    MO_ERROR(("not support"));
    return 0;
}

// 0 for succeed
// 1 for failed
int mo_FirmwareUpdateClass_st_system_clearreset_hal(void)
{
    intorobot_system_param.reset_flag = 0;
    saveSystemParams( &intorobot_system_param );

    MO_ERROR(("not support"));
    return 0;
}







//=================================================================================================================
//output
/****************************************************************************
*Public Included Files
****************************************************************************/
//=================================================================================================================




