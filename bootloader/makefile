MODULE=bootloader
BOOTLOADER_MODULE_PATH=.

export COMPILE_LTO=n

include ../build/platform-id.mk

ifeq ("$(ARCH)","arm")
MAKE_DEPENDENCIES = platform services
LIB_DEPS += $(PLATFORM_LIB_DEP) $(SERVICES_LIB_DEP)
else
MAKE_DEPENDENCIES = platform
LIB_DEPS += $(PLATFORM_LIB_DEP)
endif
DEPENDENCIES = platform hal services
# dependent on HAL headers, but not the library
LIBS += $(MAKE_DEPENDENCIES)
LIB_DIRS += $(dir $(LIB_DEPS))

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

