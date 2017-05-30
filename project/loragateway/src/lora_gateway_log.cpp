
#include "lora_gateway_config.h"
#include "lora_gateway_log.h"


#define MAX_DEBUG_MESSAGE_LENGTH        512


const char * pathToFileName(const char * path)
{
    size_t i = 0;
    size_t pos = 0;
    char * p = (char *)path;
    while(*p){
        i++;
        if(*p == '/' || *p == '\\') {
            pos = i;
        }
        p++;
    }
    return path+pos;
}

void log_gateway_int(void)
{
    Serial.begin(115200);   // As fast as possible for bus
}

void log_gateway_print(const char *fmt, ...)
{
    char _buffer[MAX_DEBUG_MESSAGE_LENGTH];
    va_list args;

    va_start(args, fmt);
    int trunc = vsnprintf(_buffer, arraySize(_buffer), fmt, args);
    GATEWAY_DEBUG_STREAM.print(_buffer);
    if (trunc > arraySize(_buffer)) {
        GATEWAY_DEBUG_STREAM.print("...");
    }
    va_end(args);
}

void log_gateway_print_dump(uint8_t *buf, uint16_t len)
{
    if(len > 0) {
        for(int i = 0; i < len-1; i++)
        {
            GATEWAY_DEBUG_STREAM.printf("%02x", buf[i]);
            GATEWAY_DEBUG_STREAM.print(':');
        }
        GATEWAY_DEBUG_STREAM.printf("%02x", buf[len-1]);
        GATEWAY_DEBUG_STREAM.print("\r\n");
    }
}

void log_gateway_failed(const char *file, uint16_t line) {
    log_e("Program failed in file: %s, line: %d\r\n", file, line);
    while(1);
}

