MODULE=user
USER_MODULE_PATH=.

BUILD_PATH_EXT = $(USER_BUILD_PATH_EXT)

include ../build/platform-id.mk

DEPENDENCIES = platform board

WARNINGS_AS_ERRORS ?= n

# Target this makefile is building.
TARGET_TYPE=a

include ../build/arm-tlm.mk
