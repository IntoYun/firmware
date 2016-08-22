INCLUDE_DIRS += $(BOOTLOADER_TYPE_PATH)/inc

LDFLAGS += -Tlinker_$(PLATFORM_DEVICE_LC)_boot.ld
LDFLAGS += -L$(COMMON_BUILD)/linker/esp8266/$(PLATFORM_NAME)

USE_PRINTF_FLOAT ?= y
ifeq ("$(USE_PRINTF_FLOAT)","y")
LDFLAGS += -u _printf_float
endif
LDFLAGS += -Wl,-Map,$(TARGET_BASE).map

LDFLAGS += -nostdlib -Wl,--no-check-sections -umain

ASRC +=

#LDFLAGS += -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static -Wl,--gc-sections -Wl,-wrap,system_restart_local -Wl,-wrap,register_chipv6_phy
#LIBS += m gcc hal phy pp net80211 wpa crypto main wps axtls smartconfig airkiss mesh wpa2 lwip_gcc stdc++


#LDFLAGS += -nostdlib -Wl,--no-check-sections -umain -Wl,-static -Wl,--gc-sections
#LIBS += m gcc hal phy crypto wps axtls mesh lwip_gcc stdc++

#ASRC +=

