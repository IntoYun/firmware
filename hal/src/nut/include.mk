
# Define the prefix to this directory.
# Note: The name must be unique within this build and should be
#       based on the root of the project
TARGET_HAL_SRC_INC_PATH = $(TARGET_HAL_PATH)/src/$(PLATFORM_NAME)

INCLUDE_DIRS += $(TARGET_HAL_SRC_INC_PATH)
INCLUDE_DIRS += $(TARGET_HAL_SRC_INC_PATH)/esp8266
INCLUDE_DIRS += $(TARGET_HAL_SRC_INC_PATH)/socket
INCLUDE_DIRS += $(TARGET_HAL_SRC_INC_PATH)/esp8266/umm_malloc


# if hal is used as a make dependency (linked) then add linker commands
ifneq (,$(findstring hal,$(MAKE_DEPENDENCIES)))
ifneq (,$(findstring wiring,$(MAKE_DEPENDENCIES)))

LDFLAGS += -Tlinker_$(PLATFORM_DEVICE_LC)_app.ld
LDFLAGS += -L$(COMMON_BUILD)/linker/esp8266/$(PLATFORM_NAME)

USE_PRINTF_FLOAT ?= n
ifeq ("$(USE_PRINTF_FLOAT)","y")
LDFLAGS += -u _printf_float
endif
LDFLAGS += -Wl,-Map,$(TARGET_BASE).map

LDFLAGS += -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static -Wl,--gc-sections -Wl,-wrap,system_restart_local -Wl,-wrap,register_chipv6_phy
LIBS += m gcc halhal phy pp net80211 wpa crypto main wps axtls smartconfig airkiss mesh wpa2 lwip_gcc stdc++

endif
endif
