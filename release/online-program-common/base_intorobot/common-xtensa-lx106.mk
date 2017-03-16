CFLAGS += -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ -DF_CPU=80000000L -DARDUINO=10605 -DESP8266

# C 编译参数
CFLAGS += -g -w -mlongcalls -mtext-section-literals -falign-functions=4 -MMD
CFLAGS += -Os -ffunction-sections -fdata-sections

#only for firmware libs build, not use in the ota makefile
CONLYFLAGS += -Wpointer-arith -Wno-implicit-function-declaration -Wl,-EL -fno-inline-functions -nostdlib -std=gnu99

# C++ 编译参数  -fpermissive 把代码的语法错误作为警告,并继续编译进程
CPPFLAGS += -fno-exceptions -fno-rtti -std=c++11 -fpermissive

ASFLAGS += -c -g -x assembler-with-cpp -MMD

CFLAGS += -DINTOROBOT=1 -DINTOYUN=1

# Board definitions
FLASH_SIZE ?= 16M    #此处必须是16M  此时用户参数区才会在1M 的最后16K上
FLASH_MODE ?= qio
FLASH_SPEED ?= 40

# Create a bin file from ELF file
%.bin : %.elf
	$(call echo,'Invoking: XTENSA GNU Create Flash Image')
	$(ESP8266_TOOL) -eo $< -bo $@ -bs .irom0.text -bs .text -bs .data -bs .rodata -bc -ec
