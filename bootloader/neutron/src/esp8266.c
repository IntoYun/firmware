#include "esp8266.h"
/* #include "string.h" */
#include <string.h>
#include <stdlib.h>
#include "hw_config.h"
#include "crc16.h"

extern UART_HandleTypeDef UartHandleA;
extern UART_HandleTypeDef UartHandle;
extern SDK_QUEUE USART_Rx_Queue;
extern uint8_t UsartRxByte; // Buffer Definitions //
//#define ESP8266_RX_BUFFER_LEN 4096 // Number of bytes in the serial receive buffer
#define ESP8266_RX_BUFFER_LEN 1024*20 // Number of bytes in the serial receive buffer
char esp8266RxBuffer[ESP8266_RX_BUFFER_LEN];
unsigned int bufferHead; // Holds position of latest byte placed in buffer.

/*
uint32_t crc_update(uint32_t crc, const uint8_t *data, size_t length)
{
    uint32_t i;
    bool bit;
    uint8_t c;

    while (length--) {
        c = *data++;
        for (i = 0x80; i > 0; i >>= 1) {
            bit = crc & 0x80000000;
            if (c & i) {
                bit = !bit;
            }
            crc <<= 1;
            if (bit) {
                crc ^= 0x04c11db7;
            }
        }
    }
    return crc;
}
*/
void print(char *p)
{
    while(*p)
    {
        HAL_UART_Transmit(&UartHandleA, (uint8_t *)p++, 1, 30);
    }
}

void printn(char *p, int n)
{
    while(n--)
    {
        HAL_UART_Transmit(&UartHandleA, (uint8_t *)p++, 1, 30);
    }
}

void ESP8266_Test(void)
{
    do
    {
        sendCommand(ESP8266_TEST, ESP8266_CMD_EXECUTE, NULL);
    }while(readForResponses(RESPONSE_OK, RESPONSE_ERROR, 1000)<0);
}

void ESP8266_Init(void)
{
    do
    {
        sendCommand(ESP8266_ECHO_DISABLE, ESP8266_CMD_EXECUTE, NULL);
    }while(readForResponses(RESPONSE_OK, RESPONSE_ERROR, 1000)<0);
}

int16_t ESP8266_Firmware_Size(Firmware_TypeDef FmType, uint32_t PacketSize)
{
    char params[16];

    memset(params, 0, sizeof(params));
    if(FmType == DEFAULT_FIRWARE)
    {
        sprintf(params, "%d,1",(int)PacketSize);
    }
    else
    {
        sprintf(params, "%d,0", (int)PacketSize);
    }
    sendCommand(ESP8266_FW_FWSIZE, ESP8266_CMD_SETUP, params);

    int16_t rsp = readForResponses(RESPONSE_OK, RESPONSE_ERROR, COMMAND_RESPONSE_TIMEOUT);
    if (rsp > 0)
    {
        char * p = strstr(esp8266RxBuffer, ESP8266_FW_FWSIZE);
        if (p != NULL)
        {
            p += strlen(ESP8266_FW_FWSIZE);
            p++; //跳过:
            char *pp = strstr(p, "\r\n");
            if (pp != NULL)
            {
                char tempRsp[10]={0};
                strncpy(tempRsp, p, pp - p);
                return atoi(tempRsp);
            }
        }
    }
    return rsp;
}

int16_t ESP8266_Firware_Packet(Firmware_TypeDef FmType, uint32_t PacketSize, uint32_t PacketIndex, uint32_t timeout)
{
    char params[64],temp[16];
    uint32_t DataStart=0;
    uint32_t timeIn = millis();	// Timestamp coming into function
    uint32_t datalen=0, count = 0, step = 0;

    memset(params, 0, sizeof(params));
    if(FmType == DEFAULT_FIRWARE)
    {
        sprintf(params, "%d,%d,1", (int)PacketSize, (int)PacketIndex);
    }
    else
    {
        sprintf(params, "%d,%d,0", (int)PacketSize, (int)PacketIndex);
    }
    sendCommand(ESP8266_FW_FWPACKET, ESP8266_CMD_SETUP, params);
    clearBuffer();	// Clear the class receive buffer (esp8266RxBuffer)
    while (timeIn + timeout > millis()) // While we haven't timed out
    {
        if(readByteToBuffer())
        {
            timeIn = millis();	// Timestamp coming into function
            switch(step)
            {
                case 0://get keyword
                    if (searchBuffer(ESP8266_FW_FWPACKET))	// Search the buffer for goodRsp
                    {
                        count=0;
                        step=1;
                    }
                    break;
                case 1://get data len position
                    if( esp8266RxBuffer[bufferHead - 1] == ',')
                    {
                        memset(temp,0,sizeof(temp));
                        count=0;
                        step=2;
                    }
                    break;
                case 2://get data len
                    if(esp8266RxBuffer[bufferHead - 1] != ':')
                    {
                        temp[count++]=esp8266RxBuffer[bufferHead - 1];
                        if(count > sizeof(temp))
                        {
                            step=5;
                        }
                    }
                    else
                    {
                        datalen=atoi(temp); // two bytes data for CRC checker
                        print(temp);
                        count=1;
                        DataStart=bufferHead;
                        step=3;
                    }
                    break;
                case 3://store the data length
                    if( ++count == datalen)
                    {
                        step=4;
                        count=0;
                    }
                    break;
                case 4:// check crc
                    crc_reset();
                    crc_update_n((uint8_t *)&esp8266RxBuffer[DataStart], datalen-2);
                    uint16_t crc16 = crc_get_reseult();
                    if( crc16 == ((esp8266RxBuffer[bufferHead - 1] << 8) | (esp8266RxBuffer[bufferHead - 2]) ))
                    {
                        datalen = datalen-2;//去掉crc校验值
                        print("crc32 success  flash begin\r\n");
                        uint32_t Internal_Flash_Address;
                        uint32_t Internal_Flash_Data;
                        HAL_StatusTypeDef status = HAL_OK;
                        for(count=0,Internal_Flash_Address=PacketIndex*PacketSize+CORE_FW_ADDRESS;(count < datalen) && (Internal_Flash_Address < INTERNAL_FLASH_END_ADDRESS) && (status == HAL_OK);)
                        {
                            Internal_Flash_Data = *((uint32_t *)&esp8266RxBuffer[DataStart+count]);
                            status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Internal_Flash_Address, Internal_Flash_Data);
                            Internal_Flash_Address += 4;
                            count += 4;
                        }
                        print("crc32 success flash end\r\n");
                        return ESP8266_RSP_SUCCESS;
                    }
                    else
                    {
                        step = 5;
                    }
                    break;
                case 5:
                    return ESP8266_RSP_UNKNOWN;
                    break;
                default:
                    break;
            }
        }
    }
    return ESP8266_RSP_TIMEOUT; // Return the timeout error code
}

void sendCommand(const char * cmd, Esp8266_Cmd_TypeDef CmdType, const char * params)
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
    printn(temp, strlen(temp));
    HAL_UART_Transmit(&UartHandle, (uint8_t *)temp, strlen(temp), 1000);
    //print("transmit\r\n");
}

int16_t readForResponses(const char * pass, const char * fail, unsigned int timeout)
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

// Buffer Stuff //
void clearBuffer()
{
    memset(esp8266RxBuffer, 0, ESP8266_RX_BUFFER_LEN);
    bufferHead = 0;
}


extern uint8_t testI;
unsigned int readByteToBuffer()
{
    uint8_t c;

    if(!sdkIsQueueEmpty(&USART_Rx_Queue))
    {
        if(bufferHead < (ESP8266_RX_BUFFER_LEN-1))
        {
            sdkGetQueueData(&USART_Rx_Queue, &c);
            esp8266RxBuffer[bufferHead++] = c;
            //printn(&c,1);
            return 1;
        }
    }
    return 0;
}

char *searchBuffer(const char * buffer)
{
    return strstr((const char *)esp8266RxBuffer, buffer);
}
