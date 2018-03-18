# 设备固件库

## 项目背景

随着物联网的飞速发展，物联网硬件方案层出不穷。开发者选择硬件方案后，通常需要对该硬件方案进行详细的了解，包括硬件资源，硬件框架，底层驱动，开发环境等等。开发者在开发过程中，通常会碰到如下问题：

* 开发项目之前需要花很长时间熟悉硬件平台，熟悉硬件开发环境。
* 嵌入式硬件一般牵扯到寄存器操作，甚至存在很多特殊性，这通常要求开发者具备一定实践经验才能开发出稳定的产品。
* 碰到问题，资料少，难以解决问题。
* 程序难以却换到其他硬件平台。

这些问题通常导致开发者开发项目周期长，碰到的问题多，产品不稳定。

## 项目介绍

![](/docs/images/hardware-solution-master.png)

根据市场需求，IntoYun启动了该项目，致力于解决物联网设备开发难，开发慢的问题。

该项目采取了`Arduino`编程方式(Arduino采取C/C++编程方式，把常用的功能和硬件接口封装成统一且通俗易懂的SDK接口，在软件执行效率和编程方式与常规C/C++开发无差别，非常适合用于开发项目)，具备如下特点：

* 向用户封装屏蔽硬件方案，提供统一的SDK接口。SDK接口基本覆盖硬件资源操作，保证所有硬件方案，编程SDK接口一致。用户开发过程中不需要了解具体的硬件方案。
* 向用户提供统一的在线离线开发环境，支持不同的硬件方案，支持跨平台(MacOS、Windows、Linux)开发。
* 提出公共库的概念，公共库是居于SDK之上开发的功能库。通过库的完善，能显著降低开发难度和加快开发速度。

通过固件库项目、开发环境、库资源等。开发者不需要对硬件方案深入了解，可以在极短的时间内开发完项目，极大地降低了开发成本和提升开发速度。

## 项目功能

设备固件库软件框架图如下：

![架构框图](/docs/images/firmware-architecture.png)

向用户提供了如下功能：

* 提供设备bootloader。bootloader实现了用户程序的启动和升级功能。
* 提供了操作硬件资源的统一接口，包括GPIO、SPI、串口、USB口、I2C、ADC、DAC、PWM、中断、定时器等。
* 内置与IntoYun平台通讯交互接口。支持自定义和数据点数据格式。
* 内置常用功能接口。例如WiFi设备，支持TCP、UDP、HTTP、MQTT等接口。
* 支持公共库开发和添加。

## 适用设备

<table>
<thead>
<tr> <th style="width:20%;">硬件方案 </th> <th style="width:60%;">开发板/模组 </th> <th style="width:40%;">通讯方案 </th> </tr>
</thead>
<tbody>
<tr> <td rowspan="2">ESP8266</td> <td>IntoRobot-Nut</td> <td rowspan="2">WiFi</td> </td> </tr>
<tr> <td>IntoRobot-W6 / IntoRobot-W7</td> </tr>
<tr> <td rowspan="2">ESP32</td> <td>IntoRobot-Fig</td> <td rowspan="2">WiFi</td> </td> </tr>
<tr> <td>IntoRobot-W32 / IntoRobot-W33</td> </tr>
<tr> <td rowspan="1">STM32 + GPRS</td> <td>IntoRobot-Fox</td> <td rowspan="1">GPRS</td> </td> </tr>
<tr> <td rowspan="2">STM32 + LoRa</td> <td>IntoRobot-Ant</td> <td rowspan="2">Lora</td> </td> </tr>
<tr> <td>IntoRobot-L6</td> </tr>
</tbody>
</table>

# 开发指南

- [开发环境搭建](docs/gettingstarted.md)
- [应用开发教程](docs/gettingstarted.md)
- [固件库API接口](docs/gettingstarted.md)

# 更新记录

- [最新更新](http://github.com/intoyun/firmware/releases/)
- [更新日志](CHANGELOG.md)

# 资源列表

- [neutron stm32+esp8266 通讯AT扩展指令](docs/gettingstarted.md)
- [MolmcBridge工具使用说明](http://docs.intoyun.com/devicedev/develop-tools/molmc-bridge-manual/)
- [在线编程开发工具](http://docs.intoyun.com/devicedev/develop-tools/online-ide-user-manual/ant/)
- [离线编程开发工具](http://docs.intoyun.com/devicedev/develop-tools/offline-ide-user-manual/)



