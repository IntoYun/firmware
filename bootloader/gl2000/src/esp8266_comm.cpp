#include "hw_config.h"
#include "esp8266_comm.h"
#include "system_config.h"
#include "boot_debug.h"

extern UART_HandleTypeDef UartHandleEsp8266;
extern SDK_QUEUE USART_Esp8266_Queue;

#define ESP8266_RX_BUFFER_LEN 1024*5  // Number of bytes in the serial receive buffer

char esp8266RxBuffer[ESP8266_RX_BUFFER_LEN];
unsigned int bufferHead; // Holds position of latest byte placed in buffer.

// Buffer Stuff //
static void clearBuffer()
{
    memset(esp8266RxBuffer, 0, ESP8266_RX_BUFFER_LEN);
    bufferHead = 0;
}

static unsigned int readByteToBuffer()
{
    uint8_t c;

    if(!sdkIsQueueEmpty(&USART_Esp8266_Queue))
    {
        if(bufferHead < (ESP8266_RX_BUFFER_LEN-1))
        {
            sdkGetQueueData(&USART_Esp8266_Queue, &c);
            esp8266RxBuffer[bufferHead++] = c;
            //BOOT_DEBUG("%c", c);
            return 1;
        }
    }
    return 0;
}

static char *searchBuffer(const char * buffer)
{
    return strstr((const char *)esp8266RxBuffer, buffer);
}

static void sendCommand(const char * cmd, Esp8266_Cmd_TypeDef CmdType, const char * params)
{
    char temp[64];

    memset(temp,0,sizeof(temp));
    if (CmdType == ESP8266_CMD_QUERY)
    {
        sprintf(temp, "AT%s?\r\n", cmd);
    }
    else if (CmdType == ESP8266_CMD_EXECUTE)
    {
        sprintf(temp, "AT%s\r\n", cmd);
    }
    else if (CmdType == ESP8266_CMD_SETUP)
    {
        sprintf(temp, "AT%s=%s\r\n", cmd, params);
    }
    //BOOT_DEBUG("%s", temp);
    HAL_UART_Transmit(&UartHandleEsp8266, (uint8_t *)temp, strlen(temp), 1000);
}

static int16_t readForResponses(const char * pass, const char * fail, unsigned int timeout)
{
    unsigned long timeIn = millis();	// Timestamp coming into function
    unsigned int received = 0; // received keeps track of number of chars read

    clearBuffer();	// Clear the class receive buffer (esp8266RxBuffer)
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
                return ESP8266_RSP_FAIL;
            }
        }
    }

    if (received > 0) // If we received any characters
        return ESP8266_RSP_UNKNOWN; // Return unkown response error code
    else // If we haven't received any characters
        return ESP8266_RSP_TIMEOUT; // Return the timeout error code
}

bool ESP8266_Init(void)
{
    int count = 10;
    do {
        if(count <= 0) {
            return false;
        }
        count--;
        sendCommand(ESP8266_ECHO_DISABLE, ESP8266_CMD_EXECUTE, NULL);
    } while(readForResponses(RESPONSE_OK, RESPONSE_ERROR, 500) < 0);
    return true;
}

bool ESP8266_Test(void)
{
    int count = 10;
    do {
        if(count <= 0) {
            return false;
        }
        count--;
        sendCommand(ESP8266_TEST, ESP8266_CMD_EXECUTE, NULL);
    } while(readForResponses(RESPONSE_OK, RESPONSE_ERROR, 500) < 0);
    return true;
}

bool ESP8266_SelfCheck(void)
{
    if(false == ESP8266_Init()) {
        return false;
    }
    if(false == ESP8266_Test()) {
        return false;
    }
    return true;
}

