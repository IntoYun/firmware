/**
 ******************************************************************************
 * @file     : wiring_eerprom.h
 * @author   : robin
 * @version	 : V1.0.0
 * @date     : 6-December-2014
 * @brief    :
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef   WIRING_EEPROM_H_
#define   WIRING_EEPROM_H_

#include "variant.h"
#include <stdint.h>

#define Param_Area_Size (512)    //参数区大小
#define Param_Eeprom_ADDR   (EEPROM_SIZE-Param_Area_Size)      /*总共最后Param_Area_Size字节用来存储系统参数*/

/* Internal Flash Page size = 16KByte */
#define PAGE_SIZE  (uint16_t)0x4000

/* EEPROM emulation start address in Flash (just after the write protected bootloader program space) */

#define EEPROM_START_ADDRESS    ((uint32_t)0x08008000)						//如果是boot引导模式则把8008000  16k+16k给eeprom使用

/* EraseSector_ID */
#define ID_ErasePAGE0 FLASH_SECTOR_2
#define ID_ErasePAGE1 FLASH_SECTOR_3

/* EEPROM Emulation Size */
#define EEPROM_SIZE             ((uint16_t)1024*4)       /* 最大为:扇区/4   4K*/




/* Pages 0 and 1 base and end addresses */
#define PAGE0_BASE_ADDRESS      ((uint32_t)(EEPROM_START_ADDRESS + 0x000))
#define PAGE0_END_ADDRESS       ((uint32_t)(EEPROM_START_ADDRESS + (PAGE_SIZE - 1)))

#define PAGE1_BASE_ADDRESS      ((uint32_t)(EEPROM_START_ADDRESS + PAGE_SIZE))
#define PAGE1_END_ADDRESS       ((uint32_t)(EEPROM_START_ADDRESS + (2 * PAGE_SIZE - 1)))



/* Used Flash pages for EEPROM emulation */
#define PAGE0                   ((uint16_t)0x0000)
#define PAGE1                   ((uint16_t)0x0001)

/* No valid page define */
#define NO_VALID_PAGE           ((uint16_t)0x00AB)

/* Page status definitions */
#define ERASED                  ((uint16_t)0xFFFF)     /* PAGE is empty */
#define RECEIVE_DATA            ((uint16_t)0xEEEE)     /* PAGE is marked to receive data */
#define VALID_PAGE              ((uint16_t)0x0000)     /* PAGE containing valid data */

/* Valid pages in read and write defines */
#define READ_FROM_VALID_PAGE    ((uint8_t)0x00)
#define WRITE_IN_VALID_PAGE     ((uint8_t)0x01)

/* Page full define */
#define PAGE_FULL               ((uint8_t)0x80)



typedef enum
{
  FLASH_BUSY_ = 1,
  FLASH_ERROR_RD_,
  FLASH_ERROR_PGS_,
  FLASH_ERROR_PGP_,
  FLASH_ERROR_PGA_,
  FLASH_ERROR_WRP_,
  FLASH_ERROR_PROGRAM_,
  FLASH_ERROR_OPERATION_,
  FLASH_COMPLETE
}FLASH_Status;


uint16_t EEPROM_Init(void);
uint16_t EEPROM_ReadVariable(uint16_t EepromAddress, uint16_t *EepromData);
uint16_t EEPROM_WriteVariable(uint16_t EepromAddress, uint16_t EepromData);

/* Arduino Compatibility Class -----------------------------------------------*/
class EEPROMClass
{
    public:
        EEPROMClass();
        uint8_t read(int);
        void write(int, uint8_t);
        void write_system(int, uint8_t);
};

extern EEPROMClass EEPROM;

#endif /* WIRING_EEPROM_H_ */
