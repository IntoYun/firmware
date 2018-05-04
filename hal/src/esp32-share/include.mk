
# Define the prefix to this directory.
# Note: The name must be unique within this build and should be
#       based on the root of the project
# TARGET_HAL_SRC_INC_PATH = $(TARGET_HAL_PATH)/src/$(PLATFORM_NAME)
TARGET_HAL_ESP32_SHARE_PATH = $(TARGET_HAL_PATH)/src/esp32-share

INCLUDE_DIRS += $(TARGET_HAL_ESP32_SHARE_PATH)
INCLUDE_DIRS += $(TARGET_HAL_ESP32_SHARE_PATH)/esp32
INCLUDE_DIRS += $(TARGET_HAL_ESP32_SHARE_PATH)/socket
include $(TARGET_HAL_PATH)/src/esp32/include.mk

# if hal is used as a make dependency (linked) then add linker commands
ifneq (,$(findstring hal,$(MAKE_DEPENDENCIES)))

LDFLAGS += -Tesp32_out.ld -Tesp32.common.ld -Tesp32.rom.ld -Tesp32.peripherals.ld -Tesp32.rom.spiram_incompatible_fns.ld
LDFLAGS += -L$(COMMON_BUILD)/linker/esp32/$(PLATFORM_NAME)

USE_PRINTF_FLOAT = n
ifeq ("$(USE_PRINTF_FLOAT)","y")
LDFLAGS += -u _printf_float
endif

LDFLAGS += -Wl,-Map,$(TARGET_BASE).map

LDFLAGS += -nostdlib -u ld_include_panic_highint_hdl -u call_user_start_cpu0 -Wl,--gc-sections -Wl,-static -Wl,--undefined=uxTopUsedPriority -u __cxa_guard_dummy -u __cxx_fatal_exception
LIBS += gcc openssl btdm_app fatfs wps coexist wear_levelling halhal newlib driver bootloader_support pp smartconfig jsmn wpa ethernet phy app_trace console ulp wpa_supplicant freertos bt micro-ecc cxx xtensa-debug-module mdns vfs soc core sdmmc coap tcpip_adapter c_nano rtc spi_flash wpa2 esp32 app_update nghttp spiffs espnow nvs_flash esp_adc_cal log expat m c heap mbedtls lwip net80211 pthread json stdc++

endif
