## 开发环境安装

目前支持Linux和MacOS开发。

### Linux安装(以Ubuntu为例)

#### 1. 安装python工具

```sh
> sudo apt-get install python
```

#### 1. 安装开发依赖工具

```sh
cd tools
./get.py
```

### MacOS安装

#### 1. 安装python工具

```sh
> brew install python
```

#### 1. 安装开发依赖工具

```sh
cd tools
./get.py
```

## 依赖工具说明

### 1. arm-none-eabi-gcc

IntoRobot系列产品采用了ARM Cortex M3/M4 CPU处理器，该工具为arm处理器的交叉编译工具。

### 2. xtensa-lx106

该工具为ESP8266交叉编译工具。

### 3. xtensa-esp32

该工具为ESP32交叉编译工具。

### 4. st-flash

该工具为ST ARM处理器烧录工具。

### 5. dfu-util

该工具为DFU(Device Firmware Upgrade)升级工具。

### 6. esptool8266

该工具为ESP8266芯片下载工具。

### 7. esptool32

该工具为ESP32芯片下载工具。
