#ifndef   WIRING_EX_CONSOLE_H_
#define   WIRING_EX_CONSOLE_H_

#include "wiring_ex_bridge.h"

#define SP_CONSOLE      "CO"
#define CO_WRITE        "W"
#define CO_READ         "R"
#define CO_CONNECT      "C"

class ConsoleClass : public Stream
{
    public:
    // Default constructor uses global Bridge instance
    ConsoleClass();
    // Constructor with a user provided BridgeClass instance
    ConsoleClass(BridgeClass &_b);
    ~ConsoleClass();

    void begin(void);
    void end(void);

    void buffer(uint8_t size);
    void noBuffer(void);

    bool connected(void);

    // Stream methods
    // (read from console socket)
    int available(void);
    int read(void);
    int peek(void);
    // (write to console socket)
    size_t write(uint8_t);
    size_t write(const uint8_t *buffer, size_t size);
    void flush(void);

    operator bool (void)
    {
        return connected();
    }

    private:
    BridgeClass &bridge;

    void doBuffer();
    uint8_t inBuffered;
    uint8_t inReadPos;
    static const int BUFFER_SIZE = 32;
    uint8_t *inBuffer;

    bool autoFlush;
    uint8_t outBuffered;
    uint8_t outBufferSize;
    uint8_t *outBuffer;
};

extern ConsoleClass Console;

#endif /*LIB_CONSOLE_H_*/

