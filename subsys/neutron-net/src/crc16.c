/**
 ******************************************************************************
 * @file
 * @authors
 * @version V1.0.0
 * @date    2014_11_13
 * @brief
 ******************************************************************************
 */
/****************************************************************************
 *Private Included Files
 ****************************************************************************/
#include "osapi.h"
#include "at_custom.h"
#include "user_interface.h"

/************************************************************************************
 * Private Types
 ************************************************************************************/

/************************************************************************************
 * Private Data
 ************************************************************************************/
static uint16_t CRCSUM;

/************************************************************************************
 * Private Functions
 ************************************************************************************/
uint16_t _crc_ccitt_update(uint16_t crc, uint8_t data){
    data ^= crc & 0xff;
    data ^= data << 4;
    return ((((uint16_t)data << 8) | ((crc >> 8) & 0xff)) ^
            (uint8_t)(data >> 4) ^
            ((uint16_t)data << 3));
}

/************************************************************************************
 *  Public Functions
 ************************************************************************************/
void ICACHE_FLASH_ATTR
crc_reset(){
    CRCSUM = 0xFFFF;
}

void ICACHE_FLASH_ATTR
crc_update(uint8_t c){
    CRCSUM = _crc_ccitt_update(CRCSUM,c);
}

void ICACHE_FLASH_ATTR
crc_update_n(uint8_t *p_dat,int len){
    while(len--)
    {
        crc_update( *(p_dat++) );
    }
}

uint16_t ICACHE_FLASH_ATTR
crc_get_reseult(){
    return CRCSUM;
}

uint8_t ICACHE_FLASH_ATTR
crc_check(uint16_t _CRC){
    return CRCSUM == _CRC;
}


