GCC_ARM_PATH ?= $(PROJECT_ROOT)/tools/xtensa-esp32-elf/bin/
GCC_PREFIX ?= xtensa-esp32-elf-
include common-tools.mk
include common-xtensa-esp32.mk

CFLAGS += -DINTOROBOT_ARCH_XTENSA

# FLAGS For fig
CFLAGS += -I fig/inc/hal/
CFLAGS += -I fig/inc/platform/
CFLAGS += -I fig/inc/services/
CFLAGS += -I fig/inc/system/
CFLAGS += -I fig/inc/user/
CFLAGS += -I fig/inc/wiring/
CFLAGS += -I fig/inc/wiring_ex/

LDFLAGS += -L fig/linker/
LDFLAGS += -Tesp32_out.ld -Tesp32.common.ld -Tesp32.rom.ld -Tesp32.peripherals.ld

USE_PRINTF_FLOAT = n
ifeq ("$(USE_PRINTF_FLOAT)","y")
LDFLAGS += -u _printf_float
endif

#ifeq ("$(DEBUG_BUILD)","y")
#CFLAGS += -DDEBUG_BUILD
#COMPILE_LTO ?= n
#else
#CFLAGS += -DRELEASE_BUILD
#endif

#CFLAGS += -MD -MP -MF $@.d

LDFLAGS += -nostdlib -u call_user_start_cpu0 -Wl,--gc-sections -Wl,-static -Wl,--undefined=uxTopUsedPriority

LDFLAGS += -L fig/lib -L fig/lib/esp32
LIBS += hal platform services system wiring wiring_ex
LIBS += gcc stdc++ app_update bootloader_support bt btdm_app c c_nano coap coexist core cxx driver esp32 ethernet expat fatfs freertos halhal json log lwip m mbedtls mdns micro-ecc net80211 newlib nghttp nvs_flash openssl phy pp rtc rtc_clk sdmmc smartconfig spi_flash tcpip_adapter ulp vfs wpa wpa2 wpa_supplicant wps xtensa-debug-module
LDFLAGS += -Wl,--start-group $(patsubst %,-l%,$(LIBS)) -Wl,--end-group

STARTUP_OBJFILE +=

