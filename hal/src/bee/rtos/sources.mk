# This file is a makefile included from the top level makefile which
# defines the sources built for the target.

# Define the prefix to this directory.
# Note: The name must be unique within this build and should be
#       based on the root of the project

# C source files included in this build.
TARGET_HAL_FREERTOS_SRC=$(TARGET_HAL_FREERTOS_PATH)/Source


CSRC += $(addsuffix .c,$(addprefix $(TARGET_HAL_FREERTOS_SRC)/,event_groups list queue tasks timers))
CSRC += $(call target_files,$(TARGET_HAL_FREERTOS_SRC)/portable/GCC/ARM_CM4F/,*.c)
# use malloc/free
CSRC += $(TARGET_HAL_FREERTOS_SRC)/portable/MemMang/heap_3.c

# enumerate target cpp files
CPPSRC +=

# ASM source files included in this build.
ASRC +=


