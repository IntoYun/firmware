MODULE=system_ex
SYSTEM_EX_MODULE_PATH=.
BUILD_PATH_EXT = $(SYSTEM_EX_BUILD_PATH_EXT)

DEPENDENCIES = hal services system wiring
ifeq ("","$(INTOROBOT_NO_PLATFORM)")
DEPENDENCIES += platform
CFLAGS += -DINTOROBOT_PLATFORM
endif
TARGET_TYPE = a


include ../build/arm-tlm.mk
