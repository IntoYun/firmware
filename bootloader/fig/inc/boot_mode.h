/* Copyright (c) 2015 Ivan Grokhotkov. All rights reserved.
 * This file is part of eboot bootloader.
 *
 * Redistribution and use is permitted according to the conditions of the
 * 3-clause BSD license to be found in the LICENSE file.
 */

#ifndef BOOT_MODE_H
#define BOOT_MODE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "esp_flash_data_types.h"

#define SPI_SEC_SIZE 0x1000
#define IROM_LOW    0x400D0000
#define IROM_HIGH   0x40400000
#define DROM_LOW    0x3F400000
#define DROM_HIGH   0x3F800000
#define RTC_IRAM_LOW  0x400C0000
#define RTC_IRAM_HIGH 0x400C2000
#define RTC_DATA_LOW  0x50000000
#define RTC_DATA_HIGH 0x50002000


typedef enum
{
    DEFAULT_FIRWARE = 0, OTA_FIRWARE = 1
} Firmware_TypeDef;

void start_app(void);
void Enter_Default_RESTORE_Mode(void);
void Enter_Serail_Com_Mode(void);
void Enter_Factory_RESTORE_Mode(void);
void Enter_Factory_ALL_RESTORE_Mode(void);
void Enter_OTA_Update_Mode(void);

#ifdef __cplusplus
}
#endif


#endif //BOOT_MODE_H
