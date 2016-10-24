TARGET_ESP832_SDK=$(PLATFORM_MCU_PATH)/sdk
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include
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

TARGET_INTOROBOT_PATH = $(PLATFORM_MCU_PATH)/IntoRobot_Firmware_Driver
INCLUDE_DIRS += $(TARGET_INTOROBOT_PATH)/inc

LIB_DIRS += $(TARGET_ESP32_SDK)/lib
LIBS +=
