/**
 ******************************************************************************
 * @file     : lib_pubsubclient.h
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

#ifndef  MQTT_CLIENT_H_
#define  MQTT_CLIENT_H_

#include "lib_tcpclient.h"


// MQTT_MAX_PACKET_SIZE : Maximum packet size
#define MQTT_MAX_PACKET_SIZE 256

// MQTT_KEEPALIVE : keepAlive interval in Seconds
#define MQTT_KEEPALIVE 60

#define MQTTPROTOCOLVERSION 3
#define MQTTCONNECT     1 << 4  // Client request to connect to Server
#define MQTTCONNACK     2 << 4  // Connect Acknowledgment
#define MQTTPUBLISH     3 << 4  // Publish message
#define MQTTPUBACK      4 << 4  // Publish Acknowledgment
#define MQTTPUBREC      5 << 4  // Publish Received (assured delivery part 1)
#define MQTTPUBREL      6 << 4  // Publish Release (assured delivery part 2)
#define MQTTPUBCOMP     7 << 4  // Publish Complete (assured delivery part 3)
#define MQTTSUBSCRIBE   8 << 4  // Client Subscribe request
#define MQTTSUBACK      9 << 4  // Subscribe Acknowledgment
#define MQTTUNSUBSCRIBE 10 << 4 // Client Unsubscribe request
#define MQTTUNSUBACK    11 << 4 // Unsubscribe Acknowledgment
#define MQTTPINGREQ     12 << 4 // PING Request
#define MQTTPINGRESP    13 << 4 // PING Response
#define MQTTDISCONNECT  14 << 4 // Client is Disconnecting
#define MQTTReserved    15 << 4 // Reserved

#define MQTTQOS0        (0 << 1)
#define MQTTQOS1        (1 << 1)
#define MQTTQOS2        (2 << 1)

/*
class Stream
{
    public:
        int write(unsigned char *a)
        {
           return 0;
        }

        int write(unsigned char a)
        {
           return 0;
        }
};

*/


class MqttClientClass
{
    private:
        uint8_t buffer[MQTT_MAX_PACKET_SIZE];
        uint16_t nextMsgId;
        unsigned long lastOutActivity;
        unsigned long lastInActivity;
        bool pingOutstanding;
        uint8_t *ip;
        char *domain;
        uint16_t port;
        Stream *stream;
        TcpClient *_client;
        void (*callback)(char*, uint8_t*, uint32_t);

        uint8_t readByte(void);
        uint16_t readPacket(uint8_t* lengthLength);
        uint8_t write(uint8_t header, uint8_t* buf, uint16_t length);
        uint16_t writeString(const char* string, uint8_t* buf, uint16_t pos);
        uint8_t SendAckBag(uint16_t msgId, unsigned char bagtype);

    public:
        MqttClientClass(void);
        MqttClientClass(uint8_t *ip, uint16_t port, void (*callback)(char*, uint8_t*, uint32_t), TcpClient& client);
        MqttClientClass(char* domain, uint16_t port, void (*callback)(char*, uint8_t*, uint32_t), TcpClient& client);
        MqttClientClass(uint8_t *ip, uint16_t port, void (*callback)(char*, uint8_t*, uint32_t), TcpClient& client, Stream& stream);
        MqttClientClass(char* domain, uint16_t port, void (*callback)(char*, uint8_t*, uint32_t), TcpClient& client, Stream& stream);
        void setMqtt(uint8_t *ip, uint16_t port);
        void setMqtt(char* domain, uint16_t port);
        uint8_t connect(const char *id);
        uint8_t connect(const char *id, const char *user, const char *pass);
        uint8_t connect(const char *id, const char* willTopic, uint8_t willQos, uint8_t willRetain, const char* willMessage);
        uint8_t connect(const char *id, const char *user, const char *pass, const char* willTopic, uint8_t willQos, uint8_t willRetain, const char* willMessage);
        void disconnect(void);
        void stop(void);
        uint8_t connected(void);
        uint8_t loop(void);
        uint8_t publish(const char* topic, char* payload);
        uint8_t publish(const char* topic, uint8_t* payload, unsigned int plength);
        uint8_t publish(const char* topic, uint8_t* payload, unsigned int plength, uint8_t retained);
        uint8_t publish(const char* topic, uint8_t* payload, unsigned int plength, uint8_t qos, uint8_t retained);
        uint8_t subscribe(const char* topic);
        uint8_t subscribe(const char* topic, uint8_t qos);
        uint8_t unsubscribe(const char* topic);
};


#endif /*PUB_SUB_CLIENT_H_*/


