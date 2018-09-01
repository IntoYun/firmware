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

/* Includes -----------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#include "hw_config.h"
#include "mdm_hal.h"
#include "tick_hal.h"
#include "delay_hal.h"
#include "gpio_hal.h"
#include "concurrent_hal.h"
#include "net_hal.h"
#include "intorobot_macros.h"

const static char *TAG = "hal-modem";

#ifdef putc
#undef putc
#undef getc
#endif

extern "C"
{
    static void __modem_lock(void);
    static void __modem_unlock(void);
}

/* Private typedef ----------------------------------------------------------*/

/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/

#define PROFILE         "1"   //!< this is the psd profile used
#define MAX_SIZE        2048  //!< max expected messages (used with RX)
#define USO_MAX_WRITE   1024  //!< maximum number of bytes to write to socket (used with TX)
// num sockets
#define NUMSOCKETS      ((int)(sizeof(_sockets)/sizeof(*_sockets)))
//! test if it is a socket is ok to use
#define ISSOCKET(s)     (((s) >= 0) && ((s) < NUMSOCKETS) && (_sockets[s].handle != MDM_SOCKET_ERROR))
//! check for timeout
#define TIMEOUT(t, ms)  ((ms != TIMEOUT_BLOCKING) && ((HAL_Tick_Get_Milli_Seconds() - t) > ms))
//! registration ok check helper
#define REG_OK(r)       ((r == REG_HOME) || (r == REG_ROAMING))
//! registration done check helper (no need to poll further)
#define REG_DONE(r)     ((r == REG_HOME) || (r == REG_ROAMING) || (r == REG_DENIED))
//! helper to make sure that lock unlock pair is always balanced
#define LOCK()      __modem_lock()
//! helper to make sure that lock unlock pair is always balanced
#define UNLOCK()    __modem_unlock()

static os_mutex_recursive_t modem_mutex = 0;

void init_modem_mutex(void)
{
    os_mutex_recursive_create(&modem_mutex);
}

static void __modem_lock(void)
{
    if (modem_mutex)
        os_mutex_recursive_lock(modem_mutex);
}

static void __modem_unlock(void)
{
    if (modem_mutex)
        os_mutex_recursive_unlock(modem_mutex);
}

enum eModemDuration
{
    SIM_QUERY_DURATION = S2M(5),
    ATTCH_QUERY_DURATION = S2M(3),
};

static volatile uint32_t cellular_timeout_start;
static volatile uint32_t cellular_timeout_duration;

inline void ARM_CELLULAR_TIMEOUT(uint32_t dur) {
    cellular_timeout_start = HAL_Tick_Get_Milli_Seconds();
    cellular_timeout_duration = dur;
    MOLMC_LOGD(TAG, "CELLULAR WD Set %d",(dur));
}
inline bool IS_CELLULAR_TIMEOUT(void) {
    return cellular_timeout_duration && ((HAL_Tick_Get_Milli_Seconds()-cellular_timeout_start)>cellular_timeout_duration);
}

inline void CLR_CELLULAR_TIMEOUT(void) {
    cellular_timeout_duration = 0;
    MOLMC_LOGD(TAG, "CELLULAR WD Cleared, was %d", cellular_timeout_duration);
}

#ifdef MDM_DEBUG
void dumpAtCmd(const char* buf, int len)
{
    MOLMC_LOGD_TEXT(TAG, "(%3d): \"", len);
    while (len --) {
        char ch = *buf++;
        if ((ch > 0x1F) && (ch < 0x7F)) { // is printable
            if      (ch == '%')  MOLMC_LOGD_TEXT(TAG, "%%");
            else if (ch == '"')  MOLMC_LOGD_TEXT(TAG, "\\\"");
            else if (ch == '\\') MOLMC_LOGD_TEXT(TAG, "\\\\");
            else MOLMC_LOGD_TEXT(TAG, "%c", ch);
        } else {
            if      (ch == '\a') MOLMC_LOGD_TEXT(TAG, "\\a"); // BEL (0x07)
            else if (ch == '\b') MOLMC_LOGD_TEXT(TAG, "\\b"); // Backspace (0x08)
            else if (ch == '\t') MOLMC_LOGD_TEXT(TAG, "\\t"); // Horizontal Tab (0x09)
            else if (ch == '\n') MOLMC_LOGD_TEXT(TAG, "\\n"); // Linefeed (0x0A)
            else if (ch == '\v') MOLMC_LOGD_TEXT(TAG, "\\v"); // Vertical Tab (0x0B)
            else if (ch == '\f') MOLMC_LOGD_TEXT(TAG, "\\f"); // Formfeed (0x0C)
            else if (ch == '\r') MOLMC_LOGD_TEXT(TAG, "\\r"); // Carriage Return (0x0D)
            else                 MOLMC_LOGD_TEXT(TAG, "\\x%02x", (unsigned char)ch);
        }
    }
    MOLMC_LOGD_TEXT(TAG, "\"\r\n");
}
#endif
/* Private variables --------------------------------------------------------*/

MDMParser* MDMParser::inst;

/* Extern variables ---------------------------------------------------------*/

/* Private function prototypes ----------------------------------------------*/

MDMParser::MDMParser(void)
{
    inst = this;
    memset(&_dev, 0, sizeof(_dev));
    memset(&_net, 0, sizeof(_net));
    _net.lac = 0xFFFF;
    _net.ci = 0xFFFFFFFF;
    _init = false;
    _state = CELLULAR_STATE_INVALID;
    _cancel_all_operations = false;
    sms_cb = NULL;
    memset(_sockets, 0, sizeof(_sockets));
    for (int socket = 0; socket < NUMSOCKETS; socket ++)
        _sockets[socket].handle = MDM_SOCKET_ERROR;
}

void MDMParser::cancel(void) {
    MOLMC_LOGI(TAG, "[ Modem::cancel ] = = = = = = = = = = = = = = =");
    _cancel_all_operations = true;
}

void MDMParser::resume(void) {
    MOLMC_LOGI(TAG, "[ Modem::resume ] = = = = = = = = = = = = = = =");
    _cancel_all_operations = false;
}

void MDMParser::setSMSreceivedHandler(_CELLULAR_SMS_CB cb, void* data) {
    sms_cb = cb;
    sms_data = data;
}

void MDMParser::SMSreceived(int index) {
    sms_cb(sms_data, index); // call the SMS callback with the index of the new SMS
}

int MDMParser::send(const char* buf, int len)
{
#ifdef MDM_DEBUG
    if(MOLMC_LOGE_SHOULD_OUTPUT(TAG)) {
        MOLMC_LOGD_HEADER(TAG);
        MOLMC_LOGD_TEXT(TAG, "AT Send ");
        dumpAtCmd(buf, len);
    }
#endif
    return _send(buf, len);
}

int MDMParser::sendFormated(const char* format, ...) {
    if (_cancel_all_operations) return 0;

    char buf[MAX_SIZE];
    va_list args;
    va_start(args, format);
    int len = vsnprintf(buf,sizeof(buf), format, args);
    va_end(args);
    return send(buf, len);
}

int MDMParser::waitFinalResp(_CALLBACKPTR cb /* = NULL*/,
        void* param /* = NULL*/,
        system_tick_t timeout_ms /*= 5000*/)
{
    if (_cancel_all_operations) return WAIT;

    char buf[MAX_SIZE + 64 /* add some more space for framing */];
    system_tick_t start = HAL_Tick_Get_Milli_Seconds();
    do {
        int ret = getLine(buf, sizeof(buf));

#ifdef MDM_DEBUG
        if(MOLMC_LOGE_SHOULD_OUTPUT(TAG)) {
            if ((ret != WAIT) && (ret != NOT_FOUND))
            {
                int len = LENGTH(ret);
                int type = TYPE(ret);
                const char* s = (type == TYPE_UNKNOWN)? "UNK" :
                    (type == TYPE_TEXT)   ? "TXT" :
                    (type == TYPE_OK   )  ? "OK " :
                    (type == TYPE_ERROR)  ? "ERR" :
                    (type == TYPE_ABORTED) ? "ABT" :
                    (type == TYPE_PLUS)   ? " + " :
                    (type == TYPE_PROMPT) ? " > " :
                    "..." ;
                MOLMC_LOGD_HEADER(TAG);
                MOLMC_LOGD_TEXT(TAG, "AT Read %s ", s);
                dumpAtCmd(buf, len);
                (void)s;
            }
        }
#endif

        if ((ret != WAIT) && (ret != NOT_FOUND))
        {
            int type = TYPE(ret);
            int sk, socket;
            // handle unsolicited commands here
            if (type == TYPE_PLUS) {
                const char* cmd = buf+3;
                int a, b, c, d, r;
                int n;
                char *p;
                char s[32];

                // SMS Command ---------------------------------
                // +CNMI: <mem>,<index>
                if (sscanf(cmd, "CMTI: \"%*[^\"]\",%d", &a) == 1) {
                    MOLMC_LOGD(TAG, "New SMS at index %d", a);
                    if (sms_cb) SMSreceived(a);
                }
                // Socket Specific Command ---------------------------------
                // +RECEIVE,<socket>,<length>:
                else if ((sscanf(cmd, "RECEIVE,%d,%d", &a, &b) == 2)) {
                    socket = _findSocket(a);
                    //MOLMC_LOGD(TAG, "Socket %d: handle %d has %d bytes pending!", socket, a, b);
                    if (socket != MDM_SOCKET_ERROR) {
                        p = strchr(buf, ':');
                        for(n=0; n < b; n++) {
                            if (_sockets[socket].pipe->writeable()) {
                                _sockets[socket].pipe->putc(p[n+3]);
                            }
                            else{
                                break;
                            }
                        }
                        _sockets[socket].pending += n;
                    }
                }
            } // end ==TYPE_PLUS
            else if (type == TYPE_CONNECTCLOSTED) {
                if ((sscanf(buf, "%d", &sk) == 1)) {
                    socket = _findSocket(sk);
                    if (socket != MDM_SOCKET_ERROR) {
                        _sockets[socket].connected = 0;
                    }
                }
            }
            else if (type == TYPE_DISCONNECT) {
                _state = CELLULAR_STATE_DISCONNECTED;
                HAL_NET_notify_disconnected();
            }

            if (cb) {
                int len = LENGTH(ret);
                int ret = cb(type, buf, len, param);
                if (WAIT != ret)
                    return ret;
            }
            if (type == TYPE_OK)
                return RESP_OK;
            if (type == TYPE_ERROR)
                return RESP_ERROR;
            if (type == TYPE_PROMPT)
                return RESP_PROMPT;
            if (type == TYPE_ABORTED)
                return RESP_ABORTED; // This means the current command was ABORTED, so retry your command if critical.
        }
        // relax a bit
        //HAL_Delay_Milliseconds(10);
    }
    while (!TIMEOUT(start, timeout_ms) && !_cancel_all_operations);

    return WAIT;
}

int MDMParser::_cbString(int type, const char* buf, int len, char* str)
{
    if (str && (type == TYPE_UNKNOWN)) {
        if (sscanf(buf, "\r\n%s\r\n", str) == 1)
            /*nothing*/;
    }
    return WAIT;
}

int MDMParser::_cbInt(int type, const char* buf, int len, int* val)
{
    if (val && (type == TYPE_UNKNOWN)) {
        if (sscanf(buf, "\r\n%d\r\n", val) == 1)
            /*nothing*/;
    }
    return WAIT;
}

void MDMParser::reset(void)
{
    MOLMC_LOGI(TAG, "[ Modem reset ]");

    GPIO_InitTypeDef   GPIO_InitStruct;

    //sim800c PWK_KEY  开关机控制管脚
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET); //PWK_KEY 高电平

    //sim800c VDD_EXT
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    //sim800c VBAT 电源开关控制
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    //sim800c 为了加快上电速度 PWK_KEY 上电默认低电平 此时VDD_EXT不能使用
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);   //PWK_KEY 低电平

    //sim800c VBAT Power On
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
    HAL_Delay(200);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
}

bool MDMParser::_powerOn(void)
{
    LOCK();

    //sim800c VBAT Power On
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
    HAL_Delay(200);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);

    if (!_init) {
        MOLMC_LOGI(TAG, "[ CellularSerialPipe::begin ] = = = = = = = =");

        /* Instantiate the USART3 hardware */
        CellularMDM.begin(115200);

        /* Initialize only once */
        _init = true;
    }

    MOLMC_LOGI(TAG, "[ Modem::powerOn ] = = = = = = = = = = = = = =");
    bool continue_cancel = false;
    bool retried_after_reset = false;

    int i = 10;
    while (i--) {
        // purge any messages
        purge();

        // Save desire to cancel, but since we are already here
        // trying to power up the modem when we received a cancel
        // resume AT parser to ensure it's ready to receive
        // power down commands.
        if (_cancel_all_operations) {
            continue_cancel = true;
            resume(); // make sure we can talk to the modem
        }

        // check interface
        sendFormated("AT\r\n");
        int r = waitFinalResp(NULL,NULL,1000);
        if(RESP_OK == r) {
            _pwr = true;
            break;
        }
        else if (i==0 && !retried_after_reset) {
            retried_after_reset = true; // only perform reset & retry sequence once
            i = 10;
            reset();
        }
    }

    if (i < 0) {
        MOLMC_LOGE(TAG, "[ No Reply from Modem ]");
    }

    if (continue_cancel) {
        cancel();
        goto failure;
    }

    // echo off
    sendFormated("ATE0\r\n");
    if(RESP_OK != waitFinalResp())
        goto failure;
    // enable verbose error messages
    sendFormated("AT+CMEE=2\r\n");
    if(RESP_OK != waitFinalResp())
        goto failure;
    // set baud rate
    sendFormated("AT+IPR=115200\r\n");
    if (RESP_OK != waitFinalResp())
        goto failure;
    // wait some time until baudrate is applied
    HAL_Delay_Milliseconds(100); // SARA-G > 40ms

    UNLOCK();
    return true;
failure:
    UNLOCK();
    return false;
}

bool MDMParser::powerOn(const char* simpin)
{
    LOCK();
    memset(&_dev, 0, sizeof(_dev));

    /* Power on the modem and perform basic initialization */
    if (!_powerOn())
        goto failure;

    for (int i = 0; (i < 5) && (_dev.sim != SIM_READY); i++) {
        sendFormated("AT+CPIN?\r\n");
        int ret = waitFinalResp(_cbCPIN, &_dev.sim);
        // having an error here is ok (sim may still be initializing)
        if ((RESP_OK != ret) && (RESP_ERROR != ret)) {
            goto failure;
        }

        if (_dev.sim != SIM_READY) {
            // wait for up to one second while looking for slow "+CPIN: READY" URCs
            waitFinalResp(_cbCPIN, &_dev.sim, 1000);
        }
    }

    if (_dev.sim != SIM_READY) {
        if (_dev.sim == SIM_MISSING) {
            MOLMC_LOGD(TAG, "SIM not inserted\r\n");
        }
        goto failure;
    }

    init();
    _state = CELLULAR_STATE_READY;
    UNLOCK();
    return true;
failure:
    _state = CELLULAR_STATE_INVALID;
    UNLOCK();
    return false;
}

bool MDMParser::powerOff(void)
{
    LOCK();
    bool ok = false;
    bool continue_cancel = false;
    if (_init && _pwr) {
        MOLMC_LOGI(TAG, "[ Modem::powerOff ] = = = = = = = = = = = = = =");
        if (_cancel_all_operations) {
            continue_cancel = true;
            resume(); // make sure we can use the AT parser
        }

        //sim800c VBAT Power off
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);

        _pwr = false;
        // todo - add if these are automatically done on power down
        _state = CELLULAR_STATE_INVALID;
        ok = true;
    }

    if (continue_cancel) cancel();
    UNLOCK();
    return ok;
}

bool MDMParser::init(void)
{
    LOCK();
    MOLMC_LOGI(TAG, "[ Modem::init ] = = = = = = = = = = = = = = =");

    // 获取模组参数 ---------------------------------
    sendFormated("ATI\r\n");
    if (RESP_OK != waitFinalResp(_cbATI, &_dev.dev))
        goto failure;
    if (_dev.dev == DEV_UNKNOWN)
        goto failure;
    // Returns the product serial number, IMEI (International Mobile Equipment Identity)
    sendFormated("AT+CGSN\r\n");
    if (RESP_OK != waitFinalResp(_cbString, _dev.imei))
        goto failure;
    // get the manufacturer
    sendFormated("AT+CGMI\r\n");
    if (RESP_OK != waitFinalResp(_cbString, _dev.manu))
        goto failure;
    // get the model identification
    sendFormated("AT+CGMM\r\n");
    if (RESP_OK != waitFinalResp(_cbString, _dev.model))
        goto failure;
    // get the version
    sendFormated("AT+CGMR\r\n");
    if (RESP_OK != waitFinalResp(_cbString, _dev.ver))
        goto failure;
    // Returns the ICCID (Integrated Circuit Card ID) of the SIM-card.
    // ICCID is a serial number identifying the SIM.
    sendFormated("AT+CCID\r\n");
    if (RESP_OK != waitFinalResp(_cbCCID, _dev.ccid))
        goto failure;
    // Setup SMS in text mode
    sendFormated("AT+CMGF=1\r\n");
    if (RESP_OK != waitFinalResp())
        goto failure;
    // setup new message indication
    sendFormated("AT+CNMI=2,1\r\n");
    if (RESP_OK != waitFinalResp())
        goto failure;
    // Request IMSI (International Mobile Subscriber Identification)
    sendFormated("AT+CIMI\r\n");
    if (RESP_OK != waitFinalResp(_cbString, _dev.imsi))
        goto failure;

#ifdef MDM_DEBUG
    if(MOLMC_LOGE_SHOULD_OUTPUT(TAG)) {
        dumpDevStatus(&_dev);
    }
#endif

    // 设置网络参数 ---------------------------------
    // start up multi-ip connection
    sendFormated("AT+CIPMUX=1\r\n");
    if (RESP_OK != waitFinalResp())
        goto failure;

    // Get Data from Network derect
    sendFormated("AT+CIPRXGET=0\r\n");
    if (RESP_OK != waitFinalResp())
        goto failure;

    // Add an IP Head at the Beginning of a Package Received
    sendFormated("AT+CIPHEAD=1\r\n");
    if (RESP_OK != waitFinalResp())
        goto failure;

    // select the quick send mode
    sendFormated("AT+CIPQSEND=1\r\n");
    if (RESP_OK != waitFinalResp())
        goto failure;

    UNLOCK();
    return true;
failure:
    UNLOCK();

    return false;
}

int MDMParser::_cbATI(int type, const char* buf, int len, Dev* dev)
{
    if ((type == TYPE_UNKNOWN) && dev) {
        if (strstr(buf, "SIM800")) {
            *dev = DEV_SIM800;
        }
    }
    return WAIT;
}

int MDMParser::_cbCPIN(int type, const char* buf, int len, Sim* sim)
{
    if (sim) {
        if (type == TYPE_PLUS){
            char s[16];
            if (sscanf(buf, "\r\n+CPIN: %[^\r]\r\n", s) >= 1)
                *sim = (0 == strcmp("READY", s)) ? SIM_READY : SIM_PIN;
        } else if (type == TYPE_ERROR) {
            if (strstr(buf, "+CME ERROR: SIM not inserted"))
                *sim = SIM_MISSING;
        }
    }
    return WAIT;
}

int MDMParser::_cbCCID(int type, const char* buf, int len, char* ccid)
{
    if ((type == TYPE_PLUS) && ccid) {
        if (sscanf(buf, "\r\n+CCID: %[^\r]\r\n", ccid) == 1) {
            //MOLMC_LOGD(TAG, "Got CCID: %s", ccid);
        }
    }
    return WAIT;
}

bool MDMParser::getSignalStrength(NetStatus &status)
{
    bool ok = false;
    LOCK();
    if (_init && _pwr) {
        MOLMC_LOGI(TAG, "[ Modem::getSignalStrength ] = = = = = = = = = =");
        sendFormated("AT+CSQ\r\n");
        if (RESP_OK == waitFinalResp(_cbCSQ, &_net)) {
            ok = true;
            status.rssi = _net.rssi;
            status.qual = _net.qual;
        }
    }
    UNLOCK();
    return ok;
}

int MDMParser::_cbCOPS(int type, const char* buf, int len, NetStatus* status)
{
    if ((type == TYPE_PLUS) && status){
        int act = 99;
        // +COPS: <mode>[,<format>,<oper>[,<AcT>]]
        if (sscanf(buf, "\r\n+COPS: %*d,%*d,\"%[^\"]\",%d",status->opr,&act) >= 1) {
            if      (act == 0) status->act = ACT_GSM;      // 0: GSM,
            else if (act == 2) status->act = ACT_UTRAN;    // 2: UTRAN
        }
    }
    return WAIT;
}

int MDMParser::_cbCNUM(int type, const char* buf, int len, char* num)
{
    if ((type == TYPE_PLUS) && num){
        int a;
        if ((sscanf(buf, "\r\n+CNUM: \"My Number\",\"%31[^\"]\",%d", num, &a) == 2) &&
            ((a == 129) || (a == 145))) {
        }
    }
    return WAIT;
}

int MDMParser::_cbCSQ(int type, const char* buf, int len, NetStatus* status)
{
    if ((type == TYPE_PLUS) && status){
        int a,b;
        char _qual[] = { 49, 43, 37, 25, 19, 13, 7, 0 }; // see 3GPP TS 45.008 [20] subclause 8.2.4
        // +CSQ: <rssi>,<qual>
        if (sscanf(buf, "\r\n+CSQ: %d,%d",&a,&b) == 2) {
            if (a != 99) { // 0: -115 1: -111 2: -110 ... 31: -52 dBm with 2 dBm steps
                if(a == 0) {
                    status->rssi = -115;
                } if(a == 1) {
                    status->rssi = -111;
                } else {
                    status->rssi = -114 + 2*a;
                }
            }
            if ((b != 99) && (b < (int)sizeof(_qual))) status->qual = _qual[b];  //
        }
    }
    return WAIT;
}

// ----------------------------------------------------------------
// internet connection
int MDMParser::_cbIPSHUT(int type, const char* buf, int len, char *temp)
{
    if (type == TYPE_IPSHUT) {
        return RESP_OK;
    }
    return WAIT;
}

int MDMParser::_cbCGATT(int type, const char* buf, int len, char *state)
{
    if ((type == TYPE_PLUS) && state) {
        if (sscanf(buf, "\r\n+CGATT: %d\r\n", state) == 1) {
            MOLMC_LOGD(TAG, "CGATT: %d", *state);
        }
    }
    return WAIT;
}

int MDMParser::_cbCIFSR(int type, const char* buf, int len, MDM_IP* ip)
{
    if ((type == TYPE_UNKNOWN) && ip) {
        int a,b,c,d;
        if (sscanf(buf, "\r\n" IPSTR "", &a,&b,&c,&d) == 4)
            *ip = IPADR(a,b,c,d);
    }
    return RESP_OK;
}

int MDMParser::_cbGetIpStatus(int type, const char* buf, int len, ip_status_t* result)
{
    int rst;
    char tmp[32]="";

    if ((type == TYPE_STATUS) && result) {
        if (sscanf(buf, "\r\nSTATE: %[^\r]", tmp) == 1) {
            if(strstr(tmp, "IP INITIAL")) {
                *result = IPSTATUS_INITIAL;
            }
            else if(strstr(tmp, "IP START")) {
                *result = IPSTATUS_START;
            }
            else if(strstr(tmp, "IP CONFIG")) {
                *result = IPSTATUS_CONFIG;
            }
            else if(strstr(tmp, "IP GPRSACT")) {
                *result = IPSTATUS_GPRSACT;
            }
            else if(strstr(tmp, "IP STATUS")) {
                *result = IPSTATUS_STATUS;
            }
            else if(strstr(tmp, "IP PROCESSING")) {
                *result = IPSTATUS_PROCESSING;
            }
            else if(strstr(tmp, "PDP DEACT")) {
                *result = IPSTATUS_DEACT;
            }
            else {
                *result = IPSTATUS_ATERROR;
            }
        }
        return RESP_OK;
    }
    return WAIT;
}

ip_status_t MDMParser::getIpStatus(void)
{
    ip_status_t result = IPSTATUS_ATERROR;
    LOCK();

    if (_init) {
        sendFormated("AT+CIPSTATUS\r\n");
        if (RESP_OK == waitFinalResp()){
            if (RESP_OK == waitFinalResp(_cbGetIpStatus, &result)){
            }
            else {
                result = IPSTATUS_ATERROR;
            }
        }
    }
    UNLOCK();
    return result;
}

void MDMParser::drive(void)
{
#if 0
    int ret = RESP_OK;
    char state = 0;

    LOCK();
    if(_dev.sim != SIM_READY) {
        if(IS_CELLULAR_TIMEOUT()) {
            sendFormated("AT+CPIN?\r\n");
            ret = waitFinalResp(_cbCPIN, &_dev.sim, 1000);
            if ((RESP_OK == ret) && (_dev.sim == SIM_READY)) {
                CLR_CELLULAR_TIMEOUT();
                init();
                _state = CELLULAR_STATE_READY;
            } else {
                if (_dev.sim == SIM_MISSING) {
                    MOLMC_LOGE(TAG, "SIM not inserted");
                }
                ARM_CELLULAR_TIMEOUT(SIM_QUERY_DURATION);
            }
        }
    }

    if(_state == CELLULAR_STATE_CONNECTING) {
        if(IS_CELLULAR_TIMEOUT()) {
            sendFormated("AT+CGATT?\r\n");
            ret = waitFinalResp(_cbCGATT, &state, 1000);
            if ((RESP_OK == ret) && (state)) {
                CLR_CELLULAR_TIMEOUT();
                pdp();
                _state = CELLULAR_STATE_CONNECTED;
                HAL_NET_notify_connected();
                HAL_NET_notify_dhcp(true);
            } else {
                ARM_CELLULAR_TIMEOUT(ATTCH_QUERY_DURATION);
            }
        }
    }

    // 处理接收数据
    waitFinalResp(NULL, NULL, 0);
    UNLOCK();
#else
    LOCK();
    // 处理接收数据
    waitFinalResp(NULL, NULL, 0);
    UNLOCK();
#endif
}

// ----------------------------------------------------------------
bool MDMParser::connect(const char* apn, const char* username, const char* password, Auth auth)
{
    bool ok = true;

    LOCK();
    if (_init && _pwr) {
        switch(_state) {
            case CELLULAR_STATE_READY:
            case CELLULAR_STATE_DISCONNECTED:
                /* perform GPRS disattach */
                sendFormated("AT+CGATT=0\r\n");
                if (RESP_OK != waitFinalResp())
                    goto failure;

                /* Start Task */
                sendFormated("AT+CSTT=\"CMNET\"\r\n");
                if (RESP_OK != waitFinalResp())
                    goto failure;

                /* perform GPRS attach .*/
                sendFormated("AT+CGATT=1\r\n");
#if 1
                if (RESP_OK != waitFinalResp(NULL, NULL, 30*1000))
                    goto failure;

                /* setup the PDP context*/
                if(!pdp())
                    goto failure;

                HAL_NET_notify_connected();
                HAL_NET_notify_dhcp(true);
                _state = CELLULAR_STATE_CONNECTED;
#else
                ARM_CELLULAR_TIMEOUT(ATTCH_QUERY_DURATION);
                _state = CELLULAR_STATE_CONNECTING;
#endif
                break;
            case CELLULAR_STATE_CONNECTED:
                break;
            default:
                ok = false;
                break;
        }
    }
    UNLOCK();
    return ok;

failure:
    UNLOCK();
    return false;
}

bool MDMParser::disconnect(void)
{
    MOLMC_LOGI(TAG, "[ Modem::disconnect ] = = = = = = = = = = = = =");
    bool ok = true;
    bool continue_cancel = false;
    char temp;

    LOCK();
    if (_init && _pwr) {
        if (_cancel_all_operations) {
            continue_cancel = true;
            resume(); // make sure we can use the AT parser
        }

        /* Deactivates the PDP context assoc */
        sendFormated("AT+CIPSHUT\r\n");
        if (RESP_OK != waitFinalResp(_cbIPSHUT, &temp))
            ok = false;

        /* perform GPRS disattach */
        sendFormated("AT+CGATT=0\r\n");
        if (RESP_OK != waitFinalResp())
            ok = false;

        if (continue_cancel) cancel();
    }
    _state = CELLULAR_STATE_DISCONNECTED;
    UNLOCK();
    return ok;
}

// ----------------------------------------------------------------
// setup the PDP context
bool MDMParser::pdp(void)
{
    MDM_IP ip = NOIP;

    LOCK();
    if (_init && _pwr) {
        /* Bring Up Wireless Connection with GPRS or CSD */
        sendFormated("AT+CIICR\r\n");
        if (RESP_OK != waitFinalResp(NULL, NULL, 85*1000))
            goto failure;

        /* Get local IP address */
        sendFormated("AT+CIFSR\r\n");
        if (RESP_OK != waitFinalResp(_cbCIFSR, &ip))
            goto failure;

        UNLOCK();
        return true;
    }
failure:
    UNLOCK();
    return false;
}

// ----------------------------------------------------------------
int MDMParser::_cbCDNSGIP(int type, const char* buf, int len, MDM_IP* ip)
{
    if ((type == TYPE_PLUS) && ip) {
        int r,a,b,c,d;
        if (sscanf(buf, "\r\n+CDNSGIP: %d,\"%*[^\"]\", \"" IPSTR "\"\r\n", &r,&a,&b,&c,&d) == 5) {
            if(r == 1) {
                *ip = IPADR(a,b,c,d);
                return RESP_OK;
            }
            else {
                return RESP_ERROR;
            }
        }
    }
    return WAIT;
}

MDM_IP MDMParser::gethostbyname(const char* host)
{
    MDM_IP ip = NOIP;
    int a,b,c,d;
    if (sscanf(host, IPSTR, &a,&b,&c,&d) == 4)
        ip = IPADR(a,b,c,d);
    else {
        LOCK();
        sendFormated("AT+CDNSGIP=\"%s\"\r\n", host);
        if (RESP_OK == waitFinalResp()) {
            if (RESP_OK != waitFinalResp(_cbCDNSGIP, &ip)) {
                ip = NOIP;
            }
        }
        UNLOCK();
    }
    return ip;
}

MDM_IP MDMParser::getIpAddress(void)
{
    MDM_IP ip = NOIP;

    LOCK();
    sendFormated("AT+CIFSR\r\n");
    if (RESP_OK != waitFinalResp(_cbCIFSR, &ip)) {
        ip = NOIP;
    }
    UNLOCK();
    return ip;
}

// ----------------------------------------------------------------
// sockets

int MDMParser::socketCreate(IpProtocol ipproto, int port)
{
    int socket;
    LOCK();

    //MOLMC_LOGD(TAG, "socketCreate(%s)", (ipproto?"UDP":"TCP"));
    // find an free socket
    socket = _findSocket(MDM_SOCKET_ERROR);
    if (socket != MDM_SOCKET_ERROR) {
        //MOLMC_LOGD(TAG, "Socket %d: handle %d was created", socket, socket);
        _sockets[socket].handle     = socket;
        _sockets[socket].ipproto    = ipproto;
        _sockets[socket].localip    = port;
        _sockets[socket].connected  = false;
        _sockets[socket].pending    = 0;
        _sockets[socket].open       = true;
        _sockets[socket].pipe = new Pipe<char>(MAX_SIZE);
    }
    //MOLMC_LOGD(TAG, "socketCreate(%s)", (ipproto?"UDP":"TCP"));
    UNLOCK();
    return socket;
}

bool MDMParser::socketConnect(int socket, const char * host, int port)
{
    MDM_IP ip = gethostbyname(host);
    if (ip == NOIP)
        return false;
    //MOLMC_LOGD(TAG, "socketConnect(host: %s)", host);
    // connect to socket
    return socketConnect(socket, ip, port);
}

bool MDMParser::socketConnect(int socket, const MDM_IP& ip, int port)
{
    bool ok = false;
    LOCK();
    if (ISSOCKET(socket) && (!_sockets[socket].connected)) {
        //MOLMC_LOGD(TAG, "socketConnect(%d,port:%d)", socket,port);
        if(_sockets[socket].ipproto)
            sendFormated("AT+CIPSTART=%d,\"%s\",\"" IPSTR "\",\"%d\"\r\n", _sockets[socket].handle, "UDP", IPNUM(ip), port);
        else
            sendFormated("AT+CIPSTART=%d,\"%s\",\"" IPSTR "\",\"%d\"\r\n", _sockets[socket].handle, "TCP", IPNUM(ip), port);
        if (RESP_OK == waitFinalResp()) {
            if (RESP_OK == waitFinalResp()) {
                _sockets[socket].remoteip = ip;
                _sockets[socket].remoteport = port;
                ok = _sockets[socket].connected = true;
            }
        }
    }
    UNLOCK();
    return ok;
}

bool MDMParser::socketIsConnected(int socket)
{
    bool ok = false;
    LOCK();
    ok = ISSOCKET(socket) && _sockets[socket].connected;
    //MOLMC_LOGD(TAG, "socketIsConnected(%d) %s", socket, ok?"yes":"no");
    UNLOCK();
    return ok;
}

bool MDMParser::socketClose(int socket)
{
    bool ok = false;
    LOCK();
    if (ISSOCKET(socket)
            && (_sockets[socket].connected || _sockets[socket].open))
    {
        //MOLMC_LOGD(TAG, "socketClose(%d)", socket);
        sendFormated("AT+CIPCLOSE=%d,1\r\n", _sockets[socket].handle);
        if (RESP_ERROR == waitFinalResp()) {
        }
        // Assume RESP_OK in most situations, and assume closed
        // already if we couldn't close it, even though this can
        // be false. Recovery added to socketCreate();
        _sockets[socket].connected = false;
        _sockets[socket].open = false;
        ok = true;
    }
    UNLOCK();
    return ok;
}

bool MDMParser::_socketFree(int socket)
{
    bool ok = false;
    LOCK();
    if ((socket >= 0) && (socket < NUMSOCKETS)) {
        if (_sockets[socket].handle != MDM_SOCKET_ERROR) {
            //MOLMC_LOGD(TAG, "socketFree(%d)",  socket);
            _sockets[socket].handle     = MDM_SOCKET_ERROR;
            _sockets[socket].localip    = 0;
            _sockets[socket].connected  = false;
            _sockets[socket].pending    = 0;
            _sockets[socket].open       = false;
            if (_sockets[socket].pipe)
                delete _sockets[socket].pipe;
        }
        ok = true;
    }
    UNLOCK();
    return ok; // only false if invalid socket
}

bool MDMParser::socketFree(int socket)
{
    // make sure it is closed
    socketClose(socket);
    return _socketFree(socket);
}

int MDMParser::socketSend(int socket, const char * buf, int len)
{
    //MOLMC_LOGD(TAG, "socketSend(%d,%d)", socket,len);
    int cnt = len;
    while (cnt > 0) {
        int blk = USO_MAX_WRITE;
        if (cnt < blk)
            blk = cnt;
        bool ok = false;
        {
            LOCK();
            if (ISSOCKET(socket)) {
                sendFormated("AT+CIPSEND=%d,%d\r\n",_sockets[socket].handle,blk);
                if (RESP_PROMPT == waitFinalResp()) {
                    send(buf, blk);
                    if (RESP_OK == waitFinalResp()) {
                        ok = true;
                    }
                }
            }
            UNLOCK();
        }
        if (!ok)
            return MDM_SOCKET_ERROR;
        buf += blk;
        cnt -= blk;
    }
    return (len - cnt);
}

int MDMParser::socketSendTo(int socket, MDM_IP ip, int port, const char * buf, int len)
{
    //MOLMC_LOGD(TAG, "socketSendTo(%d," IPSTR ",%d,%d)", socket,IPNUM(ip),port,len);
    int cnt = len;
    while (cnt > 0) {
        int blk = USO_MAX_WRITE;
        if (cnt < blk)
            blk = cnt;
        bool ok = false;
        {
            LOCK();
            if (ISSOCKET(socket)) {
                sendFormated("AT+CIPSEND=%d,%d\r\n",_sockets[socket].handle, blk);
                if (RESP_PROMPT == waitFinalResp()) {
                    send(buf, blk);
                    if (RESP_OK == waitFinalResp()) {
                        ok = true;
                    }
                }
            }
            UNLOCK();
        }
        if (!ok)
            return MDM_SOCKET_ERROR;
        buf += blk;
        cnt -= blk;
    }
    return (len - cnt);
}

int MDMParser::socketReadable(int socket)
{
    waitFinalResp(NULL, NULL, 0);
    int pending = MDM_SOCKET_ERROR;
    if (_cancel_all_operations)
        return MDM_SOCKET_ERROR;

    if (ISSOCKET(socket)) {
        // allow to receive unsolicited commands
        pending = _sockets[socket].pending;
    }

    return pending;
}

int MDMParser::socketRecv(int socket, char* buf, int len)
{
    if (ISSOCKET(socket)) {
        int available = socketReadable(socket);
        if (available>0)  {
            if (len > available)    // only read up to the amount available. When 0,
                len = available;// skip reading and check timeout.
            _sockets[socket].pipe->get(buf,len,false);
            _sockets[socket].pending -= len;
            return len;
        }
    }
    return 0;
}

int MDMParser::socketRecvFrom(int socket, MDM_IP* ip, int* port, char* buf, int len)
{
    if (ISSOCKET(socket)) {
        int available = socketReadable(socket);
        if (available>0)  {
            if (len > available)    // only read up to the amount available. When 0,
                len = available;// skip reading and check timeout.
            _sockets[socket].pipe->get(buf,len,false);
            _sockets[socket].pending -= len;
            *ip = _sockets[socket].remoteip;
            *port = _sockets[socket].remoteport;
            return len;
        }
    }
    return 0;
}

int MDMParser::_findSocket(int handle) {
    for (int socket = 0; socket < NUMSOCKETS; socket ++) {
        if (_sockets[socket].handle == handle)
            return socket;
    }
    return MDM_SOCKET_ERROR;
}

// ----------------------------------------------------------------

int MDMParser::_cbCMGL(int type, const char* buf, int len, CMGLparam* param)
{
    if ((type == TYPE_PLUS) && param && param->num) {
        // +CMGL: <ix>,...
        int ix;
        if (sscanf(buf, "\r\n+CMGL: %d,", &ix) == 1)
        {
            *param->ix++ = ix;
            param->num--;
        }
    }
    return WAIT;
}

int MDMParser::smsList(const char* stat /*= "ALL"*/, int* ix /*=NULL*/, int num /*= 0*/) {
    int ret = -1;
    LOCK();
    sendFormated("AT+CMGL=\"%s\"\r\n", stat);
    CMGLparam param;
    param.ix = ix;
    param.num = num;
    if (RESP_OK == waitFinalResp(_cbCMGL, &param))
        ret = num - param.num;
    UNLOCK();
    return ret;
}

bool MDMParser::smsSend(const char* num, const char* buf)
{
    bool ok = false;
    LOCK();
    sendFormated("AT+CMGS=\"%s\"\r\n",num);
    if (RESP_PROMPT == waitFinalResp(NULL,NULL,150*1000)) {
        send(buf, strlen(buf));
        const char ctrlZ = 0x1A;
        send(&ctrlZ, sizeof(ctrlZ));
        ok = (RESP_OK == waitFinalResp());
    }
    UNLOCK();
    return ok;
}

bool MDMParser::smsDelete(int ix)
{
    bool ok = false;
    LOCK();
    sendFormated("AT+CMGD=%d\r\n",ix);
    ok = (RESP_OK == waitFinalResp());
    UNLOCK();
    return ok;
}

int MDMParser::_cbCMGR(int type, const char* buf, int len, CMGRparam* param)
{
    if (param) {
        if (type == TYPE_PLUS) {
            if (sscanf(buf, "\r\n+CMGR: \"%*[^\"]\",\"%[^\"]", param->num) == 1) {
            }
        } else if ((type == TYPE_UNKNOWN) && (buf[len-2] == '\r') && (buf[len-1] == '\n')) {
            memcpy(param->buf, buf, len-2);
            param->buf[len-2] = '\0';
        }
    }
    return WAIT;
}

bool MDMParser::smsRead(int ix, char* num, char* buf, int len)
{
    bool ok = false;
    LOCK();
    CMGRparam param;
    param.num = num;
    param.buf = buf;
    sendFormated("AT+CMGR=%d\r\n",ix);
    ok = (RESP_OK == waitFinalResp(_cbCMGR, &param));
    UNLOCK();
    return ok;
}

// ----------------------------------------------------------------

int MDMParser::_cbCUSD(int type, const char* buf, int len, char* resp)
{
    if ((type == TYPE_PLUS) && resp) {
        // +USD: \"%*[^\"]\",\"%[^\"]\",,\"%*[^\"]\",%d,%d,%d,%d,\"*[^\"]\",%d,%d"..);
        if (sscanf(buf, "\r\n+CUSD: %*d,\"%[^\"]\",%*d", resp) == 1) {
            /*nothing*/
        }
    }
    return WAIT;
}

bool MDMParser::ussdCommand(const char* cmd, char* buf)
{
    bool ok = false;
    LOCK();
    *buf = '\0';
    // 2G/3G devices only
    sendFormated("AT+CUSD=1,\"%s\"\r\n",cmd);
    ok = (RESP_OK == waitFinalResp(_cbCUSD, buf));
    UNLOCK();
    return ok;
}

// ----------------------------------------------------------------
void MDMParser::dumpDevStatus(DevStatus* status)
{
    MOLMC_LOGI(TAG, "[ Modem::devStatus ] = = = = = = = = = = = = = =");
    const char* txtDev[] = { "Unknown", "SIM800C" };
    if (status->dev < sizeof(txtDev)/sizeof(*txtDev) && (status->dev != DEV_UNKNOWN))
        MOLMC_LOGD(TAG, "  Device:       %s", txtDev[status->dev]);
    const char* txtLpm[] = { "Disabled", "Enabled", "Active" };
    if (status->lpm < sizeof(txtLpm)/sizeof(*txtLpm))
        MOLMC_LOGD(TAG, "  Power Save:   %s", txtLpm[status->lpm]);
    const char* txtSim[] = { "Unknown", "Missing", "Pin", "Ready" };
    if (status->sim < sizeof(txtSim)/sizeof(*txtSim) && (status->sim != SIM_UNKNOWN))
        MOLMC_LOGD(TAG, "  SIM:          %s", txtSim[status->sim]);
    if (*status->ccid)
        MOLMC_LOGD(TAG, "  CCID:         %s", status->ccid);
    if (*status->imei)
        MOLMC_LOGD(TAG, "  IMEI:         %s", status->imei);
    if (*status->imsi)
        MOLMC_LOGD(TAG, "  IMSI:         %s", status->imsi);
    if (*status->meid)
        MOLMC_LOGD(TAG, "  MEID:         %s", status->meid); // LISA-C
    if (*status->manu)
        MOLMC_LOGD(TAG, "  Manufacturer: %s", status->manu);
    if (*status->model)
        MOLMC_LOGD(TAG, "  Model:        %s", status->model);
    if (*status->ver)
        MOLMC_LOGD(TAG, "  Version:      %s", status->ver);
}

void MDMParser::dumpNetStatus(NetStatus *status)
{
    MOLMC_LOGI(TAG, "[ Modem::netStatus ] = = = = = = = = = = = = = =");
    const char* txtReg[] = { "Unknown", "Denied", "None", "Home", "Roaming" };
    if (status->csd < sizeof(txtReg)/sizeof(*txtReg) && (status->csd != REG_UNKNOWN))
        MOLMC_LOGD(TAG, "  CSD Registration:   %s", txtReg[status->csd]);
    if (status->psd < sizeof(txtReg)/sizeof(*txtReg) && (status->psd != REG_UNKNOWN))
        MOLMC_LOGD(TAG, "  PSD Registration:   %s", txtReg[status->psd]);
    const char* txtAct[] = { "Unknown", "GSM", "Edge", "3G", "CDMA" };
    if (status->act < sizeof(txtAct)/sizeof(*txtAct) && (status->act != ACT_UNKNOWN))
        MOLMC_LOGD(TAG, "  Access Technology:  %s", txtAct[status->act]);
    if (status->rssi)
        MOLMC_LOGD(TAG, "  Signal Strength:    %d dBm", status->rssi);
    if (status->qual)
        MOLMC_LOGD(TAG, "  Signal Quality:     %d", status->qual);
    if (*status->opr)
        MOLMC_LOGD(TAG, "  Operator:           %s", status->opr);
    if (status->lac != 0xFFFF)
        MOLMC_LOGD(TAG, "  Location Area Code: %04X", status->lac);
    if (status->ci != 0xFFFFFFFF)
        MOLMC_LOGD(TAG, "  Cell ID:            %08X", status->ci);
    if (*status->num)
        MOLMC_LOGD(TAG, "  Phone Number:       %s", status->num);
}

void MDMParser::dumpIp(MDM_IP ip)
{
    if (ip != NOIP) {
        MOLMC_LOGD(TAG, "[ Modem:IP " IPSTR " ] = = = = = = = = = = = = = =", IPNUM(ip));
    }
}

// ----------------------------------------------------------------
int MDMParser::_parseMatch(Pipe<char>* pipe, int len, const char* sta, const char* end)
{
    int o = 0;
    if (sta) {
        while (*sta) {
            if (++o > len)                  return WAIT;
            char ch = pipe->next();
            if (*sta++ != ch)               return NOT_FOUND;
        }
    }
    if (!end)                               return o; // no termination
    // at least any char
    if (++o > len)                      return WAIT;
    pipe->next();
    // check the end
    int x = 0;
    while (end[x]) {
        if (++o > len)                      return WAIT;
        char ch = pipe->next();
        x = (end[x] == ch) ? x + 1 :
            (end[0] == ch) ? 1 :
            0;
    }
    return o;
}

int MDMParser::_parseFormated(Pipe<char>* pipe, int len, const char* fmt)
{
    int o = 0;
    int num = 0;
    if (fmt) {
        while (*fmt) {
            if (++o > len)                  return WAIT;
            char ch = pipe->next();
            if (*fmt == '%') {
                fmt++;
                if (*fmt == 'd') { // numeric
                    fmt ++;
                    while (ch >= '0' && ch <= '9') {
                        if (++o > len)      return WAIT;
                        ch = pipe->next();
                    }
                }
                else if (*fmt == 'n') { // data len
                    fmt ++;
                    num = 0;
                    while (ch >= '0' && ch <= '9') {
                        num = num * 10 + (ch - '0');
                        if (++o > len)      return WAIT;
                        ch = pipe->next();
                    }
                }
                else if (*fmt == 'c') { // char buffer (takes last numeric as length)
                    fmt ++;
                    while (--num) {
                        if (++o > len)      return WAIT;
                        ch = pipe->next();
                    }
                    continue;
                }
                else if (*fmt == 's') {
                    fmt ++;
                    if (ch != '\"')         return NOT_FOUND;
                    do {
                        if (++o > len)      return WAIT;
                        ch = pipe->next();
                    } while (ch != '\"');
                    if (++o > len)          return WAIT;
                    ch = pipe->next();
                }
            }
            if (*fmt++ != ch)               return NOT_FOUND;
        }
    }
    return o;
}

int MDMParser::_getLine(Pipe<char>* pipe, char* buf, int len)
{
    int unkn = 0;
    int sz = pipe->size();
    int fr = pipe->free();
    if (len > sz)
        len = sz;
    while (len > 0)
    {
        static struct {
            const char* fmt;                              int type;
        } lutF[] = {
            { "\r\n%d, CLOSED\r\n",                         TYPE_CONNECTCLOSTED  },
            { "\r\n%d, CONNECT OK\r\n",                     TYPE_OK         },
            { "\r\n%d, CONNECT FAIL\r\n",                   TYPE_ERROR      },
            { "\r\n%d, ALREADY CONNECT\r\n",                TYPE_OK         },
            { "\r\n%d, SEND OK\r\n",                        TYPE_OK         },
            { "\r\n%d, SEND FAIL\r\n",                      TYPE_ERROR      },
            { "\r\nDATA ACCEPT:%d,%d\r\n",                  TYPE_OK         },
            { "\r\n%d, CLOSE OK\r\n",                       TYPE_OK         },
            { "\r\n+RECEIVE,%d,%n:\r\n%c",                  TYPE_PLUS       },
        };
        static struct {
            const char* sta;          const char* end;    int type;
        } lut[] = {
            { "\r\nOK\r\n",             NULL,               TYPE_OK         },
            { "\r\nERROR\r\n",          NULL,               TYPE_ERROR      },
            { "\r\n+CME ERROR:",        "\r\n",             TYPE_ERROR      },
            { "\r\n+CMS ERROR:",        "\r\n",             TYPE_ERROR      },
            { "\r\n+CDNSGIP:",          "\r\n",             TYPE_PLUS       },
            { "\r\nRING\r\n",           NULL,               TYPE_RING       },
            { "\r\nCONNECT\r\n",        NULL,               TYPE_CONNECT    },
            { "\r\n+PDP: DEACT\r\n",    "\r\n",             TYPE_DISCONNECT },
            { "\r\n+CPIN: NOT READY",   "\r\n",             TYPE_DISCONNECT },
            { "\r\nNO CARRIER\r\n",     NULL,               TYPE_NOCARRIER  },
            { "\r\nNO DIALTONE\r\n",    NULL,               TYPE_NODIALTONE },
            { "\r\nBUSY\r\n",           NULL,               TYPE_BUSY       },
            { "\r\nNO ANSWER\r\n",      NULL,               TYPE_NOANSWER   },
            { "\r\nSHUT OK\r\n",        NULL,               TYPE_IPSHUT     },
            { "\r\n+",                  "\r\n",             TYPE_PLUS       },
            { "\r\n@",                  NULL,               TYPE_PROMPT     }, // Sockets
            { "\r\n>",                  NULL,               TYPE_PROMPT     }, // Sockets
            { "\n>",                    NULL,               TYPE_PROMPT     }, // File
            { "\r\nABORTED\r\n",        NULL,               TYPE_ABORTED    }, // Current command aborted
            { "\r\nSTATE:",             "\r\n",             TYPE_STATUS     }, // ip status
            { "\r\n\r\n",               "\r\n",             TYPE_DBLNEWLINE }, // Double CRLF detected
            { "\r\n",                   "\r\n",             TYPE_UNKNOWN    }, // If all else fails, break up generic strings
        };
        for (int i = 0; i < (int)(sizeof(lutF)/sizeof(*lutF)); i ++) {
            pipe->set(unkn);
            int ln = _parseFormated(pipe, len, lutF[i].fmt);
            if (ln == WAIT && fr)
                return WAIT;
            if ((ln != NOT_FOUND) && (unkn > 0))
                return TYPE_UNKNOWN | pipe->get(buf, unkn);
            if (ln > 0)
                return lutF[i].type  | pipe->get(buf, ln);
        }
        for (int i = 0; i < (int)(sizeof(lut)/sizeof(*lut)); i ++) {
            pipe->set(unkn);
            int ln = _parseMatch(pipe, len, lut[i].sta, lut[i].end);
            if (ln == WAIT && fr)
                return WAIT;

            // Double CRLF detected, discard it.
            // This resolves a case on G350 where "\r\n" is generated after +USORF response, but missing
            // on U260/U270, which would otherwise generate "\r\n\r\nOK\r\n" which is not parseable.
            if ((ln > 0) && (lut[i].type == TYPE_DBLNEWLINE) && (unkn == 0)) {
                return TYPE_UNKNOWN | pipe->get(buf, 2);
            }
            if ((ln != NOT_FOUND) && (unkn > 0))
                return TYPE_UNKNOWN | pipe->get(buf, unkn);
            if (ln > 0)
                return lut[i].type | pipe->get(buf, ln);
        }
        // UNKNOWN
        unkn ++;
        len--;
    }
    return TYPE_UNKNOWN | pipe->get(buf, unkn); //应该返回TYPE_UNKNOWN 并且应该从缓存里面清掉。否则会一直接受到相同的数据 并且应该从缓存里面清掉。否则会一直接受到相同的数据 并且应该从缓存里面清掉。否则会一直接受到相同的数据 并且应该从缓存里面清掉。否则会一直处理相同的数据  chenkaiyao  2016-01-09
    //return WAIT;
}

// ----------------------------------------------------------------
// Electron Serial Implementation
// ----------------------------------------------------------------

MDMCellularSerial::MDMCellularSerial(int rxSize /*= 256*/, int txSize /*= 256*/) :
    CellularSerialPipe(rxSize, txSize)
{
    // Important to set _dev.lpm = LPM_ENABLED; when HW FLOW CONTROL enabled.
}

MDMCellularSerial::~MDMCellularSerial(void)
{
    powerOff();
}

int MDMCellularSerial::_send(const void* buf, int len)
{
    return put((const char*)buf, len, true/*=blocking*/);
}

int MDMCellularSerial::getLine(char* buffer, int length)
{
    return _getLine(&_pipeRx, buffer, length);
}

void MDMCellularSerial::pause()
{
    LOCK();
    rxPause();
}

void MDMCellularSerial::resume()
{
    LOCK();
    rxResume();
}

