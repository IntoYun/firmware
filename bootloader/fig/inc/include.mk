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
LIBS += gcc stdc++ app_update bootloader_support bt btdm_app c c_nano coap coexist core cxx driver esp32 ethernet expat fatfs freertos halhal json log lwip m mbedtls mdns micro-ecc net80211 newlib nghttp nvs_flash openssl phy pp rtc sdmmc smartconfig spi_flash tcpip_adapter ulp vfs wpa wpa2 wpa_supplicant wps xtensa-debug-module
#LIBS += gcc stdc++ app_update bootloader_support bt btdm_app c c_nano coap coexist core cxx driver esp32 ethernet expat fatfs freertos halhal json log lwip m mbedtls mdns micro-ecc net80211 newlib nghttp nvs_flash openssl phy pp rtc sdmmc smartconfig spi_flash tcpip_adapter ulp vfs wpa wpa2 wpa_supplicant wps xtensa-debug-module

#LDFLAGS += -nostdlib -Wl,--no-check-sections -umain -Wl,-static -Wl,--gc-sections
#LIBS += m gcc bootloader_support spi_flash log micro-ecc stdc++

ASRC +=

