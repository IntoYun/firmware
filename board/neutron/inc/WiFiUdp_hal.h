#ifndef WIFIUDP_HAL_H_
#define WIFIUDP_HAL_H_


#include "stdint.h"

uint8_t WiFiUDP_begin_hal(uint8_t ahandle,uint16_t aport);
/* uint8_t WiFiUDP_endPacket_hal(uint8_t handle,uint16_t len,char *p_ip,char *p_dat,uint16_t port); */
uint8_t WiFiUDP_endPacket_hal(uint8_t handle, uint16_t len, char *p_dat);
int WiFiUDP_read_hal(unsigned char *buff, int size,unsigned char handle);
void WiFiUDP_peek_hal(uint8_t *p);
void WiFiUDP_remoteIP_hal();
void WiFiUDP_remotePort_hal();


#endif

