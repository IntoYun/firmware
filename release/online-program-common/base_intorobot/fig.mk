GCC_ARM_PATH ?= $(PROJECT_ROOT)/tools/xtensa-esp32-elf/bin/
GCC_PREFIX ?= xtensa-esp32-elf-
include common-tools.mk
include common-xtensa-esp32.mk

CFLAGS += -DINTOROBOT_ARCH_XTENSA

# FLAGS For nut
CFLAGS += -I nut/inc/hal/
CFLAGS += -I nut/inc/platform/
CFLAGS += -I nut/inc/services/
CFLAGS += -I nut/inc/system/
CFLAGS += -I nut/inc/user/
CFLAGS += -I nut/inc/wiring/
CFLAGS += -I nut/inc/wiring_ex/

#ifeq ("$(DEBUG_BUILD)","y")
#CFLAGS += -DDEBUG_BUILD
#COMPILE_LTO ?= n
#else
#CFLAGS += -DRELEASE_BUILD
#endif

#CFLAGS += -MD -MP -MF $@.d

LDFLAGS += -nostdlib -u call_user_start_cpu0 -Wl,--gc-sections -Wl,-static -Wl,--undefined=uxTopUsedPriority

LDFLAGS += -L fig/lib
LIBS += hal platform services system wiring wiring_ex
LIBS += gcc stdc++ app_update bootloader_support bt btdm_app c c_nano coap coexist core cxx driver esp32 ethernet expat fatfs freertos halhal json log lwip m mbedtls mdns micro-ecc net80211 newlib nghttp nvs_flash openssl phy pp rtc rtc_clk sdmmc smartconfig spi_flash tcpip_adapter ulp vfs wpa wpa2 wpa_supplicant wps xtensa-debug-module
LDFLAGS += -L nut/lib/esp8266 -Wl,--start-group $(patsubst %,-l%,$(LIBS)) -Wl,--end-group

STARTUP_OBJFILE +=

LDFLAGS += -Tesp32_out.ld -Tesp32.common.ld -Tesp32.rom.ld -Tesp32.peripherals.ld
LDFLAGS += -L fig/linker/

USE_PRINTF_FLOAT = n
ifeq ("$(USE_PRINTF_FLOAT)","y")
LDFLAGS += -u _printf_float
endif


