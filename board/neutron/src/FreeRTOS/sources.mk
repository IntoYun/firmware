# This file is a makefile included from the top level makefile which
# defines the sources built for the target.

# Define the prefix to this directory.
# Note: The name must be unique within this build and should be
#       based on the root of the project

# C source files included in this build.
CSRC += $(call target_files,$(TARGET_FREERTOS_PATH)/,*.c)
# enumerate target cpp files
CPPSRC +=

# ASM source files included in this build.
ASRC +=


