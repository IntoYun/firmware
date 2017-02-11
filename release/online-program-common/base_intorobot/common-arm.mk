##### Setup Common FLAGS #####
CFLAGS += -g3 -gdwarf-2 -Os -mthumb -fno-strict-aliasing -Wfatal-errors -w -fno-common -ffunction-sections -fdata-sections -Wno-switch -Wno-error=deprecated-declarations -fmessage-length=0

# C++ 编译参数  -fpermissive 把代码的语法错误作为警告,并继续编译进程
CPPFLAGS += -fno-exceptions -fno-rtti -fcheck-new -std=gnu++11 -fpermissive

LDFLAGS += -nostartfiles -mlittle-endian -Xlinker --gc-sections

USE_PRINTF_FLOAT ?= y
ifeq ("$(USE_PRINTF_FLOAT)","y")
LDFLAGS += -u _printf_float
endif

WARNINGS_AS_ERRORS ?= y
ifeq ($(WARNINGS_AS_ERRORS),y)
CFLAGS += -Werror
endif

CFLAGS += -DINTOROBOT=1 -DINTOYUN=1

# Generate dependency files automatically.
CFLAGS += -MD -MP -MF $@.d

# Create a bin file from ELF file
%.bin : %.elf
	@echo Invoking: ARM GNU Create Flash Image
	$(OBJCOPY) -O binary $< $@
	@echo

