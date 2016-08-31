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
#include "esp8266_conn.h"
#include "timer_hal.h"
#include "service_debug.h"


#ifdef putc
#undef putc
#undef getc
#endif

extern "C" {
void* ICACHE_RAM_ATTR pvPortZalloc(size_t size, const char* file, int line);

void yield(void);
void optimistic_yield(uint32_t interval_us);

void esp_yield();
void esp_schedule();
}

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
volatile uint8_t _socketConnectConfirmed=0;
volatile uint8_t _socketDisconnected=0;
volatile uint8_t _socketSended=0;
/* Private macro ------------------------------------------------------------*/
#define PROFILE         "0"   //!< this is the psd profile used
#define MAX_SIZE        1024  //!< max expected messages (used with RX)
#define USO_MAX_WRITE   1024  //!< maximum number of bytes to write to socket (used with TX)
// num sockets
#define NUMSOCKETS      ((int)(sizeof(_sockets)/sizeof(*_sockets)))
//! test if it is a socket is ok to use
#define ISSOCKET(s)     (((s) >= 0) && ((s) < NUMSOCKETS) && (_sockets[s].handle != MDM_SOCKET_ERROR))

static volatile uint32_t esp8266_conn_timeout_start;
static volatile uint32_t esp8266_conn_timeout_duration;

inline void ARM_CONN_TIMEOUT(uint32_t dur) {
    esp8266_conn_timeout_start = HAL_Timer_Get_Milli_Seconds();
    esp8266_conn_timeout_duration = dur;
    DEBUG("esp8266 CONN WD Set %d",(dur));
}
inline bool IS_CONN_TIMEOUT() {
    return esp8266_conn_timeout_duration && ((HAL_Timer_Get_Milli_Seconds()-esp8266_conn_timeout_start)>esp8266_conn_timeout_duration);
}

inline void CLR_CONN_TIMEOUT() {
    esp8266_conn_timeout_duration = 0;
    DEBUG("esp8266 CONN WD Cleared, was %d", esp8266_conn_timeout_duration);
}

SockCtrl Esp8266ConnClass::_sockets[5];

Esp8266ConnClass::Esp8266ConnClass(void)
{
    memset(_sockets, 0, sizeof(_sockets));
    for (int socket = 0; socket < NUMSOCKETS; socket ++)
        _sockets[socket].handle = MDM_SOCKET_ERROR;
}

int Esp8266ConnClass::socketSocket(IpProtocol ipproto, int port)
{
    int socket;

    // find an free socket
    socket = _findSocket(MDM_SOCKET_ERROR);
    if (socket != MDM_SOCKET_ERROR) {
        DEBUG_D("Socket %d: handle %d was created\r\n", socket, socket);
        _sockets[socket].handle       = socket;
        _sockets[socket].ipproto      = ipproto;
        _sockets[socket].local_port   = port;
        _sockets[socket].remote_port  = 0;
        memset((void *)_sockets[socket].remote_ip, 0, 4);
        _sockets[socket].connected  = false;
        _sockets[socket].pending    = 0;
        _sockets[socket].open       = true;
        _sockets[socket].pipe = new Pipe<char>(MAX_SIZE);
        _sockets[socket].esp8266_conn_ptr = (struct espconn *)os_zalloc(sizeof(struct espconn));
    }
    DEBUG_D("socketSocket(%s)\r\n", (ipproto?"UDP":"TCP"));
    return socket;
}

bool Esp8266ConnClass::socketConnect(int socket, const MDM_IP& ip, int port)
{
    bool ok = false;
    if (ISSOCKET(socket) && (!_sockets[socket].connected)) {
        DEBUG_D("socketConnect(%d, ip:%x port:%d)\r\n", socket, ip, port);
        _sockets[socket].remote_port  = port;
        memcpy((void *)_sockets[socket].remote_ip, &ip, 4);
        if(_sockets[socket].ipproto) {
            _sockets[socket].esp8266_conn_ptr->type = ESPCONN_UDP;
            _sockets[socket].esp8266_conn_ptr->state = ESPCONN_NONE;
            _sockets[socket].esp8266_conn_ptr->proto.udp = (esp_udp *)os_zalloc(sizeof(esp_udp));
            _sockets[socket].esp8266_conn_ptr->proto.udp->local_port = _sockets[socket].local_port;
            memcpy(_sockets[socket].esp8266_conn_ptr->proto.udp->remote_ip,&ip,4);
            _sockets[socket].esp8266_conn_ptr->proto.udp->remote_port = port;
            _sockets[socket].connected = true;
            espconn_create(_sockets[socket].esp8266_conn_ptr);
        }
        else {
            _sockets[socket].esp8266_conn_ptr->type = ESPCONN_TCP;
            _sockets[socket].esp8266_conn_ptr->state = ESPCONN_NONE;
            _sockets[socket].esp8266_conn_ptr->proto.tcp = (esp_tcp *)os_zalloc(sizeof(esp_tcp));
            _sockets[socket].esp8266_conn_ptr->proto.tcp->local_port = espconn_port();
            memcpy(_sockets[socket].esp8266_conn_ptr->proto.tcp->remote_ip, &ip, 4);
            _sockets[socket].esp8266_conn_ptr->proto.tcp->remote_port = port;
            espconn_regist_connectcb(_sockets[socket].esp8266_conn_ptr, _espconn_connect_callback);
            espconn_regist_reconcb(_sockets[socket].esp8266_conn_ptr, _espconn_reconnect_callback);
            espconn_regist_disconcb(_sockets[socket].esp8266_conn_ptr, _espconn_disconnect_callback);
            ARM_CONN_TIMEOUT(2000);
            _socketConnectConfirmed =0;
            espconn_connect(_sockets[socket].esp8266_conn_ptr);
            while (!_socketConnectConfirmed) {
                optimistic_yield(100);
                if(IS_CONN_TIMEOUT()) {
                    CLR_CONN_TIMEOUT();
                    break;
                }
            }
        }

        if(_sockets[socket].connected == true) {
            espconn_regist_recvcb(_sockets[socket].esp8266_conn_ptr, _espconn_recv_callback);
            espconn_regist_sentcb(_sockets[socket].esp8266_conn_ptr, _espconn_sent_callback);
            ok = true;
        }
    }
    return ok;
}

bool Esp8266ConnClass::socketIsConnected(int socket)
{
    bool ok = false;
    ok = ISSOCKET(socket) && _sockets[socket].connected;
    //DEBUG_D("socketIsConnected(%d) %s\r\n", socket, ok?"yes":"no");
    return ok;
}

bool Esp8266ConnClass::socketClose(int socket)
{
    bool ok = false;
    if (ISSOCKET(socket)
            && (_sockets[socket].connected || _sockets[socket].open))
    {
        DEBUG_D("socketClose(%d)\r\n", socket);
        ARM_CONN_TIMEOUT(1000);
        _socketDisconnected =0;
        espconn_disconnect(_sockets[socket].esp8266_conn_ptr);
        while (!_socketDisconnected) {
            optimistic_yield(100);
            if(IS_CONN_TIMEOUT()) {
                CLR_CONN_TIMEOUT();
                break;
            }
        }
        // Assume RESP_OK in most situations, and assume closed
        // already if we couldn't close it, even though this can
        // be false. Recovery added to socketSocket();
        _sockets[socket].connected = false;
        _sockets[socket].open = false;
        ok = true;
    }
    return ok;
}

bool Esp8266ConnClass::_socketFree(int socket)
{
    bool ok = false;
    if ((socket >= 0) && (socket < NUMSOCKETS)) {
        if (_sockets[socket].handle != MDM_SOCKET_ERROR) {
            DEBUG_D("socketFree(%d)\r\n",  socket);
            _sockets[socket].handle     = MDM_SOCKET_ERROR;
            _sockets[socket].local_port    = 0;
            _sockets[socket].remote_port  = 0;
            memset((void *)_sockets[socket].remote_ip, 0, 4);
            _sockets[socket].connected  = false;
            _sockets[socket].pending    = 0;
            _sockets[socket].open       = false;
            /*
            if(_sockets[socket].ipproto) {
                free(_sockets[socket].esp8266_conn_ptr->proto.udp);
            }
            else {
                free(_sockets[socket].esp8266_conn_ptr->proto.tcp);
            }
            if (_sockets[socket].esp8266_conn_ptr)
                free(_sockets[socket].esp8266_conn_ptr);
            */
            if (_sockets[socket].pipe)
                delete _sockets[socket].pipe;
        }
        ok = true;
    }
    return ok; // only false if invalid socket
}

bool Esp8266ConnClass::socketFree(int socket)
{
    // make sure it is closed
    socketClose(socket);
    return _socketFree(socket);
}

int Esp8266ConnClass::socketSend(int socket, const char * buf, int len)
{
    DEBUG_D("socketSend(%d,,%d)\r\n", socket,len);
    int cnt = len;
    while (cnt > 0) {
        int blk = USO_MAX_WRITE;
        if (cnt < blk)
            blk = cnt;
        bool ok = false;
        {
            if (ISSOCKET(socket)) {
                ARM_CONN_TIMEOUT(2000);
                _socketSended =0;
                espconn_sent(_sockets[socket].esp8266_conn_ptr, (uint8 *)buf, blk);
                while (!_socketSended) {
                    optimistic_yield(100);
                    if(IS_CONN_TIMEOUT()) {
                        CLR_CONN_TIMEOUT();
                        break;
                    }
                }
                ok = true;
            }
        }
        if (!ok)
            return MDM_SOCKET_ERROR;
        buf += blk;
        cnt -= blk;
    }
    return (len - cnt);
}

int Esp8266ConnClass::socketSendTo(int socket, MDM_IP ip, int port, const char * buf, int len)
{
    //DEBUG_D("socketSendTo(%d," IPSTR ",%d,,%d)\r\n", socket,IPNUM(ip),port,len)
    //Todo 后期实现
    return socketSend(socket, buf, len);
}

int Esp8266ConnClass::socketReadable(int socket)
{
    int pending = MDM_SOCKET_ERROR;

    optimistic_yield(100);
    if (ISSOCKET(socket) && _sockets[socket].connected) {
        //DEBUG_D("socketReadable(%d)\r\n", socket);
        // allow to receive unsolicited commands
        if (_sockets[socket].connected)
            pending = _sockets[socket].pending;
    }
    return pending;
}

int Esp8266ConnClass::socketRecv(int socket, char* buf, int len)
{
    if (ISSOCKET(socket)) {
        if (_sockets[socket].connected) {
            int available = socketReadable(socket);
            if (available>0)  {
                if (len > available)    // only read up to the amount available. When 0,
                    len = available;// skip reading and check timeout.
                _sockets[socket].pipe->get(buf,len,false);
                _sockets[socket].pending -= len;
                return len;
            }
        }
    }
    return 0;
}

int Esp8266ConnClass::socketRecvFrom(int socket, MDM_IP* ip, int* port, char* buf, int len)
{
    return socketRecv(socket, buf, len);
}

int Esp8266ConnClass::_findSocket(int handle) {
    for (int socket = 0; socket < NUMSOCKETS; socket ++) {
        if (_sockets[socket].handle == handle)
            return socket;
    }
    return MDM_SOCKET_ERROR;
}

void Esp8266ConnClass::_espconn_connect_callback(void *arg) {
    int socket=0;
    struct espconn *pespconn = (struct espconn *)arg;

    DEBUG_D("_espconn_connect_callback\r\n");
    for (socket=0; socket < NUMSOCKETS; socket++) {
        if (ISSOCKET(socket)) {
            if((pespconn->proto.tcp->remote_port == _sockets[socket].remote_port)
                    &&(!memcmp(pespconn->proto.tcp->remote_ip, (void *)_sockets[socket].remote_ip, 4))) {
                break;
            }
        }
    }
    if (NUMSOCKETS != socket) {
        _sockets[socket].connected = true;
    }
    _socketConnectConfirmed = 1;
}

void Esp8266ConnClass::_espconn_reconnect_callback(void *arg, sint8 errType) {
    int socket=0;
    struct espconn *pespconn = (struct espconn *)arg;

    DEBUG_D("_espconn_reconnect_callback\r\n");
    for (socket=0; socket < NUMSOCKETS; socket++) {
        if (ISSOCKET(socket)) {
            if((pespconn->proto.tcp->remote_port == _sockets[socket].remote_port)
                    &&(!memcmp(pespconn->proto.tcp->remote_ip, (void *)_sockets[socket].remote_ip, 4))) {
                break;
            }
        }
    }
    if (NUMSOCKETS != socket) {
        if(_sockets[socket].ipproto) {
            free(pespconn->proto.udp);
            pespconn->proto.udp = NULL;
        }
        else {
            free(pespconn->proto.tcp);
            pespconn->proto.tcp = NULL;
        }
        free(pespconn);
        pespconn = NULL;
    }
    _socketFree(socket);
    _socketConnectConfirmed = 1;
}

void Esp8266ConnClass::_espconn_sent_callback(void *arg) {
    DEBUG_D("_espconn_sent_callback\r\n");
    _socketSended = 1;
}

void Esp8266ConnClass::_espconn_recv_callback(void *arg, char *pusrdata, unsigned short len) {
    int socket=0, n=0;
    struct espconn *pespconn = (struct espconn *)arg;

    DEBUG_D("_espconn_recv_callback\r\n");
    for (socket=0; socket < NUMSOCKETS; socket++) {
        if (ISSOCKET(socket)) {
            if((pespconn->proto.tcp->remote_port == _sockets[socket].remote_port)
                    &&(!memcmp(pespconn->proto.tcp->remote_ip, (void *)_sockets[socket].remote_ip, 4))) {
                break;
            }
        }
    }
    if (NUMSOCKETS != socket) {
        DEBUG_D("Socket %d: has %d bytes pending\r\n", socket, len);
        for(n=0; n < len ; n++) {
            if (_sockets[socket].pipe->writeable()) {
                _sockets[socket].pipe->putc(pusrdata[n]);
            }
            else{
                break;
            }
        }
        _sockets[socket].pending += n;
    }
}

void Esp8266ConnClass::_espconn_disconnect_callback(void *arg) {
    int socket=0;
    struct espconn *pespconn = (struct espconn *)arg;

    DEBUG_D("_espconn_disconnect_callback\r\n");
    for (socket=0; socket < NUMSOCKETS; socket++) {
        if (ISSOCKET(socket)) {
            if((pespconn->proto.tcp->remote_port == _sockets[socket].remote_port)
                    &&(!memcmp(pespconn->proto.tcp->remote_ip, (void *)_sockets[socket].remote_ip, 4))) {
                break;
            }
        }
    }
    if (NUMSOCKETS != socket) {
        if(_sockets[socket].ipproto) {
            free(pespconn->proto.udp);
            pespconn->proto.udp = NULL;
        }
        else {
            free(pespconn->proto.tcp);
            pespconn->proto.tcp = NULL;
        }
        free(pespconn);
        pespconn = NULL;
    }
    _socketDisconnected = 1;
}

