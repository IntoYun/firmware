

#ifndef   WIRING_TCPSERVER_H_HAL_
#define   WIRING_TCPSERVER__HALH_


//功能:建立tcpserver
//参数:useLocalhost 1  127.0.0.1     0    0.0.0.0
//返回:值成功1  失败0       

uint8_t mo_TcpServer_begin_hal(uint16_t port,uint8_t useLocalhost);

/*
功能:返回接入的tcp通道(句柄0--4)
返回:tcp通道    -1无
*/

int8_t mo_TcpServer_accept_hal(void); 

/*
功能:给所有tcp客户端发送一个字节
返回:无
*/

void mo_TcpServer_write_hal(uint8_t c); 



#endif