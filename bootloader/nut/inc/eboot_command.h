/* Copyright (c) 2015 Ivan Grokhotkov. All rights reserved.
 * This file is part of eboot bootloader.
 *
 * Redistribution and use is permitted according to the conditions of the
 * 3-clause BSD license to be found in the LICENSE file.
 */

#ifndef EBOOT_COMMAND_H
#define EBOOT_COMMAND_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

enum action_t {
    ACTION_COPY_RAW = 0x00000001,
    ACTION_LOAD_APP = 0xffffffff
};

#define EBOOT_MAGIC 	 0xeb001000
#define EBOOT_MAGIC_MASK 0xfffff000

struct eboot_command{
    uint32_t magic;
    enum action_t action;
    uint32_t esp8266_app_addr[2];   //[0]:src  [1]:dst
    uint32_t esp8266_app_size;
    uint32_t default_stm32_app_addr[2];
    uint32_t default_stm32_app_size;
    uint32_t neutron_boot_size;
    uint32_t online_stm32_app_size;
    uint32_t crc32;
};

void eboot_command_init(struct eboot_command* cmd);
void eboot_command_read(struct eboot_command* cmd);
void eboot_command_write(struct eboot_command* cmd);

#endif //EBOOT_COMMAND_H
