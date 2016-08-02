
TARGET_HAL_SRC_PATH = $(TARGET_HAL_PATH)/src/$(PLATFORM_NAME)

CSRC += $(call here_files,$(TARGET_HAL_SRC_PATH)/,*.c)

CPPSRC += $(call here_files,$(TARGET_HAL_SRC_PATH)/,*.cpp)

# ASM source files included in this build.
ASRC +=

