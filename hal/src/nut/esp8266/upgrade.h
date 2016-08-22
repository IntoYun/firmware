#ifndef __UPGRADE_H__
#define __UPGRADE_H__

#define SPI_FLASH_SEC_SIZE      4096       //Flash 扇区大小
#define LIMIT_ERASE_SIZE        0x10000    //Flash 擦除扇区大小限制


#define UPGRADE_FLAG_IDLE       0x00       //更新空闲
#define UPGRADE_FLAG_START      0x01       //更新开始
#define UPGRADE_FLAG_FINISH     0x02       //更新完成


enum file_type_t {
    ONLINE_APP_FILE,            //在线编程程序文件
    DEFAULT_APP_FILE,           //默认应用程序文件
    BOOT_FILE                   //bootloader程序文件
};

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

/*
 *  28k  bootloader
 *  4k   指令保存区
 *  720k esp8266应用程序
 *  256k 默认stm32应用程序
 *  16k  用户参数区
 *
 *  32k  备份区
 *  720k esp8266应用程序 stm32在线编程下载缓冲区
 *  256k 默认stm32应用程序 下载缓冲区
 **
 */

#define BOOT_SEC_START                   0    //BOOT占用16个扇区
#define BOOT_PARAMS_SEC_START            16   //boot参数区
#define SYSTEM_PARAMS_SEC_START          17   //系统参数区
#define COMMAND_INFO_SEC_START           23   //指令属性区
#define EEPROM_SEC_START                 24   //eeprom模拟区
#define APP_SEC_START                    32   //应用程序运行起始扇区
#define DEFAULT_APP_SEC_START            219  //默认应用程序存放起始扇区

#define APP_SEC_NUM                      187  //应用程序扇区总个数
#define DEFAULT_APP_SEC_NUM              89   //默认应用程序扇区总个数

#define CACHE_BOOT_SEC_START             308  //boot存放起始扇区
#define CACHE_ONLINE_APP_SEC_START       324  //在线编程 缓冲起始扇区
#define CACHE_DEFAULT_APP_SEC_START      324  //stm32默认固件 缓冲起始扇区

#define CACHE_BOOT_SEC_NUM               16   //boot程序扇区总个数
#define CACHE_ONLINE_APP_SEC_NUM         187  //在线应用程序  缓冲扇区总个数
#define CACHE_DEFAULT_APP_SEC_NUM        187  //默认应用程序 缓冲扇区总个数

#define APP_ADDR                         APP_SEC_START * SPI_FLASH_SEC_SIZE               //应用程序存放地址
#define DEFAULT_APP_ADDR                 DEFAULT_APP_SEC_START * SPI_FLASH_SEC_SIZE       //默认程序 存放地址
#define CACHE_BOOT_ADDR                  CACHE_BOOT_SEC_START * SPI_FLASH_SEC_SIZE        //bootloader下载存放位置
#define CACHE_ONLINE_APP_ADDR            CACHE_ONLINE_APP_SEC_START * SPI_FLASH_SEC_SIZE  //在线编程程序下载存放地址
#define CACHE_DEFAULT_APP_ADDR           CACHE_DEFAULT_APP_SEC_START * SPI_FLASH_SEC_SIZE //默认程序下载存放地址
#define COMMAND_INFO_ADDR                COMMAND_INFO_SEC_START * SPI_FLASH_SEC_SIZE      //参数存储位置

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


#endif
