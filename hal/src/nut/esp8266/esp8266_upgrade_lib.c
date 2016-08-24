#include "ets_sys.h"
#include "spi_flash.h"
#include "lwip/mem.h"
#include "esp8266_upgrade.h"

LOCAL struct upgrade_param *upgrade;
extern enum file_type_t filetype;

extern SpiFlashChip *flashchip;

/******************************************************************************
 * FunctionName : system_upgrade_internal
 * Description  : a
 * Parameters   :
 * Returns      :
*******************************************************************************/
LOCAL bool ICACHE_FLASH_ATTR
system_upgrade_internal(struct upgrade_param *upgrade, uint8 *data, uint16 len)
{
    bool ret = false;
    if(data == NULL || len == 0)
    {
    	return true;
    }
    upgrade->buffer = (uint8 *)os_zalloc(len + upgrade->extra);

    memcpy(upgrade->buffer, upgrade->save, upgrade->extra);
    memcpy(upgrade->buffer + upgrade->extra, data, len);

    len += upgrade->extra;
    upgrade->extra = len & 0x03;
    len -= upgrade->extra;

    memcpy(upgrade->save, upgrade->buffer + len, upgrade->extra);

    do {
        if (upgrade->fw_bin_addr + len >= (upgrade->fw_bin_sec + upgrade->fw_bin_sec_num) * SPI_FLASH_SEC_SIZE) {
            break;
        }

        if (len > SPI_FLASH_SEC_SIZE) {

        } else {
			//os_printf("%x %x\n",upgrade->fw_bin_sec_earse,upgrade->fw_bin_addr);
            /* earse sector, just earse when first enter this zone */
            if (upgrade->fw_bin_sec_earse != (upgrade->fw_bin_addr + len) >> 12) {
                upgrade->fw_bin_sec_earse = (upgrade->fw_bin_addr + len) >> 12;
                spi_flash_erase_sector(upgrade->fw_bin_sec_earse);
				//os_printf("%x\n",upgrade->fw_bin_sec_earse);
            }
        }

        if (spi_flash_write(upgrade->fw_bin_addr, (uint32 *)upgrade->buffer, len) != SPI_FLASH_RESULT_OK) {
            break;
        }

        ret = true;
        upgrade->fw_bin_addr += len;
    } while (0);

    os_free(upgrade->buffer);
    upgrade->buffer = NULL;
    return ret;
}

/******************************************************************************
 * FunctionName : system_upgrade
 * Description  : a
 * Parameters   :
 * Returns      :
*******************************************************************************/
bool ICACHE_FLASH_ATTR
system_upgrade(uint8 *data, uint16 len)
{
    bool ret;

    ret = system_upgrade_internal(upgrade, data, len);
    return ret;
}

/******************************************************************************
 * FunctionName : system_upgrade_init
 * Description  : a
 * Parameters   :
 * Returns      :
*******************************************************************************/
void ICACHE_FLASH_ATTR
system_upgrade_init(void)
{
    uint32 user_bin2_start;
    uint8 flash_buf[4];
    uint8 high_half;

    spi_flash_read(0, (uint32 *)flash_buf, 4);
    high_half = (flash_buf[3] & 0xF0) >> 4;

    if (upgrade == NULL) {
        upgrade = (struct upgrade_param *)os_zalloc(sizeof(struct upgrade_param));
    }

    system_upgrade_flag_set(UPGRADE_FLAG_IDLE);

    if (ONLINE_APP_FILE == filetype){
		upgrade->fw_bin_sec = CACHE_ONLINE_APP_SEC_START;
		upgrade->fw_bin_sec_num = CACHE_ONLINE_APP_SEC_NUM;
    }
    else if (BOOT_FILE == filetype){
		upgrade->fw_bin_sec = CACHE_BOOT_SEC_START;
		upgrade->fw_bin_sec_num = CACHE_BOOT_SEC_NUM;
    }
    else {
		upgrade->fw_bin_sec = CACHE_DEFAULT_APP_SEC_START;
		upgrade->fw_bin_sec_num = CACHE_DEFAULT_APP_SEC_NUM;
	}
    upgrade->fw_bin_addr = upgrade->fw_bin_sec * SPI_FLASH_SEC_SIZE;
}

/******************************************************************************
 * FunctionName : system_upgrade_deinit
 * Description  : a
 * Parameters   :
 * Returns      :
*******************************************************************************/
void ICACHE_FLASH_ATTR
system_upgrade_deinit(void)
{
    os_free(upgrade);
    upgrade = NULL;
}

