INCLUDE_DIRS += $(BOOTLOADER_TYPE_PATH)/inc
INCLUDE_DIRS += $(BOOTLOADER_TYPE_PATH)/inc/bootloader_support/include
INCLUDE_DIRS += $(BOOTLOADER_TYPE_PATH)/inc/bootloader_support/include_priv

LDFLAGS += -Tesp32.bootloader.ld -Tesp32.bootloader.rom.ld -Tesp32.rom.ld
LDFLAGS += -L$(COMMON_BUILD)/linker/esp32/$(PLATFORM_NAME)

USE_PRINTF_FLOAT = n
ifeq ("$(USE_PRINTF_FLOAT)","y")
LDFLAGS += -u _printf_float
endif

LDFLAGS += -Wl,-Map,$(TARGET_BASE).map

LDFLAGS +=  -nostdlib -u call_user_start_cpu0 -Wl,--gc-sections -Wl,-static -Wl,--undefined=uxTopUsedPriority

LIB_DIRS += $(BOOTLOADER_TYPE_PATH)/lib
LIBS += gcc stdc++ bootloader_support log micro-ecc rtc spi_flash

ASRC +=

