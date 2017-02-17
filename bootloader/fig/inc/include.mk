INCLUDE_DIRS += $(BOOTLOADER_TYPE_PATH)/inc

LDFLAGS += -Tlinker_$(PLATFORM_DEVICE_LC)_boot.ld
LDFLAGS += -L$(COMMON_BUILD)/linker/esp32/$(PLATFORM_NAME)

USE_PRINTF_FLOAT = n
ifeq ("$(USE_PRINTF_FLOAT)","y")
LDFLAGS += -u _printf_float
endif

LDFLAGS += -Wl,-Map,$(TARGET_BASE).map

<<<<<<< 995b7a22b99de56315fe8c967b822e5dd72d10bb
LDFLAGS += -nostdlib -Wl,--no-check-sections -umain -Wl,-static -Wl,--gc-sections
# LIBS += m gcc halhal phy crypto wps axtls mesh lwip_gcc stdc++
LIBS += m gcc halhal bootloader_support esp32 spi_flash micro-ecc phy log 
=======
LDFLAGS +=  -nostdlib -u call_user_start_cpu0 -Wl,--gc-sections -Wl,-static -Wl,--undefined=uxTopUsedPriority
LIBS += gcc stdc++ app_update bootloader_support bt btdm_app c c_nano coap coexist core cxx driver esp32 ethernet expat fatfs freertos halhal json log lwip m mbedtls mdns micro-ecc net80211 newlib nghttp nvs_flash openssl phy pp rtc sdmmc smartconfig spi_flash tcpip_adapter ulp vfs wpa wpa2 wpa_supplicant wps xtensa-debug-module
#LIBS += gcc stdc++ app_update bootloader_support bt btdm_app c c_nano coap coexist core cxx driver esp32 ethernet expat fatfs freertos halhal json log lwip m mbedtls mdns micro-ecc net80211 newlib nghttp nvs_flash openssl phy pp rtc sdmmc smartconfig spi_flash tcpip_adapter ulp vfs wpa wpa2 wpa_supplicant wps xtensa-debug-module

#LDFLAGS += -nostdlib -Wl,--no-check-sections -umain -Wl,-static -Wl,--gc-sections
#LIBS += m gcc bootloader_support spi_flash log micro-ecc stdc++
>>>>>>> 1. fig 更新 esp32 SDK库至 v2.0. app能正常编译成功  bootloader还编译不成功

ASRC +=

