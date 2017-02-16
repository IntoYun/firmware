INCLUDE_DIRS += $(BOOTLOADER_TYPE_PATH)/inc

LDFLAGS += -Tlinker_$(PLATFORM_DEVICE_LC)_boot.ld
LDFLAGS += -L$(COMMON_BUILD)/linker/esp32/$(PLATFORM_NAME)

USE_PRINTF_FLOAT ?= y
ifeq ("$(USE_PRINTF_FLOAT)","y")
LDFLAGS += -u _printf_float
endif
LDFLAGS += -Wl,-Map,$(TARGET_BASE).map

LDFLAGS += -nostdlib -Wl,--no-check-sections -umain -Wl,-static -Wl,--gc-sections
# LIBS += m gcc halhal phy crypto wps axtls mesh lwip_gcc stdc++
LIBS += m gcc halhal bootloader_support esp32 spi_flash micro-ecc phy log 

ASRC +=

