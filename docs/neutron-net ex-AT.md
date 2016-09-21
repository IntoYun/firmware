# Neutron-NET 扩展AT指令
neutron-net 扩展AT指令 是在esp8266 AT指令的基础上添加的，涉及在线编程程序，esp8266程序，neutron默认程序，neutron bootloader程序等的下载和升级。


## 1. 获取NET版本信息
 **指令：**  `AT+IR_GETVERSION?`

 **说明：**  获取esp8266端程序版本号

 **返回：**

    +IR_GETVERSION:版本号

    OK
    版本号说明：版本号前3位为esp8266的版本号、最后面一位为stm32 bootloader版本号。 版本号个位为10进制。

 **实例：**

    AT+IR_GETVERSION?

    +IR_GETVERSION:1.0.0.0
    OK


## 2. 获取文件下载进度
 **指令：**  `AT+IR_DOWNPROGRESS?`

 **说明：**  获取文件下载的进度。包括在线编程程序、esp8266程序。

 **返回：**

    +IR_DOWNPROGRESS:进度

    OK

 **实例：**

    AT+IR_DOWNPROGRESS?

    +IR_DOWNPROGRESS:进度

    OK

## 3. 下载在线编程程序
 **指令：**  `AT+IR_DOWNFILE = "域名或者IP地址"，“下载地址”，“文件md5码”`

 **说明：**  下载在线编程程序

 **返回：**

    +IR_DOWNFILE:结果

    OK
    结果说明：0:下载成功、1:下载失败、2:正在下载

 **实例：**

    AT+IR_DOWNFILE="112.124.117.64","/v1/bins?dwn_token=fec38b214cd126efbd2f68cea7ab879a","4255357ef8cfa75567226417b113582f"

    +IR_DOWNFILE:2

    OK
    +IR_DOWNFILE:0

    OK

## 4. 获取应用程序大小
 **指令：**  `AT+IR_GETFILESIZE=数据包大小，文件类型`

 **说明：**  获取指定应用程序大小。文件类型说明：0：在线编程程序  1：stm32默认程序 2：stm32 bootloader程序

 **返回：**

    +IR_GETFILESIZE:数据包个数

    OK

 **实例：**

    AT+IR_GETFILESIZE=16384,1

    +IR_GETFILESIZE:14

    OK

## 5. 获取应用程序指定数据包
 **指令：**  `AT+IR_GETFILEPACKET=数据包大小，文件类型，数据包索引`

 **说明：**  获取指定应用程序数据包。

    文件类型说明：0：在线编程程序  1：stm32默认程序 2：stm32 bootloader程序
    数据包索引：从0开始

 **返回：**

    +IR_GETFILEPACKET,16386:数据

 **实例：**

    AT+IR_GETFILEPACKET=16384,0,1

    +IR_GETFILEPACKET,16388:数据

## 6. 下载net固件程序
 **指令：**  `AT+IR_NETDOWN="域名或者IP地址"，“下载地址”`

 **说明：**  下载net固件程序。包括esp8266程序、stm32默认程序、stm32 bootloader程序

 **返回：**

    +IR_NETDOWN:结果

    OK
    结果说明：0:下载成功、1:下载失败、2:正在下载

 **实例：**

    AT+IR_NETDOWN="112.124.117.64","/downloads/neutron/1.0.0"

    +IR_NETDOWN:2

    OK
    +IR_NETDOWN:0

    OK

## 7. 升级net固件程序
 **指令：**  `AT+IR_NETUPDATE`

 **说明：**  升级net固件程序。包括esp8266程序、stm32默认程序。

 **返回：**

    OK

 **实例：**

    AT+IR_NETUPDATE

    OK


# 常用AT指令总结

## 1. 设置esp8266为station模式
AT+CWMODE_DEF=1
## 2. ping
AT+PING="www.intorobot.com"
## 3. 域名解析
AT+CIPDOMAIN="www.intorobot.com"
## 4. 关闭和打开反显
ATE0
ATE1


