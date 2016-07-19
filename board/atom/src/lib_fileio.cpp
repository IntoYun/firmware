/**
 ******************************************************************************
 * @file     : lib_fileio.cpp
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

#include "lib_fileio.h"


/*********************************************************************************
  *Function          :    File::File(BridgeClass &b)    
  *Description      :    constructor function
  *Input              :
  *Output            :         
  *Return            :        
  *author            :        
  *date               :           
  *Others            :       
**********************************************************************************/
File::File(BridgeClass &b) : mode(255), bridge(b) 
{
    // Empty
}

/*********************************************************************************
  *Function          :    File::File()         
  *Description      :    constructor function
  *Input              :
  *Output            :         
  *Return            :        
  *author            :        
  *date               :           
  *Others            :       
**********************************************************************************/
File::File(const char *_filename, uint8_t _mode, BridgeClass &b) : mode(_mode), bridge(b) 
{
    filename = _filename;
    char modes[] = {'r', 'w', 'a'};
    uint8_t cmd[] = {'F', modes[mode]};
    uint8_t res[2];
    dirPosition = 1;
    bridge.transfer(cmd, 2, (uint8_t*)filename.c_str(), filename.length(), res, 2);
    if (res[0] != 0) 
    { // res[0] contains error code
        mode = 255; // In case of error keep the file closed
        return;
    }
    handle = res[1];
    buffered = 0;
}

/*********************************************************************************
  *Function          :  File::operator bool(void)      
  *Description      :
  *Input              :
  *Output            :         
  *Return            :        
  *author            :        
  *date               :           
  *Others            :        
**********************************************************************************/
File::operator bool(void) 
{
    return (mode != 255);
}

File::~File() 
{
    close();
}

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
size_t File::write(uint8_t c) 
{
    return write(&c, 1);
}

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
size_t File::write(const uint8_t *buf, size_t size) 
{
    if (mode == 255)
    return -1;
    uint8_t cmd[] = {'g', handle};
    uint8_t res[1];
    bridge.transfer(cmd, 2, buf, size, res, 1);
    if (res[0] != 0) // res[0] contains error code
    return -res[0];
    return size;
}

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
int File::read(void) 
{
    doBuffer();
    if (buffered == 0)
        return -1; // no chars available
    else 
    {
        buffered--;
        return buffer[readPos++];
    }
}

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
int File::peek(void) 
{
    doBuffer();
    if (buffered == 0)
        return -1; // no chars available
    else
        return buffer[readPos];
}


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
boolean File::seek(uint32_t position) 
{
    uint8_t cmd[] = 
    {
        's',
        (uint8_t)handle,
        (uint8_t)((position >> 24) & 0xFF),
        (uint8_t)((position >> 16) & 0xFF),
        (uint8_t)((position >> 8) & 0xFF),
        (uint8_t)(position & 0xFF)
    };
    uint8_t res[1];
    bridge.transfer(cmd, 6, res, 1);
    if (res[0] == 0) 
    {
        // If seek succeed then flush buffers
        buffered = 0;
        return true;
    }
    return false;
}

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
uint32_t File::position(void) 
{
    uint8_t cmd[] = {'S', handle};
    uint8_t res[5];
    bridge.transfer(cmd, 2, res, 5);
    //err = res[0]; // res[0] contains error code
    uint32_t pos = res[1] << 24;
    pos += res[2] << 16;
    pos += res[3] << 8;
    pos += res[4];
    return pos - buffered;
}

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
void File::doBuffer(void) 
{
    // If there are already char in buffer exit
    if (buffered > 0)
    return;

    // Try to buffer up to BUFFER_SIZE characters
    readPos = 0;
    uint8_t cmd[] = {'G', handle, BUFFER_SIZE - 1};
    buffered = bridge.transfer(cmd, 3, buffer, BUFFER_SIZE);
    //err = buff[0]; // First byte is error code
    if (BridgeClass::TRANSFER_TIMEOUT == buffered || 0 == buffered) 
    {
        // transfer failed to retrieve any data
        buffered = 0;
    } 
    else 
    {
        // transfer retrieved at least one byte of data so skip the error code character
        readPos++;
        buffered--;
    }
}

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
int File::available(void)
{
    // Look if there is new data available
    doBuffer();
    return buffered;
}

void File::flush(void) 
{
}

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
int File::read(void *buff, uint16_t nbyte) 
{
    uint16_t n = 0;
    uint8_t *p = reinterpret_cast<uint8_t *>(buff);
    while (n < nbyte) 
    {
        if (buffered == 0) 
        {
            doBuffer();
            if (buffered == 0)
            break;
        }
        *p++ = buffer[readPos++];
        buffered--;
        n++;
    }
    return n;
}

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
uint32_t File::size(void) 
{
    if (bridge.getBridgeVersion() < 101)
    return 0;
    uint8_t cmd[] = {'t', handle};
    uint8_t buff[5];
    bridge.transfer(cmd, 2, buff, 5);
    //err = res[0]; // First byte is error code
    uint32_t res;
    res  = ((uint32_t)buff[1]) << 24;
    res |= ((uint32_t)buff[2]) << 16;
    res |= ((uint32_t)buff[3]) << 8;
    res |= ((uint32_t)buff[4]);
    return res;
}

/*********************************************************************************
  *Function          :   void File::close(void)     
  *Description      :   Close an open file, and ensure that any data written to it is physically saved to the SD card.
  *Input              :   none
  *Output            :   none     
  *Return            :   none  
  *author            :   robot    
  *date               :   2015-02-01        
  *Others            :       
**********************************************************************************/
void File::close(void) 
{
    if (mode == 255)
    return;
    uint8_t cmd[] = {'f', handle};
    bridge.transfer(cmd, 2);
    mode = 255;
}

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
const char *File::name(void) 
{
    return filename.c_str();
}

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
boolean File::isDirectory(void) 
{
    uint8_t res[1];

    uint8_t cmd[] = {'i'};
    if (mode != 255)
    return 0;

    bridge.transfer(cmd, 1, (uint8_t *)filename.c_str(), filename.length(), res, 1);
    return res[0];
}

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
File File::openNextFile(uint8_t mode) 
{
    Process awk;
    char tmp;
    String command;
    String filepath;
    if (dirPosition == 0xFFFF) return File();

    command = "ls ";
    command += filename;
    command += " | awk 'NR==";
    command += dirPosition;
    command += "'";

    awk.runShellCommand(command);

    while (awk.running());

    command = "";

    while (awk.available()) 
    {
        tmp = awk.read();
        if (tmp != '\n') command += tmp;
    }
    if (command.length() == 0)
    return File();
    dirPosition++;
    filepath = filename + "/" + command;
    return File(filepath.c_str(), mode);
}

void File::rewindDirectory(void) 
{
    dirPosition = 1;
}

/*********************************************************************************
  *Function          :   boolean FileSystemClass::begin(void)       
  *Description      :  Initializes the SD card and FileIO class. This communicates with the Linux distribution through Bridge. 
                             While functionally similar to the SD library, the underlying code and connections are different.
  *Input              :   none
  *Output            :   none
  *Return            :   boolean : true on success; false on failure      
  *author            :   robot   
  *date               :   2015-02-01        
  *Others            :         
**********************************************************************************/
boolean FileSystemClass::begin(void) 
{
    return true;
}

/*********************************************************************************
  *Function          :    File FileSystemClass::open(const char *filename, uint8_t mode)    
  *Description      :    Opens a file on the SD card. If the file is opened for writing, 
                               it will be created if it doesn't already exist (but the directory containing it must already exist).
  *Input              :     filename:  the name the file to open, which can:include directories (delimited by forward slashes, /) - char
                               mode (optional): the mode in which to open the file, defaults to FILE_READ. Can be either "
                                    FILE_READ: open the file for reading, starting at the beginning of the file.
                                    FILE_WRITE: open the file for reading and writing, starting at the end of the file.
  *Output            :     none
  *Return            :     a File object referring to the opened file; if the file couldn't be opened, 
                               this object will evaluate to false in a boolean context, i.e. you can test the return value with "if (f)".
  *author            :     robot  
  *date               :     2015-02-01      
  *Others            :          
**********************************************************************************/
File FileSystemClass::open(const char *filename, uint8_t mode) 
{
    return File(filename, mode);
}

/*********************************************************************************
  *Function          :     boolean FileSystemClass::exists(const char *filepath)   
  *Description      :     Tests whether a file or directory exists on the SD card.
  *Input              :     filename: the name of the file to test for existence, which can include directories (delimited by forward-slashes, /)
  *Output            :     none  
  *Return            :     boolean : true if the file or directory exists, false if not   
  *author            :     robot 
  *date               :     2015-02-01
  *Others            :          
**********************************************************************************/
boolean FileSystemClass::exists(const char *filepath) 
{
    Process ls;
    ls.begin("ls");
    ls.addParameter(filepath);
    int res = ls.run();
    return (res == 0);
}

/*********************************************************************************
  *Function          :      boolean FileSystemClass::mkdir(const char *filepath)  
  *Description      :      A wrapper for the mkdir command with the -p flag, FileSystem.mkdir() creates a named directory on an SD card.
  *Input              :      directory: the name of the name of the directory to create. Nested directories can be created with a /.
  *Output            :      none   
  *Return            :      boolean : true if the file or directory exists, false if not
  *author            :       robot 
  *date               :       2015-02-01    
  *Others            :         
**********************************************************************************/
boolean FileSystemClass::mkdir(const char *filepath) 
{
    Process mk;
    mk.begin("mkdir");
    mk.addParameter("-p");
    mk.addParameter(filepath);
    int res = mk.run();
    return (res == 0);
}

/*********************************************************************************
  *Function          :    boolean FileSystemClass::remove(const char *filepath)    
  *Description      :    A wrapper for the rm command, FileSystem.remove() removes a file or directory from an SD card.
  *Input              :    file: the name of the directory to remove.
  *Output            :    none   
  *Return            :    boolean : true if successful, false if not    
  *author            :    robot    
  *date               :    2015-02-01       
  *Others            :    
**********************************************************************************/
boolean FileSystemClass::remove(const char *filepath) 
{
    Process rm;
    rm.begin("rm");
    rm.addParameter(filepath);
    int res = rm.run();
    return (res == 0);
}

/*********************************************************************************
  *Function          :     boolean FileSystemClass::rmdir(const char *filepath)   
  *Description      :     A wrapper for the rmdir command, FileSystem.rmdir() removes an empty directory 
                                from an SD card. Before removing a directory with a file in it, you must call FileSystem.remove().
  *Input              :     directory: the name of the directory to remove.
  *Output            :     none  
  *Return            :     boolean : true if successful, false if not   
  *author            :     robot   
  *date               :     2015-02-01      
  *Others            :       
**********************************************************************************/
boolean FileSystemClass::rmdir(const char *filepath) 
{
    Process rm;
    rm.begin("rmdir");
    rm.addParameter(filepath);
    int res = rm.run();
    return (res == 0);
}

FileSystemClass FileSystem;

