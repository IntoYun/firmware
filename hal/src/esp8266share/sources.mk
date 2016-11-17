
# TARGET_HAL_ESP8266_SHARE_PATH = $(TARGET_HAL_PATH)/src/esp8266share

CSRC += $(call target_files,$(TARGET_HAL_ESP8266_SHARE_PATH)/,*.c)

CPPSRC += $(call target_files,$(TARGET_HAL_ESP8266_SHARE_PATH)/,*.cpp)

# ASM source files included in this build.
ASRC += $(call target_files,$(TARGET_HAL_ESP8266_SHARE_PATH)/,*.S)
