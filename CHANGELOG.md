## v1.5.4-rc.2


### FEATURES(功能)


### ENHANCEMENTS(优化)
- [[PR #75]](https://github.com/HITSZ-NRSL/firmware/pull/75) [nut/fig/w67/w323] esp8266 esp32 添加u8等定义
- [[PR #75]](https://github.com/HITSZ-NRSL/firmware/pull/75) [nut/w67] esp8266移植sdk2.0后添加支持servo功能


### BUG FIXES(漏洞修复)
- [[PR #75]](https://github.com/HITSZ-NRSL/firmware/pull/75) [wifi/gprs] 修正IntoRobot.Publish()发送错误。版本号当做了字符串长度，导致每次只发一个数据
- [[PR #75]](https://github.com/HITSZ-NRSL/firmware/pull/75) [nut/w67] 修正esp8266 AD读值bug 之前乘以2倍


### INTERNAL(内部)


## v1.5.4-rc.1


### FEATURES(功能)


### ENHANCEMENTS(优化)
- [[PR #63]](https://github.com/HITSZ-NRSL/firmware/pull/63) [neutron/nut/fig/ant/fox] 简化设备bootloader和app设备配置模式
- [[PR #63]](https://github.com/HITSZ-NRSL/firmware/pull/63) [neutron/nut/fig/ant/fox] 简化设备bootloader颜色变化
- [[PR #63]](https://github.com/HITSZ-NRSL/firmware/pull/63) [neutron/nut/fig/w67/w323] 添加imlink失败重新进入imlink配置功能
- [[PR #65]](https://github.com/HITSZ-NRSL/firmware/pull/65) [nut/w67] esp8266 sdk更新至2.0
- [[PR #67]](https://github.com/HITSZ-NRSL/firmware/pull/67) [neutron] 去掉neutron modem AT指令解析函数延时，加快平台控制响应
- [[PR #67]](https://github.com/HITSZ-NRSL/firmware/pull/67) [neutron] 优化neutron wlan_setup流程，区别设备进入配置模式再手动退出配置模式，能够正常联网.


### BUG FIXES(漏洞修复)
- [[PR #63]](https://github.com/HITSZ-NRSL/firmware/pull/62) [neutron/nut/fig/w67/w323] 修正tcp stop没有清除缓存，导致连接状态没有变化
- [[PR #65]](https://github.com/HITSZ-NRSL/firmware/pull/65) [nut/w67] 修正串口下载后应用运行软复位无法重启的问题。解决方法:下载后硬件复位模块。


### INTERNAL(内部)
- [[PR #62]](https://github.com/HITSZ-NRSL/firmware/pull/62) [all] 添加独立工程编译
- [[PR #62]](https://github.com/HITSZ-NRSL/firmware/pull/62) [all] 修改编译目标文件名称
- [[PR #63]](https://github.com/HITSZ-NRSL/firmware/pull/63) [all] 子系统版本版本统一放入build/subsys_version.mk统一管理
- [[PR #67]](https://github.com/HITSZ-NRSL/firmware/pull/67) [all] dfu-util upload-reset放入tools文件夹 修正dfu-util下载成功提示失败问题
- [[PR #68]](https://github.com/HITSZ-NRSL/firmware/pull/68) [L6/Ant] 变更LoRaWan驱动库



## v1.5.3-rc.2

### FEATURES(功能)

### ENHANCEMENTS(优化)
- [[PR #60]](https://github.com/HITSZ-NRSL/firmware/pull/60) [all] 字符串和透传型数据点定义添加最大长度
- [[PR #60]](https://github.com/HITSZ-NRSL/firmware/pull/60) [all] 去除数据点功能开关

### BUG FIXES(漏洞修复)
- [[PR #59]](https://github.com/HITSZ-NRSL/firmware/pull/59) [fig] 修复fig第一次上电配置完成后无法转白灯问题
- [[PR #61]](https://github.com/HITSZ-NRSL/firmware/pull/61) [all] 添加默认数据点处理。包括复位和获取全部可上送数据点


### INTERNAL(内部)



## v1.5.3-rc.1

### FEATURES(功能)
- [[PR #58]](https://github.com/HITSZ-NRSL/firmware/pull/58) [all] 数据点组织格式统一使用二进制方式
- [[PR #58]](https://github.com/HITSZ-NRSL/firmware/pull/58) [all] 添加事件通知接口
- [[PR #58]](https://github.com/HITSZ-NRSL/firmware/pull/58) [all] 数据点发送添加系统自动处理方式

### ENHANCEMENTS(优化)
- [[PR #58]](https://github.com/HITSZ-NRSL/firmware/pull/58) [nut/w6/w7] esp8266 process 放入到system层实现


### BUG FIXES(漏洞修复)
- [[PR #58]](https://github.com/HITSZ-NRSL/firmware/pull/58) [wifi] 修正恢复出厂设置后，设备无法注册和激活, http地址错误
- [[PR #58]](https://github.com/HITSZ-NRSL/firmware/pull/58) [nut/w6/w7] 修正nut下载软件包后，通过串口配置wifi无法成功


### INTERNAL(内部)


## v1.5.2-rc.1

### FEATURES(功能)
- [[PR #51]](https://github.com/HITSZ-NRSL/firmware/pull/51) [ant] 添加ant的支持
- [[PR #52]](https://github.com/HITSZ-NRSL/firmware/pull/52) [ant] fig支持pwm功能
- [[PR #54]](https://github.com/HITSZ-NRSL/firmware/pull/54) [fox] 添加fox支持,包括bootloader和应用程序


### ENHANCEMENTS(优化)
- [[PR #51]](https://github.com/HITSZ-NRSL/firmware/pull/51) [all] 优化IntoRobot 数据点处理接口
- [[PR #53]](https://github.com/HITSZ-NRSL/firmware/pull/53) [all] IntoRobot 数据点接口从DataPoint改成Datapoint
- [[PR #54]](https://github.com/HITSZ-NRSL/firmware/pull/54) [neutron/nut/fig/w67/w323] 获取网络状态采取通知机制


### BUG FIXES(漏洞修复)
- [[PR #54]](https://github.com/HITSZ-NRSL/firmware/pull/55) [all] STM32L151xB 芯片 ajson print申请变量大小改成512个字节
- [[PR #57]](https://github.com/HITSZ-NRSL/firmware/pull/56) [all] 修正调用IntoRobot.printf()函数编译出错，原因在于基类的继承属性


### INTERNAL(内部)
- [[PR #54]](https://github.com/HITSZ-NRSL/firmware/pull/54) [all] PRODUCT_ID 默认值修改成空, PRODUCT_SECRET 默认值修改成空, PRODUCT_VERSION 默认值修改成1
- [[PR #57]](https://github.com/HITSZ-NRSL/firmware/pull/55) [all] intorobot上送版本信息添加board字段



## v1.5.1-rc.1

### FEATURES(功能)
- [[PR #45]](https://github.com/HITSZ-NRSL/firmware/pull/45) [w67/w323] 发布w67/w322
- [[PR #48]](https://github.com/HITSZ-NRSL/firmware/pull/48) [fig] fig bootloader添加模式处理
- [[PR #49]](https://github.com/HITSZ-NRSL/firmware/pull/49) [fig] fig 支持SPI I2C AD功能


### ENHANCEMENTS(优化)


### BUG FIXES(漏洞修复)
- [[PR #45]](https://github.com/HITSZ-NRSL/firmware/pull/45) [fig] 修正fig管教映射
- [[PR #47]](https://github.com/HITSZ-NRSL/firmware/pull/47) [neutron] 修正neutron 运行过程中死机问题, 该问题与freeRtos中断优先级设置有关系.
- [[PR #50]](https://github.com/HITSZ-NRSL/firmware/pull/50) [neutron] neutron bootloader 版本升级至v2版本 修正esp8266在不同操作系统升级失败问题。通过USB选择波特率来置esp8266串口通讯波特率。


### INTERNAL(内部)
- [[PR #45]](https://github.com/HITSZ-NRSL/firmware/pull/45) [all] i2c i2s spi usartserial usbserial 为了防止其他类构造函数调用，统一采取函数方式实现
- [[PR #46]](https://github.com/HITSZ-NRSL/firmware/pull/46) [fig] fig AD功能调试OK，引脚读写功能OK
- [[PR #50]](https://github.com/HITSZ-NRSL/firmware/pull/50) [neutron] 更新neutron子系统版本至2.0.0.2
- [[PR #50]](https://github.com/HITSZ-NRSL/firmware/pull/50) [neutron] 修改neutron的USB中断优先级为2，防止插拔USB阻止esp8266串口接收数据



## v1.5.0-rc.2

### FEATURES(功能)


### ENHANCEMENTS(优化)
- [[PR #41]](https://github.com/HITSZ-NRSL/firmware/pull/41) [neutron] neutron添加单任务和多任务开关。默认单任务。

### BUG FIXES(漏洞修复)
- [[PR #41]](https://github.com/HITSZ-NRSL/firmware/pull/41) [neutron] 因为neutron需要实时接收esp8266数据，所有不允许关闭总中断。ext_hal.c 里面的开启和关闭中断，只能处理用户中断.
- [[PR #42]](https://github.com/HITSZ-NRSL/firmware/pull/42) [nut] 修改nut jsonGetValue()重启bug

### INTERNAL(内部)


## v1.5.0-rc.1

### FEATURES(功能)
- [[PR #28]](https://github.com/HITSZ-NRSL/firmware/pull/28) [fig] 1.添加fig/w32/w33 bootloader和应用
- [[PR #28]](https://github.com/HITSZ-NRSL/firmware/pull/28) [neutron/nut/fig] neutron/nut/fig 支持子系统版本升级

### ENHANCEMENTS(优化)


### BUG FIXES(漏洞修复)
- [[PR #26]](https://github.com/HITSZ-NRSL/firmware/pull/26) [nut] 解决nut编译出现iram1_0_seg空间不足.
- [[PR #27]](https://github.com/HITSZ-NRSL/firmware/pull/27) [all] 1.解决usb串口配置，传输大数据指令无效. 2.减少配置等待时间.
- [[PR #34]](https://github.com/HITSZ-NRSL/firmware/pull/34) [all] 修正bug, 如果imlink配置成功，在进入配置模式配置，无法配置成功
- [[PR #35]](https://github.com/HITSZ-NRSL/firmware/pull/35) [nut] 修改nut tcp重连重启bug
- [[PR #35]](https://github.com/HITSZ-NRSL/firmware/pull/35) [nut/fig] 修改fig/nut/w323/w67 bootloader ota升级bug, 如果ota大小为0, 进入ota升级将陷入死循环

### INTERNAL(内部)
- [[PR #35]](https://github.com/HITSZ-NRSL/firmware/pull/35) [all] 去掉sv_select相关处理



## v1.4.3-rc.2

### FEATURES(功能)


### ENHANCEMENTS(优化)
- [[PR #19]](https://github.com/HITSZ-NRSL/firmware/pull/19) [neutron/nut] WiFi 增加status() 函数以兼容之前的固件


### BUG FIXES(漏洞修复)
- [[PR #22]](https://github.com/HITSZ-NRSL/firmware/pull/22) [neutron/nut/pwm] 解决 neutorn和nut的pwm 分辨率不能到16bit问题,解决pwm分辨率保存时互相错乱问题
- [[PR #19]](https://github.com/HITSZ-NRSL/firmware/pull/19) [neutron/pwm] 解决 neutorn的pwm 分辨率不可调问题


### INTERNAL(内部)



## v1.4.3-rc.1

### FEATURES(功能)

- [neutron]新固件添加对neutron的支持
- [neutron]neutron添加获取wifi列表功能
- [nut]nut添加获取wifi列表功能
- [neutron/nut]添加用户与云端通用的接口，支持IntoYun服务器
- [neutron/nut]添加IntoYun注册和激活功能。

### ENHANCEMENTS(优化)

- [neutorn]bootloader进入应用不初始化esp8266芯片，加快开机速度
- [all]去除固件所有警告信息
- [neutorn/nut]获取网络时间不从mqtt获取，直接通过网络ntp服务器获取

### BUG FIXES(漏洞修复)

- [nut]解决nut bootloader有时上电三色灯高亮的问题
- [neutorn]修正neutron运行一段时间后死机问题
- [neutorn]修正neutron运行一段时间后esp8266挂掉问题。neutron-net 更新至v2.0 sdk库
- [neutorn]neutron多任务处理，分为系统任务和应用任务，把应用任务对mqtt的处理和与modem通讯统一放入到system任务处理。以避免任务之间冲突

### INTERNAL(内部)


## v1.4.1

### FEATURES(功能)


### ENHANCEMENTS(优化)


### BUG FIXES(漏洞修复)


### INTERNAL(内部)


