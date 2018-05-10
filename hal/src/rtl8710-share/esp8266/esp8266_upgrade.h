#ifndef ESP8266_UPGRADE_H_
#define ESP8266_UPGRADE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "flash_map.h"

#define UPGRADE_FLAG_IDLE       0x00       //更新空闲
#define UPGRADE_FLAG_START      0x01       //更新开始
#define UPGRADE_FLAG_FINISH     0x02       //更新完成

typedef void (*upgrade_states_check_callback)(void * arg);

//#define UPGRADE_SSL_ENABLE

struct upgrade_server_info {
    uint8 ip[4];
    uint16 port;
    uint8 upgrade_flag;
    uint8 pre_version[16];
    uint8 upgrade_version[16];
    char md5[36]; //文件md5码 32个字节  asc码
    uint32 check_times;
    uint8 *url;
    upgrade_states_check_callback check_cb;
    struct espconn *pespconn;
};


struct upgrade_param {
    uint32 fw_bin_addr;
    uint32 fw_bin_sec;
    uint32 fw_bin_sec_num;
    uint32 fw_bin_sec_earse;
    uint32 extra;
    uint8 save[4];
    uint8 *buffer;
};


void system_upgrade_init();
void system_upgrade_deinit();
bool system_upgrade(uint8 *data, uint16 len);
bool system_upgrade_start(struct upgrade_server_info *server);


#ifdef __cplusplus
}
#endif

#endif
