#
# 定义xtensa编译工具相关参数
#

# 定义编译器和工具的前缀
GCC_ARM_PATH ?= $(PROJECT_ROOT)/tools/xtensa-esp32-elf/bin/
GCC_PREFIX ?= xtensa-esp32-elf-

include $(COMMON_BUILD)/common-tools.mk

CDEFINES += -DESP_PLATFORM -DMBEDTLS_CONFIG_FILE='"mbedtls/esp_config.h"' -DHAVE_CONFIG_H -DESP32

CFLAGS += -g3 -Os -w -Wpointer-arith -Wno-error=unused-function -Wno-error=unused-but-set-variable -Wno-error=unused-variable -Wfatal-errors -ffunction-sections -fdata-sections -mlongcalls -nostdlib -MMD -fstrict-volatile-bitfields

CONLYFLAGS += -std=gnu99

# C++ 编译参数  -fpermissive 把代码的语法错误作为警告,并继续编译进程
CPPFLAGS += -fno-exceptions -fno-rtti -std=gnu++11 -fpermissive

ASFLAGS += -g3 -x assembler-with-cpp -MMD -mlongcalls

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
UPLOAD_PORT ?= /dev/ttyUSB0
SUDO ?= sudo
endif

ifeq ($(UNAME_S),Darwin)
#UPLOAD_PORT ?= /dev/cu.usbmodem1411
UPLOAD_PORT ?= /dev/cu.SLAB_USBtoUART
endif

# UPLOAD_SPEED ?= 230400
UPLOAD_SPEED ?= 921600
FLASH_MODE ?= qio
FLASH_SPEED ?= 40m

