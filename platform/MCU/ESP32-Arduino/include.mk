TARGET_ESP32_SDK=$(PLATFORM_MCU_PATH)/sdk
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/bt
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/config
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/driver
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/esp32
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/expat
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/freertos
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/json
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/log
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/lwip
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/mbedtls
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/newlib
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/nghttp
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/nvs_flash
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/spi_flash
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/tcpip_adapter
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/vfs

LDFLAGS += -L$(TARGET_ESP32_SDK)/lib -L$(TARGET_ESP32_SDK)/ld -nostdlib -T esp32_out.ld -T esp32.common.ld -T esp32.rom.ld -T esp32.peripherals.ld -u call_user_start_cpu0 -Wl,--gc-sections -Wl,-static -Wl,--undefined=uxTopUsedPriority

#LIBS += gcc c m halhal core net80211 phy rtc pp wpa smartconfig btdm_app bt driver esp32 crypto expat freertos json log lwip mbedtls nghttp nvs_flash spi_flash tcpip_adapter newlib vfs
LIBS += gcc c m halhal core net80211 phy rtc pp wpa smartconfig btdm_app bt driver esp32 expat freertos json log lwip mbedtls nghttp nvs_flash spi_flash tcpip_adapter newlib vfs


TARGET_INTOROBOT_PATH = $(PLATFORM_MCU_PATH)/IntoRobot_Firmware_Driver
INCLUDE_DIRS += $(TARGET_INTOROBOT_PATH)/inc

LIB_DIRS += $(TARGET_ESP32_SDK)/lib
LIBS +=
