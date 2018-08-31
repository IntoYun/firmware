/**
 ******************************************************************************
 * @file    wlan_hal.h
 * @author  Matthew McGowan
 * @version V1.0.0
 * @date    25-Sept-2014
 * @brief
 ******************************************************************************
  Copyright (c) 2013-2015 IntoRobot Industries, Inc.  All rights reserved.

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

#ifndef WLAN_HAL_H
#define	WLAN_HAL_H

#include <stdint.h>
#include <stdbool.h>
#include "molmc_log.h"
#include "inet_hal.h"
#include "socket_hal.h"
#include "tick_hal.h"
#include "net_hal.h"


#ifdef	__cplusplus
extern "C" {

#include <string.h> // for memset
#endif

typedef int wlan_result_t;

typedef struct __attribute__((__packed__))  _WLanConfig_t {
    uint16_t size;
    NetworkConfig nw;
    uint8_t uaSSID[33];
    uint8_t BSSID[6];			// since V2
} WLanConfig;

#define WLanConfig_Size_V1   (sizeof(NetworkConfig)+2+33)
#define WLanConfig_Size_V2   (WLanConfig_Size_V1+6)

STATIC_ASSERT(WLanConfigSize, sizeof(WLanConfig)==WLanConfig_Size_V2);

/**
 * Called once at startup to initialize the wlan hardware.
 */
void HAL_WLAN_Setup(void);

/**
 * Enable wifi without connecting.
 */
wlan_result_t HAL_WLAN_Activate(void);

/**
 * Disable wifi.
 */
wlan_result_t HAL_WLAN_Deactivate(void);

/**
 * Connect start the wireless connection.
 */
wlan_result_t  HAL_WLAN_Connect_Init(void);

/**
 * Finalize the connection by connecting to stored profiles.
 */
wlan_result_t  HAL_WLAN_Connect_Finalize(void);

wlan_result_t HAL_WLAN_Disconnect_Now(void);
/**
 * Cancel a previous call to any blocking wifi connect method.
 * @param called_from_isr - set to true if this is being called from an ISR.
 */
void HAL_WLAN_Connect_Cancel(bool called_from_isr);

/**
 * @return <0 for a valid signal strength, in db.
 *         0 for rssi not found (caller could retry)
 *         >0 for an error
 */
int HAL_WLAN_Connected_Rssi(void);

void HAL_WLAN_Drive_Now(void);

// Provide compatibility with the original cc3000 headers.
#ifdef WLAN_SEC_UNSEC
#undef WLAN_SEC_UNSEC
#undef WLAN_SEC_WEP
#undef WLAN_SEC_WPA
#undef WLAN_SEC_WPA2
#endif
typedef enum {
    WLAN_SEC_UNSEC = 0,
    WLAN_SEC_WEP,
    WLAN_SEC_WPA,
    WLAN_SEC_WPA2,
    WLAN_SEC_NOT_SET = 0xFF
} WLanSecurityType;

typedef enum {
    WLAN_CIPHER_NOT_SET = 0,
    WLAN_CIPHER_AES = 1,
    WLAN_CIPHER_TKIP = 2,
    WLAN_CIPHER_AES_TKIP = 3   // OR of AES and TKIP
} WLanSecurityCipher;

typedef struct {
    unsigned size;           // the size of this structure. allows older clients to work with newer HAL.
    const char* ssid;
    unsigned ssid_len;
    const char* password;
    unsigned password_len;
    WLanSecurityType security;
    WLanSecurityCipher cipher;
    unsigned channel;
    unsigned flags;
} WLanCredentials;

#define WLAN_SET_CREDENTIALS_FLAGS_DRY_RUN  (1<<0)

#define WLAN_SET_CREDENTIALS_UNKNOWN_SECURITY_TYPE (-1)
#define WLAN_SET_CREDENTIALS_CIPHER_REQUIRED (-2)

typedef struct WiFiAccessPoint {
   size_t size;
   char ssid[33];
   uint8_t ssidLength;
   uint8_t bssid[6];
   WLanSecurityType security;
   WLanSecurityCipher cipher;
   uint8_t channel;
   int maxDataRate;   // the mdr in bits/s
   int rssi;        // when scanning

#ifdef __cplusplus
   WiFiAccessPoint()
   {
       memset(this, 0, sizeof(*this));
       size = sizeof(*this);
   }
#endif
} WiFiAccessPoint;

typedef void (*wlan_scan_result_t)(WiFiAccessPoint* ap, void* cookie);

int HAL_WLAN_Clear_Credentials(void);
int HAL_WLAN_Has_Credentials(void);
bool HAL_WLAN_Reset_Credentials_Store_Required(void);
wlan_result_t  HAL_WLAN_Reset_Credentials_Store(void);
int HAL_WLAN_Set_Credentials(WLanCredentials* credentials);
int HAL_WLAN_Get_Credentials(wlan_scan_result_t callback, void* callback_data);

/**
 * Initialize imlink config mode.
 */
typedef enum {
    IMLINK_SUCCESS = 0,
    IMLINK_FAIL = 1,
    IMLINK_DOING = 2,
    IMLINK_IDLE = 3,
} imlink_status_t;

void HAL_WLAN_Imlink_Start(void);
void HAL_WLAN_Imlink_Stop(void);
imlink_status_t HAL_WLAN_Imlink_Get_Status(void);

void HAL_WLAN_Set_Error_Count(uint32_t error_count);

/**
 * Retrieve IP address info. Available after HAL_WLAN_notify_dhcp() has been callted.
 */
void HAL_WLAN_Fetch_Ipconfig(WLanConfig* config);

typedef enum {
    DYNAMIC_IP,
    STATIC_IP
} IPAddressSource;

/**
 * Sets the IP source - static or dynamic.
 */
void HAL_WLAN_Set_Ipaddress_Source(IPAddressSource source, bool persist, void* reserved);

/**
 * Sets the IP Addresses to use when the device is in static IP mode.
 * @param device
 * @param netmask
 * @param gateway
 * @param dns1
 * @param dns2
 * @param reserved
 */
void HAL_WLAN_Set_Ipaddress(const HAL_IPAddress* device, const HAL_IPAddress* netmask,
        const HAL_IPAddress* gateway, const HAL_IPAddress* dns1, const HAL_IPAddress* dns2, void* reserved);


/**
 * @param callback  The callback that receives each scanned AP
 * @param cookie    An opaque handle that is passed to the callback.
 * @return negative on error.
 */
int HAL_WLAN_Scan(wlan_scan_result_t callback, void* cookie);

/**
 * Lists all WLAN credentials currently stored on the device
 * @param callback  The callback that receives each stored AP
 * @param callback_data An opaque handle that is passed to the callback.
 * @return count of stored credentials, negative on error.
 */

int HAL_WLAN_Get_Macaddr(uint8_t *stamacaddr, uint8_t *apmacaddr);

#ifdef	__cplusplus
}
#endif

#endif	/* WLAN_H */

