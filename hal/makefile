
MODULE=hal
HAL_MODULE_PATH=.

# Target this makefile is building.
TARGET_TYPE = a
BUILD_PATH_EXT = $(HAL_BUILD_PATH_EXT)

# ensure PLATFORM_NAME is defined
include ../build/platform-id.mk

ifeq ($(PLATFORM_ID),0)# atom
DEPENDENCIES = wiring wiring_ex platform services
else
DEPENDENCIES = platform services
endif

-include $(HAL_MODULE_PATH)/src/$(PLATFORM_NAME)/tlm.mk


include ../build/arm-tlm.mk

