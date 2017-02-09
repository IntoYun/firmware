##### Setup Common FLAGS #####
CFLAGS += -g3 -gdwarf-2 -Os -mthumb -fno-strict-aliasing -Wfatal-errors -w -fno-common -ffunction-sections -fdata-sections -Wno-switch -Wno-error=deprecated-declarations -fmessage-length=0

# C++ 编译参数
#CPPFLAGS += -fno-exceptions -fno-rtti -std=gnu++11 -fcheck-new
CPPFLAGS += -fno-exceptions -fno-rtti -std=gnu++11 -fcheck-new

#ASFLAGS += -g3 -gdwarf-2 -mthumb -x assembler-with-cpp -fmessage-length=0
LDFLAGS += -mlittle-endian -Xlinker --gc-sections --specs=nano.specs --specs=nosys.specs -lc -lnosys
#LDFLAGS += -Wl,-Map,$(BIN_PLATFORM_OUT)/platform.map
LDFLAGS += -Wl,--start-group -lgcc -lc -Wl,--end-group

USE_PRINTF_FLOAT ?= y
ifeq ("$(USE_PRINTF_FLOAT)","y")
LDFLAGS += -u _printf_float
endif

WARNINGS_AS_ERRORS ?= y
ifeq ($(WARNINGS_AS_ERRORS),y)
CFLAGS += -Werror
endif

# Generate dependency files automatically.
CFLAGS += -MD -MP -MF $@.d

# Create a bin file from ELF file
%.bin : %.elf
	@echo Invoking: ARM GNU Create Flash Image
	$(OBJCOPY) -O binary $< $@
	@echo

