
# TARGET_HAL_SRC_PATH = $(TARGET_HAL_PATH)/src/$(PLATFORM_NAME)
# TARGET_HAL_ESP32_SHARE_PATH = $(TARGET_HAL_PATH)/src/esp32share

CSRC += $(call target_files,$(TARGET_HAL_ESP32_SHARE_PATH)/,*.c)

CPPSRC += $(call target_files,$(TARGET_HAL_ESP32_SHARE_PATH)/,*.cpp)

# ASM source files included in this build.
ASRC +=

