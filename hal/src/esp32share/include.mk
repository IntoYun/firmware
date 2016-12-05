
# Define the prefix to this directory.
# Note: The name must be unique within this build and should be
#       based on the root of the project
# TARGET_HAL_SRC_INC_PATH = $(TARGET_HAL_PATH)/src/$(PLATFORM_NAME)
TARGET_HAL_ESP32_SHARE_PATH = $(TARGET_HAL_PATH)/src/esp32share

INCLUDE_DIRS += $(TARGET_HAL_ESP32_SHARE_PATH)
INCLUDE_DIRS += $(TARGET_HAL_ESP32_SHARE_PATH)/esp32
INCLUDE_DIRS += $(TARGET_HAL_ESP32_SHARE_PATH)/esp32/libb64

# if hal is used as a make dependency (linked) then add linker commands
ifneq (,$(findstring hal,$(MAKE_DEPENDENCIES)))
ifneq (,$(findstring wiring,$(MAKE_DEPENDENCIES)))

# LDFLAGS += -Tlinker_$(PLATFORM_DEVICE_LC).ld

USE_PRINTF_FLOAT ?= y
ifeq ("$(USE_PRINTF_FLOAT)","y")
LDFLAGS += -u _printf_float
endif
LDFLAGS += -Wl,-Map,$(TARGET_BASE).map

# assembler startup script
# ASRC_STARTUP += $(COMMON_BUILD)/startup/arm/startup_$(PLATFORM_DEVICE_LC).S
# ASFLAGS += -I$(COMMON_BUILD)/startup/arm
# ASFLAGS +=  -Wa,--defsym -Wa,INTOROBOT_INIT_STARTUP=1
#


# LDFLAGS += -L$(TARGET_ESP32_SDK)/lib -L$(TARGET_ESP32_SDK)/ld -nostdlib -T esp32_out.ld -T esp32.common.ld -T esp32.rom.ld -T esp32.peripherals.ld -u call_user_start_cpu0 -Wl,--gc-sections -Wl,-static -Wl,--undefined=uxTopUsedPriority

# LIBS += gcc c m halhal core net80211 phy rtc pp wpa smartconfig btdm_app bt driver esp32 crypto expat freertos json log lwip mbedtls nghttp nvs_flash spi_flash tcpip_adapter newlib vfs

endif
endif
