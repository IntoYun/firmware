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

LIBS += -lgcc -lc -lm -lhal -lcore -lnet80211 -lphy -lrtc -lpp -lwpa -lsmartconfig -lbtdm_app -lbt -ldriver -lesp32 -lcrypto -lexpat -lfreertos -ljson -llog -llwip -lmbedtls -lnghttp -lnvs_flash -lspi_flash -ltcpip_adapter -lnewlib -lvfs




TARGET_INTOROBOT_PATH = $(PLATFORM_MCU_PATH)/IntoRobot_Firmware_Driver
INCLUDE_DIRS += $(TARGET_INTOROBOT_PATH)/inc

LIB_DIRS += $(TARGET_ESP32_SDK)/lib
LIBS +=
