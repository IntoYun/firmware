INCLUDE_DIRS += $(BOARD_TYPE_PATH)/inc

ifneq (,$(findstring board,$(MAKE_DEPENDENCIES)))

LDFLAGS += -T$(COMMON_BUILD)/linker/esp8266/linker_esp8266-net.ld

LDFLAGS += -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static
LIBS += c gcc hal phy pp net80211 lwip wpa crypto main json ssl pwm wps smartconfig airkiss at

endif
