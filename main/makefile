MODULE=main
MAIN_MODULE_PATH=.

include ../build/platform-id.mk
include ../build/top-level-module.mk
include ../build/macros.mk

ifeq ($(PLATFORM_ID),0)
export COMPILE_LTO?=n
endif

ifeq ($(PLATFORM_ID),1)
export COMPILE_LTO?=n
endif

PLATFORM_DFU_LEAVE = y
BUILD_PATH_EXT = $(BUILD_TARGET_PLATFORM)$(USER_FLAVOR)

USE_PRINTF_FLOAT ?= y

# It's critical that user comes before wiring so that the setup()/loop() functions are linked in preference
# to the weak functions from wiring
ifeq ($(PLATFORM_ID),3)# neutron-net
MAKE_DEPENDENCIES = platform board
else
#MAKE_DEPENDENCIES = newlib_nano user wiring hal system services platform
MAKE_DEPENDENCIES = user wiring hal system services platform
endif

DEPENDENCIES = $(MAKE_DEPENDENCIES)

LIBS += $(MAKE_DEPENDENCIES)
LIB_DEPS += $(USER_LIB_DEP) $(WIRING_LIB_DEP) $(SYSTEM_LIB_DEP) $(SERVICES_LIB_DEP) $(HAL_LIB_DEP) $(PLATFORM_LIB_DEP)
LIB_DIRS += $(dir $(LIB_DEPS))

# Target this makefile is building.
ifeq ("$(ARCH)","arm")
TARGET=elf bin hex size
else
ifeq ("$(ARCH)","xtensa")
TARGET=elf bin size
else
TARGET=exe
endif
endif

ifeq ($(PLATFORM_ID),3)# neutron-net neutron esp8266程序 不是默认应用程序
TARGET_FILE_NAME ?= $(PLATFORM)
else
TARGET_FILE_NAME ?= default-$(PLATFORM)
endif

include ../build/arm-tlm.mk

