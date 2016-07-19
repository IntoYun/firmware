/**
 ******************************************************************************
 * @file     : lib_wifi.h
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
#ifndef   WIRING_WIFI_H_
#define   WIRING_WIFI_H_

#include <inttypes.h>

#include "lib_tcpclient.h"
#include "lib_tcpserver.h"
#include "lib_process.h"

// Maximum size of ap list string
#define AP_LIST_MAX_LENGTH 2000
// Maximum size of a SSID
#define WL_SSID_MAX_LENGTH 32
// Size of a MAC-address or BSSID
#define WL_MAC_ADDR_LENGTH 6
// Size of a MAC-address or BSSID
#define WL_IPV4_LENGTH 4


/* Encryption modes */
#define ENC_TYPE_ERRER 0
#define ENC_TYPE_WEP   1
#define ENC_TYPE_WAP   2
#define ENC_TYPE_WAP2  3
#define ENC_TYPE_NONE  4


class WiFiClass : public Process
{
    private:
        // settings of current selected network
        char _ssid[WL_SSID_MAX_LENGTH];
        uint8_t _bssid[WL_MAC_ADDR_LENGTH];
        uint8_t _mac[WL_MAC_ADDR_LENGTH];
        uint8_t _localIp[WL_IPV4_LENGTH];
        uint8_t _subnetMask[WL_IPV4_LENGTH];
        uint8_t _gatewayIp[WL_IPV4_LENGTH];

    public:
        WiFiClass();

        void connect(void);
        uint8_t status(void);

        uint8_t setCredentials(const char *ssid);
        uint8_t setCredentials(const char *ssid, const char *password);
        uint8_t setCredentials(const char *ssid, const char *password, const char *channel, const char *security);

        void config(IPAddress local_ip);
        void config(IPAddress local_ip, IPAddress dns_server);
        void config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway);
        void config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet);

        void config(const char *local_ip);
        void config(const char *local_ip, const char *dns_server);
        void config(const char *local_ip, const char *dns_server, const char *gateway);
        void config(const char *local_ip, const char *dns_server, const char *gateway, const char *subnet);

        void setDNS(IPAddress dns_server1);
        void setDNS(IPAddress dns_server1, IPAddress dns_server2);
        void setDNS(const char *dns_server1);
        void setDNS(const char *dns_server1, const char *dns_server2);

        uint8_t* macAddress(uint8_t* mac);
        IPAddress localIP(void);
        IPAddress subnetMask(void);
        IPAddress gatewayIP(void);

        char* SSID(void);
        uint8_t* BSSID(uint8_t* bssid);
        int32_t RSSI(void);
        uint8_t encryptionType(void);

        int8_t scanNetworks(void);
        bool getApList(char *aplist);
        void restartNetwork(void);
        char* SSID(uint8_t networkItem);
        uint8_t* BSSID(uint8_t networkItem, uint8_t* bssid);
        int32_t RSSI(uint8_t networkItem);
        uint8_t encryptionType(uint8_t networkItem);


        int hostByName(const char* aHostname, IPAddress& aResult);
        uint8_t ping(IPAddress remoteIP);
        uint8_t ping(IPAddress remoteIP, uint8_t nTries);
        uint8_t ping(const char *remoteIP);
        uint8_t ping(const char *remoteIP, uint8_t nTries);

	/* uint8_t startWiFiSmartconfig(void); */
	/* uint8_t killSmartconfigProcess(void); */

        friend class TcpClient;
        friend class TcpServer;
        friend class UdpServer;
};

extern WiFiClass WiFi;

#endif /*WIRING_WIFI_H_*/
