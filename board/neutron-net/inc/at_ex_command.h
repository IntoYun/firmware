#ifndef __AT_EX_COMMAND_H__
#define __AT_EX_COMMAND_H__



#define CMD_GETVERSION          "+IR_GETVERSION"       //获取wifi程序版本信息
#define CMD_DOWN_PROGRESS       "+IR_DOWNPROGRESS"     //获取文件下载进度
#define CMD_DOWN_FILE           "+IR_DOWNFILE"         //在线编程文件  wifi应用升级 默认应用程序
#define CMD_GET_FILE_SIZE       "+IR_GETFILESIZE"      //获取应用程序大小
#define CMD_GET_FILE_PACKET     "+IR_GETFILEPACKET"    //获取应用程序数据包
#define CMD_NET_DOWN            "+IR_NETDOWN"          //网络程序esp8266程序下载
#define CMD_NET_UPDATE          "+IR_NETUPDATE"        //网络程序esp8266程序升级

void ICACHE_FLASH_ATTR downFile(char *hostname, char *httppara, char *md5para, void *check_cb);
void ICACHE_FLASH_ATTR at_cmddownfile_rsp(void *arg);
void ICACHE_FLASH_ATTR at_queryCmdGetVersion(uint8_t id);
void ICACHE_FLASH_ATTR at_queryCmdGetDownProgress(uint8_t id);
void ICACHE_FLASH_ATTR at_setupCmdDownFile(uint8_t id, char *pPara);
void ICACHE_FLASH_ATTR at_setupCmdGetFileSize(uint8_t id, char *pPara);
void ICACHE_FLASH_ATTR at_setupCmdGetFilePacket(uint8_t id, char *pPara);
void ICACHE_FLASH_ATTR at_setupCmdNetDown(uint8_t id, char *pPara);
void ICACHE_FLASH_ATTR at_exeCmdNetUpdate(uint8_t id);


#endif
