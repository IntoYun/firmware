/**
 ******************************************************************************
 * @file     : lib_wifi.cpp
 * @author   : robin
 * @version  : V1.0.0
 * @date     : 6-December-2014
 * @brief    :
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
#include "lib_wifi.h"
#include "lib_wifi_hal.h"

#include <stdint.h>
#include <stdbool.h>


#include "wiring.h"



// The following three global variables to store apscan ssid, rssi, encry


/*********************************************************************************
  *Function       :
  *Description   :
  *Input           :
  *Output         :
  *Return         :
  *author         :
  *date            :
  *Others         :
**********************************************************************************/
WiFiClass::WiFiClass()
{
    // settings of current selected network
    memset(_ssid, 0, WL_SSID_MAX_LENGTH);
    memset(_bssid, 0, WL_MAC_ADDR_LENGTH);
    memset(_mac, 0, WL_MAC_ADDR_LENGTH);
    memset(_localIp, 0, WL_IPV4_LENGTH);
    memset(_subnetMask, 0, WL_IPV4_LENGTH);
    memset(_gatewayIp, 0, WL_IPV4_LENGTH);
}

/*********************************************************************************
  *Function      :  Note: connect() is a Asynchronous process
  *Description  :   wifi connect
  *Input          :
  *Output        :
  *Return        :
  *author        :
  *date           :
  *Others        :
**********************************************************************************/
void WiFiClass::connect(void)
{
    mo_WiFiClass_connect_hal();

    /*
    begin("wifi_connect");
    addParameter("BEGIN");
    runAsynchronously();*/
}

/*********************************************************************************
  *Function     :
  *Description  :   get the status of connection
  *Input        :
  *Output       :
  *Return       :     status of connect
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
uint8_t WiFiClass::status(void)
{



	return mo_WiFiClass_status_hal();

	#if 0
	static uint32_t time_count=0; //connected 执行计时器
	static uint8_t cache_conect=0;   //1连接  0断开

 // MO_INFO(("time_count=%d now_time:%d",time_count,millis()));

  if(timerIsEnd(time_count,10*1000))
  {
    time_count=millis();
    cache_conect=mo_WiFiClass_status_hal();
  }

  return cache_conect;
	#endif



/*
    begin("wifi_connect");
    addParameter("STATUS");
    int res = run();
    if(res == 0)
    {
        return true;
    }
    return false;*/
}

/*********************************************************************************
  *Function     :
  *Description  :   Restart the network, Asynchronous process
  *Input        :
  *Output       :
  *Return       :
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
void WiFiClass::restartNetwork(void)
{

    mo_WiFiClass_restartNetwork_hal();


/*
    begin("wifi_reload");
    run();*/
}

/*********************************************************************************
  *Function     :
  *Description  :   set che wifi  credentials.  <------- Deprecated
  *Input        :
  *Output       :
  *Return       :
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
uint8_t WiFiClass::setCredentials(const char *ssid)
{
    return setCredentials(ssid, NULL, "auto", "NONE");
}

/*********************************************************************************
  *Function     :
  *Description  :   set che wifi  credentials.  <------- Deprecated
  *Input        :
  *Output       :
  *Return       :
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
uint8_t WiFiClass::setCredentials(const char *ssid, const char *password)
{
    return setCredentials(ssid, password, "auto", "NONE");
}

/*********************************************************************************
  *Function     :
  *Description  :   set che wifi  credentials
  *Input        :
  *Output       :
  *Return       :
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
uint8_t WiFiClass::setCredentials(const char *ssid, const char *password, const char *channel, const char *security)
{
    return mo_WiFiClass_setCredentials_hal(ssid, password, channel, security);
}

/*********************************************************************************
  *Function     :
  *Description  :   Change Ip configuration settings disabling the dhcp client
  *Input        :       Static ip configuration
  *Output       :
  *Return       :
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
void WiFiClass::config(IPAddress local_ip)
{
    char local_ip_tmp[20];

    //local_ip
    memset(local_ip_tmp,0,sizeof(local_ip_tmp));
    sprintf(local_ip_tmp,"%d.%d.%d.%d",local_ip[0],local_ip[1],local_ip[2],local_ip[3]);

    config(local_ip_tmp);
}

/*********************************************************************************
  *Function     :
  *Description  :   Change Ip configuration settings disabling the dhcp client
  *Input        :     local_ip  dns_server
  *Output       :
  *Return       :
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
void WiFiClass::config(IPAddress local_ip, IPAddress dns_server)
{
    char local_ip_tmp[20],dns_server_tmp[20];

    //local_ip
    memset(local_ip_tmp,0,sizeof(local_ip_tmp));
    sprintf(local_ip_tmp,"%d.%d.%d.%d",local_ip[0],local_ip[1],local_ip[2],local_ip[3]);
    //dns_server
    memset(dns_server_tmp,0,sizeof(dns_server_tmp));
    sprintf(dns_server_tmp,"%d.%d.%d.%d",dns_server[0],dns_server[1],dns_server[2],dns_server[3]);

    config(local_ip_tmp, dns_server_tmp);
}

/*********************************************************************************
  *Function     :
  *Description  :    Change Ip configuration settings disabling the dhcp client
  *Input        :       local_ip  dns_server gateway
  *Output       :
  *Return       :
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
void WiFiClass::config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway)
{
    char local_ip_tmp[20],dns_server_tmp[20],gateway_tmp[20];

    //local_ip
    memset(local_ip_tmp,0,sizeof(local_ip_tmp));
    sprintf(local_ip_tmp,"%d.%d.%d.%d",local_ip[0],local_ip[1],local_ip[2],local_ip[3]);
    //dns_server
    memset(dns_server_tmp,0,sizeof(dns_server_tmp));
    sprintf(dns_server_tmp,"%d.%d.%d.%d",dns_server[0],dns_server[1],dns_server[2],dns_server[3]);
    //gateway
    memset(gateway_tmp,0,sizeof(gateway_tmp));
    sprintf(gateway_tmp,"%d.%d.%d.%d",gateway[0],gateway[1],gateway[2],gateway[3]);

    config(local_ip_tmp, dns_server_tmp, gateway_tmp);
}

/*********************************************************************************
  *Function     :
  *Description  :   Change Ip configuration settings disabling the dhcp client
  *Input        :     local_ip    dns_server    gateway    subnet
  *Output       :
  *Return       :
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
void WiFiClass::config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet)
{
    char local_ip_tmp[20],dns_server_tmp[20],gateway_tmp[20],subnet_tmp[20];

    //local_ip
    memset(local_ip_tmp,0,sizeof(local_ip_tmp));
    sprintf(local_ip_tmp,"%d.%d.%d.%d",local_ip[0],local_ip[1],local_ip[2],local_ip[3]);
    //dns_server
    memset(dns_server_tmp,0,sizeof(dns_server_tmp));
    sprintf(dns_server_tmp,"%d.%d.%d.%d",dns_server[0],dns_server[1],dns_server[2],dns_server[3]);
    //gateway
    memset(gateway_tmp,0,sizeof(gateway_tmp));
    sprintf(gateway_tmp,"%d.%d.%d.%d",gateway[0],gateway[1],gateway[2],gateway[3]);
    //subnet
    memset(subnet_tmp,0,sizeof(subnet_tmp));
    sprintf(subnet_tmp,"%d.%d.%d.%d",subnet[0],subnet[1],subnet[2],subnet[3]);

    config(local_ip_tmp, dns_server_tmp, gateway_tmp, subnet_tmp);
}

/*********************************************************************************
  *Function     :
  *Description  :   Change Ip configuration settings disabling the dhcp client
  *Input        :     local_ip
  *Output       :
  *Return       :
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
void WiFiClass::config(const char *local_ip)
{
    mo_WiFiClass_config_hal1(local_ip);

/*
    begin("wifi_set_ifconfig");
    //local_ip
    addParameter(local_ip);
    run();*/
}

/*********************************************************************************
  *Function     :
  *Description  :   Change Ip configuration settings disabling the dhcp client
  *Input        :     local_ip  dns_server
  *Output       :
  *Return       :
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
void WiFiClass::config(const char *local_ip, const char *dns_server)
{

    mo_WiFiClass_config_hal2(local_ip, dns_server);


/*
    begin("wifi_set_ifconfig");
    //local_ip
    addParameter(local_ip);
    //dns_server
    addParameter(dns_server);
    run();*/
}

/*********************************************************************************
  *Function     :
  *Description  :   Change Ip configuration settings disabling the dhcp client
  *Input        :      local_ip     dns_server     gateway
  *Output       :
  *Return       :
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
void WiFiClass::config(const char *local_ip, const char *dns_server, const char *gateway)
{

    mo_WiFiClass_config_hal3(local_ip, dns_server, gateway);

/*
    begin("wifi_set_ifconfig");
    //local_ip
    addParameter(local_ip);
    //dns_server
    addParameter(dns_server);
    //gateway
    addParameter(gateway);
    run();*/
}

/*********************************************************************************
  *Function     :
  *Description  :   Change Ip configuration settings disabling the dhcp client
  *Input        :     local_ip        dns_server    gateway    subnet
  *Output       :
  *Return       :
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
void WiFiClass::config(const char *local_ip, const char *dns_server, const char *gateway, const char *subnet)
{

    mo_WiFiClass_config_hal4(local_ip, dns_server, gateway, subnet);



/*
    begin("wifi_set_ifconfig");
    //local_ip
    addParameter(local_ip);
    //dns_server
    addParameter(dns_server);
    //gateway
    addParameter(gateway);
    //subnet
    addParameter(subnet);
    run();*/
}

/*********************************************************************************
  *Function     :
  *Description  :   Change DNS Ip configuration
  *Input        :     dns_server1
  *Output       :
  *Return       :
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
void WiFiClass::setDNS(IPAddress dns_server1)
{
    char dns_server1_tmp[20];

    //dns_server1
    memset(dns_server1_tmp,0,sizeof(dns_server1_tmp));
    sprintf(dns_server1_tmp,"%d.%d.%d.%d",dns_server1[0],dns_server1[1],dns_server1[2],dns_server1[3]);

    setDNS(dns_server1_tmp);
}

/*********************************************************************************
  *Function     :
  *Description  :    Change DNS Ip configuration
  *Input        :      dns_server1   dns_server2
  *Output       :
  *Return       :
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
void WiFiClass::setDNS(IPAddress dns_server1, IPAddress dns_server2)
{
    char dns_server1_tmp[20],dns_server2_tmp[20];

    //dns_server1
    memset(dns_server1_tmp,0,sizeof(dns_server1_tmp));
    sprintf(dns_server1_tmp,"%d.%d.%d.%d",dns_server1[0],dns_server1[1],dns_server1[2],dns_server1[3]);
    //dns_server2
    memset(dns_server2_tmp,0,sizeof(dns_server2_tmp));
    sprintf(dns_server2_tmp,"%d.%d.%d.%d",dns_server2[0],dns_server2[1],dns_server2[2],dns_server2[3]);

    setDNS(dns_server1_tmp, dns_server2_tmp);
}

/*********************************************************************************
  *Function     :
  *Description  :   Change DNS Ip configuration
  *Input        :     dns_server1
  *Output       :
  *Return       :
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
void WiFiClass::setDNS(const char *dns_server1)
{

    mo_WiFiClass_setDNS_hal1(dns_server1);

/*
    begin("wifi_set_dnsserver");
    //dns_server1
    addParameter(dns_server1); //local_ip
    run();*/
}

/*********************************************************************************
  *Function     :
  *Description  :    Change DNS Ip configuration
  *Input        :      dns_server1  dns_server2
  *Output       :
  *Return       :
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
void WiFiClass::setDNS(const char *dns_server1, const char *dns_server2)
{

    mo_WiFiClass_setDNS_hal2(dns_server1,dns_server2);

/*
    begin("wifi_set_dnsserver");
    //dns_server1
    addParameter(dns_server1);
    //dns_server2
    addParameter(dns_server2);
    run();*/
}

/*********************************************************************************
  *Function     :
  *Description  :   Get the interface MAC address.
  *Input        :      mac
  *Output       :
  *Return       :
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
uint8_t* WiFiClass::macAddress(uint8_t* mac)
{

    return mo_WiFiClass_macAddress_hal(mac);

/*
    String tmp;
    int mactemp[WL_MAC_ADDR_LENGTH];

    memset(_mac,0,sizeof(_mac));
    begin("wifi_get_ifconfig");
    addParameter("MACADDRESS");
    int res = run();
    if(res == 0)
    {
        while (available())
        {
            tmp+=(char)read();
        }
        sscanf(tmp.c_str(),"%x:%x:%x:%x:%x:%x",&mactemp[5],&mactemp[4],&mactemp[3],&mactemp[2],&mactemp[1],&mactemp[0]);
        for(int i=0; i<WL_MAC_ADDR_LENGTH; i++)
        {_mac[i]=mactemp[i];}
        memcpy(mac,_mac,WL_MAC_ADDR_LENGTH);
    }
    return mac;*/
}

/*********************************************************************************
  *Function     :
  *Description  :   Get the interface IP address.
  *Input        :     localIP
  *Output       :
  *Return       :
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
IPAddress WiFiClass::localIP(void)
{

    int IpTemp[4];

    mo_WiFiClass_localIP_hal(IpTemp);

    memset(_localIp,0,sizeof(_localIp));
    for(int i=0;i<4;i++)
    {_localIp[i]=IpTemp[i];}

    return _localIp;


#if 0
    String tmp;
    int IpTemp[4];

    memset(_localIp,0,sizeof(_localIp));
    begin("wifi_get_ifconfig");
    addParameter("LOCALIP");
    int res = run();
    if(res == 0)
    {
        while (available())
        {
            tmp+=(char)read();
        }
        sscanf(tmp.c_str(),"%d.%d.%d.%d",&IpTemp[0],&IpTemp[1],&IpTemp[2],&IpTemp[3]);
        for(int i=0;i<4;i++)
        {_localIp[i]=IpTemp[i];}
    }
    return _localIp;

#endif

}

/*********************************************************************************
  *Function     :
  *Description  :   Get the interface subnet mask address.
  *Input        :
  *Output       :
  *Return       :     subnet mask address value
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
IPAddress WiFiClass::subnetMask(void)
{
    String tmp;
    int IpTemp[4];

    mo_WiFiClass_subnetMask_hal(IpTemp);

    memset(_subnetMask,0,sizeof(_subnetMask));
    for(int i=0;i<4;i++)
    {_subnetMask[i]=IpTemp[i];}

    return _subnetMask;



#if 0
    String tmp;
    int IpTemp[4];

    memset(_subnetMask,0,sizeof(_subnetMask));
    begin("wifi_get_ifconfig");
    addParameter("SUBNETMASK");
    int res = run();
    if(res == 0)
    {
        while (available())
        {
            tmp+=(char)read();
        }
        sscanf(tmp.c_str(),"%d.%d.%d.%d",&IpTemp[0],&IpTemp[1],&IpTemp[2],&IpTemp[3]);
        for(int i=0;i<4;i++)
        {_subnetMask[i]=IpTemp[i];}
    }
    return _subnetMask;

#endif


}

/*********************************************************************************
  *Function     :
  *Description  :   Get the gateway ip address.
  *Input        :
  *Output       :
  *Return       :    gateway ip address value
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
IPAddress WiFiClass::gatewayIP(void)
{


    String tmp;
    int IpTemp[4];

    memset(_gatewayIp,0,sizeof(_gatewayIp));

    mo_WiFiClass_gatewayIP_hal(IpTemp);

    for(int i=0;i<4;i++)
    {_gatewayIp[i]=IpTemp[i];}

    return _gatewayIp;


    #if 0
    String tmp;
    int IpTemp[4];

    memset(_gatewayIp,0,sizeof(_gatewayIp));
    begin("wifi_get_ifconfig");

    addParameter("GATEWAYIP");
    int res = run();
    if(res == 0)
    {
        while (available())
        {
            tmp+=(char)read();
        }
        sscanf(tmp.c_str(),"%d.%d.%d.%d",&IpTemp[0],&IpTemp[1],&IpTemp[2],&IpTemp[3]);
        for(int i=0;i<4;i++)
        {_gatewayIp[i]=IpTemp[i];}
    }
    return _gatewayIp;
    #endif
}

/*********************************************************************************
  *Function     :
  *Description  :   Return the current SSID associated with the network
  *Input        :
  *Output       :
  *Return       :    ssid string
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
char* WiFiClass::SSID(void)
{
        String tmp;

        memset(_ssid,0,sizeof(_ssid));


        mo_WiFiClass_SSID_hal(_ssid);

        /*
        begin("wifi_get_current");
        addParameter("SSID");
        int res = run();
        if(res == 0)
        {
            while (available())
            {
                tmp+=(char)read();
            }
            if(tmp.length()<WL_SSID_MAX_LENGTH)
            {strcpy(_ssid,tmp.c_str());}
            else
            {memcpy(_ssid,tmp.c_str(),WL_SSID_MAX_LENGTH-1);}
        }
        */

        return _ssid;



    #if 0
    String tmp;

    memset(_ssid,0,sizeof(_ssid));
    begin("wifi_get_current");
    addParameter("SSID");
    int res = run();
    if(res == 0)
    {
        while (available())
        {
            tmp+=(char)read();
        }
        if(tmp.length()<WL_SSID_MAX_LENGTH)
        {strcpy(_ssid,tmp.c_str());}
        else
        {memcpy(_ssid,tmp.c_str(),WL_SSID_MAX_LENGTH-1);}
    }
    return _ssid;
    #endif

}


/*********************************************************************************
  *Function     :
  *Description  :   Return the current BSSID associated with the network. It is the MAC address of the Access Point
  *Input        :
  *Output       :
  *Return       :     pointer to uint8_t array with length WL_MAC_ADDR_LENGTH
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
uint8_t* WiFiClass::BSSID(uint8_t* bssid)
{
#if 1
        String tmp;
        int mactemp[WL_MAC_ADDR_LENGTH];

        memset(_bssid,0,sizeof(_bssid));

        mo_WiFiClass_BSSID(_bssid);

        memcpy(bssid,_bssid,WL_MAC_ADDR_LENGTH);

        return bssid;

#endif


#if 0
    String tmp;
    int mactemp[WL_MAC_ADDR_LENGTH];

    memset(_bssid,0,sizeof(_bssid));
    begin("wifi_get_current");
    addParameter("BSSID");
    int res = run();
    if(res == 0)
    {
        while (available())
        {
            tmp+=(char)read();
        }
        sscanf(tmp.c_str(),"%x:%x:%x:%x:%x:%x",&mactemp[5],&mactemp[4],&mactemp[3],&mactemp[2],&mactemp[1],&mactemp[0]);
        for(int i=0; i<WL_MAC_ADDR_LENGTH; i++)
        {_bssid[i]=mactemp[i];}
        memcpy(bssid,_bssid,WL_MAC_ADDR_LENGTH);
    }
    return bssid;

#endif

}

/*********************************************************************************
  *Function     :
  *Description  :   Return the current RSSI /Received Signal Strength  percent
  *Input        :
  *Output       :
  *Return       :     signed value
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
int32_t WiFiClass::RSSI(void)
{


    return mo_WiFiClass_RSSI_hal();

#if 0
    String tmp;
    int32_t rssi = 0;

    begin("wifi_get_current");
    addParameter("RSSI");
    int res = run();
    if(res == 0)
    {
        while (available())
        {
            tmp+=(char)read();
        }
        rssi=tmp.toInt();
    }
    return rssi;

#endif

}

/*********************************************************************************
  *Function     :
  *Description  :   Return the Encryption Type associated with the network
  *Input        :
  *Output       :
  *Return       :
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
uint8_t WiFiClass::encryptionType(void)
{


    return mo_WiFiClass_encryptionType_hal();


#if 0

    String tmp;

    uint8_t encType = ENC_TYPE_ERRER;
    memset(_bssid,0,sizeof(_bssid));
    begin("wifi_get_current");
    addParameter("SEC");
    int res = run();
    if(res == 0)
    {
        while (available())
        {
            tmp+=(char)read();
        }
        if(tmp.equals("WEP"))
        {
            encType=ENC_TYPE_WEP;
        }
        else if(tmp.equals("WPA"))
        {
            encType=ENC_TYPE_WAP;
        }
        else if(tmp.equals("WPA2"))
        {
            encType=ENC_TYPE_WAP2;
        }
        else if(tmp.equals("NONE"))
        {
            encType=ENC_TYPE_NONE;
        }
        else
        {
            encType=ENC_TYPE_ERRER;
        }
    }
    return encType;


#endif

}

/*********************************************************************************
  *Function     :
  *Description  :   Start scan WiFi networks available
  *Input        :
  *Output       :
  *Return       :     Number of discovered networks
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
int8_t WiFiClass::scanNetworks(void)
{
    return mo_WiFiClass_scanNetworks_hal();

/*
    String tmp;
    uint8_t numOfNetworks = -1;

    begin("wifi_get_ap_list");
    addParameter("LISTNUM");
    int res = run();
    if(res == 0)
    {
        while (available())
        {
            tmp+=(char)read();
        }
        numOfNetworks=tmp.toInt();
    }
    return numOfNetworks;*/
}

/*********************************************************************************
  *Function     :
  *Description  :   Return the SSID discovered during the network scan.
  *Input        :     networkItem: specify from which network item want to get the information
  *Output       :
  *Return       :     ssid string of the specified item on the networks scanned list
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
char* WiFiClass::SSID(uint8_t networkItem)
{

        memset(_ssid,0,sizeof(_ssid));
        mo_WiFiClass_SSID_hal(networkItem,_ssid);
        return _ssid;
/*
    String tmp;

    memset(_ssid,0,sizeof(_ssid));
    begin("wifi_get_ap_list");
    addParameter("SSID");
    addParameter(String(networkItem));
    int res = run();
    if(res == 0)
    {
        while (available())
        {
            tmp+=(char)read();
        }
        if(tmp.length()<WL_SSID_MAX_LENGTH)
        {strcpy(_ssid,tmp.c_str());}
        else
        {memcpy(_ssid,tmp.c_str(),WL_SSID_MAX_LENGTH-1);}
    }
    return _ssid;*/
}

/*********************************************************************************
  *Function     :
  *Description  :   Return the current BSSID associated with the network.It is the MAC address of the Access Point
  *Input        :
  *Output       :
  *Return       :    pointer to uint8_t array with length WL_MAC_ADDR_LENGTH
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
uint8_t* WiFiClass::BSSID(uint8_t networkItem, uint8_t* bssid)
{

        memset(_bssid,0,sizeof(_bssid));

        mo_WiFiClass_BSSID_hal(networkItem, _bssid);


        memcpy(bssid,_bssid,WL_MAC_ADDR_LENGTH);

        return bssid;



/*
    String tmp;
    int mactemp[WL_MAC_ADDR_LENGTH];

    memset(_bssid,0,sizeof(_bssid));
    begin("wifi_get_ap_list");
    addParameter("BSSID");
    addParameter(String(networkItem));
    int res = run();
    if(res == 0)
    {
        while (available())
        {
            tmp+=(char)read();
        }
        sscanf(tmp.c_str(),"%x:%x:%x:%x:%x:%x",&mactemp[5],&mactemp[4],&mactemp[3],&mactemp[2],&mactemp[1],&mactemp[0]);
        for(int i=0; i<WL_MAC_ADDR_LENGTH; i++)
        {_bssid[i]=mactemp[i];}
        memcpy(bssid,_bssid,WL_MAC_ADDR_LENGTH);
    }
    return bssid;*/
}

/*********************************************************************************
  *Function     :
  *Description  :   Return the RSSI of the networks discovered during the scanNetworks
  *Input        :     networkItem: specify from which network item want to get the information
  *Output       :
  *Return       :     signed value of RSSI of the specified item on the networks scanned list
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
int32_t WiFiClass::RSSI(uint8_t networkItem)
{

    return mo_WiFiClass_RSSI_hal(networkItem);


/*
    String tmp;
    int32_t rssi = 0;

    begin("wifi_get_ap_list");
    addParameter("RSSI");
    addParameter(String(networkItem));
    int res = run();
    if(res == 0)
    {
        while (available())
        {
            tmp+=(char)read();
        }
        rssi=tmp.toInt();
    }
    return rssi;*/
}

/*********************************************************************************
  *Function     :
  *Description  :   Return the encryption type of the networks discovered during the scanNetworks
  *Input        :     networkItem: specify from which network item want to get the information
  *Output       :
  *Return       :    encryption type of the specified item on the networks scanned list
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
uint8_t WiFiClass::encryptionType(uint8_t networkItem)
{

    return mo_WiFiClass_encryptionType(networkItem);




/*
    String tmp;

    uint8_t encType = ENC_TYPE_ERRER;
    memset(_bssid,0,sizeof(_bssid));
    begin("wifi_get_ap_list");
    addParameter("SEC");
    addParameter(String(networkItem));
    int res = run();
    if(res == 0)
    {
        while (available())
        {
            tmp+=(char)read();
        }
        if(tmp.equals("WEP"))
        {
            encType=ENC_TYPE_WEP;
        }
        else if(tmp.equals("WPA"))
        {
            encType=ENC_TYPE_WAP;
        }
        else if(tmp.equals("WPA2"))
        {
            encType=ENC_TYPE_WAP2;
        }
        else if(tmp.equals("NONE"))
        {
            encType=ENC_TYPE_NONE;
        }
        else
        {
            encType=ENC_TYPE_ERRER;
        }
    }
    return encType;
*/
}

/*********************************************************************************
  *Function     :
  *Description  :   Resolve the given hostname to an IP address.
  *Input        :     aHostname: Name to be resolved      aResult: IPAddress structure to store the returned IP address
  *Output       :
  *Return       :     1 if aIPAddrString was successfully converted to an IP address  else error code
  *author       :
  *date         :
  *Others       :
**********************************************************************************/
int WiFiClass::hostByName(const char* aHostname, IPAddress& aResult)
{

        String tmp;
        int IpTemp[WL_IPV4_LENGTH];

        if(mo_WiFiClass_hostByName_hal(aHostname,IpTemp))
        {
            for(int i=0;i<WL_IPV4_LENGTH;i++)
            {aResult[i]=IpTemp[i];}
            return true;
        }
        return false;

/*
    String tmp;
    int IpTemp[WL_IPV4_LENGTH];

    begin("wifi_get_hostip");
    addParameter(aHostname);
    int res = run();
    if(res == 0)
    {
        while (available())
        {
            tmp+=(char)read();
        }
        sscanf(tmp.c_str(),"%d.%d.%d.%d",&IpTemp[0],&IpTemp[1],&IpTemp[2],&IpTemp[3]);
        for(int i=0;i<WL_IPV4_LENGTH;i++)
        {aResult[i]=IpTemp[i];}
        return true;
    }
    return false;*/
}

/*********************************************************************************
  *Function		:
  *Description	:    ping
  *Input		      :    the ip to ping
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
uint8_t WiFiClass::ping(IPAddress remoteIP)
{
    return ping(remoteIP, 1);
}

/*********************************************************************************
  *Function		:
  *Description	:    ping
  *Input		      :    remoteIP:the ip to ping       nTries:  try count
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
uint8_t WiFiClass::ping(IPAddress remoteIP, uint8_t nTries)
{
    char ip[20];

    memset(ip,0,sizeof(ip));
    sprintf(ip,"%d.%d.%d.%d",remoteIP[0],remoteIP[1],remoteIP[2],remoteIP[3]);
    return ping(ip, nTries);
}

/*********************************************************************************
  *Function		:
  *Description	:    ping
  *Input		      :    remoteIP:the ip to ping
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
uint8_t WiFiClass::ping(const char *remoteIP)
{
    return ping(remoteIP, 1);
}

/*********************************************************************************
  *Function		:
  *Description	:    ping
  *Input		      :    remoteIP:the ip to ping       nTries:  try count
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/

//typedef int int8_t;


uint8_t WiFiClass::ping(const  char *remoteIP, uint8_t nTries)
{

    return mo_WiFiClass_ping_hal(remoteIP, nTries);




/*
    begin("ping");
    addParameter(remoteIP);
    addParameter("-W");
    addParameter(String(1));
    addParameter("-c");
    addParameter(String(nTries));
    int res = run();
    if(res == 0)
    {
        return true;
    }
    return false;*/
}

WiFiClass WiFi;

