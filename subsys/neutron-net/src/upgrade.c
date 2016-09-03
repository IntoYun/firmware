#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "md5.h"

#include "lwip/err.h"
#include "lwip/ip_addr.h"
#include "lwip/mem.h"
#include "lwip/app/espconn.h"

#include "user_config.h"
#include "upgrade_lib.c"



LOCAL md5_context_t _ctx;
LOCAL struct espconn *upgrade_conn;
LOCAL uint8 *pbuf;
LOCAL os_timer_t upgrade_rev_timer;
LOCAL os_timer_t upgrade_connect_timer;
LOCAL os_timer_t upgrade_timer;
LOCAL uint32 totallength = 0;
LOCAL uint32 sumlength = 0;

extern uint8_t down_progress;

/******************************************************************************
 * FunctionName : upgrade_disconcb
 * Description  : The connection has been disconnected successfully.
 * Parameters   : arg -- Additional argument to pass to the callback function
 * Returns      : none
*******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
upgrade_disconcb(void *arg){
    struct espconn *pespconn = arg;

    if (pespconn == NULL) {
        return;
    }

    os_free(pespconn->proto.tcp);
    pespconn->proto.tcp = NULL;
    os_free(pespconn);
    pespconn = NULL;
    upgrade_conn = NULL;
}

/******************************************************************************
 * FunctionName : upgrade_datasent
 * Description  : Data has been sent successfully,This means that more data can
 *                be sent.
 * Parameters   : arg -- Additional argument to pass to the callback function
 * Returns      : none
*******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
upgrade_datasent(void *arg){
    struct espconn *pespconn = arg;

    if (pespconn ->state == ESPCONN_CONNECT) {
    }
}

/******************************************************************************
 * FunctionName : upgrade_deinit
 * Description  : disconnect the connection with the host
 * Parameters   : bin -- server number
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
LOCAL upgrade_deinit(void){
    if (system_upgrade_flag_check() != UPGRADE_FLAG_START) {
        system_upgrade_deinit();
    }
}

//连接服务器超时回调
LOCAL void ICACHE_FLASH_ATTR
upgrade_connect_timeout_cb(struct espconn *pespconn){
    struct upgrade_server_info *server;

	DEBUG("upgrade_connect_timeout_cb\n");
    if (pespconn == NULL) {
        return;
    }
    server = pespconn->reverse;
    os_timer_disarm(&upgrade_timer);
    system_upgrade_flag_set(UPGRADE_FLAG_IDLE);
    server->upgrade_flag = false;
    if (server->check_cb != NULL) {
        server->check_cb(server);
    }
    system_upgrade_deinit();
    os_free(pespconn->proto.tcp);
    pespconn->proto.tcp = NULL;
    os_free(pespconn);
    pespconn = NULL;
    upgrade_conn = NULL;
}

//下载结果检查
LOCAL void ICACHE_FLASH_ATTR
upgrade_check(struct upgrade_server_info *server){
	DEBUG("upgrade_check\n");
    if (server == NULL) {
        return;
    }

    os_timer_disarm(&upgrade_timer);
    if (system_upgrade_flag_check() != UPGRADE_FLAG_FINISH) {
    	totallength = 0;
    	sumlength = 0;
        server->upgrade_flag = false;
        system_upgrade_flag_set(UPGRADE_FLAG_IDLE);
    } else {
        server->upgrade_flag = true;
        os_timer_disarm(&upgrade_timer);
    }
    upgrade_deinit();

    if (server->check_cb != NULL) {
        server->check_cb(server);
    }
    espconn_disconnect(upgrade_conn);
}

/******************************************************************************
 * FunctionName : upgrade_download
 * Description  : Processing the upgrade data from the host
 * Parameters   : bin -- server number
 *                pusrdata -- The upgrade data (or NULL when the connection has been closed!)
 *                length -- The length of upgrade data
 * Returns      : none
*******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
upgrade_download(void *arg, char *pusrdata, unsigned short length){
    char *ptr = NULL;
    char *ptmp2 = NULL;
    char lengthbuffer[32], returncode[4];
    uint8_t md5_calc[16],i = 0,progress = 0;
    char output[64] = {0};
    struct upgrade_server_info *server = (struct upgrade_server_info *)upgrade_conn->reverse;
    uint32_t  count;

    //检查返回码
    if (totallength == 0){
        DEBUG("httpdata:%s\n", pusrdata);
        ptr = (char *)os_strstr(pusrdata, "HTTP/1.1 ");
        os_memset(returncode, 0, sizeof(returncode));
        os_memcpy(returncode, ptr+9, 3);

        if(os_strcmp(returncode ,"200")){ //下载失败
            DEBUG("http download return code  error\n");
            upgrade_check(server);
            return;
        }
        else
        {
            os_memset(output, 0, sizeof(output));
            if (ONLINE_STM32_APP_FILE == filetype) {
                os_sprintf(output,"%s:2\r\n", CMD_DOWN_FILE); //正在下载
                at_port_print(output);
                at_response_ok();
            }
            else if (ESP8266_APP_FILE == filetype) {
                os_sprintf(output,"%s:2\r\n", CMD_NET_DOWN); //正在下载
                at_port_print(output);
                at_response_ok();
            }
        }
    }

    if (totallength == 0 && (ptr = (char *)os_strstr(pusrdata, "\r\n\r\n")) != NULL &&
            (ptr = (char *)os_strstr(pusrdata, "Content-Length")) != NULL) {
        ptr = (char *)os_strstr(pusrdata, "\r\n\r\n");
        length -= ptr - pusrdata;
        length -= 4;
        totallength += length;
        DEBUG("upgrade file download start.\n");
        MD5Init(&_ctx);
        MD5Update(&_ctx, ptr + 4, length);
        system_upgrade(ptr + 4, length);
        ptr = (char *)os_strstr(pusrdata, "Content-Length: ");

        if (ptr != NULL) {
            ptr += 16;
            ptmp2 = (char *)os_strstr(ptr, "\r\n");

            if (ptmp2 != NULL) {
                os_memset(lengthbuffer, 0, sizeof(lengthbuffer));
                os_memcpy(lengthbuffer, ptr, ptmp2 - ptr);
                sumlength = atoi(lengthbuffer);
                uint32_t  limit_size = 0;
                if (ONLINE_STM32_APP_FILE == filetype) {
                    limit_size = CACHE_ONLINE_STM32_APP_SEC_NUM * SPI_FLASH_SEC_SIZE;
                }
                else if (ESP8266_APP_FILE == filetype) {
                    limit_size = CACHE_ESP8266_APP_SEC_NUM * SPI_FLASH_SEC_SIZE;
                }
                else if (NEUTRON_BOOT_FILE == filetype) {
                    limit_size = CACHE_NEUTRON_BOOT_SEC_NUM * SPI_FLASH_SEC_SIZE;
                }
                else if (DEFAULT_STM32_APP_FILE == filetype) {
                    limit_size = CACHE_DEF_STM32_APP_SEC_NUM * SPI_FLASH_SEC_SIZE;
                }

                if(sumlength >= limit_size){
                    DEBUG("sumlength failed\n");
                    upgrade_check(server);
                    return;
                }
                //获取文件MD5码
                ptr = (char *)os_strstr(pusrdata, "X-Md5: ");
                if (ptr != NULL) {
                    ptr += 7;
                    ptmp2 = (char *)os_strstr(ptr, "\r\n");

                    if (ptmp2 != NULL) {
                        os_memset(server->md5, 0, sizeof(server->md5));
                        os_memcpy(server->md5, ptr, ptmp2 - ptr);
                    } else {
                        DEBUG("X-Mdt failed\n");
                        upgrade_check(server);
                        return;
                    }
                }
            } else {
                DEBUG("sumlength failed\n");
                upgrade_check(server);
                return;
            }
        } else {
            upgrade_check(server);
            DEBUG("Content-Length: failed\n");
            return;
        }
    } else {
        if(totallength + length > sumlength)
        {length = sumlength - totallength;}
        totallength += length;
        DEBUG("totallen = %d\n",totallength);
        MD5Update(&_ctx, pusrdata, length);
        system_upgrade(pusrdata, length);
    }

    if (ONLINE_STM32_APP_FILE == filetype) {
        down_progress = totallength*100/sumlength;
    }
    else if (ESP8266_APP_FILE == filetype) { //下载完毕进度50
        down_progress = totallength*50/sumlength;
    }
    else if (NEUTRON_BOOT_FILE == filetype) { //下载完毕进度60
        down_progress = 50+totallength*10/sumlength;
    }
    else if (DEFAULT_STM32_APP_FILE == filetype) { //下载完毕进度100
        down_progress = 60+totallength*40/sumlength;
    }

    if ((totallength == sumlength)) {
        DEBUG("upgrade file download finished.\n");
        MD5Final(md5_calc, &_ctx);
        os_memset(output, 0, sizeof(output));
        for(i = 0; i < 16; i++)
        {
            os_sprintf(output + (i * 2), "%02x", md5_calc[i]);
        }
        DEBUG("md5 = %s\n",output);
        DEBUG("server->md5 = %s\n",server->md5);
        if(!os_strcmp(server->md5,output)){
            DEBUG("md5 check ok.\n");
            system_upgrade_flag_set(UPGRADE_FLAG_FINISH);
            if (ONLINE_STM32_APP_FILE == filetype) {
                cmd_info.online_stm32_app_size=sumlength;
            }
            else if (ESP8266_APP_FILE == filetype) {
                cmd_info.esp8266_app_size = sumlength;
                cmd_info.online_stm32_app_size = 0;
            }
            else if (NEUTRON_BOOT_FILE == filetype) {
                cmd_info.neutron_boot_size = sumlength;
            }
            else if (DEFAULT_STM32_APP_FILE == filetype) {
                cmd_info.default_stm32_app_size = sumlength;
                cmd_info.esp8266_app_addr[0] = CACHE_ESP8266_APP_ADDR;
                cmd_info.esp8266_app_addr[1] = ESP8266_APP_ADDR;
                cmd_info.default_stm32_app_addr[0] = CACHE_DEF_STM32_APP_ADDR;
                cmd_info.default_stm32_app_addr[1] = DEF_STM32_APP_ADDR;
                cmd_info.action = ACTION_COPY_RAW;
            }
            else {
            }
            eboot_command_write(&cmd_info);
            totallength = 0;
            sumlength = 0;
            upgrade_check(server);
            return;
        }
        DEBUG("md5 check error.\n");
        upgrade_check(server);
        return;
    }

    if (upgrade_conn->state != ESPCONN_READ) {
        totallength = 0;
        sumlength = 0;
        os_timer_disarm(&upgrade_rev_timer);
        os_timer_setfn(&upgrade_rev_timer, (os_timer_func_t *)upgrade_check, server);
        os_timer_arm(&upgrade_rev_timer, 10, 0);
    }
}

/******************************************************************************
 * FunctionName : upgrade_connect
 * Description  : client connected with a host successfully
 * Parameters   : arg -- Additional argument to pass to the callback function
 * Returns      : none
*******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
upgrade_connect_cb(void *arg){
    struct espconn *pespconn = arg;

    DEBUG("upgrade_connect_cb\n");
    os_timer_disarm(&upgrade_connect_timer);

    espconn_regist_disconcb(pespconn, upgrade_disconcb);
    espconn_regist_sentcb(pespconn, upgrade_datasent);

    if (pbuf != NULL) {
        DEBUG("%s\n", pbuf);
        espconn_sent(pespconn, pbuf, os_strlen(pbuf));
    }
}

/******************************************************************************
 * FunctionName : upgrade_connection
 * Description  : connect with a server
 * Parameters   : bin -- server number
 *                url -- the url whitch upgrade files saved
 * Returns      : none
*******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
upgrade_connect(struct upgrade_server_info *server){
	DEBUG("upgrade_connect\n");

    pbuf = server->url;
    espconn_regist_connectcb(upgrade_conn, upgrade_connect_cb);
    espconn_regist_recvcb(upgrade_conn, upgrade_download);

    system_upgrade_init();
    system_upgrade_flag_set(UPGRADE_FLAG_START);

    espconn_connect(upgrade_conn);

    os_timer_disarm(&upgrade_connect_timer);
    os_timer_setfn(&upgrade_connect_timer, (os_timer_func_t *)upgrade_connect_timeout_cb, upgrade_conn);
    os_timer_arm(&upgrade_connect_timer, 5000, 0);
}

/******************************************************************************
 * FunctionName : user_upgrade_init
 * Description  : parameter initialize as a client
 * Parameters   : server -- A point to a server parmer which connected
 * Returns      : none
*******************************************************************************/
bool ICACHE_FLASH_ATTR
system_upgrade_start(struct upgrade_server_info *server){

    if (system_upgrade_flag_check() == UPGRADE_FLAG_START) {
        return false;
    }
    if (server == NULL) {
    	DEBUG("server is NULL\n");
    	return false;
    }
    if (upgrade_conn == NULL) {
        upgrade_conn = (struct espconn *)os_zalloc(sizeof(struct espconn));
    }

    if (upgrade_conn != NULL) {
        upgrade_conn->proto.tcp = NULL;
        upgrade_conn->type = ESPCONN_TCP;
        upgrade_conn->state = ESPCONN_NONE;
        upgrade_conn->reverse = server;

        if (upgrade_conn->proto.tcp == NULL) {
            upgrade_conn->proto.tcp = (esp_tcp *)os_zalloc(sizeof(esp_tcp));
        }

        if (upgrade_conn->proto.tcp != NULL) {
            upgrade_conn->proto.tcp->local_port = espconn_port();
            upgrade_conn->proto.tcp->remote_port = server->port;

            os_memcpy(upgrade_conn->proto.tcp->remote_ip, server->ip, 4);

            DEBUG("%s\n", __func__);
            upgrade_connect(server);

            if (server->check_cb !=  NULL) {
                os_timer_disarm(&upgrade_timer);
                os_timer_setfn(&upgrade_timer, (os_timer_func_t *)upgrade_check, server);
                os_timer_arm(&upgrade_timer, server->check_times, 0);
            }
        }
    }
    return true;
}

