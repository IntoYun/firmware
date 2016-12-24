
# TARGET_HAL_STM32F4XX_SHARE_PATH = $(TARGET_HAL_PATH)/src/stm32f4xx-share

CSRC += $(call here_files,$(TARGET_HAL_STM32F4XX_SHARE_PATH)/,*.c)

CPPSRC += $(call here_files,$(TARGET_HAL_STM32F4XX_SHARE_PATH)/,*.cpp)

# ASM source files included in this build.
ASRC += $(call here_files,$(TARGET_HAL_STM32F4XX_SHARE_PATH)/,*.S)

include $(call rwildcard,$(TARGET_HAL_STM32F4XX_SHARE_PATH)/rtos,sources.mk)

