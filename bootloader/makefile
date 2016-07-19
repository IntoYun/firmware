MODULE=bootloader
BOOTLOADER_MODULE_PATH=.
#PLATFORM_DFU=0x8000000
#BUILD_PATH_EXT = $(BOOTLOADER_BUILD_PATH_EXT)

DEPENDENCIES = platform
MAKE_DEPENDENCIES = platform
# dependent on HAL headers, but not the library
LIBS += $(MAKE_DEPENDENCIES)
LIB_DEPS += $(PLATFORM_LIB_DEP)
LIB_DIRS += $(dir $(LIB_DEPS))

export COMPILE_LTO=n

include ../build/platform-id.mk

# Target this makefile is building.
ifeq ("$(ARCH)","arm")
TARGET=elf bin hex size
else
ifeq ("$(ARCH)","xtensa")
TARGET=elf bin
else
TARGET=none
endif
endif

TARGET_FILE_NAME ?= $(PLATFORM)-boot

include ../build/arm-tlm.mk

