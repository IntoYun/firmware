INCLUDE_DIRS += $(BOOTLOADER_TYPE_PATH)/inc

LDFLAGS += -Tlinker_esp8266-net_boot.ld
LDFLAGS += -L$(COMMON_BUILD)/linker/esp8266

LDFLAGS += -nostdlib -Wl,--no-check-sections -umain

ASRC +=

