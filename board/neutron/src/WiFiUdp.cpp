/*
  WiFiUdp.cpp - Library for Arduino Wifi shield.
  Copyright (c) 2011-2014 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include <string.h>
#include "lib_system_all.h"

#include "WiFiUdp.h"

#include "lib_tcpclient_hal.h"
#include "WiFiUdp_hal.h"
#include "lib_wifi_drv.h"

/* Constructor */
WiFiUDP::WiFiUDP()
{

}

/*
成功 1
失败 0
*/
uint8_t WiFiUDP::begin(uint16_t port)
{
  _port = port;
  //repeat check
  if(_begin)
  {
    MO_ERROR(("begin repeat"));
    return 1;
  }

  //get one handle
  handle=TcpClient_open_hal();    //创建fifo  获取句柄
  if(handle==MO_FAILED)
  {
    MO_ERROR(("TcpClient_open_hal failed"));
    return 0;
  }

  _begin=1;

  //set udp port
  return WiFiUDP_begin_hal(handle,port);  //发送监听端口命令
}

/* return number of bytes available in the current packet,
   will return zero if parsePacket hasn't been called yet */

/*
有数据 1
无数据 0


*/
int WiFiUDP::available()
{
  //初始化检查
  if(!_begin)
  {
    MO_ERROR(("No begin"));
    return 0;
  }

  return TcpClient_available_hal(handle);



  //判断sock是否正常并返回 可读取数据个数   0 没有parsePacket

/*
	 if (_sock != NO_SOCKET_AVAIL)
	 {
	      return ServerDrv::availData(_sock);
	 }
	 return 0;

*/

}

/* Release any resources being used by this WiFiUDP instance */
void WiFiUDP::stop()
{
    //判断已经停止 直接返回
    //都则断开 udp
  if(!_begin) return;


  TcpClient_stop_hal(handle); //发送结束cmd   删除fifo  回收句柄
}

int WiFiUDP::beginPacket(const char *host, uint16_t port)
{

    //发送头
  //创建缓冲区

  //初始化检查
  if(!_begin)
  {
    MO_ERROR(("No begin"));
    return 0;
  }

  //发送ip  端口存储
  _index_buf=0;
  memset(_send_ip,0x00,sizeof(_send_ip));

  strncpy(_send_ip,host,sizeof(_send_ip)-1);
  _port_send=port;

  return 1;
}

int WiFiUDP::beginPacket(IPAddress ip, uint16_t port)
{
  //发送头
  //创建缓冲区

  //repeat check
  if(!_begin)
  {
    MO_ERROR(("No begin"));
    return 0;
  }


  String address;
  address.reserve(18);
  address += ip[0];
  address += '.';
  address += ip[1];
  address += '.';
  address += ip[2];
  address += '.';
  address += ip[3];

  return beginPacket(address.c_str(),port);

}

/*

ok  1

failed 0

*/

int WiFiUDP::endPacket()
{
  return WiFiUDP_endPacket_hal(handle, _index_buf, _send_buf);
}

size_t WiFiUDP::write(uint8_t byte)
{
  //写入数据到缓冲区

  return write(&byte, 1);
}

size_t WiFiUDP::write(const uint8_t *buffer, size_t size)
{
  //数据写入缓冲区

	//ServerDrv::insertDataBuf(_sock, buffer, size);

  if( (_index_buf+size)>(sizeof(_send_buf)) )
  {
    MO_ERROR(("over flow"));
    return 0;
  }

  memset(_send_buf, 0, sizeof(_send_buf));
  memcpy(_send_buf+_index_buf,buffer,size);
  _index_buf+=size;

	return size;

}

int WiFiUDP::parsePacket()
{
  //返回数据个数
	return available();
}

int WiFiUDP::read()
{
  uint8_t b;
  if (available())
  {
    //返回一个字节数据
	  //ServerDrv::getData(_sock, &b);
	  read(&b,1);
  	return b;
  }
  else
  {
	  return -1;
  }
}

int WiFiUDP::read(unsigned char* buffer, size_t len)
{
  if (available())
  {
    return WiFiUDP_read_hal(buffer,len,handle);
  }
  else
  {
	  return -1;
  }
}

int WiFiUDP::peek()
{
  uint8_t b;
  if (!available()) return -1;
  //暂不实现
  WiFiUDP_peek_hal(&b);
  // ServerDrv::getData(_sock, &b, 1);
  return b;
}

void WiFiUDP::flush()
{
  while (available())
    read();
}

IPAddress  WiFiUDP::remoteIP()
{
	uint8_t _remoteIp[4] = {0};
	uint8_t _remotePort[2] = {0};

  //获取当前包的ip地址
  //	WiFiDrv::getRemoteData(_sock, _remoteIp, _remotePort);
  //暂不支持
  WiFiUDP_remoteIP_hal();

	IPAddress ip(_remoteIp);
	return ip;
}

uint16_t  WiFiUDP::remotePort()
{
	uint8_t _remoteIp[4] = {0};
	uint8_t _remotePort[2] = {0};

  //获取当前包的端口号
	//WiFiDrv::getRemoteData(_sock, _remoteIp, _remotePort);
	WiFiUDP_remotePort_hal();
	uint16_t port = (_remotePort[0]<<8)+_remotePort[1];
	return port;
}

