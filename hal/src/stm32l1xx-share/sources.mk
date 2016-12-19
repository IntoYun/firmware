
#TARGET_HAL_STM32L1XX_SHARE_PATH = $(TARGET_HAL_PATH)/src/stm32l1xx

CSRC += $(call target_files,$(TARGET_HAL_STM32L1XX_SHARE_PATH)/,*.c)

CPPSRC += $(call target_files,$(TARGET_HAL_STM32L1XX_SHARE_PATH)/,*.cpp)

# ASM source files included in this build.
ASRC += $(call target_files,$(TARGET_HAL_STM32L1XX_SHARE_PATH)/,*.S)
