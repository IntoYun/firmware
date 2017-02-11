/* 
 * Software License Agreement (BSD License)
 *
 * Copyright (c) 2011, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *  * Neither the name of Willow Garage, Inc. nor the names of its
 *    contributors may be used to endorse or promote prducts derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ROS_INTOROBOT_STM32_HARDWARE_H_
#define ROS_INTOROBOT_STM32_HARDWARE_H_

#include <application.h>      // IntoRobot STM32 lib

#define DEFAULT_SERVERIP      "192.168.8.216"
#define DEFAULT_PORTNUM       11411

class IntoRobotSTM32Hardware {
  public:
    IntoRobotSTM32Hardware(byte type = 0, long baud= 57600){
      serial_ = &Serial;
      baud_      = baud;
      strncpy(serverip_, DEFAULT_SERVERIP, 16);
      port_      = DEFAULT_PORTNUM;
      type_      = type;
    }

    void setType(byte type) {
      type_ = type;
      if(type_ == 1)
        tcpclient_.stop();
      //else if(type_ == 0)
      //serial_->stop();
      //no need to stop serial, cause others may use it
    }
    
    void setBaud(long baud){
      this->baud_= baud;
    }

    int getBaud() {return baud_;}

    bool connected() {
      if(type_ == 0)
        return tcpclient_.connected();
      else
        return false;
    }

    void init() {
      tcpclient_.stop(); //stop tcplink anyway
      
      if(type_ == 0){
        tcpclient_.connect(serverip_, port_);
      }
      else if(type_ == 1) {
        serial_->begin(baud_);
      }
      delay(100); //make sure the connection or serial initialization
    }
        
    void init(const char *pName) {
      tcpclient_.stop();
      
      if(*pName == 'S') {  //serial port names: Serial starts with 'S' (A2 A3)
        type_ = 1;
        serial_->begin(baud_);
      }
      else { //Split connection string into IP address and port.
        type_ = 0;
        const char* tcpPortNumString = strchr(pName, ':');
        if (!tcpPortNumString) {
          port_ = DEFAULT_PORTNUM;
          strncpy(serverip_, pName, 16);
        }
        else{
          port_ = strtol(tcpPortNumString + 1, NULL, 10);
          strncpy(serverip_, pName, tcpPortNumString - pName);
        }
        tcpclient_.connect(serverip_, port_);
      }
      delay(100); //make sure the connection or serial initialization
    }

    int read() {
      int c;
      if(type_ == 0)
        c = tcpclient_.read();
      else if(type_ == 1)
        c = serial_->read();
      return c;
    }
    
    void write(uint8_t* data, int length) {
      if(type_ == 0)
        tcpclient_.write(data, length);
      else if(type_ == 1) {
        for(int i=0; i<length; i++)
          serial_->write(data[i]);
      }
    }
    
    unsigned long time(){return millis();}   //about 49days overflow
    
 protected:
    int type_;               //type_:0 tcp, type_:1 serial
    USARTSerial* serial_;    //connect to IntoRobot-Atom A2(RX) A3(TX)
    TcpClient tcpclient_; 
    char serverip_[16];
    long port_;
    long baud_;
};

#endif
