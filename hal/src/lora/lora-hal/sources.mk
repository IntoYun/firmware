# This file is a makefile included from the top level makefile which
# defines the sources built for the target.

# Define the prefix to this directory.
# Note: The name must be unique within this build and should be
#       based on the root of the project

# C source files included in this build.

TARGET_HAL_LORA_HAL_SRC_PATH = $(TARGET_HAL_PATH)/src/$(PLATFORM_NAME)/lora-hal/src

CSRC += $(call here_files,$(TARGET_HAL_LORA_HAL_SRC_PATH)/,*.c)

CPPSRC += $(call here_files,$(TARGET_HAL_LORA_HAL_SRC_PATH)/,*.cpp)

# ASM source files included in this build.
ASRC +=


