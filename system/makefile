
MODULE=system
SYSTEM_MODULE_PATH=.
BUILD_PATH_EXT = $(SYSTEM_BUILD_PATH_EXT)

ifeq ("","$(INTOROBOT_NO_PLATFORM)")
DEPENDENCIES += platform
CFLAGS += -DSPARK_PLATFORM
endif
DEPENDENCIES = platform hal services wiring
TARGET_TYPE = a


include ../build/arm-tlm.mk
