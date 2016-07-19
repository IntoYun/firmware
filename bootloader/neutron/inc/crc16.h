#ifndef __CRC16_H__
#define __CRC16_H__

#define CRC_BYTE 2   //16bit的crc

void crc_reset();

void crc_update(uint8_t c);

void crc_update_n(uint8_t *p_dat,int len);

uint16_t crc_get_reseult();

uint8_t crc_check(uint16_t _CRC);


#endif
