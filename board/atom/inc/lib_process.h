/**
 ******************************************************************************
 * @file       :  lib_process.h
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

#ifndef   LIB_PROCESS_H_
#define   LIB_PROCESS_H_

#include "lib_bridge.h"

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

