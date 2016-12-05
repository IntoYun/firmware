
TARGET_HAL_SRC_PATH = $(TARGET_HAL_PATH)/src/$(PLATFORM_NAME)

CSRC += $(call target_files,$(TARGET_HAL_SRC_PATH)/,*.c)

CPPSRC += $(call target_files,$(TARGET_HAL_SRC_PATH)/,*.cpp)

# ASM source files included in this build.
ASRC += $(call target_files,$(TARGET_HAL_SRC_PATH)/,*.S)

include $(call rwildcard,$(TARGET_HAL_ESP32_SHARE_PATH)/,sources.mk)
