## v1.4.3-rc.3

### FEATURES(功能)


### ENHANCEMENTS(优化)


### BUG FIXES(漏洞修复)
- [[PR #25]](https://github.com/HITSZ-NRSL/firmware/pull/25) [nut] 部分应用程序在nut上编译出错，提示空间不足，修改该问题


### INTERNAL(内部)




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


