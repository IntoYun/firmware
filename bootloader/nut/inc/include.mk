INCLUDE_DIRS += $(BOOTLOADER_TYPE_PATH)/inc

LDFLAGS += -Teboot.ld
LDFLAGS += -L$(COMMON_BUILD)/linker/esp8266/$(PLATFORM_NAME)

USE_PRINTF_FLOAT ?= y
ifeq ("$(USE_PRINTF_FLOAT)","y")
LDFLAGS += -u _printf_float
endif
LDFLAGS += -Wl,-Map,$(TARGET_BASE).map

LDFLAGS += -nostdlib -Wl,--no-check-sections -umain -Wl,-static -Wl,--gc-sections
LIBS += m gcc halhal phy crypto wps axtls mesh lwip_gcc stdc++

ASRC +=

