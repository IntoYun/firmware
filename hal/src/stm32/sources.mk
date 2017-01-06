
CSRC += $(call here_files,$(TARGET_HAL_STM32_PATH)/,*.c)

CPPSRC += $(call here_files,$(TARGET_HAL_STM32_PATH)/,*.cpp)

# ASM source files included in this build.
ASRC += $(call here_files,$(TARGET_HAL_STM32_PATH)/,*.S)

