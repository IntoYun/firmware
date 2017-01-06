
TARGET_SUBSYS_PATH = $(SUBSYS_MODULE_PATH)
INCLUDE_DIRS += $(TARGET_SUBSYS_PATH)/$(PLATFORM_NAME)/inc/

ifneq (,$(findstring subsys,$(MAKE_DEPENDENCIES)))

LDFLAGS += -Tlinker_$(PLATFORM_DEVICE_LC)_app.ld
LDFLAGS += -L$(COMMON_BUILD)/linker/esp8266/$(PLATFORM_NAME)

LDFLAGS += -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static
#LIBS += c gcc hal phy pp net80211 wpa crypto main wps json ssl pwm smartconfig airkiss at lwip
LIBS += c gcc phy pp net80211 wpa crypto main wps json ssl pwm smartconfig airkiss at lwip

endif
