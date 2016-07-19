/****************************************************************************
 * include/ctype.h
 *
 *   Copyright (C) 2007-2009, 2011, 2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/


/****************************************************************************
 *Private Included Files
 ****************************************************************************/
#include <stdint.h>
#include "lib_system_all.h"
#include "lib_wifi_drv.h"


/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/


/************************************************************************************
 * Private Types
 ************************************************************************************/


/************************************************************************************
 * Private Variables
 ************************************************************************************/

/****************************************************************************
 * Private Prototypes
 ****************************************************************************/


/************************************************************************************
 * Private Functions
 ************************************************************************************/



/************************************************************************************
 * Public Variables
 ************************************************************************************/

/************************************************************************************
 * Public Type
 ************************************************************************************/

/************************************************************************************
 *  Public Functions
 ************************************************************************************/


#define MAX_LIST 15
uint8_t encry[MAX_LIST] = {0};
int8_t rssi[MAX_LIST] = {0};
char ssid[MAX_LIST][WL_SSID_MAX_LENGTH] ={0};



// 8266 will connect to the wifi automatic, so leave it empty
void mo_WiFiClass_connect_hal(void)
{
}

//是否连接上互联网   ping 114    是   1   否 0
int8_t mo_WiFiClass_status_hal(void)
{
    char temp_cmd[32];
    memset(temp_cmd,0x00,sizeof(temp_cmd));
    sprintf(temp_cmd,"AT+CIPSTATUS");

    char temp[16];
    memset(temp,0x00,sizeof(temp));

    if(mo_drv_wifi_cmd_transfer(temp_cmd,strlen(temp_cmd),temp,sizeof(temp),10,0x03)>0)
    {
        if(memcmp(temp,"STATUS:5",8)==0)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 0;
    }
}

//重启wifi
void mo_WiFiClass_restartNetwork_hal(void)
{
    mo_drv_wifi_run_cmd("AT+RST","OK",10);
}

//配置wifi    成功0   失败1
uint8_t mo_WiFiClass_setCredentials_hal(const char *ssid, const char *password, const char *channel, const char *security)
{
    char temp_cmd[128];
    memset(temp_cmd,0x00,sizeof(temp_cmd));
    if (NULL == password)
    {
        sprintf(temp_cmd,"AT+CWJAP_DEF=\"%s\"",ssid);
    }
    else
    {
        sprintf(temp_cmd,"AT+CWJAP_DEF=\"%s\",\"%s\"",ssid,password);
    }

    if(mo_drv_wifi_run_cmd(temp_cmd,"OK",10))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

//设置IP地址
void mo_WiFiClass_config_hal1(const char *local_ip)
{
    char temp_cmd[128];
    memset(temp_cmd,0x00,sizeof(temp_cmd));
    sprintf(temp_cmd,"AT+CIPSTA_DEF=\"%s\"",local_ip);

    mo_drv_wifi_run_cmd(temp_cmd,"OK",10);
}

//set ip and dns
void mo_WiFiClass_config_hal2(const char *local_ip, const char *dns_server)
{
}

//set ip dns gateway
void mo_WiFiClass_config_hal3(const char *local_ip, const char *dns_server, const char *gateway)
{
    char temp_cmd[128];
    memset(temp_cmd,0x00,sizeof(temp_cmd));
    sprintf(temp_cmd,"AT+CIPSTA_DEF=\"%s\",\"%s\"",local_ip,gateway);

    mo_drv_wifi_run_cmd(temp_cmd,"OK",10);
}


void mo_WiFiClass_config_hal4(const char *local_ip, const char *dns_server, const char *gateway, const char *subnet)
{
    char temp_cmd[128];
    memset(temp_cmd,0x00,sizeof(temp_cmd));
    sprintf(temp_cmd,"AT+CIPSTA_DEF=\"%s\",\"%s\",\"%s\"",local_ip,gateway,subnet);

    mo_drv_wifi_run_cmd(temp_cmd,"OK",10);
}

//set dns
void mo_WiFiClass_setDNS_hal1(const char *dns_server1)
{
}

void mo_WiFiClass_setDNS_hal2(const char *dns_server1, const char *dns_server2)
{
}

//get mac
uint8_t *mo_WiFiClass_macAddress_hal(uint8_t* mac)
{
    return 0;
}

//get ip
void mo_WiFiClass_localIP_hal(int *p_ip)
{
}

//get netmask
void mo_WiFiClass_subnetMask_hal(int *p_net_mask)
{
}

void mo_WiFiClass_gatewayIP_hal(int *p_gate_way)
{
}

//获取字符串的ssid  长度不超过32
void mo_WiFiClass_SSID_hal(char *pssid)
{
    char cmdRun[] = "AT+CWJAP_CUR?";

    char bufRet[60];
    memset(bufRet, 0x00, sizeof(bufRet));

    if(mo_drv_wifi_cmd_transfer(cmdRun, strlen(cmdRun), bufRet, sizeof(bufRet), 5, 0x03)>0)
    {
        MO_DEBUG(("bufRet: %s", bufRet));
        char cmdRet[]  = "+CWJAP_CUR:";
        if ( ((char *)strstr(bufRet, cmdRet)) != NULL )
        {
            char *ptr1 = (char *)strstr(bufRet, ",");
            if (ptr1 != NULL)
            {
                memcpy(pssid, bufRet + strlen(cmdRet) + 1, ptr1 - bufRet - strlen(cmdRet) -2);
                MO_DEBUG(("ssid:%s", pssid));
                return;
            }
        }
    }
    pssid = NULL;
}


//获取bssid
void mo_WiFiClass_BSSID(uint8_t* bssid)
{
}

//获取当前wifi信号强度
int32_t mo_WiFiClass_RSSI_hal(void)
{
    return 0;
}

//获取当前加密方式
uint8_t mo_WiFiClass_encryptionType_hal(void)
{
    return 0;
}

//aps到的wifi热点数量
uint8_t mo_WiFiClass_scanNetworks_hal(void)
{
    memset(rssi, 0, sizeof(rssi));
    memset(encry, 0, sizeof(encry));
    memset(ssid, 0, sizeof(ssid));
    char cmdRun[] = "AT+CWLAP";
    char cmdRet[]  = "+CWLAP";

    //char bufRet[1024] = {0};  // os_malloc
    char *bufRet = (char *) pvPortMalloc(1024);
    if (bufRet == NULL)
    {
        MO_ERROR(("malloc failed"));
    }
    memset(bufRet, 0x00, sizeof(bufRet));
    uint8_t apNum = 0;

    uint8_t numFlag = 0;

    mo_drv_wifi_run_cmd("AT+CWLAPOPT=1,31","OK",5); // set sort by rssi

    if(mo_drv_wifi_cmd_transfer(cmdRun, strlen(cmdRun), bufRet, sizeof(bufRet), 10, 0x03)>0)
    {
        MO_DEBUG(("scanNetworks bufRet: %s", bufRet));
        char *ptr0 = NULL;
        char *ptr1 = NULL;
        char *ptr2 = NULL;
        ptr0 = bufRet;
        // +CWLAP:(3,"IntoRobot-Atom03F6",-64,"18:dd:aa:00:03:f7",1)
        while ( ((char *)strstr(ptr0, cmdRet)) != NULL )
        {
            ptr1 = (char *)strstr(ptr0, "\r\n");
            if (ptr1 != NULL)
            {
                char tmp[8] = {0};
                ptr2 = (char *)strstr( ptr0 + strlen(cmdRet) + 1, ",");
                if (ptr2 != NULL) // for encry
                {
                    memset(tmp, 0, sizeof(tmp));
                    memcpy(tmp, ptr0 + strlen(cmdRet) + 1, ptr2 - (ptr0 + strlen(cmdRet) +1) );
                    encry[numFlag] = atoi(tmp);
                    MO_DEBUG(("encry[%d]: %d", numFlag, encry[numFlag]));
                    ptr1 = (char *)strstr(ptr2 + 1, ",");
                    if (ptr1 != NULL) // for ssid
                    {
                        memcpy(ssid[numFlag], ptr2 + 2, ptr1 - (ptr2 + 2) - 1);
                        MO_DEBUG(("ssid[%d]: %s", numFlag, ssid[numFlag]));
                        ptr2 = (char *)strstr( ptr1 + 1, ",");
                        if (ptr2 != NULL) // for rssi
                        {
                            memset(tmp, 0, sizeof(tmp));
                            memcpy(tmp, ptr1 + 1, ptr2 - ptr1 - 1);
                            rssi[numFlag] = atoi(tmp);
                            MO_DEBUG(("rssi[%d]: %d", numFlag, rssi[numFlag]));
                            numFlag++;
                            MO_DEBUG(("numFlag: %d", numFlag));
                        }
                    }
                }
            }
        }
    }
    vPortFree(bufRet);
    return numFlag;
}

//获取第n个ssid   ssid长度小于32

void mo_WiFiClass_SSID_hal(unsigned char networkItem,char *ret_but)
{
    ret_but = ssid[networkItem];
}

//获取第n个 bssid
void mo_WiFiClass_BSSID_hal(unsigned char networkItem, unsigned char *bssid)
{
}

//获取第n个信号强度
int32_t mo_WiFiClass_RSSI_hal(uint8_t networkItem)
{
    return rssi[networkItem];
}

uint8_t mo_WiFiClass_encryptionType(uint8_t networkItem)
{
    return encry[networkItem];
}

// host---->ip   succeed 1  failed 0
uint8_t mo_WiFiClass_hostByName_hal(const char* aHostname,int *ret_buf)
{
    return 0;
}

//typedef char int8_t;
uint8_t mo_WiFiClass_ping_hal(const char *remoteIP, uint8_t nTries)
{
    return 0;
}

//typedef char int8_t;
uint8_t mo_set_mac_hal(const char *staMac, const char *apMac)
{

    char temp_cmd[128];
    // stop the smartconfig
    if(mo_drv_wifi_run_cmd("AT+CWSTOPSMART","OK",10))
    {
        MO_INFO(("STOP SMART OK!\r\n"));
    }
    mo_drv_wifi_run_cmd("AT+CWMODE_CUR=3","OK", 5);

    memset(temp_cmd,0x00,sizeof(temp_cmd));
    sprintf(temp_cmd,"AT+CIPSTAMAC_DEF=\"%s\"",staMac);
    if(!mo_drv_wifi_run_cmd(temp_cmd,"OK",10))
    {
        return 1;
    }

    memset(temp_cmd,0x00,sizeof(temp_cmd));
    sprintf(temp_cmd,"AT+CIPAPMAC_DEF=\"%s\"",apMac);
    if(mo_drv_wifi_run_cmd(temp_cmd,"OK",10))
    {
        return 0;
    }
    mo_drv_wifi_run_cmd("AT+CWMODE_DEF=1","OK",10);
    return 1;
}

void mo_get_mac_hal(char *pstamac, char *papmac)
{
    char cmdRunSTA[] = "AT+CIPSTAMAC_DEF?";
    char cmdRunAP[] = "AT+CIPAPMAC_DEF?";
    char cmdRetSTA[]  = "+CIPSTAMAC_DEF:";
    char cmdRetAP[]  = "+CIPAPMAC_DEF:";

    char bufRet[40] = {0};
    if(mo_drv_wifi_cmd_transfer(cmdRunSTA, strlen(cmdRunSTA), bufRet, sizeof(bufRet), 10, 0x03)>0)
    {
        MO_DEBUG(("bufRet: %s", bufRet));
        if ( ((char *)strstr(bufRet, cmdRetSTA)) != NULL )
        {
            char *ptr1 = (char *)strstr(bufRet, "\r\n");
            if (ptr1 != NULL)
            {
                memcpy(pstamac, bufRet + strlen(cmdRetSTA) + 1, ptr1 - bufRet - strlen(cmdRetSTA) - 2);
                MO_DEBUG(("stamac:%s", pstamac));
            }
        }
    }
    memset(bufRet, 0, sizeof(bufRet));
    if(mo_drv_wifi_cmd_transfer(cmdRunAP, strlen(cmdRunAP), bufRet, sizeof(bufRet), 10, 0x03)>0)
    {
        MO_DEBUG(("bufRet: %s", bufRet));
        if ( ((char *)strstr(bufRet, cmdRetAP)) != NULL )
        {
            char *ptr1 = (char *)strstr(bufRet, "\r\n");
            if (ptr1 != NULL)
            {
                memcpy(papmac, bufRet + strlen(cmdRetAP) + 1, ptr1 - bufRet - strlen(cmdRetAP) -2);
                MO_DEBUG(("apmac:%s", papmac));
            }
        }
    }
}



void mo_get_ssi_hal(char *pssid, char *pbssid, int &channel, int &rssi)
{
    char cmdRun[] = "AT+CWJAP_CUR?";
    char bufRet[60] = {0};
    // memset(bufRet, 0x00, sizeof(bufRet));
    if(mo_drv_wifi_cmd_transfer(cmdRun, strlen(cmdRun), bufRet, sizeof(bufRet), 10, 0x03)>0)
    {
        MO_DEBUG(("bufRet: %s", bufRet));
        char cmdRet[]  = "+CWJAP_CUR:";
        if ( ((char *)strstr(bufRet, cmdRet)) != NULL )
        {
            char *ptr1 = (char *)strstr(bufRet, ",");
            if (ptr1 != NULL)
            {
                memcpy(pssid, bufRet + strlen(cmdRet) + 1, ptr1 - bufRet - strlen(cmdRet) -2);
                MO_DEBUG(("ssid:%s", pssid));
                char *ptr2 = (char *)strstr(ptr1+1, ",");
                if (ptr2 != NULL)
                {
                    memcpy(pbssid, bufRet + (ptr1 - bufRet) + 1, ptr2 - ptr1 - 2);
                    MO_DEBUG(("bssid:%s", pbssid));
                    char *ptr3 = (char *)strstr(ptr2+1, ",");
                    if (ptr3 != NULL)
                    {
                        char chan[4] = {0};
                        memcpy(chan, bufRet + (ptr2 - bufRet) + 1, ptr3 - ptr2 - 2);
                        channel = atoi(chan);
                        MO_DEBUG(("channel: %d", channel));
                        char *ptr4 = (char *)strstr(ptr3+1, "\r\n");
                        if (ptr4 != NULL)
                        {
                            char rss[8] = {0};
                            memcpy(rss, bufRet + (ptr3 - bufRet) + 1, ptr4 - ptr3 -2);
                            rssi = atoi(rss);
                            MO_DEBUG(("rssi:%d", rssi));
                            return;
                        }
                    }
                }

            }
        }
    }
    pssid = NULL;
    pbssid = NULL;
    channel = 0;
    rssi = 0;
}
