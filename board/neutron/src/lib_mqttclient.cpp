/**
 ******************************************************************************
 * @file     : lib_pubsubclient.cpp
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
#include "lib_mqttclient.h"

#include "wiring.h"
#include "stdint.h"

#include "lib_system_all.h"


/*********************************************************************************
  *Function		:    MqttClientClass::MqttClientClass(void)
  *Description	:    constructor function
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
MqttClientClass::MqttClientClass(void)
{
    this->_client = NULL;
    this->stream = NULL;
}

/*********************************************************************************
  *Function		:   MqttClientClass
  *Description	:   constructor function
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
MqttClientClass::MqttClientClass(uint8_t *ip, uint16_t port, void (*callback)(char*, uint8_t*, uint32_t), TcpClient& client)
{
    this->_client = &client;
    this->callback = callback;
    this->ip = ip;
    this->port = port;
    this->domain = NULL;
    this->stream = NULL;
}

/*********************************************************************************
  *Function		:    MqttClientClass
  *Description	:    constructor function
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
MqttClientClass::MqttClientClass(char* domain, uint16_t port, void (*callback)(char*, uint8_t*, uint32_t), TcpClient& client)
{
    this->_client = &client;
    this->callback = callback;
    this->domain = domain;
    this->port = port;
    this->stream = NULL;
}

/*********************************************************************************
  *Function		:   MqttClientClass
  *Description	:   constructor function
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
MqttClientClass::MqttClientClass(uint8_t *ip, uint16_t port, void (*callback)(char*, uint8_t*, uint32_t), TcpClient& client, Stream& stream)
{
    this->_client = &client;
    this->callback = callback;
    this->ip = ip;
    this->port = port;
    this->domain = NULL;
    this->stream = &stream;
}

/*********************************************************************************
  *Function		:   MqttClientClass
  *Description	:   constructor function
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
MqttClientClass::MqttClientClass(char* domain, uint16_t port, void (*callback)(char*, uint8_t*, uint32_t), TcpClient& client, Stream& stream)
{
    this->_client = &client;
    this->callback = callback;
    this->domain = domain;
    this->port = port;
    this->stream = &stream;
}

/*********************************************************************************
  *Function		:   void MqttClientClass::setMqtt(uint8_t *ip, uint16_t port)
  *Description	:   constructor function
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
void MqttClientClass::setMqtt(uint8_t *ip, uint16_t port)
{
    this->ip = ip;
    this->port = port;
    this->domain = NULL;
}

/*********************************************************************************
  *Function		:   void MqttClientClass::setMqtt(char* domain, uint16_t port)
  *Description	:   constructor function
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
void MqttClientClass::setMqtt(char* domain, uint16_t port)
{
    this->domain = domain;
    this->port = port;
}

/*********************************************************************************
  *Function		:    uint8_t MqttClientClass::connect(const char *id)
  *Description	:    connect the mqtt server
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
uint8_t MqttClientClass::connect(const char *id)
{
    return connect(id,NULL,NULL,0,0,0,0);
}

/*********************************************************************************
  *Function		:   uint8_t MqttClientClass::connect()
  *Description	:    connect the mqtt server
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
uint8_t MqttClientClass::connect(const char *id, const char *user, const char *pass)
{
    return connect(id,user,pass,0,0,0,0);
}

/*********************************************************************************
  *Function		:  uint8_t MqttClientClass::connect()
  *Description	:   connect the mqtt server
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
uint8_t MqttClientClass::connect(const char *id, const char* willTopic, uint8_t willQos, uint8_t willRetain, const char* willMessage)
{
    return connect(id,NULL,NULL,willTopic,willQos,willRetain,willMessage);
}





/*********************************************************************************
  *Function		:    uint8_t MqttClientClass::connect()
  *Description	:    connect the mqtt server
  *Input		      :
  *Output		:
  *Return		:      succeed 1
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
uint8_t MqttClientClass::connect(const char *id, const char *user, const char *pass, const char* willTopic, uint8_t willQos, uint8_t willRetain, const char* willMessage)
{

    if (!connected())
    {
        int result = 0;

        if (domain != NULL)
        {
            result = _client->connect((const char *)(this->domain), this->port);
        }
        else
        {
            result = _client->connect((const char *)(this->ip), this->port);
        }

        if (result)
        {
            nextMsgId = 1;
            uint8_t d[9] = {0x00,0x06,'M','Q','I','s','d','p',MQTTPROTOCOLVERSION};
            // Leave room in the buffer for header and variable length field
            uint16_t length = 5;
            unsigned int j;
            for (j = 0;j<9;j++)
            {
                buffer[length++] = d[j];
            }

            uint8_t v;
            if (willTopic)
            {
                v = 0x06|(willQos<<3)|(willRetain<<5);
            }
            else
            {
                v = 0x02;
            }

            if(user != NULL)
            {
                v = v|0x80;

                if(pass != NULL)
                {
                    v = v|(0x80>>1);
                }
            }

            buffer[length++] = v;

            buffer[length++] = ((MQTT_KEEPALIVE) >> 8);
            buffer[length++] = ((MQTT_KEEPALIVE) & 0xFF);
            length = writeString(id,buffer,length);
            if (willTopic)
            {
                length = writeString(willTopic,buffer,length);
                length = writeString(willMessage,buffer,length);
            }

            if(user != NULL)
            {
                length = writeString(user,buffer,length);
                if(pass != NULL)
                {
                    length = writeString(pass,buffer,length);
                }
            }

            write(MQTTCONNECT,buffer,length-5);

            lastInActivity = lastOutActivity = millis();

            while (!_client->available())
            {
                unsigned long t = millis();
                if (t-lastInActivity > MQTT_KEEPALIVE*1000UL)
                {
                    _client->stop();
                    return false;
                }
            }
            uint8_t llen;
            uint16_t len = readPacket(&llen);


            if (len == 4 && buffer[3] == 0)
            {
                lastInActivity = millis();
                pingOutstanding = false;
                return true;
            }
        }
        _client->stop();
    }
    return false;
}

/*********************************************************************************
  *Function		:    void MqttClientClass::disconnect(void)
  *Description	:    disconnect the mqtt server
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
void MqttClientClass::disconnect(void)
{
    buffer[0] = MQTTDISCONNECT;
    buffer[1] = 0;
    _client->write(buffer,2);
    _client->stop();
    lastInActivity = lastOutActivity = millis();
}


void MqttClientClass::stop(void)
{
  //  buffer[0] = MQTTDISCONNECT;
  //  buffer[1] = 0;
  //  _client->write(buffer,2);
    _client->stop();
    lastInActivity = lastOutActivity = millis();
}


/*********************************************************************************
  *Function		:     uint8_t MqttClientClass::connected(void)
  *Description	:    check if connected the mqttserver
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
uint8_t MqttClientClass::connected(void)
{
    uint8_t rc;
    if (_client == NULL )
    {
        rc = false;
    }
    else
    {
        rc = (int)_client->connected();
        if (!rc) _client->stop();
    }
    return rc;
}

/*********************************************************************************
  *Function		:     uint8_t MqttClientClass::loop(void)
  *Description	:     the loop of mqttclient
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
uint8_t MqttClientClass::loop(void)
{
    if (connected())
    {
        unsigned long t = millis();
        if ((t - lastInActivity > MQTT_KEEPALIVE*1000UL) || (t - lastOutActivity > MQTT_KEEPALIVE*1000UL))
        {
            if (pingOutstanding)
            {
                _client->stop();
                return false;
            }
            else
            {
                buffer[0] = MQTTPINGREQ;
                buffer[1] = 0;
                _client->write(buffer,2);
                lastOutActivity = t;
                lastInActivity = t;
                pingOutstanding = true;
            }
        }

        if (_client->available())
        {
            uint8_t llen;
            uint16_t len = readPacket(&llen);
            uint16_t msgId = 0;
            uint8_t *payload;
            if (len > 0)
            {
                lastInActivity = t;
                uint8_t type = buffer[0]&0xF0;
                if (type == MQTTPUBLISH)
                {
                    if (callback)
                    {
                        uint16_t tl = (buffer[llen+1]<<8)+buffer[llen+2];
                        char topic[tl+1];
                        for (uint16_t i=0;i<tl;i++)
                        {
                            topic[i] = buffer[llen+3+i];
                        }
                        topic[tl] = 0;
                        // msgId only present for QOS>0
                        if ((buffer[0]&0x06) == MQTTQOS1)
                        {
                            msgId = (buffer[llen+3+tl]<<8)+buffer[llen+3+tl+1];
                            payload = buffer+llen+3+tl+2;
                            callback(topic, payload, len-llen-3-tl-2);
                            SendAckBag(msgId, MQTTPUBACK);
                            lastOutActivity = t;
                        }
                        else if ((buffer[0]&0x06) == MQTTQOS2)
                        {
                            msgId = (buffer[llen+3+tl]<<8)+buffer[llen+3+tl+1];
                            payload = buffer+llen+3+tl+2;
                            callback(topic, payload,len-llen-3-tl-2);
                            SendAckBag(msgId, MQTTPUBREC);
                            lastOutActivity = t;
                        }
                        else
                        {
                            payload = buffer+llen+3+tl;
                            callback(topic, payload,len-llen-3-tl);
                        }
                    }
                }
                else if (type == MQTTPUBREC)//send mqttpubrel bag
                {
                    uint16_t tl = (buffer[llen+1]<<8)+buffer[llen+2];
                    if ((buffer[0]&0x06) == MQTTQOS2)
                    {
                        msgId = (buffer[llen+3+tl]<<8)+buffer[llen+3+tl+1];
                        SendAckBag(msgId, MQTTPUBREL);
                    }
                }
                else if (type == MQTTPUBCOMP) //qos2
                {
                }
                else if (type == MQTTPINGREQ)
                {
                    buffer[0] = MQTTPINGRESP;
                    buffer[1] = 0;
                    _client->write(buffer,2);
                }
                else if (type == MQTTPINGRESP)
                {
                    pingOutstanding = false;
                }
            }
        }
        return true;
    }
    return false;
}

/*********************************************************************************
  *Function		:     uint8_t MqttClientClass::publish(const char* topic, char* payload)
  *Description	:     publish the topic
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
#include <string.h>


uint8_t MqttClientClass::publish(const char* topic, char* payload)
{
    return publish(topic, (uint8_t*)payload, strlen(payload), false);
}

/*********************************************************************************
  *Function		:    uint8_t MqttClientClass::publish()
  *Description	:    publish the topic
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
uint8_t MqttClientClass::publish(const char* topic, uint8_t* payload, unsigned int plength)
{
    return publish(topic, payload, plength, false);
}

/*********************************************************************************
  *Function		:    uint8_t MqttClientClass::publish()
  *Description	:     publish the topic
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
uint8_t MqttClientClass::publish(const char* topic, uint8_t* payload, unsigned int plength, uint8_t retained)
{
    if (connected())
    {
        // Leave room in the buffer for header and variable length field
        uint16_t length = 5;
        length = writeString(topic,buffer,length);
        uint16_t i;
        for (i=0;i<plength;i++)
        {
            buffer[length++] = payload[i];
        }
        uint8_t header = MQTTPUBLISH;

        if (retained)
        {
            header |= 1;
        }
        return write(header,buffer,length-5);
    }
    return false;
}

/*********************************************************************************
  *Function		:      uint8_t MqttClientClass::publish()
  *Description	:     publish the topic
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
uint8_t MqttClientClass::publish(const char* topic, uint8_t* payload, unsigned int plength, uint8_t qos, uint8_t retained)
{
    if(qos > 2)
    {return false;}

    if (connected())
    {
        // Leave room in the buffer for header and variable length field
        uint16_t length = 5;
        length = writeString(topic, buffer, length);
        uint16_t i;
        for (i=0;i<plength;i++)
        {
            buffer[length++] = payload[i];
        }
        uint8_t header = MQTTPUBLISH;

        if (retained)
        {
            header |= 1;
        }
        return write(header,buffer,length-5);
    }
    return false;
}

/*********************************************************************************
  *Function		:     uint8_t MqttClientClass::subscribe(const char* topic)
  *Description	:     subscribe the topic
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
uint8_t MqttClientClass::subscribe(const char* topic)
{
    return subscribe(topic,0);
}

/*********************************************************************************
  *Function		:    uint8_t MqttClientClass::subscribe(const char* topic, uint8_t qos)
  *Description	:    subscribe the topic
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
uint8_t MqttClientClass::subscribe(const char* topic, uint8_t qos)
{
    if(qos > 2)
    {return false;}

    if (connected())
    {
        // Leave room in the buffer for header and variable length field
        uint16_t length = 5;
        nextMsgId++;
        if (nextMsgId == 0)
        {
            nextMsgId = 1;
        }
        buffer[length++] = (nextMsgId >> 8);
        buffer[length++] = (nextMsgId & 0xFF);
        length = writeString(topic, buffer,length);
        buffer[length++] = qos;
        return write(MQTTSUBSCRIBE|MQTTQOS1,buffer,length-5);
    }
    return false;
}

/*********************************************************************************
  *Function		:      uint8_t MqttClientClass::unsubscribe(const char* topic)
  *Description	:      unsubscribe the topic
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
uint8_t MqttClientClass::unsubscribe(const char* topic)
{
    if (connected())
    {
        uint16_t length = 5;
        nextMsgId++;
        if (nextMsgId == 0)
        {
            nextMsgId = 1;
        }
        buffer[length++] = (nextMsgId >> 8);
        buffer[length++] = (nextMsgId & 0xFF);
        length = writeString(topic, buffer,length);
        return write(MQTTUNSUBSCRIBE|MQTTQOS1,buffer,length-5);
    }
    return false;
}

/*********************************************************************************
  *Function		:   uint8_t MqttClientClass::readByte(void)
  *Description	:   Read the next byte received from the mqtt server.
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
uint8_t MqttClientClass::readByte(void)
{
    while(!_client->available())
    {

    }
    int a=_client->read();
    return a;
    //return _client->read();
}

/*********************************************************************************
  *Function		:   uint16_t MqttClientClass::readPacket(uint8_t* lengthLength)
  *Description	:   Read the packet received from the mqtt server.
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
uint16_t MqttClientClass::readPacket(uint8_t* lengthLength)
{
    uint16_t len = 0;
    buffer[len++] = readByte();
    bool isPublish = (buffer[0]&0xF0) == MQTTPUBLISH;
    uint32_t multiplier = 1;
    uint16_t length = 0;
    uint8_t digit = 0;
    uint16_t skip = 0;
    uint8_t start = 0;

    do
    {
        digit = readByte();
        buffer[len++] = digit;
        length += (digit & 127) * multiplier;
        multiplier *= 128;
    } while ((digit & 128) != 0);
    *lengthLength = len-1;

    if (isPublish)
    {
        // Read in topic length to calculate bytes to skip over for Stream writing
        buffer[len++] = readByte();
        buffer[len++] = readByte();
        skip = (buffer[*lengthLength+1]<<8)+buffer[*lengthLength+2];
        start = 2;
        if (buffer[0]&MQTTQOS1)
        {
            // skip message id
            skip += 2;
        }
    }

    for (uint16_t i = start;i<length;i++)
    {
        digit = readByte();
        if (this->stream)
        {
            if (isPublish && len-*lengthLength-2>skip)
            {
            this->stream->write(digit);
            }
        }

        if (len < MQTT_MAX_PACKET_SIZE)
        {
            buffer[len] = digit;
        }
        len++;
    }

    if (!this->stream && len > MQTT_MAX_PACKET_SIZE)
    {
        len = 0; // This will cause the packet to be ignored.
    }

    return len;
}

/*********************************************************************************
  *Function		:    uint8_t MqttClientClass::write(uint8_t header, uint8_t* buf, uint16_t length)
  *Description	:
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
uint8_t MqttClientClass::write(uint8_t header, uint8_t* buf, uint16_t length)
{
    uint8_t lenBuf[4];
    uint8_t llen = 0;
    uint8_t digit;
    uint8_t pos = 0;
    uint8_t rc;
    uint8_t len = length;
    do
    {
        digit = len % 128;
        len = len / 128;
        if (len > 0)
        {
            digit |= 0x80;
        }
        lenBuf[pos++] = digit;
        llen++;
    } while(len>0);

    buf[4-llen] = header;
    for (int i=0;i<llen;i++)
    {
        buf[5-llen+i] = lenBuf[i];
    }
    rc = _client->write(buf+(4-llen),length+1+llen);

    lastOutActivity = millis();
    return (rc == 1+llen+length);
}

/*********************************************************************************
  *Function		:   uint16_t MqttClientClass::writeString(const char* string, uint8_t* buf, uint16_t pos)
  *Description	:
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
uint16_t MqttClientClass::writeString(const char* string, uint8_t* buf, uint16_t pos)
{
    const char* idp = string;
    uint16_t i = 0;
    pos += 2;
    while (*idp)
    {
        buf[pos++] = *idp++;
        i++;
    }
    buf[pos-i-2] = (i >> 8);
    buf[pos-i-1] = (i & 0xFF);
    return pos;
}

/*********************************************************************************
  *Function		:    uint8_t MqttClientClass::SendAckBag(uint16_t msgId, unsigned char bagtype)
  *Description	:    send the maqtt ack bag
  *Input		      :
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
uint8_t MqttClientClass::SendAckBag(uint16_t msgId, unsigned char bagtype)
{
    buffer[0] = MQTTPUBACK;
    buffer[1] = 2;
    buffer[2] = (msgId >> 8);
    buffer[3] = (msgId & 0xFF);
    return _client->write(buffer,4);
}

