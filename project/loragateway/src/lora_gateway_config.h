/**
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

#ifndef LORA_GATEWAY_CONFIG_H_
#define LORA_GATEWAY_CONFIG_H_

#include "arduino.h"
// ----------------------------------------------------------------------------------------

// The spreading factor is the most important parameter to set for a single channel
// gateway. It specifies the speed/datarate in which the gateway and node communicate.
// As the name says, in principle the single channel gateway listens to one channel/frequency
// and to one spreading factor only.
// This parameters contains the default value of SF, the actual version can be set with
// the webserver and it will be stored in SPIFF
#define CONFIG_SPREADING_FACTOR             7         // SF7

//#define CONFIG_FREQUENCY                    434000000 // Hz
#define CONFIG_FREQUENCY                      433575000 // Hz
// Single channel gateways if they behave strict should only use one frequency
// channel and one spreading factor. However, the TTN backend replies on RX2
// timeslot for spreading factors SF9-SF12.
// Also, the server will respond with SF12 in the RX2 timeslot.
// If the 1ch gateway is working in and for nodes that ONLY transmit and receive on the set
// and agreed frequency and spreading factor. make sure to set STRICT to 1.
// In this case, the frequency and spreading factor for downlink messages is adapted by this
// gateway
// NOTE: If your node has only one frequency enabled and one SF, you must set this to 1
//      in order to receive downlink messages
#define CONFIG_STRICT_1CH                   0     // 1 is strict, 0 is as driven by backend

// Channel Activity Detection
// This function will scan for valid LoRa headers and determine the Spreading
// factor accordingly. If set to 1 we will use this function which means the
// 1-channel gateway will become even more versatile. If set to 0 we will use the
// continuous listen mode.
// Using this function means that we HAVE to use more dio pins on the RFM95/sx1276
// device and also connect enable dio1 to detect this state.
#define CONFIG_CAD_ON                       0

// Define the tx correct time
#define CONFIG_TX_CORRECT_TIME              0    //us

// Define the correct radio type that you are using
#define CONFIG_SX1278_RADIO


#define CONFIG_PULL_INTERVAL                30            // PULL_DATA messages to server to get downstream
#define CONFIG_STAT_INTERVAL                65            // Send a 'stat' message to server

//gateway与服务器的通讯方式 upd 和mqtt  如果不定义，UDP通讯
#define CONFIG_GATEWAY_COMMUNICATION_MQTT

#define CONFIG_LOCAL_UDP_PORT               1700

// loragateway udp definitions
#define CONFIG_LOCAL_UDP_PORT               1700

#define CONFIG_LORAWAN_UDP_SERVER           "115.29.177.202"
#define CONFIG_LORAWAN_UDP_PORT             1700

// Gateway Ident definitions
#define CONFIG_GATEWAY_DESCRIPTION          "IntoRobot GL1000"
#define CONFIG_GATEWAY_EMAIL                "robin@molmc.com"
#define CONFIG_GATEWAY_PLATFORM             "IntoRobot"
#define CONFIG_GATEWAY_LAT                  52.0000
#define CONFIG_GATEWAY_LON                  5.90000
#define CONFIG_GATEWAY_ALT                  1

#define CONFIG_NOASSERT

// Uncomment to allow the debut output
#define GATEWAY_DEBUG_STREAM                Serial
// #define GATEWAY_DEBUG_STREAM SerialUSB

#define LORAWAN_DEBUG_LEVEL                 5


#endif

