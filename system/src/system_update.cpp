/**
 ******************************************************************************
 Copyright (c) 2013-2015 IntoRobot Industries, Inc.  All rights reserved.

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

#include "intorobot_config.h"
#include "system_update.h"
#include "core_hal.h"

/*debug switch*/
#define SYSTEM_UPDATE_DEBUG

#ifdef SYSTEM_UPDATE_DEBUG
#define SUPDATE_DEBUG(...)  do {DEBUG(__VA_ARGS__);}while(0)
#define SUPDATE_DEBUG_D(...)  do {DEBUG_D(__VA_ARGS__);}while(0)
#else
#define SUPDATE_DEBUG(...)
#define SUPDATE_DEBUG_D(...)
#endif

#ifdef START_DFU_FLASHER_SERIAL_SPEED
static uint32_t start_dfu_flasher_serial_speed = START_DFU_FLASHER_SERIAL_SPEED;
#endif
#ifdef START_YMODEM_FLASHER_SERIAL_SPEED
static uint32_t start_ymodem_flasher_serial_speed = START_YMODEM_FLASHER_SERIAL_SPEED;
#endif

void set_start_dfu_flasher_serial_speed(uint32_t speed)
{
#ifdef START_DFU_FLASHER_SERIAL_SPEED
    start_dfu_flasher_serial_speed = speed;
#endif
}

void set_start_ymodem_flasher_serial_speed(uint32_t speed)
{
#ifdef START_YMODEM_FLASHER_SERIAL_SPEED
    start_ymodem_flasher_serial_speed = speed;
#endif
}

void system_lineCodingBitRateHandler(uint32_t bitrate)
{
#ifdef START_DFU_FLASHER_SERIAL_SPEED
    if (bitrate == start_dfu_flasher_serial_speed)
    {
        HAL_Core_Enter_DFU_Mode(false);
    }
#endif
}

#ifndef configNO_NETWORK

UpdaterClass::UpdaterClass()
    : _error(0)
    , _buffer(0)
    , _bufferLen(0)
    , _size(0)
    , _startAddress(0)
    , _currentAddress(0)
    , _command(U_FLASH)
{
}

void UpdaterClass::_reset() {
    if (_buffer)
        delete[] _buffer;
    _buffer = 0;
    _bufferLen = 0;
    _startAddress = 0;
    _currentAddress = 0;
    _size = 0;
    _command = U_FLASH;
}

bool UpdaterClass::begin(size_t size, int command) {
    if(_size > 0){
        SUPDATE_DEBUG("[begin] already running");
        return false;
    }

    if(size == 0) {
        printError(UPDATE_ERROR_SIZE);
        return false;
    }

    _reset();

    uint32_t updateStartAddress = 0;
    if (command == U_FLASH) {
        //size of current sketch rounded to a sector
        uint32_t currentSketchSize = (ESP.getSketchSize() + FLASH_SECTOR_SIZE - 1) & (~(FLASH_SECTOR_SIZE - 1));
        //address of the end of the space available for sketch and update
        uint32_t updateEndAddress = (uint32_t)&_SPIFFS_start - 0x40200000;
        //size of the update rounded to a sector
        uint32_t roundedSize = (size + FLASH_SECTOR_SIZE - 1) & (~(FLASH_SECTOR_SIZE - 1));
        //address where we will start writing the update
        updateStartAddress = updateEndAddress - roundedSize;

        SUPDATE_DEBUG("[begin] roundedSize:       0x%08X (%d)\n", roundedSize, roundedSize);
        SUPDATE_DEBUG("[begin] updateEndAddress:  0x%08X (%d)\n", updateEndAddress, updateEndAddress);
        SUPDATE_DEBUG("[begin] currentSketchSize: 0x%08X (%d)\n", currentSketchSize, currentSketchSize);

        //make sure that the size of both sketches is less than the total space (updateEndAddress)
        if(updateStartAddress < currentSketchSize) {
            printError(UPDATE_ERROR_SPACE);
            return false;
        }
    }
    else {
        // unknown command
        SUPDATE_DEBUG("[begin] Unknown update command.");
        return false;
    }

    //initialize
    _startAddress = updateStartAddress;
    _currentAddress = _startAddress;
    _size = size;
    _buffer = new uint8_t[FLASH_SECTOR_SIZE];
    _command = command;

    SUPDATE_DEBUG("[begin] _startAddress:     0x%08X (%d)\n", _startAddress, _startAddress);
    SUPDATE_DEBUG("[begin] _currentAddress:   0x%08X (%d)\n", _currentAddress, _currentAddress);
    SUPDATE_DEBUG("[begin] _size:             0x%08X (%d)\n", _size, _size);

    _md5.begin();
    return true;
}

bool UpdaterClass::setMD5(const char * expected_md5){
    if(strlen(expected_md5) != 32)
    {
        return false;
    }
    _target_md5 = expected_md5;
    return true;
}

bool UpdaterClass::end(bool evenIfRemaining){
    if(_size == 0){
        SUPDATE_DEBUG("no update");
        return false;
    }

    if(hasError() || (!isFinished() && !evenIfRemaining)){
        SUPDATE_DEBUG("premature end: res:%u, pos:%u/%u\n", getError(), progress(), _size);
        _reset();
        return false;
    }

    if(evenIfRemaining) {
        if(_bufferLen > 0) {
            _writeBuffer();
        }
        _size = progress();
    }

    _md5.calculate();
    if(_target_md5.length()) {
        if(_target_md5 != _md5.toString()){
            _error = UPDATE_ERROR_MD5;
            SUPDATE_DEBUG("MD5 Failed: expected:%s, calculated:%s\n", _target_md5.c_str(), _md5.toString().c_str());
            _reset();
            return false;
        }
        else {
            SUPDATE_DEBUG("MD5 Success: %s\n", _target_md5.c_str());
        }
    }

    if(!_verifyEnd()) {
        printError(DEBUG_UPDATER);
        _reset();
        return false;
    }

    if (_command == U_FLASH) {
        eboot_command ebcmd;
        ebcmd.action = ACTION_COPY_RAW;
        ebcmd.args[0] = _startAddress;
        ebcmd.args[1] = 0x00000;
        ebcmd.args[2] = _size;
        eboot_command_write(&ebcmd);
        SUPDATE_DEBUG("Staged: address:0x%08X, size:0x%08X\n", _startAddress, _size);
    }

    _reset();
    return true;
}

bool UpdaterClass::_writeBuffer(){

    bool result = ESP.flashEraseSector(_currentAddress/FLASH_SECTOR_SIZE);
    if (result) {
        result = ESP.flashWrite(_currentAddress, (uint32_t*) _buffer, _bufferLen);
    }

    if (!result) {
        _currentAddress = (_startAddress + _size);
        printError(UPDATE_ERROR_WRITE);
        return false;
    }
    _md5.add(_buffer, _bufferLen);
    _currentAddress += _bufferLen;
    _bufferLen = 0;
    return true;
}

size_t UpdaterClass::write(uint8_t *data, size_t len) {
    if(hasError() || !isRunning())
        return 0;

    if(len > remaining()){
        //fail instead
        _error = UPDATE_ERROR_SPACE;
        return 0;
    }

    size_t left = len;

    while((_bufferLen + left) > FLASH_SECTOR_SIZE) {
        size_t toBuff = FLASH_SECTOR_SIZE - _bufferLen;
        memcpy(_buffer + _bufferLen, data + (len - left), toBuff);
        _bufferLen += toBuff;
        if(!_writeBuffer()){
            return len - left;
        }
        left -= toBuff;
    }
    //lets see whats left
    memcpy(_buffer + _bufferLen, data + (len - left), left);
    _bufferLen += left;
    if(_bufferLen == remaining()){
        //we are at the end of the update, so should write what's left to flash
        if(!_writeBuffer()){
            return len - left;
        }
    }
    return len;
}

bool UpdaterClass::_verifyHeader(uint8_t data) {
    if(_command == U_FLASH) {
        // check for valid first magic byte (is always 0xE9)
        if(data != 0xE9) {
            _error = UPDATE_ERROR_MAGIC_BYTE;
            _currentAddress = (_startAddress + _size);
            return false;
        }
        return true;
    }
    return false;
}

bool UpdaterClass::_verifyEnd() {
    if(_command == U_FLASH) {

        uint8_t buf[4];
        if(!ESP.flashRead(_startAddress, (uint32_t *) &buf[0], 4)) {
            _error = UPDATE_ERROR_READ;
            _currentAddress = (_startAddress);
            return false;
        }

        // check for valid first magic byte
        if(buf[0] != 0xE9) {
            _error = UPDATE_ERROR_MAGIC_BYTE;
            _currentAddress = (_startAddress);
            return false;
        }

        uint32_t bin_flash_size = ESP.magicFlashChipSize((buf[3] & 0xf0) >> 4);

        // check if new bin fits to SPI flash
        if(bin_flash_size > ESP.getFlashChipRealSize()) {
            _error = UPDATE_ERROR_NEW_FLASH_CONFIG;
            _currentAddress = (_startAddress);
            return false;
        }

        return true;
    }
    return false;
}

size_t UpdaterClass::writeStream(Stream &data) {
    size_t written = 0;
    size_t toRead = 0;
    if(hasError() || !isRunning())
        return 0;

    if(!_verifyHeader(data.peek())) {
        printError(DEBUG_UPDATER);
        _reset();
        return 0;
    }

    while(remaining()) {
        toRead = data.readBytes(_buffer + _bufferLen,  (FLASH_SECTOR_SIZE - _bufferLen));
        if(toRead == 0) { //Timeout
            delay(100);
            toRead = data.readBytes(_buffer + _bufferLen, (FLASH_SECTOR_SIZE - _bufferLen));
            if(toRead == 0) { //Timeout
                _currentAddress = (_startAddress + _size);
                printError(UPDATE_ERROR_STREAM);
                _reset();
                return written;
            }
        }
        _bufferLen += toRead;
        if((_bufferLen == remaining() || _bufferLen == FLASH_SECTOR_SIZE) && !_writeBuffer())
            return written;
        written += toRead;
    }
    return written;
}

void UpdaterClass::printError(Stream &out){
    SUPDATE_DEBUG("ERROR[%u]: ", _error);
    if(_error == UPDATE_ERROR_OK){
        SUPDATE_DEBUG("No Error");
    } else if(_error == UPDATE_ERROR_WRITE){
        SUPDATE_DEBUG("Flash Write Failed");
    } else if(_error == UPDATE_ERROR_ERASE){
        SUPDATE_DEBUG("Flash Erase Failed");
    } else if(_error == UPDATE_ERROR_READ){
        SUPDATE_DEBUG("Flash Read Failed");
    } else if(_error == UPDATE_ERROR_SPACE){
        SUPDATE_DEBUG("Not Enough Space");
    } else if(_error == UPDATE_ERROR_SIZE){
        SUPDATE_DEBUG("Bad Size Given");
    } else if(_error == UPDATE_ERROR_STREAM){
        SUPDATE_DEBUG("Stream Read Timeout");
    } else if(_error == UPDATE_ERROR_MD5){
        SUPDATE_DEBUG("MD5 Check Failed");
    } else if(_error == UPDATE_ERROR_FLASH_CONFIG){
        SUPDATE_DEBUG("Flash config wrong real: %d IDE: %d\n", ESP.getFlashChipRealSize(), ESP.getFlashChipSize());
    } else if(_error == UPDATE_ERROR_NEW_FLASH_CONFIG){
        SUPDATE_DEBUG("new Flash config wrong real: %d\n", ESP.getFlashChipRealSize());
    } else if(_error == UPDATE_ERROR_MAGIC_BYTE){
        SUPDATE_DEBUG("Magic byte is wrong, not 0xE9");
    } else {
        SUPDATE_DEBUG("UNKNOWN");
    }
}

UpdaterClass Update;


HTTPUpdate::HTTPUpdate(void)
{
}

HTTPUpdate::~HTTPUpdate(void)
{
}

HTTPUpdateResult HTTPUpdate::update(const String& url, const String& currentVersion)
{
    HTTPClient http;
    http.begin(url);
    return handleUpdate(http, currentVersion);
}

HTTPUpdateResult HTTPUpdate::update(const String& host, uint16_t port, const String& uri,
        const String& currentVersion)
{
    HTTPClient http;
    http.begin(host, port, uri);
    return handleUpdate(http, currentVersion);
}

/**
 * return error code as int
 * @return int error code
 */
int HTTPUpdate::getLastError(void)
{
    return _lastError;
}

/**
 * return error code as String
 * @return String error
 */
String HTTPUpdate::getLastErrorString(void)
{
    if(_lastError == 0) {
        return String(); // no error
    }

    // error from Update class
    if(_lastError > 0) {
        StreamString error;
        Update.printError(error);
        error.trim(); // remove line ending
        return String(F("Update error: ")) + error;
    }

    // error from http client
    if(_lastError > -100) {
        return String(F("HTTP error: ")) + HTTPClient::errorToString(_lastError);
    }

    switch(_lastError) {
        case HTTP_UE_TOO_LESS_SPACE:
            return F("To less space");
        case HTTP_UE_SERVER_NOT_REPORT_SIZE:
            return F("Server not Report Size");
        case HTTP_UE_SERVER_FILE_NOT_FOUND:
            return F("File not Found (404)");
        case HTTP_UE_SERVER_FORBIDDEN:
            return F("Forbidden (403)");
        case HTTP_UE_SERVER_WRONG_HTTP_CODE:
            return F("Wrong HTTP code");
        case HTTP_UE_SERVER_FAULTY_MD5:
            return F("Faulty MD5");
        case HTTP_UE_BIN_VERIFY_HEADER_FAILED:
            return F("Verify bin header failed");
        case HTTP_UE_BIN_FOR_WRONG_FLASH:
            return F("bin for wrong flash size");
    }

    return String();
}

/**
 *
 * @param http HTTPClient *
 * @param currentVersion const char *
 * @return HTTPUpdateResult
 */
HTTPUpdateResult HTTPUpdate::handleUpdate(HTTPClient& http, const String& currentVersion)
{
    HTTPUpdateResult ret = HTTP_UPDATE_FAILED;

    // use HTTP/1.0 for update since the update handler not support any transfer Encoding
    http.useHTTP10(true);
    http.setTimeout(8000);
    http.setUserAgent(F("ESP8266-http-Update"));
    http.addHeader(F("x-ESP8266-STA-MAC"), WiFi.macAddress());
    http.addHeader(F("x-ESP8266-AP-MAC"), WiFi.softAPmacAddress());
    http.addHeader(F("x-ESP8266-free-space"), String(ESP.getFreeSketchSpace()));
    http.addHeader(F("x-ESP8266-sketch-size"), String(ESP.getSketchSize()));
    http.addHeader(F("x-ESP8266-sketch-md5"), String(ESP.getSketchMD5()));
    http.addHeader(F("x-ESP8266-chip-size"), String(ESP.getFlashChipRealSize()));
    http.addHeader(F("x-ESP8266-sdk-version"), ESP.getSdkVersion());
    http.addHeader(F("x-ESP8266-mode"), F("sketch"));

    if(currentVersion && currentVersion[0] != 0x00) {
        http.addHeader(F("x-ESP8266-version"), currentVersion);
    }

    const char * headerkeys[] = { "x-MD5" };
    size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);

    // track these headers
    http.collectHeaders(headerkeys, headerkeyssize);


    int code = http.GET();
    int len = http.getSize();

    if(code <= 0) {
        DEBUG_HTTP_UPDATE("[httpUpdate] HTTP error: %s\n", http.errorToString(code).c_str());
        _lastError = code;
        http.end();
        return HTTP_UPDATE_FAILED;
    }

    DEBUG_HTTP_UPDATE("[httpUpdate] Header read fin.\n");
    DEBUG_HTTP_UPDATE("[httpUpdate] Server header:\n");
    DEBUG_HTTP_UPDATE("[httpUpdate]  - code: %d\n", code);
    DEBUG_HTTP_UPDATE("[httpUpdate]  - len: %d\n", len);

    if(http.hasHeader("x-MD5")) {
        DEBUG_HTTP_UPDATE("[httpUpdate]  - MD5: %s\n", http.header("x-MD5").c_str());
    }

    DEBUG_HTTP_UPDATE("[httpUpdate] ESP8266 info:\n");
    DEBUG_HTTP_UPDATE("[httpUpdate]  - free Space: %d\n", ESP.getFreeSketchSpace());
    DEBUG_HTTP_UPDATE("[httpUpdate]  - current Sketch Size: %d\n", ESP.getSketchSize());

    if(currentVersion && currentVersion[0] != 0x00) {
        DEBUG_HTTP_UPDATE("[httpUpdate]  - current version: %s\n", currentVersion.c_str() );
    }

    switch(code) {
        case HTTP_CODE_OK:  ///< OK (Start Update)
            if(len > 0) {
                bool startUpdate = true;
                if(len > (int) ESP.getFreeSketchSpace()) {
                    DEBUG_HTTP_UPDATE("[httpUpdate] FreeSketchSpace to low (%d) needed: %d\n", ESP.getFreeSketchSpace(), len);
                    startUpdate = false;
                }

                if(!startUpdate) {
                    _lastError = HTTP_UE_TOO_LESS_SPACE;
                    ret = HTTP_UPDATE_FAILED;
                } else {
                    WiFiClient * tcp = http.getStreamPtr();

                    WiFiUDP::stopAll();
                    WiFiClient::stopAllExcept(tcp);

                    delay(100);

                    int command;

                    command = U_FLASH;
                    DEBUG_HTTP_UPDATE("[httpUpdate] runUpdate flash...\n");

                    uint8_t buf[4];
                    if(tcp->peekBytes(&buf[0], 4) != 4) {
                        DEBUG_HTTP_UPDATE("[httpUpdate] peekBytes magic header failed\n");
                        _lastError = HTTP_UE_BIN_VERIFY_HEADER_FAILED;
                        http.end();
                        return HTTP_UPDATE_FAILED;
                    }

                    // check for valid first magic byte
                    if(buf[0] != 0xE9) {
                        DEBUG_HTTP_UPDATE("[httpUpdate] magic header not starts with 0xE9\n");
                        _lastError = HTTP_UE_BIN_VERIFY_HEADER_FAILED;
                        http.end();
                        return HTTP_UPDATE_FAILED;
                    }

                    uint32_t bin_flash_size = ESP.magicFlashChipSize((buf[3] & 0xf0) >> 4);

                    // check if new bin fits to SPI flash
                    if(bin_flash_size > ESP.getFlashChipRealSize()) {
                        DEBUG_HTTP_UPDATE("[httpUpdate] magic header, new bin not fits SPI Flash\n");
                        _lastError = HTTP_UE_BIN_FOR_WRONG_FLASH;
                        http.end();
                        return HTTP_UPDATE_FAILED;
                    }

                    if(runUpdate(*tcp, len, http.header("x-MD5"), command)) {
                        ret = HTTP_UPDATE_OK;
                        DEBUG_HTTP_UPDATE("[httpUpdate] Update ok\n");
                        http.end();

                        if(_rebootOnUpdate && !spiffs) {
                            ESP.restart();
                        }
                    } else {
                        ret = HTTP_UPDATE_FAILED;
                        DEBUG_HTTP_UPDATE("[httpUpdate] Update failed\n");
                    }
                }
            } else {
                _lastError = HTTP_UE_SERVER_NOT_REPORT_SIZE;
                ret = HTTP_UPDATE_FAILED;
                DEBUG_HTTP_UPDATE("[httpUpdate] Content-Length is 0 or not set by Server?!\n");
            }
            break;
        case HTTP_CODE_NOT_MODIFIED:
            ///< Not Modified (No updates)
            ret = HTTP_UPDATE_NO_UPDATES;
            break;
        case HTTP_CODE_NOT_FOUND:
            _lastError = HTTP_UE_SERVER_FILE_NOT_FOUND;
            ret = HTTP_UPDATE_FAILED;
            break;
        case HTTP_CODE_FORBIDDEN:
            _lastError = HTTP_UE_SERVER_FORBIDDEN;
            ret = HTTP_UPDATE_FAILED;
            break;
        default:
            _lastError = HTTP_UE_SERVER_WRONG_HTTP_CODE;
            ret = HTTP_UPDATE_FAILED;
            DEBUG_HTTP_UPDATE("[httpUpdate] HTTP Code is (%d)\n", code);
            //http.writeToStream(&Serial1);
            break;
    }

    http.end();
    return ret;
}

/**
 * write Update to flash
 * @param in Stream&
 * @param size uint32_t
 * @param md5 String
 * @return true if Update ok
 */
bool HTTPUpdate::runUpdate(Stream& in, uint32_t size, String md5, int command)
{
    StreamString error;

    if(!Update.begin(size, command)) {
        _lastError = Update.getError();
        Update.printError(error);
        error.trim(); // remove line ending
        DEBUG_HTTP_UPDATE("[httpUpdate] Update.begin failed! (%s)\n", error.c_str());
        return false;
    }

    if(md5.length()) {
        if(!Update.setMD5(md5.c_str())) {
            _lastError = HTTP_UE_SERVER_FAULTY_MD5;
            DEBUG_HTTP_UPDATE("[httpUpdate] Update.setMD5 failed! (%s)\n", md5.c_str());
            return false;
        }
    }

    if(Update.writeStream(in) != size) {
        _lastError = Update.getError();
        Update.printError(error);
        error.trim(); // remove line ending
        DEBUG_HTTP_UPDATE("[httpUpdate] Update.writeStream failed! (%s)\n", error.c_str());
        return false;
    }

    if(!Update.end()) {
        _lastError = Update.getError();
        Update.printError(error);
        error.trim(); // remove line ending
        DEBUG_HTTP_UPDATE("[httpUpdate] Update.end failed! (%s)\n", error.c_str());
        return false;
    }

    return true;
}

HTTPUpdate httpUpdate;
#endif
