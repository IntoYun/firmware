/**
 ******************************************************************************
 * @file     : lib_bridge.cpp
 * @author   : robin
 * @version  : V1.0.0
 * @date     : 6-December-2014
 * @brief    :
 ******************************************************************************
  Copyright (c) 2013-2014 IntoRobot Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
  ******************************************************************************
 */

#include "lib_bridge.h"

/*********************************************************************************
  *Function          :       BridgeClass::BridgeClass(Stream &_stream)
  *Description      :       constructor function
                                  Bridge is the base class for all Bridge based calls. It is not called directly,
                                  but invoked whenever you use a function that relies on it.
  *Input              :
  *Output            :
  *Return            :
  *author            :        robot
  *date               :        2015-04-03
  *Others            :
**********************************************************************************/
BridgeClass::BridgeClass(Stream &_stream) :
    index(0), stream(_stream), started(false), max_retries(0)
{
    // Empty
}

/*********************************************************************************
  *Function          :       void BridgeClass::begin(void)
  *Description      :       Starts Bridge, facilitating communication between the stm32 and Linux processor.
                                  This should be called once in setup().begin() is a blocking function.
                                  Once you call Bridge.begin(), nothing else will happen in your sketch until it has completed.
                                  This process takes approximately three seconds.
  *Input              :        none
  *Output            :        none
  *Return            :        none
  *author            :        robot
  *date               :        2015-04-03
  *Others            :
**********************************************************************************/
void BridgeClass::begin(void)
{
    if (started)
    return;
    started = true;

    //Reset the brigde to check if it is running
    uint8_t cmd[] = {'X', 'X', '1', '0', '0'};
    uint8_t res[4];
    max_retries = 1;
    uint16_t l = transfer(cmd, 5, res, 4);
    if (l == TRANSFER_TIMEOUT)
    {
        // Wait for U-boot to finish startup
        do
        {
            dropAll();
            delay(1000);
        } while (stream.available() > 0);
    }
    while (true)
    {
        // Bridge interrupt:
        // - Ask the bridge to close itself
        uint8_t quit_cmd[] = {'X', 'X', 'X', 'X', 'X'};
        max_retries = 1;
        transfer(quit_cmd, 5);

        // Bridge startup:
        // - If the bridge is not running starts it safely
        stream.print(CTRL_C);
        delay(150);
        stream.print(F("\n"));
        delay(150);
        stream.print(F("\n"));
        delay(150);
        // Wait for OpenWRT message
        // "Press enter to activate console"
        stream.print(F("run-bridge\n"));
        delay(250);
        dropAll();

        // Reset the brigde to check if it is running
        uint8_t cmd[] = {'X', 'X', '1', '0', '0'};
        uint8_t res[4];
        max_retries = 50;
        uint16_t l = transfer(cmd, 5, res, 4);
        if (l == TRANSFER_TIMEOUT)
        {
            // Bridge didn't start...
            // Maybe the board is starting-up?

            // Wait and retry
            delay(1000);
            continue;
        }
        if (res[0] != 0)
        while (true);

        // Detect bridge version
        if (l == 4)
        {
            bridgeVersion = (res[1]-'0')*100 + (res[2]-'0')*10 + (res[3]-'0');
        }
        else
        {
            // Bridge v1.0.0 didn't send any version info
            bridgeVersion = 100;
        }

        max_retries = 50;
        DEBUG("Test bridge");
        return;
    }
}

/*********************************************************************************
  *Function          :   void BridgeClass::put(const char *key, const char *value)
  *Description      :   The put() function allows you to store data on the Linux processor using a Key/Value structure.
  *Input              :   key: char or string, the key name you want to assign to identify the value.
                             value:char or string, the value you want to store.
  *Output            :   none
  *Return            :    none
  *author            :    robot
  *date               :    2015-04-03
  *Others            :
**********************************************************************************/
void BridgeClass::put(const char *key, const char *value)
{
    //TODO: do it in a more efficient way
    String cmd = "D";
    cmd += key;
    cmd += "\xFE";
    cmd += value;
    transfer((uint8_t*)cmd.c_str(), cmd.length());
}

/*********************************************************************************
  *Function          :    unsigned int BridgeClass::get(const char *key, uint8_t *value, unsigned int maxlen)
  *Description      :    get() allows you to read a key/value item previously saved on the Linux processor.
  *Input              :    key:The name of the key associated with the value you are requesting.
                              value:The support buffer used to save the value returned from the searched Key.
                              maxlen:the length of the buffer passed to the function.
  *Output            :
  *Return            :
  *author            :
  *date               :
  *Others            :
**********************************************************************************/
unsigned int BridgeClass::get(const char *key, uint8_t *value, unsigned int maxlen)
{
    uint8_t cmd[] = {'d'};
    unsigned int l = transfer(cmd, 1, (uint8_t *)key, strlen(key), value, maxlen);
    if (l < maxlen)
        value[l] = 0; // Zero-terminate string
    return l;
}

#if defined(ARDUINO_ARCH_AVR)
// AVR use an optimized implementation of CRC
#include <util/crc16.h>
#else
// Generic implementation for non-AVR architectures
/*********************************************************************************
  *Function          :
  *Description      :
  *Input              :
  *Output            :
  *Return            :
  *author            :
  *date               :
  *Others            :
**********************************************************************************/
uint16_t _crc_ccitt_update(uint16_t crc, uint8_t data)
{
    data ^= crc & 0xff;
    data ^= data << 4;
    return ((((uint16_t)data << 8) | ((crc >> 8) & 0xff)) ^
    (uint8_t)(data >> 4) ^
    ((uint16_t)data << 3));
}
#endif

/*********************************************************************************
  *Function          :     void BridgeClass::crcUpdate(uint8_t c)
  *Description      :
  *Input              :
  *Output            :
  *Return            :
  *author            :
  *date               :
  *Others            :
**********************************************************************************/
void BridgeClass::crcUpdate(uint8_t c)
{
    CRCSUM = _crc_ccitt_update(CRCSUM, c);
}

/*********************************************************************************
  *Function          :       void BridgeClass::crcReset(void)
  *Description      :
  *Input              :
  *Output            :
  *Return            :
  *author            :
  *date               :
  *Others            :
**********************************************************************************/
void BridgeClass::crcReset(void)
{
    CRCSUM = 0xFFFF;
}

/*********************************************************************************
  *Function          :     void BridgeClass::crcWrite(void)
  *Description      :
  *Input              :
  *Output            :
  *Return            :
  *author            :
  *date               :
  *Others            :
**********************************************************************************/
void BridgeClass::crcWrite(void)
{
    stream.write((char)(CRCSUM >> 8));
    stream.write((char)(CRCSUM & 0xFF));
}

/*********************************************************************************
  *Function          :       bool BridgeClass::crcCheck(uint16_t _CRC)
  *Description      :
  *Input              :
  *Output            :
  *Return            :
  *author            :
  *date               :
  *Others            :
**********************************************************************************/
bool BridgeClass::crcCheck(uint16_t _CRC)
{
    return CRCSUM == _CRC;
}

/*********************************************************************************
  *Function          :      transfer()
  *Description      :      transfer() is used by other functions that communicate between the stm32 microcontroller and the Linux processor.
  *Input              :      buff_N: is the buffer N array with the content of the message you want to send. The transfer function support up to 3 buffers to be concatenated.
                                len_N: is the number of element contained in the buffer_N.
                                rxbuff: is the support buffer that you pass as a parameter where the answer from the linux side will be stored.
                                rxLen: is the length of the rxBuffer.
  *Output            :     none
  *Return            :      The length of the buffer that contains the answer from Linux.
                                In case the rxlen is shorter than the length of the answer,
                                the function will return rxlen to indicate that the rx buffer is full.
  *author            :
  *date               :
  *Others            :
**********************************************************************************/
uint16_t BridgeClass::transfer(const uint8_t *buff1, uint16_t len1,
                               const uint8_t *buff2, uint16_t len2,
                               const uint8_t *buff3, uint16_t len3,
                               uint8_t *rxbuff, uint16_t rxlen)
{
    uint16_t len = len1 + len2 + len3;
    uint8_t retries = 0;

    NVIC_DisableIRQ(TIM1_UP_IRQn);
    for ( ; retries < max_retries; retries++, delay(100), dropAll() /* Delay for retransmission */)
    {
        // Send packet
        crcReset();
        stream.write((char)0xFF);                // Start of packet (0xFF)
        crcUpdate(0xFF);
        stream.write((char)index);               // Message index
        crcUpdate(index);
        stream.write((char)((len >> 8) & 0xFF)); // Message length (hi)
        crcUpdate((len >> 8) & 0xFF);
        stream.write((char)(len & 0xFF));        // Message length (lo)
        crcUpdate(len & 0xFF);
        for (uint16_t i = 0; i < len1; i++)
        { // Payload
            stream.write((char)buff1[i]);
            crcUpdate(buff1[i]);
        }
        for (uint16_t i = 0; i < len2; i++)
        { // Payload
            stream.write((char)buff2[i]);
            crcUpdate(buff2[i]);
        }
        for (uint16_t i = 0; i < len3; i++)
        { // Payload
            stream.write((char)buff3[i]);
            crcUpdate(buff3[i]);
        }
        crcWrite();                     // CRC

        // Wait for ACK in 100ms
        if (timedRead(100) != 0xFF)
        continue;
        crcReset();
        crcUpdate(0xFF);

        // Check packet index
        if (timedRead(5) != index)
        continue;
        crcUpdate(index);

        // Recv len
        int lh = timedRead(10);
        if (lh < 0)
        continue;
        crcUpdate(lh);
        int ll = timedRead(10);
        if (ll < 0)
        continue;
        crcUpdate(ll);
        uint16_t l = lh;
        l <<= 8;
        l += ll;

        // Recv data
        for (uint16_t i = 0; i < l; i++)
        {
            int c = timedRead(5);
            if (c < 0)
            continue;
            // Cut received data if rxbuffer is too small
            if (i < rxlen)
            rxbuff[i] = c;
            crcUpdate(c);
        }

        // Check CRC
        int crc_hi = timedRead(5);
        if (crc_hi < 0)
        continue;
        int crc_lo = timedRead(5);
        if (crc_lo < 0)
        continue;
        if (!crcCheck((crc_hi << 8) + crc_lo))
        continue;

        // Increase index
        index++;

        NVIC_EnableIRQ(TIM1_UP_IRQn);
        // Return bytes received
        if (l > rxlen)
        {return rxlen;}
        else
        {return l;}
    }
    NVIC_EnableIRQ(TIM1_UP_IRQn);

    // Max retries exceeded
    return TRANSFER_TIMEOUT;
}

/*********************************************************************************
  *Function          :       int BridgeClass::timedRead(unsigned int timeout)
  *Description      :
  *Input              :
  *Output            :
  *Return            :
  *author            :
  *date               :
  *Others            :
**********************************************************************************/
int BridgeClass::timedRead(unsigned int timeout)
{
    int c;
    unsigned long _startMillis = millis();
    do
    {
        c = stream.read();
    if (c >= 0) return c;
    } while (millis() - _startMillis < timeout);
    return -1;     // -1 indicates timeout
}

/*********************************************************************************
  *Function          :     void BridgeClass::dropAll(void)
  *Description      :
  *Input              :
  *Output            :
  *Return            :
  *author            :
  *date               :
  *Others            :
**********************************************************************************/
void BridgeClass::dropAll(void)
{
    while (stream.available() > 0)
    {
        stream.read();
    }
}

SerialBridgeClass Bridge(SerialBridge);

