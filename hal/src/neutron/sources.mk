
HAL_SRC_NEUTRON_PATH = $(TARGET_HAL_PATH)/src/$(PLATFORM_NAME)

# C source files included in this build.
CSRC += $(call here_files,$(HAL_SRC_NEUTRON_PATH)/,*.c)

# enumerate target cpp files
CPPSRC += $(call hare_files,$(HAL_SRC_NEUTRON_PATH)/,*.cpp)

# ASM source files included in this build.
ASRC +=

