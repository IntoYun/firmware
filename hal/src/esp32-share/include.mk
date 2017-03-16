
# Define the prefix to this directory.
# Note: The name must be unique within this build and should be
#       based on the root of the project
# TARGET_HAL_SRC_INC_PATH = $(TARGET_HAL_PATH)/src/$(PLATFORM_NAME)
TARGET_HAL_ESP32_SHARE_PATH = $(TARGET_HAL_PATH)/src/esp32-share

INCLUDE_DIRS += $(TARGET_HAL_ESP32_SHARE_PATH)
INCLUDE_DIRS += $(TARGET_HAL_ESP32_SHARE_PATH)/esp32

# if hal is used as a make dependency (linked) then add linker commands
ifneq (,$(findstring hal,$(MAKE_DEPENDENCIES)))

LDFLAGS += -Tesp32_out.ld -Tesp32.common.ld -Tesp32.rom.ld -Tesp32.peripherals.ld
LDFLAGS += -L$(COMMON_BUILD)/linker/esp32/$(PLATFORM_NAME)

USE_PRINTF_FLOAT = n
ifeq ("$(USE_PRINTF_FLOAT)","y")
LDFLAGS += -u _printf_float
endif

LDFLAGS += -Wl,-Map,$(TARGET_BASE).map

LDFLAGS += -nostdlib -u call_user_start_cpu0 -Wl,--gc-sections -Wl,-static -Wl,--undefined=uxTopUsedPriority
LIBS += gcc stdc++ app_update bootloader_support bt btdm_app c c_nano coap coexist core cxx driver esp32 ethernet expat fatfs freertos halhal json log lwip m mbedtls mdns micro-ecc net80211 newlib nghttp nvs_flash openssl phy pp rtc rtc_clk sdmmc smartconfig spi_flash tcpip_adapter ulp vfs wpa wpa2 wpa_supplicant wps xtensa-debug-module

endif
