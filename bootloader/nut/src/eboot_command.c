#include "eboot_command.h"
#include "flash.h"

uint32_t crc32_update(uint32_t crc, const uint8_t *data, size_t length)
{
    uint32_t i;
    bool bit;
    uint8_t c;

    while (length--) {
        c = *data++;
        for (i = 0x80; i > 0; i >>= 1) {
            bit = crc & 0x80000000;
            if (c & i) {
                bit = !bit;
            }
            crc <<= 1;
            if (bit) {
                crc ^= 0x04c11db7;
            }
        }
    }
    return crc;
}

uint32_t eboot_command_calculate_crc32(const struct eboot_command* cmd)
{
    return crc32_update(0xffffffff, (const uint8_t*) cmd,
                      offsetof(struct eboot_command, crc32));
}

void eboot_command_init(struct eboot_command* cmd)
{
    cmd->action = ACTION_LOAD_APP;
    cmd->online_app_size = 0;
    cmd->default_app_size = 0;
    cmd->boot_size = 0;
    eboot_command_write(cmd);
}

void eboot_command_read(struct eboot_command* cmd)
{
    if (!SPIRead(COMMAND_INFO_ADDR, cmd, sizeof(struct eboot_command)))
    {
        uint32_t crc32 = eboot_command_calculate_crc32(cmd);
        if ((cmd->magic & EBOOT_MAGIC_MASK) == EBOOT_MAGIC && cmd->crc32 == crc32)
        {
            return;
        }
    }
    eboot_command_init(cmd);
}

void eboot_command_write(struct eboot_command* cmd)
{
    cmd->magic = EBOOT_MAGIC;
    cmd->crc32 = eboot_command_calculate_crc32(cmd);

    SPIEraseSector(COMMAND_INFO_SEC_START);
    SPIWrite(COMMAND_INFO_ADDR, cmd, sizeof(struct eboot_command));
}

