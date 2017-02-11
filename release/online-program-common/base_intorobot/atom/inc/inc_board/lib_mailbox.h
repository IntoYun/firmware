/**
 ******************************************************************************
 * @file       :  lib_mailbox.h
 * @author  :  robot
 * @version :  V1.0.0
 * @date     :  2015-02-01
 * @brief     :    
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

#ifndef   LIB_MAILBOX_H_
#define   LIB_MAILBOX__H_

#include "lib_bridge.h"

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
