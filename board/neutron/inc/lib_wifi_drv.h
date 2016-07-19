#ifndef __LIB_WIFI_DRV_H
#define __LIB_WIFI_DRV_H


void mo_drv_wifi_init();

int mo_drv_wifi_cmd_transfer(const char *p_cmd,int len_cmd,char *p_get,int len,unsigned int timeout,unsigned char lock_flag);

int mo_drv_wifi_creat_tcpc_fifo(unsigned char fifo_num);
int mo_drv_wifi_destroy_tcpc_fifo(unsigned char fifo_num);

int mo_drv_wifi_read_tcpc_fifo(char *p_buf,int len,unsigned char fifi_num);

void mo_drv_wifi_set_cond_flag(char dat);

int mo_drv_wifi_available_tcpc_fifo(unsigned char  fifo_num);

int mo_drv_wifi_read_tcpc_fifo(char *p_buf,int len,unsigned char fifi_num);

int mo_drv_wifi_set_default_mode();

int mo_drv_wifi_run_cmd(char *cmd,char *ret_cmd,unsigned int time_out);

int mo_uart1_read(char *p_buf,int len,int timeout);

int mo_uart1_write(const char *p_buf,int len);



#endif
