#ifndef ESP8266_COMM_H_
#define ESP8266_COMM_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "boot_mode.h"

// Common Responses
#define RESPONSE_OK            "OK\r\n"
#define RESPONSE_ERROR         "ERROR\r\n"
#define RESPONSE_FAIL          "FAIL"
#define RESPONSE_READY         "READY!"

// Basic AT Commands //
#define ESP8266_TEST           ""// Test AT startup
#define ESP8266_ECHO_ENABLE    "E1" // AT commands echo
#define ESP8266_ECHO_DISABLE   "E0" // AT commands echo

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

void ESP8266_Init(void);
bool ESP8266_SelfCheck(void);

#ifdef __cplusplus
}
#endif

#endif
