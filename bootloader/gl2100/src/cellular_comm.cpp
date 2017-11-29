#include "hw_config.h"
#include "cellular_comm.h"
#include "system_config.h"
#include "boot_debug.h"

extern UART_HandleTypeDef UartHandleCellular;
extern SDK_QUEUE USART_Cellular_Queue;

#define CELLULAR_RX_BUFFER_LEN 1024*5  // Number of bytes in the serial receive buffer

char CellularRxBuffer[CELLULAR_RX_BUFFER_LEN];
unsigned int bufferHead; // Holds position of latest byte placed in buffer.

// Buffer Stuff //
static void clearBuffer()
{
    memset(CellularRxBuffer, 0, CELLULAR_RX_BUFFER_LEN);
    bufferHead = 0;
}

static unsigned int readByteToBuffer()
{
    uint8_t c;

    if(!sdkIsQueueEmpty(&USART_Cellular_Queue))
    {
        if(bufferHead < (CELLULAR_RX_BUFFER_LEN-1))
        {
            sdkGetQueueData(&USART_Cellular_Queue, &c);
            CellularRxBuffer[bufferHead++] = c;
            //BOOT_DEBUG("%c", c);
            return 1;
        }
    }
    return 0;
}

static char *searchBuffer(const char * buffer)
{
    return strstr((const char *)CellularRxBuffer, buffer);
}

static void sendCommand(const char * cmd, Cellular_Cmd_TypeDef CmdType, const char * params)
{
    char temp[64];

    memset(temp,0,sizeof(temp));
    if (CmdType == CELLULAR_CMD_QUERY)
    {
        sprintf(temp, "AT%s?\r\n", cmd);
    }
    else if (CmdType == CELLULAR_CMD_EXECUTE)
    {
        sprintf(temp, "AT%s\r\n", cmd);
    }
    else if (CmdType == CELLULAR_CMD_SETUP)
    {
        sprintf(temp, "AT%s=%s\r\n", cmd, params);
    }
    //BOOT_DEBUG("%s", temp);
    HAL_UART_Transmit(&UartHandleCellular, (uint8_t *)temp, strlen(temp), 1000);
}

static int16_t readForResponses(const char * pass, const char * fail, unsigned int timeout)
{
    unsigned long timeIn = millis();	// Timestamp coming into function
    unsigned int received = 0; // received keeps track of number of chars read

    clearBuffer();	// Clear the class receive buffer (CellularRxBuffer)
    while (timeIn + timeout > millis()) // While we haven't timed out
    {
        if (readByteToBuffer()) // If data is available on UART RX
        {
            received++;
            if (searchBuffer(pass))	// Search the buffer for goodRsp
            {
                return received;	// Return how number of chars read
            }
            if (searchBuffer(fail))
            {
                return CELLULAR_RSP_FAIL;
            }
        }
    }

    if (received > 0) // If we received any characters
        return CELLULAR_RSP_UNKNOWN; // Return unkown response error code
    else // If we haven't received any characters
        return CELLULAR_RSP_TIMEOUT; // Return the timeout error code
}

bool Cellular_Init(void)
{
    int count = 10;
    do {
        if(count <= 0) {
            return false;
        }
        count--;
        sendCommand(CELLULAR_ECHO_DISABLE, CELLULAR_CMD_EXECUTE, NULL);
    } while(readForResponses(RESPONSE_OK, RESPONSE_ERROR, 500) < 0);
    return true;
}

bool Cellular_Test(void)
{
    int count = 10;
    do {
        if(count <= 0) {
            return false;
        }
        count--;
        sendCommand(CELLULAR_TEST, CELLULAR_CMD_EXECUTE, NULL);
    } while(readForResponses(RESPONSE_OK, RESPONSE_ERROR, 500) < 0);
    return true;
}

bool Cellular_SelfCheck(void)
{
    if(false == Cellular_Init()) {
        return false;
    }
    if(false == Cellular_Test()) {
        return false;
    }
    return true;
}

