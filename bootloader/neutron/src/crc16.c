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

#include "variant.h"


static uint16_t CRCSUM;

/************************************************************************************
 * Private Functions
 ************************************************************************************/
uint16_t _crc_ccitt_update(uint16_t crc, uint8_t data)
{
    data ^= crc & 0xff;
    data ^= data << 4;
    return ((((uint16_t)data << 8) | ((crc >> 8) & 0xff)) ^
            (uint8_t)(data >> 4) ^
            ((uint16_t)data << 3));
}

/************************************************************************************
 *  Public Functions
 ************************************************************************************/
void crc_reset()
{
    CRCSUM = 0xFFFF;
}

void crc_update(uint8_t c)
{
    CRCSUM = _crc_ccitt_update(CRCSUM,c);
}

void crc_update_n(uint8_t *p_dat,int len)
{
    while(len--)
    {
        crc_update( *(p_dat++) );
    }
}

uint16_t crc_get_reseult()
{
    return CRCSUM;
}

uint8_t crc_check(uint16_t _CRC)
{
    return CRCSUM == _CRC;
}


