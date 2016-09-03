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
#include "c_types.h"
#include "user_interface.h"
#include "espconn.h"
#include "mem.h"
#include "osapi.h"
#include "crc16.h"
#include "user_config.h"

#define pheadbuffer "Connection: keep-alive\r\n\
Cache-Control: no-cache\r\n\
User-Agent: Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.101 Safari/537.36 \r\n\
Accept: */*\r\n\
Accept-Encoding: gzip,deflate\r\n\
Accept-Language: zh-CN,eb-US;q=0.8\r\n\r\n"

struct espconn *pespconn = NULL;
struct upgrade_server_info *upServer = NULL;

static ip_addr_t host_ip;
enum file_type_t filetype = ONLINE_STM32_APP_FILE;
static os_timer_t at_get_file_timer;

uint8_t down_progress=0;
os_timer_t downfile_timer;

char hostname_tmp[128] = {0};
char httppara_tmp[128] = {0};

// Description  : use uart0 to transfer buffer
void ICACHE_FLASH_ATTR
uart0_tx_buffer(uint8 *buf, uint16 len){
    #define UART0   0
    uint16 i;
    for (i = 0; i < len; i++)
    {
        uart_tx_one_char(UART0, buf[i]);
    }
}

void ICACHE_FLASH_ATTR
down_neutron_boot_cb(void){
    os_timer_disarm(&downfile_timer);
    filetype = NEUTRON_BOOT_FILE;
    char para[256] = {0};
    os_sprintf(para,"%s/%s", httppara_tmp, "neutron-boot.bin");
    downFile(hostname_tmp, para, "", at_cmddownfile_rsp);
}

void ICACHE_FLASH_ATTR
down_default_stm32_app_cb(void){
    os_timer_disarm(&downfile_timer);
    filetype = DEFAULT_STM32_APP_FILE;
    char para[256] = {0};
    os_sprintf(para,"%s/%s", httppara_tmp, "default-neutron.bin");
    downFile(hostname_tmp, para, "", at_cmddownfile_rsp);
}

//升级回调函数
void ICACHE_FLASH_ATTR
at_cmddownfile_rsp(void *arg){
    char output[64] = {0};
    struct upgrade_server_info *server = arg;

    os_memset(output, 0, sizeof(output));
    if(server->upgrade_flag == true)
    {
        if(ONLINE_STM32_APP_FILE == filetype) //esp8266升级文件
        {
            DEBUG("online_stm32_app_success\r\n");
            os_sprintf(output,"%s:0\r\n", CMD_DOWN_FILE);//返回下载成功
        }
        else if(ESP8266_APP_FILE == filetype) //esp8266升级文件
        {
            DEBUG("esp8266_app_success\r\n");
            os_timer_disarm(&downfile_timer);
            os_timer_setfn(&downfile_timer, (os_timer_func_t *)down_neutron_boot_cb, NULL);
            os_timer_arm(&downfile_timer, 500, 0);
            return;
        }
        else if(NEUTRON_BOOT_FILE == filetype) //esp8266升级文件
        {
            DEBUG("stm32_bootloader_success\r\n");
            os_timer_disarm(&downfile_timer);
            os_timer_setfn(&downfile_timer, (os_timer_func_t *)down_default_stm32_app_cb, NULL);
            os_timer_arm(&downfile_timer, 500, 0);
            return;
        }
        else if(DEFAULT_STM32_APP_FILE == filetype) //默认stm32程序文件
        {
            DEBUG("default_stm32_app_success\r\n");
            os_sprintf(output,"%s:0\r\n", CMD_NET_DOWN); //返回下载成功
        }
    }
    else
    {
        DEBUG("downfile_failed\r\n");
        if(ONLINE_STM32_APP_FILE == filetype) //esp8266升级文件
        {
            os_sprintf(output,"%s:1\r\n", CMD_DOWN_FILE);//返回下载失败
        }
        else
        {
            os_sprintf(output,"%s:1\r\n", CMD_NET_DOWN);//返回下载失败
        }
    }
    at_port_print(output);
    at_response_ok();

    os_free(server->url);
    server->url = NULL;
    os_free(server);
    server = NULL;
}

//通过host name回去ip回调函数
LOCAL void ICACHE_FLASH_ATTR
upServer_dns_found(const char *name, ip_addr_t *ipaddr, void *arg){
    struct espconn *pespconn = (struct espconn *) arg;
    char output[64] = {0};

    DEBUG("upServer_dns_found\r\n");
    if(ipaddr == NULL)
    {
        at_cmddownfile_rsp(upServer);
        return;
    }
    os_memcpy(upServer->ip, &ipaddr->addr, 4);
    system_upgrade_start(upServer);
}

/*
获取版本号
指令:AT+MOLMCGMR?
返回：
wifi version:1.0.0
OK
*/
void ICACHE_FLASH_ATTR
at_queryCmdGetVersion(uint8_t id){
    char output[128] = {0};

    os_memset(output, 0, sizeof(output));
    os_sprintf(output, "%s:%s\r\n", CMD_GETVERSION, WIFIVERSION);
    at_port_print(output);
    at_response_ok();
}

/*
获取版本号
指令:AT+CMD_DOWN_PROGRESS?
返回：
+CMD_DOWN_PROGRESS:<progress>
OK
*/
void ICACHE_FLASH_ATTR
at_queryCmdGetDownProgress(uint8_t id){
    char output[64] = {0};

    os_memset(output, 0, sizeof(output));
    os_sprintf(output, "%s:%d\r\n", CMD_DOWN_PROGRESS, down_progress);
    at_port_print(output);
    at_response_ok();
}

/*
   获取应用程序文件大小
cmd:AT+GETFILESIZE=16384,1  第1个参数:每个包的大小   第二个参数：文件类型     0 在线应用文件   1 默认应用文件  2.neutron bootloader文件
返回：
+GETFILESIZE:14             第1个参数:包个数
*/
void ICACHE_FLASH_ATTR
at_setupCmdGetFileSize(uint8_t id, char *pPara){
    char buffer[64] = {0};
    int packet_size = 1024, file_type = 0, err = 0;
    uint32_t file_sec_size;

    if (*pPara++ != '=') // skip '='
    {
        at_response_error();
        return;
    }
    //get the first parameter digit
    at_get_next_int_dec(&pPara, &packet_size, &err);
    if (*pPara++ != ',') { // skip ','
        at_response_error();
        return;
    }
    //get the second parameter digit
    at_get_next_int_dec(&pPara, &file_type, &err);
    if (*pPara != '\r') {
        at_response_error();
        return;
    }

    if( 0 == file_type){//在线下载应用文件
        file_sec_size = (cmd_info.online_stm32_app_size + packet_size -1)/packet_size;
    }
    else if( 1 == file_type){//默认应用文件
        if(0 == cmd_info.default_stm32_app_size){
            file_sec_size = (CACHE_DEF_STM32_APP_SEC_NUM * SPI_FLASH_SEC_SIZE + packet_size -1)/packet_size;
        }
        else{
            file_sec_size = (cmd_info.default_stm32_app_size + packet_size -1)/packet_size;
        }
    }
    else if( 2 == file_type){//neutron bootloader程序
        file_sec_size = (cmd_info.neutron_boot_size + packet_size -1)/packet_size;
    }
    else
    {
        at_response_error();
        return;
    }
    os_sprintf(buffer,"%s:%d\r\n", CMD_GET_FILE_SIZE ,file_sec_size);
    at_port_print(buffer);
    at_response_ok();
}

//获取应用程序数据包
//cmd:AT+GETFILEPACKET=16384,0,1  第1个参数:每个包的大小    第2个参数:第几个包   第3个参数：文件类型     0 在线应用文件   1 默认应用文件
//返回：
//+GETFILEPACKET,16388:数据
void ICACHE_FLASH_ATTR
at_setupCmdGetFilePacket(uint8_t id, char *pPara){
    char buffer[64] = {0};
    uint8_t *packet_buffer = NULL;
    int packet_size = 1024, packet_index = 0, file_type = 0, err = 0;
    uint32_t readaddr;

    if (*pPara++ != '=') // skip '='
    {
        at_response_error();
        return;
    }
    //get the first parameter digit
    at_get_next_int_dec(&pPara, &packet_size, &err);
    if (*pPara++ != ',') { // skip ','
        at_response_error();
        return;
    }
    //get the second parameter digit
    at_get_next_int_dec(&pPara, &packet_index, &err);
    if (*pPara++ != ',') { // skip ','
        at_response_error();
        return;
    }
    //get the third parameter digit
    at_get_next_int_dec(&pPara, &file_type, &err);
    if (*pPara != '\r') {
        at_response_error();
        return;
    }

    if( 0 == file_type){//在线下载应用文件
        if(0 == cmd_info.online_stm32_app_size){
            at_response_error();
            return;
        }
        readaddr = CACHE_ONLINE_STM32_APP_SEC_START * SPI_FLASH_SEC_SIZE + packet_size * packet_index;
    }
    else if( 1 == file_type){//默认应用文件
        readaddr = DEF_STM32_APP_SEC_START * SPI_FLASH_SEC_SIZE + packet_size * packet_index;
    }
    else if( 2 == file_type){//neutron bootloader文件
        if(0 == cmd_info.neutron_boot_size){
            at_response_error();
            return;
        }
        readaddr = CACHE_NEUTRON_BOOT_SEC_START * SPI_FLASH_SEC_SIZE + packet_size * packet_index;
    }
    else
    {
        at_response_error();
        return;
    }

    at_enter_special_state();
    ets_wdt_disable();
    packet_buffer = (uint8_t *)os_zalloc(packet_size + 2);
    os_memset(packet_buffer, 0, sizeof(packet_buffer));
    if (SPI_FLASH_RESULT_OK == spi_flash_read(readaddr, (uint32 *)packet_buffer, packet_size)) {
        os_memset(buffer, 0, sizeof(buffer));
        os_sprintf(buffer,"%s,%d:",CMD_GET_FILE_PACKET, packet_size+2);
        uart0_tx_buffer(buffer,strlen(buffer));

        crc_reset();
        crc_update_n(packet_buffer,packet_size);
        uint16_t crc16 = crc_get_reseult();
        os_memcpy(&packet_buffer[packet_size], (uint8 *)&crc16, 2);

        //发送一个包数据+16bitCRC
        uart0_tx_buffer(packet_buffer,packet_size+2);
        uart0_tx_buffer("\r\n",2);
    }
    else{
        at_response_error();
    }
    os_free(packet_buffer);
    packet_buffer = NULL;
    at_leave_special_state();
}

void ICACHE_FLASH_ATTR
downFile(char *hostname, char *httppara, char *md5para, void *check_cb){
    upServer = (struct upgrade_server_info *)os_zalloc(sizeof(struct upgrade_server_info));
    upServer->upgrade_version[5] = '\0';
    upServer->port = 80;
    upServer->check_cb = check_cb;
    upServer->check_times = 20000;

    if(upServer->url == NULL)
    {
        upServer->url = (uint8 *) os_zalloc(1024);
    }

    os_sprintf(upServer->url,
            "GET %s HTTP/1.0\r\nHost: %s\r\n"pheadbuffer"",
            httppara, hostname);

    os_strcpy(upServer->md5, md5para);
    DEBUG("upServer->url=%s\r\n",upServer->url);
    host_ip.addr = ipaddr_addr(hostname);
    if (host_ip.addr != IPADDR_NONE)
    {
        os_memcpy(upServer->ip, &host_ip.addr, 4);
        system_upgrade_start(upServer);
    }
    else
    {
        pespconn = (struct espconn *)os_zalloc(sizeof(struct espconn));
        espconn_gethostbyname(pespconn, hostname, &host_ip, upServer_dns_found);
    }
}

/*
在线升级应用程序
AT+DOWNFILE="112.124.117.64","/v1/bins/?dwn_token=8b66c607d5516e3b79f56afd37471a24","955d2d630866ba99711614cccf2b5ba2"
返回：
+DOWNFILE:<status>  0:下载成功 1:下载失败 2.正在下载
OK
*/
void ICACHE_FLASH_ATTR
at_setupCmdDownFile(uint8_t id, char *pPara){
    char md5para[64] = {0};

    if (*pPara++ != '=') // skip '='
    {
        at_response_error();
        return;
    }

    //获取第1个参数 hostname
    at_data_str_copy(hostname_tmp, &pPara, sizeof(hostname_tmp));
    if (*pPara++ != ',') { // skip ','
        at_response_error();
        return;
    }
    //获取第2个参数 http para
    at_data_str_copy(httppara_tmp, &pPara, sizeof(httppara_tmp));
    if (*pPara++ != ',') { // skip ','
        at_response_error();
        return;
    }
    //获取第3个参数 md5
    at_data_str_copy(md5para, &pPara, sizeof(md5para));

    filetype = ONLINE_STM32_APP_FILE;
    down_progress=0;
    downFile(hostname_tmp, httppara_tmp, md5para, at_cmddownfile_rsp);
}

/*
esp8266 程序升级
AT+NETDOWN="112.124.117.64","/downloads/neutron/1.0.0/"
返回：
+NETDOWN:<status>  0:下载成功 1:下载失败 2.正在下载
OK
*/
void ICACHE_FLASH_ATTR
at_setupCmdNetDown(uint8_t id, char *pPara){
    if (*pPara++ != '=') // skip '='
    {
        at_response_error();
        return;
    }

    //获取第1个参数 hostname
    at_data_str_copy(hostname_tmp, &pPara, sizeof(hostname_tmp));
    if (*pPara++ != ',') { // skip ','
        at_response_error();
        return;
    }
    //获取第2个参数 http para
    at_data_str_copy(httppara_tmp, &pPara, sizeof(httppara_tmp));
    if (*pPara++ != '\r') {
        at_response_error();
        return;
    }
    filetype = ESP8266_APP_FILE;
    down_progress=0;
    char para[256] = {0};
    os_sprintf(para,"%s/%s", httppara_tmp, "neutron-net.bin");
    downFile(hostname_tmp, para, "", at_cmddownfile_rsp);
}

/*
esp8266 程序升级
AT+NETUPDATE
返回：
OK
*/
void ICACHE_FLASH_ATTR
at_exeCmdNetUpdate(uint8_t id){
    system_restart();
    at_response_ok();
}

