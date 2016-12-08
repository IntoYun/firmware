#include "wiring_ex_mailbox.h"


/*********************************************************************************
  *Function		:    unsigned int readMessage(uint8_t *buff, unsigned int size)
  *Description	:    read mailbox message
  *Input		      :    size(unsigned int): max length 128       the read number of bytes in the message
  *Output		:    buff(uint8_t *): the named buffer to store the message in
  *Return		:    return bytes received
  *author		:    robot
  *date			:    2015-02-01
  *Others		:
**********************************************************************************/
unsigned int MailboxClass::readMessage(uint8_t *buff, unsigned int size)
{
    uint8_t tmp[] = { 'm' };

    return bridge.transfer(tmp, 1, buff, size);
}

/*********************************************************************************
  *Function		:     void readMessage(String &str, unsigned int maxLength)
  *Description	:     read mailbox message
  *Input		      :     maxLength(unsigned int): the max length of the read number of bytes in the message
  *Output		:     str(String):the named buffer to store the message in
  *Return		:     return bytes received
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
void MailboxClass::readMessage(String &str, unsigned int maxLength)
{
    uint8_t tmp[] = { 'm' };
    // XXX: Is there a better way to create the string?
    uint8_t buff[maxLength + 1];

    int l = bridge.transfer(tmp, 1, buff, maxLength);
    buff[l] = 0;
    str = (const char *)buff;
}

/*********************************************************************************
  *Function		:     void writeMessage(const uint8_t *buff, unsigned int size)
  *Description	:     send a message from the stm32 to the mt7620
  *Input		      :     buffer(int8_t *): the buffer to send the message to
                               size(unsigned int):max length 128: number of bytes in the message
  *Output		:     none
  *Return		:     none
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
void MailboxClass::writeMessage(const uint8_t *buff, unsigned int size)
{
    uint8_t cmd[] = {'M'};

    bridge.transfer(cmd, 1, buff, size, NULL, 0);
}

/*********************************************************************************
  *Function		:    void writeMessage(const String &str)
  *Description	:    send a message from the stm32 to the mt7620
  *Input		      :    str(String): the buffer to send the message to
  *Output		:    none
  *Return		:    none
  *author		:    robot
  *date			:    2015-02-01
  *Others		:
**********************************************************************************/
void MailboxClass::writeMessage(const String &str)
{
    writeMessage((uint8_t*) str.c_str(), str.length());
}

/*********************************************************************************
  *Function		:    void writeJSON(const String &str)
  *Description	:    send a json message from the stm32 to the mt7620
  *Input		      :    str(String): the buffer to send the message to
  *Output		:    none
  *Return		:    none
  *author		:    robot
  *date			:    2015-02-01
  *Others		:
**********************************************************************************/
void MailboxClass::writeJSON(const String &str)
{
    uint8_t cmd[] = {'J'};

    bridge.transfer(cmd, 1, (uint8_t*) str.c_str(), str.length(), NULL, 0);
}

/*********************************************************************************
  *Function		:    unsigned int messageAvailable(void)
  *Description	:    check and return the size of the next available message
  *Input		      :    none
  *Output		:    none
  *Return		:    the size of the next available message. return 0 indicate there is no message in the  queue.
  *author		:    robot
  *date			:    2015-02-01
  *Others		:
**********************************************************************************/
unsigned int MailboxClass::messageAvailable(void)
{
    uint8_t tmp[] = {'n'};
    uint8_t res[2];

    bridge.transfer(tmp, 1, res, 2);
    return (res[0] << 8) + res[1];
}

MailboxClass Mailbox(Bridge);

