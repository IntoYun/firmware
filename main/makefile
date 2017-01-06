MODULE=main
MAIN_MODULE_PATH=.

include ../build/platform-id.mk
include ../build/top-level-module.mk
include ../build/macros.mk

export COMPILE_LTO?=n

PLATFORM_DFU_LEAVE = y
BUILD_PATH_EXT = $(BUILD_TARGET_PLATFORM)$(USER_FLAVOR)

USE_PRINTF_FLOAT ?= y

# It's critical that user comes before wiring so that the setup()/loop() functions are linked in preference
# to the weak functions from wiring
ifeq ($(PLATFORM_THREADING),1)
MAKE_DEPENDENCIES = newlib_nano
endif

ifeq ("$(PLATFORM)","neutron-net")# neutron-net
MAKE_DEPENDENCIES += platform subsys
LIB_DEPS += $(PLATFORM_LIB_DEP) $(SUBSYS_LIB_DEP)
else
MAKE_DEPENDENCIES += user wiring wiring_ex hal system services platform
LIB_DEPS += $(USER_LIB_DEP) $(WIRING_LIB_DEP) $(WIRING_EX_LIB_DEP) $(HAL_LIB_DEP) $(SYSTEM_LIB_DEP) $(SERVICES_LIB_DEP) $(PLATFORM_LIB_DEP)
endif

DEPENDENCIES = $(MAKE_DEPENDENCIES)

LIBS += $(MAKE_DEPENDENCIES)
LIB_DIRS += $(dir $(LIB_DEPS))

# Target this makefile is building.
ifeq ("$(PLATFORM_ARCH)","arm")
TARGET=elf bin hex size
else ifeq ($(strip $(PLATFORM_ARCH)),$(filter $(PLATFORM_ARCH),xtensa-lx106 xtensa-esp32))
TARGET=elf bin size
else
TARGET=exe
endif

ifeq ("$(PLATFORM)","neutron-net")# neutron-net neutron esp8266程序 不是默认应用程序
TARGET_FILE_NAME ?= $(PLATFORM)
else
TARGET_FILE_NAME ?= default-$(PLATFORM)
endif

include ../build/arm-tlm.mk

