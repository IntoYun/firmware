
CFLAGS += -DESP_PLATFORM -DMBEDTLS_CONFIG_FILE='"mbedtls/esp_config.h"' -DHAVE_CONFIG_H -DESP32 -DF_CPU=240000000L

# C 编译参数
CFLAGS += -g3 -Os
CFLAGS += -w -Wpointer-arith -Wno-error=unused-function -Wno-error=unused-but-set-variable -Wno-error=unused-variable -Wfatal-errors -Wno-error=deprecated-declarations -Wno-unused-parameter -Wno-sign-compare -ffunction-sections -fdata-sections -mlongcalls -nostdlib -fstrict-volatile-bitfields -MMD

#only for firmware libs build, not use in the ota makefile
CONLYFLAGS += -std=gnu99 -Wno-old-style-declaration

# C++ 编译参数  -fpermissive 把代码的语法错误作为警告,并继续编译进程
CPPFLAGS += -fno-exceptions -fno-rtti -std=gnu++11 -fpermissive

ASFLAGS += -g3 -x assembler-with-cpp -MMD -mlongcalls

CFLAGS += -DINTOROBOT=1 -DINTOYUN=1

# Board definitions
FLASH_SIZE ?= 4MB
FLASH_MODE ?= qio
FLASH_SPEED ?= 40m

# Create a bin file from ELF file
%.bin : %.elf
	$(call echo,'Invoking: XTENSA GNU Create Flash Image')
	$(ESP32_TOOL) --chip esp32 elf2image --flash_mode $(FLASH_MODE) --flash_freq $(FLASH_SPEED) --flash_size $(FLASH_SIZE) -o $@ $<

