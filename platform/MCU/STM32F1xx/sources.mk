# This file is a makefile included from the top level makefile which
# defines the sources built for the target.

# Define the prefix to this directory.
# Note: The name must be unique within this build and should be
#       based on the root of the project
TARGET_INTOROBOT_SRC_PATH = $(TARGET_INTOROBOT_PATH)/src
TARGET_USB_FS_SRC_PATH = $(TARGET_USB_FS_PATH)/src
TARGET_STM32F1XX_HAL_SRC_PATH = $(TARGET_STM32F1XX_HAL_PATH)/Src

# C source files included in this build.
CSRC += $(call target_files,$(TARGET_INTOROBOT_SRC_PATH)/,*.c)
CSRC += $(call target_files,$(TARGET_USB_FS_SRC_PATH)/,*.c)
CSRC += $(call target_files,$(TARGET_USB_FS_PATH)/Class/CDC,*.c)
CSRC += $(call target_files,$(TARGET_USB_FS_PATH)/Class/DFU,*.c)
CSRC += $(call target_files,$(TARGET_USB_FS_PATH)/Core/Src,*.c)
CSRC += $(call target_files,$(TARGET_STM32F4XX_HAL_SRC_PATH)/,*.c)


# enumerate target cpp files
CPPSRC += $(call target_files,$(TARGET_INTOROBOT_SRC_PATH)/,*.cpp)

# ASM source files included in this build.
ASRC +=

