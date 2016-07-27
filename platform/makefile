MODULE=platform
PLATFORM_MODULE_PATH=.

WARNINGS_AS_ERRORS ?= y
# Target this makefile is building.
# makefile 编译的目标格式
TARGET_TYPE = a
BUILD_PATH_EXT = $(PLATFORM_BUILD_PATH_EXT)

# 不依靠其他模块
DEPENDENCIES = services

include ../build/arm-tlm.mk
