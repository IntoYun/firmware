INCLUDE_DIRS += $(BOOTLOADER_TYPE_PATH)/inc

LDFLAGS += -Tlinker_$(PLATFORM_DEVICE_LC)_boot.ld
LDFLAGS += -L$(COMMON_BUILD)/linker/esp8266/$(PLATFORM_NAME)

LDFLAGS += -nostdlib -Wl,--no-check-sections -umain

ASRC +=

