
HAL_SRC_NEUTRON_PATH = $(TARGET_HAL_PATH)/src/$(PLATFORM_NAME)
HAL_SRC_STM32_PATH = $(TARGET_HAL_PATH)/src/stm32

CSRC += $(call here_files,$(HAL_SRC_NEUTRON_PATH)/,*.c)
CPPSRC += $(call here_files,$(HAL_SRC_NEUTRON_PATH)/,*.cpp)

CSRC += $(call target_files,$(HAL_SRC_STM32_PATH)/,*.c)
CPPSRC += $(call target_files,$(HAL_SRC_STM32_PATH)/,*.cpp)

# ASM source files included in this build.
ASRC +=

