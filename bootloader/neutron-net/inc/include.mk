INCLUDE_DIRS += $(BOOTLOADER_TYPE_PATH)/inc

LDFLAGS += -T$(COMMON_BUILD)/linker/esp8266/linker_esp8266-net_boot.ld

LDFLAGS += -nostdlib -Wl,--no-check-sections -umain

ASRC +=

