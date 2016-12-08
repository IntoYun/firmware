#ifndef   WIRING_EX_PROCESS_H_
#define   WIRING_EX_PROCESS_H_

#include "wiring_ex_bridge.h"

class Process : public Stream
{
    public:
        // Constructor with a user provided BridgeClass instance
        Process(BridgeClass &_b = Bridge) :
        bridge(_b), started(false), buffered(0), readPos(0) { }
        ~Process();

        void begin(const String &command);
        void addParameter(const String &param);
        unsigned int run(void);
        void runAsynchronously(void);
        boolean running(void);
        unsigned int exitValue(void);
        void close(void);

        unsigned int runShellCommand(const String &command);
        void runShellCommandAsynchronously(const String &command);
        bool cleanProcess(int id);
        operator bool (void)
        {
            return started;
        }

        // Stream methods
        int available(void);// (read from process stdout)
        int read(void);
        int peek(void);
        size_t write(uint8_t);// (write to process stdin)
        void flush(void);
        // TODO: add optimized function for block write

        //mdf lbz
        BridgeClass &bridge;

    private:
        unsigned char handle;
        String *cmdline;
        boolean started;

        private:
        void doBuffer(void);
        uint8_t buffered;
        uint8_t readPos;
        static const int BUFFER_SIZE = 64;
        uint8_t buffer[BUFFER_SIZE];
};

#endif /*LIB_PROCESS_H_*/

