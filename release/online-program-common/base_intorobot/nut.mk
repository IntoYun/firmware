GCC_ARM_PATH ?= $(PROJECT_ROOT)/tools/xtensa-lx106-elf/bin/
GCC_PREFIX ?= xtensa-lx106-elf-
include common-tools.mk
include common-xtensa.mk

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

LDFLAGS += -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static -Wl,--gc-sections -Wl,-wrap,system_restart_local -Wl,-wrap,register_chipv6_phy

#LDFLAGS += -Lnut/lib/ -Wl,--whole-archive -lhal -lplatform -lservices -lsystem -lwiring -Wl,--no-whole-archive
LDFLAGS += -L nut/lib
LIBS += hal platform services system wiring wiring_ex
LIBS += m gcc halhal phy pp net80211 wpa crypto main wps axtls smartconfig airkiss mesh wpa2 lwip_gcc stdc++
LDFLAGS += -L nut/lib/esp8266 -Wl,--start-group $(patsubst %,-l%,$(LIBS)) -Wl,--end-group

STARTUP_OBJFILE +=

LDFLAGS += -L nut/linker/
LDFLAGS += -Tlinker_nut_app.ld

USE_PRINTF_FLOAT ?= y
ifeq ("$(USE_PRINTF_FLOAT)","y")
LDFLAGS += -u _printf_float
endif




