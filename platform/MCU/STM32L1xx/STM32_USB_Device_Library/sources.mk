# This file is a makefile included from the top level makefile which
# defines the sources built for the target.

# Define the prefix to this directory.
# Note: The name must be unique within this build and should be
#       based on the root of the project

# C source files included in this build.
CSRC += $(call target_files,$(TARGET_USB_FS_PATH)/Class/CDC,*.c)
CSRC += $(call target_files,$(TARGET_USB_FS_PATH)/Class/DFU,*.c)
CSRC += $(call target_files,$(TARGET_USB_FS_PATH)/Core/Src,*.c)

# C++ source files included in this build.
CPPSRC +=

# ASM source files included in this build.
ASRC +=


