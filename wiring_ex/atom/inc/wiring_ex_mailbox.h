#ifndef   WIRING_EX_MAILBOX_H_
#define   WIRING_EX_MAILBOX_H_

#include "wiring_ex_bridge.h"

class MailboxClass
{
    public:
        MailboxClass(BridgeClass &b = Bridge) : bridge(b) { }

        void begin(void) { }
        void end(void) { }

        unsigned int readMessage(uint8_t *buffer, unsigned int size);
        void readMessage(String &str, unsigned int maxLength = 128);

        void writeMessage(const uint8_t *buffer, unsigned int size);
        void writeMessage(const String &str);
        void writeJSON(const String &str);

        unsigned int messageAvailable(void);

    private:
        BridgeClass &bridge;
};

extern MailboxClass Mailbox;

#endif /*LIB_MAILBOX_H_*/
