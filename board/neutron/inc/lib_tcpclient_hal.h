#ifndef __LIB_TCPCLIENT_HAL_H
#define __LIB_TCPCLIENT_HAL_H

int TcpClient_connect_hal(const char *host, uint16_t port,unsigned char handle);

unsigned char TcpClient_open_hal();
unsigned char TcpClient_close_hal(unsigned char handle);

int TcpClient_readn_hal(unsigned char *buff, int size,unsigned char tcp_handle);

int TcpClient_available_hal(unsigned char handle);

int TcpClient_writen_hal(const unsigned char *p_dat,int len,unsigned char handle);



unsigned char TcpClient_connected_hal(unsigned char handle);

void TcpClient_stop_hal(unsigned char handle);

//有数据返回1   无返回0
int mo_TcpClient_peek_hal(unsigned char *p_dat);


#endif


