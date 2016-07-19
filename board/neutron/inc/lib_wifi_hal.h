/*
只包含对外接口、类型
*/

#ifndef   LIB_WIFI_HAL_H_
#define   LIB_WIFI_HAL_H_


#include <stdint.h>


//重启wifi   wifi reload
void mo_WiFiClass_connect_hal(void);


//是否连接上互联网   ping 114    是   1   否 0

int8_t mo_WiFiClass_status_hal(void);

//重启wifi
void mo_WiFiClass_restartNetwork_hal(void);

//配置wifi    成功0   失败1
uint8_t mo_WiFiClass_setCredentials_hal(const char *ssid, const char *password, const char *channel, const char *security);

//设置IP地址
void mo_WiFiClass_config_hal1(const char *local_ip);

//set ip and dns
void mo_WiFiClass_config_hal2(const char *local_ip, const char *dns_server);

//set ip dns gateway
void mo_WiFiClass_config_hal3(const char *local_ip, const char *dns_server, const char *gateway);


void mo_WiFiClass_config_hal4(const char *local_ip, const char *dns_server, const char *gateway, const char *subnet);

//set dns
void mo_WiFiClass_setDNS_hal1(const char *dns_server1);

void mo_WiFiClass_setDNS_hal2(const char *dns_server1, const char *dns_server2);

//get mac
uint8_t *mo_WiFiClass_macAddress_hal(uint8_t* mac);

//get ip
void mo_WiFiClass_localIP_hal(int *p_ip);


//get netmask
void mo_WiFiClass_subnetMask_hal(int *p_net_mask);



void mo_WiFiClass_gatewayIP_hal(int *p_gate_way);

//获取字符串的ssid  长度不超过32
void mo_WiFiClass_SSID_hal(char *p_ssid);


//获取bssid
void mo_WiFiClass_BSSID(uint8_t* bssid);


//获取当前wifi信号强度

int32_t mo_WiFiClass_RSSI_hal(void);


//获取当前加密方式

uint8_t mo_WiFiClass_encryptionType_hal(void);


//aps到的wifi热点数量

uint8_t mo_WiFiClass_scanNetworks_hal(void);

//获取第n个ssid   ssid长度小于32

void mo_WiFiClass_SSID_hal(unsigned char networkItem,char *ret_but);

//获取第n个 bssid
void mo_WiFiClass_BSSID_hal(unsigned char networkItem, unsigned char *bssid);


//获取第n个信号强度
int32_t mo_WiFiClass_RSSI_hal(uint8_t networkItem);



uint8_t mo_WiFiClass_encryptionType(uint8_t networkItem);

// host---->ip   succeed 1  failed 0
uint8_t mo_WiFiClass_hostByName_hal(const char* aHostname,int *ret_buf);



//ping s 1   f 0            尝试次数

//typedef char int8_t;

uint8_t mo_WiFiClass_ping_hal(const char *remoteIP, uint8_t nTries);

uint8_t mo_set_mac_hal(const char *staMac, const char *apMac);


void mo_get_mac_hal(char *pstamac, char *papmac);
void mo_get_ssi_hal(char *pssid, char *pbssid, int &channel, int &rssi);

#endif
