#
# 定义xtensa编译工具相关参数
#

# 定义编译器和工具的前缀
ifeq ($(PLATFORM_ID), 7) # for fig
GCC_ARM_PATH ?= $(PROJECT_ROOT)/tools/xtensa-esp32-elf/bin/
GCC_PREFIX ?= xtensa-esp32-elf-

include $(COMMON_BUILD)/common-tools.mk

CDEFINES += -DESP_PLATFORM -DMBEDTLS_CONFIG_FILE='"mbedtls/esp_config.h"' -DHAVE_CONFIG_H -DESP32

CFLAGS += -w -Os -g3 -Wpointer-arith -Wno-error=unused-function -Wno-error=unused-but-set-variable -Wno-error=unused-variable -ffunction-sections -fdata-sections -mlongcalls -nostdlib -MMD -std=gnu99 -fstrict-volatile-bitfields

CPPFLAGS += -w -Os -g3 -Wpointer-arith -Wno-error=unused-function -Wno-error=unused-but-set-variable -Wno-error=unused-variable -fno-rtti -ffunction-sections -fdata-sections -mlongcalls -nostdlib -MMD -std=gnu++11 -fno-exceptions -fstrict-volatile-bitfields

ASFLAGS += -g3 -x assembler-with-cpp -MMD -mlongcalls


else # for nut and neutron-net

GCC_ARM_PATH ?= $(PROJECT_ROOT)/tools/xtensa-lx106-elf/bin/
GCC_PREFIX ?= xtensa-lx106-elf-

include $(COMMON_BUILD)/common-tools.mk

CDEFINES += -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ -DF_CPU=80000000L -DARDUINO=10605 -DESP8266

# C 编译参数
ifneq ("$(MODULE)","bootloader")
CFLAGS += -Os
else
CFLAGS += -O0
endif

CFLAGS += -g -w -mlongcalls -mtext-section-literals -falign-functions=4 -MMD -ffunction-sections -fdata-sections

CONLYFLAGS += -Wpointer-arith -Wno-implicit-function-declaration -Wl,-EL -fno-inline-functions -nostdlib -std=gnu99

# C++ 编译参数
CPPFLAGS += -fno-exceptions -fno-rtti -std=c++11

ASFLAGS += -c -g -x assembler-with-cpp -MMD


# Board definitions
FLASH_SIZE ?= 16M    #此处必须是16M  此时用户参数区才会在1M 的最后16K上
FLASH_MODE ?= qio
FLASH_SPEED ?= 40

# Upload parameters
#UPLOAD_SPEED ?= 230400
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

UPLOAD_VERB ?= -v
UPLOAD_RESET ?= nodemcu

endif # endif for neutron-net and nut
