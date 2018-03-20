## 项目介绍

项目详细介绍请点击[主模式SDK固件](http://docs.intoyun.com/opensource/firmware/)

## 架构框图

![image](/docs/images/firmware-architecture.png)

## 目录和文件组成
```
    +-- bootloader              : 实现产品的bootloader, bootloader中实现应用程序的引导，DFU功能实现，恢复默认固件，恢复出厂设置等功能。
    +-- build                   : 编译目录，里面包括编译的主要makefile，以及链接文件，编译的目标文件
    +-- communication           : 通讯协议部分
    +-- docs                    : 项目文档
    +-- hal                     : 硬件描述层接口实现
    +-- main                    : 编译烧录各个固件的上层文件夹
    +-- newlib_nano             : 主要用于操作系统内存管理保护
    +-- platform                : 主要放置产品芯片厂家提供的SDK库，包括ESP8266,ESP32,STM32F1xx, STM32F4xx, STM32L1xx等。
    +-- services                : 辅助接口
    +-- system                  : 提供连接网络，配置，连接IntoYun/IntoRobot平台，在线编程，固件更新等功能
    +-- tools                   : 编译和烧录工具 
    +-- user                    : 括各个产品的默认应用代码， 各种应用代码，外部库，以及自动测试代码
    +-- wiring                  : Arduino兼容功能接口， 主要通过调用hal层接口
    +-- wiring_ex               : 针对于特定核心板扩展的函数接口， 主要通过调用hal层接口
    
```
