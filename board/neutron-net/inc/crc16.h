#ifndef __CRC16_H__
#define __CRC16_H__

#define CRC_BYTE 2   //16bit的crc

void ICACHE_FLASH_ATTR crc_reset();
void ICACHE_FLASH_ATTR crc_update(uint8_t c);
void ICACHE_FLASH_ATTR crc_update_n(uint8_t *p_dat,int len);
uint16_t ICACHE_FLASH_ATTR crc_get_reseult();
uint8_t ICACHE_FLASH_ATTR crc_check(uint16_t _CRC);


#endif
