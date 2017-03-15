#
# 定义xtensa编译工具相关参数
#

# 定义编译器和工具的前缀
GCC_ARM_PATH ?= $(PROJECT_ROOT)/tools/xtensa-esp32-elf/bin/
GCC_PREFIX ?= xtensa-esp32-elf-

include $(COMMON_BUILD)/common-tools.mk

CDEFINES += -DESP_PLATFORM -DMBEDTLS_CONFIG_FILE='"mbedtls/esp_config.h"' -DHAVE_CONFIG_H -DESP32 -DF_CPU=240000000L

ifneq ("$(MODULE)","bootloader")
CFLAGS += -g3 -Os
else
CFLAGS += -ggdb -Og
CDEFINES += -DBOOTLOADER_BUILD=1 -DIDF_VER=\"v2.0-rc1-65-g34d0b00\"
endif

CFLAGS += -w -Wpointer-arith -Wno-error=unused-function -Wno-error=unused-but-set-variable -Wno-error=unused-variable -Wfatal-errors -Wno-error=deprecated-declarations -Wno-unused-parameter -Wno-sign-compare -ffunction-sections -fdata-sections -mlongcalls -nostdlib -fstrict-volatile-bitfields -MMD

CONLYFLAGS += -std=gnu99 -Wno-old-style-declaration

# C++ 编译参数  -fpermissive 把代码的语法错误作为警告,并继续编译进程
CPPFLAGS += -fno-exceptions -fno-rtti -std=gnu++11 -fpermissive

ASFLAGS += -g3 -x assembler-with-cpp -MMD -mlongcalls

#FLASH_SIZE ?= 4MB
#FLASH_MODE ?= qio
#FLASH_SPEED ?= 40m

FLASH_SIZE ?= 4MB
FLASH_MODE ?= dio
FLASH_SPEED ?= 80m


# UPLOAD_SPEED ?= 230400
UPLOAD_SPEED ?= 921600

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
UPLOAD_PORT ?= /dev/ttyUSB0
SUDO ?= sudo
endif

ifeq ($(UNAME_S),Darwin)
#UPLOAD_PORT ?= /dev/cu.usbmodem1411
UPLOAD_PORT ?= /dev/cu.SLAB_USBtoUART
endif

