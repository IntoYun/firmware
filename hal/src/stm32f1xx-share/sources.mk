
CSRC += $(call here_files,$(TARGET_HAL_STM32F1XX_SHARE_PATH)/,*.c)

CPPSRC += $(call here_files,$(TARGET_HAL_STM32F1XX_SHARE_PATH)/,*.cpp)

# ASM source files included in this build.
ASRC += $(call here_files,$(TARGET_HAL_STM32F1XX_SHARE_PATH)/,*.S)

#include $(TARGET_HAL_PATH)/src/stm32/sources.mk

