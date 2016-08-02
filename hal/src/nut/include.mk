
# Define the prefix to this directory.
# Note: The name must be unique within this build and should be
#       based on the root of the project
TARGET_HAL_SRC_INC_PATH = $(TARGET_HAL_PATH)/src/$(PLATFORM_NAME)

INCLUDE_DIRS += $(TARGET_HAL_SRC_INC_PATH)


# if hal is used as a make dependency (linked) then add linker commands
ifneq (,$(findstring hal,$(MAKE_DEPENDENCIES)))

LDFLAGS += -Tlinker_esp8266-net.ld
LDFLAGS += -L$(COMMON_BUILD)/linker/esp8266

#LDFLAGS += -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static -Wl,--gc-sections -Wl,-wrap,system_restart_local -Wl,-wrap,register_chipv6_phy
LDFLAGS += -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static -Wl,--gc-sections
LIBS += c gcc hal phy net80211 lwip wpa main pp smartconfig wps crypto axtls airkiss

endif
