/******************************************************************************
  SparkFunESP8266WiFi.h
  ESP8266 WiFi Shield Library Main Header File
  Jim Lindblom @ SparkFun Electronics
  Original Creation Date: June 20, 2015
http://github.com/sparkfun/SparkFun_ESP8266_AT_Arduino_Library

!!! Description Here !!!

Development environment specifics:
IDE: Arduino 1.6.5
Hardware Platform: Arduino Uno
ESP8266 WiFi Shield Version: 1.0

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
 ******************************************************************************/
#ifndef ESP8266_COMM_H_
#define ESP8266_COMM_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "hw_config.h"
#include "boot_mode.h"

// Common Responses
#define RESPONSE_OK            "OK\r\n"
#define RESPONSE_ERROR         "ERROR\r\n"
#define RESPONSE_FAIL          "FAIL"
#define RESPONSE_READY         "READY!"

// Basic AT Commands //
#define ESP8266_TEST           ""// Test AT startup
#define ESP8266_RESET          "+RST"// Restart module
#define ESP8266_VERSION        "+GMR"// View version info
#define ESP8266_SLEEP          "+GSLP"// Enter deep-sleep mode
#define ESP8266_ECHO_ENABLE    "E1" // AT commands echo
#define ESP8266_ECHO_DISABLE   "E0" // AT commands echo
#define ESP8266_RESTORE        "+RESTORE" // Factory reset
#define ESP8266_UART           "+UART" // UART configuration

// WiFi Functions //
#define ESP8266_WIFI_MODE      "+CWMODE" // WiFi mode (sta/AP/sta+AP)
#define ESP8266_CONNECT_AP     "+CWJAP" // Connect to AP
#define ESP8266_LIST_AP        "+CWLAP" // List available AP's
#define ESP8266_DISCONNECT     "+CWQAP" // Disconnect from AP
#define ESP8266_AP_CONFIG      "+CWSAP" // Set softAP configuration
#define ESP8266_STATION_IP     "+CWLIF" // List station IP's connected to softAP
#define ESP8266_DHCP_EN        "+CWDHCP" // Enable/disable DHCP
#define ESP8266_AUTO_CONNECT   "+CWAUTOCONN" // Connect to AP automatically
#define ESP8266_SET_STA_MAC    "+CIPSTAMAC" // Set MAC address of station
#define ESP8266_GET_STA_MAC    "+CIPSTAMAC" // Get MAC address of station
#define ESP8266_SET_AP_MAC     "+CIPAPMAC" // Set MAC address of softAP
#define ESP8266_SET_STA_IP     "+CIPSTA" // Set IP address of ESP8266 station
#define ESP8266_SET_AP_IP      "+CIPAP" // Set IP address of ESP8266 softAP

// TCP/IP Commands //
#define ESP8266_TCP_STATUS           "+CIPSTATUS" // Get connection status
#define ESP8266_TCP_CONNECT          "+CIPSTART" // Establish TCP connection or register UDP port
#define ESP8266_TCP_SEND             "+CIPSEND" // Send Data
#define ESP8266_TCP_CLOSE            "+CIPCLOSE" // Close TCP/UDP connection
#define ESP8266_GET_LOCAL_IP         "+CIFSR"// Get local IP address
#define ESP8266_TCP_MULTIPLE         "+CIPMUX" // Set multiple connections mode
#define ESP8266_SERVER_CONFIG        "+CIPSERVER" // Configure as server
#define ESP8266_TRANSMISSION_MODE    "+CIPMODE" // Set transmission mode
#define ESP8266_SET_SERVER_TIMEOUT   "+CIPSTO"  // Set timeout when ESP8266 runs as TCP server
#define ESP8266_PING                 "+PING"    // Function PING

// Firmware Update Commands //
#define ESP8266_FW_FWDOWN            "+IR_DOWNFILE"  //
#define ESP8266_FW_FWSIZE            "+IR_GETFILESIZE"
#define ESP8266_FW_FWPACKET          "+IR_GETFILEPACKET"//

// Command Response Timeouts //
#define COMMAND_RESPONSE_TIMEOUT    1000
#define COMMAND_FWDOWN_TIMEOUT      10000

#define COMMAND_REPEAT_TIME        3

typedef enum
{
    ESP8266_CMD_BAD        = -5,
    ESP8266_RSP_MEMORY_ERR = -4,
    ESP8266_RSP_FAIL       = -3,
    ESP8266_RSP_UNKNOWN    = -2,
    ESP8266_RSP_TIMEOUT    = -1,
    ESP8266_RSP_SUCCESS    = 0
} Esp8266_Cmd_RspDef;

typedef enum
{
    ESP8266_CMD_QUERY,
    ESP8266_CMD_SETUP,
    ESP8266_CMD_EXECUTE
} Esp8266_Cmd_TypeDef;


void ESP8266_Test(void);
void ESP8266_Wait_Ready(void);
void ESP8266_Init(void);
int16_t ESP8266_Firmware_Size(Firmware_TypeDef FmType, uint32_t PacketSize);
int16_t ESP8266_Firware_Packet(Firmware_TypeDef FmType, uint32_t PacketSize, uint32_t PacketIndex, uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif
