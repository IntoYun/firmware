#if 0
// pull in the sources from the HAL. It's a bit of a hack, but is simpler than trying to link the
// full hal library.
#include "../src/lora/eeprom_hal.c"

#else

/* Includes ------------------------------------------------------------------*/
#include "eeprom_hal.h"
#include "stm32l1xx.h"
#include "service_debug.h"


//L151 EEPROM固定起始地址
#define EEPROM_START_ADDRESS    ((uint32_t)0x08080000)

/* EEPROM Emulation Size */
#define EEPROM_SIZE             ((uint16_t)4096)       /* 4096 bytes (Max 4096/0xFFF bytes) */


static uint8_t EEPROM_ReadByte(uint32_t address);
static void EEPROM_WriteByte(uint32_t address, uint8_t dataVal);


static uint8_t EEPROM_ReadByte(uint32_t address)
{
    uint32_t rAddress = EEPROM_START_ADDRESS+address;
    uint8_t  tmp = 0;
    if(IS_FLASH_DATA_ADDRESS(rAddress))
    {
        HAL_FLASHEx_DATAEEPROM_Unlock();
        while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY));
        tmp = *(__IO uint8_t*)rAddress;
        HAL_FLASHEx_DATAEEPROM_Lock();
    }
    return tmp;
}

static void EEPROM_WriteByte(uint32_t address, uint8_t dataVal)
{
    uint32_t wAddress = EEPROM_START_ADDRESS+address;
    if(IS_FLASH_DATA_ADDRESS(wAddress))
    {
        HAL_FLASHEx_DATAEEPROM_Unlock();
        while(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEERASEDATA_BYTE,wAddress,dataVal) != HAL_OK);
        HAL_FLASHEx_DATAEEPROM_Lock();
    }
}


void HAL_EEPROM_Init(void)
{
}

uint8_t HAL_EEPROM_Read(uint32_t address)
{
    if (address < EEPROM_SIZE)
    {
       return EEPROM_ReadByte(address);
    }
    else
    {
        return 0xFF;
    }
}

void HAL_EEPROM_Write(uint32_t address, uint8_t data)
{
    if (address < EEPROM_SIZE)
    {
        EEPROM_WriteByte(address,data);
    }
}

size_t HAL_EEPROM_Length()
{
   return EEPROM_SIZE;
}

#endif
