#ifndef CELLULAR_COMM_H_
#define CELLULAR_COMM_H_


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
#define CELLULAR_TEST           ""   // Test AT startup
#define CELLULAR_ECHO_ENABLE    "E1" // AT commands echo
#define CELLULAR_ECHO_DISABLE   "E0" // AT commands echo

typedef enum
{
    CELLULAR_CMD_BAD        = -5,
    CELLULAR_RSP_MEMORY_ERR = -4,
    CELLULAR_RSP_FAIL       = -3,
    CELLULAR_RSP_UNKNOWN    = -2,
    CELLULAR_RSP_TIMEOUT    = -1,
    CELLULAR_RSP_SUCCESS    = 0
} Cellular_Cmd_RspDef;

typedef enum
{
    CELLULAR_CMD_QUERY,
    CELLULAR_CMD_SETUP,
    CELLULAR_CMD_EXECUTE
} Cellular_Cmd_TypeDef;


bool Cellular_SelfCheck(void);

#ifdef __cplusplus
}
#endif

#endif

