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

#ifndef WIRING_WIFI_H_
#define WIRING_WIFI_H_

#include "firmware_platform_config.h"

#if FIRMWARE_CONFIG_WIRING_WIFI

#include <string.h>
#include "wiring_network.h"
#include "wlan_hal.h"
#include "system_network.h"
#include "inet_hal.h"
#include "wiring_thread.h"

class IPAddress;

namespace intorobot {

enum SecurityType {
    UNSEC = WLAN_SEC_UNSEC,
    WEP = WLAN_SEC_WEP,
    WPA = WLAN_SEC_WPA,
    WPA2 = WLAN_SEC_WPA2
};

class WiFiClass : public NetworkClass
{
    void setIPAddressSource(IPAddressSource source) {
        HAL_WLAN_Set_Ipaddress_Source(source, true, NULL);
    }

public:
    WiFiClass() {}
    ~WiFiClass() {}

    operator network_handle_t() {
        return 0;
    }

    WLanConfig* wifi_config() {
        return (WLanConfig*)network_config(*this, 0, NULL);
    }

    /**
     * Retrieves a 6-octet MAC address
     * @param mac
     * @return
     */
    uint8_t* macAddress(uint8_t *mac) {
        memcpy(mac, wifi_config()->nw.uaMacAddr, 6);
        return mac;
    }

    IPAddress localIP() {
        return IPAddress(wifi_config()->nw.aucIP);
    }

    IPAddress subnetMask() {
        return IPAddress(wifi_config()->nw.aucSubnetMask);
    }

    IPAddress gatewayIP() {
        return IPAddress(wifi_config()->nw.aucDefaultGateway);
    }

    IPAddress dnsServerIP() {
        return IPAddress(wifi_config()->nw.aucDNSServer);
    }

    IPAddress dhcpServerIP() {
        return IPAddress(wifi_config()->nw.aucDHCPServer);
    }

    uint8_t* BSSID(uint8_t* bssid) {
        memcpy(bssid, wifi_config()->BSSID, 6);
        return bssid;
    }

    const char *SSID() {
        return (const char *) wifi_config()->uaSSID;
    }

    int8_t RSSI();
    uint32_t ping(IPAddress remoteIP) {
        return ping(remoteIP, 5);
    }

    uint32_t ping(IPAddress remoteIP, uint8_t nTries) {
        return inet_ping(&remoteIP.raw(), *this, nTries, NULL);
    }

    void connect(unsigned flags=0) {
        network_connect(*this, flags, 0, NULL);
    }

    void disconnect(void) {
        network_disconnect(*this, 0, NULL);
    }

    bool connecting(void) {
        return network_connecting(*this, 0, NULL);
    }

    bool ready(void) {
        return network_ready(*this, 0, NULL);
    }

    void on(void) {
        network_on(*this, 0, 0, NULL);
    }

    void off(void) {
        network_off(*this, 0, 0, NULL);
    }

    void setCredentials(const char *ssid) {
        setCredentials(ssid, NULL, UNSEC);
    }

    void setCredentials(const char *ssid, const char *password) {
        setCredentials(ssid, password, WPA2);
    }

    void setCredentials(const char *ssid, const char *password, unsigned long security, unsigned long cipher=WLAN_CIPHER_NOT_SET) {
        setCredentials(ssid, strlen(ssid), password, strlen(password), security, cipher);
    }

    void setCredentials(const char *ssid, unsigned int ssidLen, const char *password,
            unsigned int passwordLen, unsigned long security=WLAN_SEC_UNSEC, unsigned long cipher=WLAN_CIPHER_NOT_SET) {

        WLanCredentials creds;
        memset(&creds, 0, sizeof(creds));
        creds.size = sizeof(creds);
        creds.ssid = ssid;
        creds.ssid_len = ssidLen;
        creds.password = password;
        creds.password_len = passwordLen;
        creds.security = WLanSecurityType(security);
        creds.cipher = WLanSecurityCipher(cipher);
        network_set_credentials(*this, 0, &creds, NULL);
    }

    bool hasCredentials(void) {
        return network_has_credentials(*this, 0, NULL);
    }

    bool clearCredentials(void) {
        return network_clear_credentials(*this, 0, NULL, NULL);
    }

    IPAddress resolve(const char* name)
    {
        HAL_IPAddress ip;
        return (inet_gethostbyname(name, strlen(name), &ip, *this, NULL)<0) ?
                IPAddress(uint32_t(0)) : IPAddress(ip);
    }

    void setStaticIP(const IPAddress& host, const IPAddress& netmask,
        const IPAddress& gateway, const IPAddress& dns)
    {
        HAL_WLAN_Set_Ipaddress(host, netmask, gateway, dns, NULL, NULL);
    }

    void useStaticIP()
    {
        setIPAddressSource(STATIC_IP);
    }

    void useDynamicIP()
    {
        setIPAddressSource(DYNAMIC_IP);
    }

    int scan(wlan_scan_result_t callback, void* cookie=NULL) {
        return HAL_WLAN_Scan(callback, cookie);
    }

    int scan(WiFiAccessPoint* results, size_t result_count);

    template <typename T>
    int scan(void (*handler)(WiFiAccessPoint* ap, T* instance), T* instance) {
        return scan((wlan_scan_result_t)handler, (void*)instance);
    }

    int getCredentials(WiFiAccessPoint* results, size_t result_count);

    //兼容之前的固件库
    bool status(void) {
        return network_ready(*this, 0, NULL);
    }
	//deprecated
    //bool status(void) __attribute__((deprecated("Please use WiFi.ready() instead")));

};

extern WiFiClass WiFi;

} // namespace intorobot

#endif // Wiring_WiFi

#endif
