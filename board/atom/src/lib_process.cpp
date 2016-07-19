/**
 ******************************************************************************
 * @file     : lib_process.cpp
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

#include "lib_process.h"

/*********************************************************************************
  *Function		:     ~Process(void)
  *Description	:     destructor function
  *Input		      :     none
  *Output		:     none
  *Return		:     none
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
Process::~Process(void)
{
    close();
}

/*********************************************************************************
  *Function		:    size_t write(uint8_t c)
  *Description	:    Writes data to a Linux process.
                              This data is sent as a byte or series of bytes. write() inherits from the Stream utility class.
  *Input		      :    c(uint8_t): a value to send as a single byte
  *Output		:    int
  *Return		:    byte : the number of bytes written. Reading the number is optional.
  *author		:    robot
  *date			:    2015-02-01
  *Others		:
**********************************************************************************/
size_t Process::write(uint8_t c)
{
    uint8_t cmd[] = {'I', handle, c};

    bridge.transfer(cmd, 3);
    return 1;
}

/*********************************************************************************
  *Function		:    void flush(void)
  *Description	:    Clears the Process buffer of any bytes. Waits for the transmission of outgoing data to complete.
  *Input		      :    none
  *Output		:    none
  *Return		:    none
  *author		:    robot
  *date			:    2015-02-01
  *Others		:
**********************************************************************************/
void Process::flush(void)
{
}

/*********************************************************************************
  *Function		:     int available(void)
  *Description	:     Get the number of bytes (characters) available for reading from the Linux connection.
                               This is data that's already arrived and stored in the receive buffer.
  *Input		      :     none
  *Output		:     none
  *Return		:     int: the number of bytes available to read
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
int Process::available(void)
{
    // Look if there is new data available
    doBuffer();
    return buffered;
}

/*********************************************************************************
  *Function		:     int read(void)
  *Description	:     Reads incoming data from a Linux process. read() inherits from the Stream utility class.
  *Input		      :     none
  *Output		:     none
  *Return		:     int : the first byte of incoming data available (or -1 if no data is available)
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
int Process::read(void)
{
    doBuffer();
    if (buffered == 0)
    {return -1;} // no chars available
    else
    {
        buffered--;
        return buffer[readPos++];
    }
}

/*********************************************************************************
  *Function		:    int peek(void)
  *Description	:    Returns the next byte (character) of incoming data from a Linux process without removing it from the internal buffer.
  *Input		      :    none
  *Output		:    none
  *Return		:    int : the first byte of incoming data available (or -1 if no data is available)
  *author		:    robot
  *date			:    2015-02-01
  *Others		:
**********************************************************************************/
int Process::peek(void)
{
    doBuffer();
    if (buffered == 0)
    {return -1;} // no chars available
    else
    {return buffer[readPos];}
}

/*********************************************************************************
  *Function		:    void doBuffer(void)
  *Description	:    read data from linux to buffer
  *Input		      :    none
  *Output		:    none
  *Return		:    none
  *author		:    robot
  *date			:    2015-02-01
  *Others		:
**********************************************************************************/
void Process::doBuffer(void)
{
    // If there are already char in buffer exit
    if (buffered > 0)
    {return;}

    // Try to buffer up to 32 characters
    readPos = 0;
    uint8_t cmd[] = {'O', handle, sizeof(buffer)};
    buffered = bridge.transfer(cmd, 3, buffer, sizeof(buffer));
}

/*********************************************************************************
  *Function		:    void begin(const String &command)
  *Description	:    start a linux named process. only the command should called hear.
                               followed by run() or runAsynchronously(), and optionally addParameter().
                               The named process does not start executing until run() is called
  *Input		      :    command(string): the linux command you wish to run
  *Output		:    none
  *Return		:    none
  *author		:    robot
  *date			:    2015-02-01
  *Others		:
**********************************************************************************/
void Process::begin(const String &command)
{
    close();
    cmdline = new String(command);
}

/*********************************************************************************
  *Function		:    void addParameter(const String &param)
  *Description	:    addParameter() adds a parameter to a Linux command initiated with begin().
                              It's not necessary to add spaces, addParameter() inserts them for you.
  *Input		      :    param (string) : The parameter you wish to add to a command
  *Output		:    none
  *Return		:    none
  *author		:    robot
  *date			:    2015-02-01
  *Others		:
**********************************************************************************/
void Process::addParameter(const String &param)
{
    *cmdline += "\xFE";
    *cmdline += param;
}

/*********************************************************************************
  *Function		:     void runAsynchronously(void)
  *Description	:     Starts a Linux process identified in Process.begin().
                                Unlike run(), runAsynchronously() is not-blocking.
                                Your sketch will continue to run while the Linux process runs in the background.
  *Input		      :      none
  *Output		:      none
  *Return		:      none
  *author		:      robot
  *date			:      2015-02-01
  *Others		:
**********************************************************************************/
void Process::runAsynchronously(void)
{
    uint8_t cmd[] = {'R'};
    uint8_t res[2];

    //DEBUG("cmdline:%s len:%d\r\n",cmdline->c_str(), cmdline->length());
    bridge.transfer(cmd, 1, (uint8_t*)cmdline->c_str(), cmdline->length(), res, 2);
    handle = res[1];

    delete cmdline;
    cmdline = NULL;

    if (res[0] == 0) // res[0] contains error code
    {started = true;}
}

/*********************************************************************************
  *Function		:    boolean running(void)
  *Description	:    Checks a process started by runAsynchronously() to see if it is still running.
  *Input		      :    none
  *Output		:    none
  *Return		:    true: running   false:runned
  *author		:    robot
  *date			:    2015-02-01
  *Others		:
**********************************************************************************/
boolean Process::running(void)
{
    uint8_t cmd[] = {'r', handle};
    uint8_t res[1];

    bridge.transfer(cmd, 2, res, 1);
    return (res[0] == 1);
}

/*********************************************************************************
  *Function		:    unsigned int exitValue(void)
  *Description	:    Returns the exit value (aka return code) of a process that was running.
                              Used by run() and runShellCommand() to return the status of the process that the user has launched.
  *Input		      :    none
  *Output		:    none
  *Return		:    the result of process. true: success  false: fail
  *author		:    robot
  *date			:    2015-02-01
  *Others		:
**********************************************************************************/
unsigned int Process::exitValue(void)
{
    uint8_t cmd[] = {'W', handle};
    uint8_t res[2];

    bridge.transfer(cmd, 2, res, 2);
    return (res[0] << 8) + res[1];
}

/*********************************************************************************
  *Function		:     unsigned int run(void)
  *Description	:     Starts a Linux process identified in Process.begin().run() is a blocking function.
                               That is, once you call Process.run(), nothing else will happen in your sketch until it has completed. The time depends on the nature of the command you are executing.
                               For a non-blocking alternative, see runAsynchronously().
  *Input		      :     none
  *Output		:     none
  *Return		:     the result of process. 0: success  1:fail
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
unsigned int Process::run(void)
{
    runAsynchronously();
    while (running())
    {delay(100);}
    return exitValue();
}

/*********************************************************************************
  *Function		:     void close(void)
  *Description	:     Closes a process started by runAsynchronously()
  *Input		      :     none
  *Output		:     none
  *Return		:     none
  *author		:     robot
  *date			:     2015-02-01
  *Others		:
**********************************************************************************/
void Process::close(void)
{
    if (started)
    {
        uint8_t cmd[] = {'w', handle};
        bridge.transfer(cmd, 2);
    }
    started = false;
}

/*********************************************************************************
  *Function		:    unsigned int runShellCommand(const String &command)
  *Description	:    Starts a shell command in Linux. runShellCommand() is a blocking function
  *Input		      :    command(string): the command
  *Output		:    none
  *Return		:    0: success  1:fail
  *author		:    robot
  *date			:    2015-02-01
  *Others		:
**********************************************************************************/
unsigned int Process::runShellCommand(const String &command)
{
    runShellCommandAsynchronously(command);
    while (running())
    {delay(100);}
    return exitValue();
}

/*********************************************************************************
  *Function		:    void runShellCommandAsynchronously(const String &command)
  *Description	:    Starts a Linux shell command.runShellCommandAsynchronously() is not-blocking
  *Input		      :    command(string): the command
  *Output		:    none
  *Return		:    none
  *author		:    robot
  *date			:    2015-02-01
  *Others		:
**********************************************************************************/
void Process::runShellCommandAsynchronously(const String &command)
{
    begin("/bin/ash");
    addParameter("-c");
    addParameter(command);
    runAsynchronously();
}

/*********************************************************************************
  *Function		:    void cleanProcess(int id)
  *Description	:
  *Input		      :
  *Output		:    none
  *Return		:    none
  *author		:    robot
  *date			:    2015-02-01
  *Others		:
**********************************************************************************/
bool Process::cleanProcess(int id)
{
    uint8_t cmd[] = {'w', (uint8_t)(id&0xFF)};
    uint8_t res[2];
    bridge.transfer(cmd, 2, res, 2);
}


