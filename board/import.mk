# this imports the various paths provided by this module into another module
# note that MODULE is not set to this module but the importing module
# 包括本模块的各种路径的定义，通过该文件可以把这些文件导入到其他模块文件
# 本文件除了MODULE没有定义外，定义了一下变量

BOARD_MODULE_NAME = board
BOARD_MODULE_PATH ?= $(PROJECT_ROOT)/$(BOARD_MODULE_NAME)
BOARD_BUILD_PATH_EXT = $(BUILD_TARGET_PLATFORM)

BOARD_LIB_DIR = $(BUILD_PATH_BASE)/$(BOARD_MODULE_NAME)/$(BOARD_BUILD_PATH_EXT)
BOARD_LIB_DEP = $(BOARD_LIB_DIR)/lib$(BOARD_MODULE_NAME).a

# Target specific defines
#CDEFINES += -DDFU_BUILD_ENABLE

ifeq ("$(USE_SWD_JTAG)","y")
CDEFINES += -DUSE_SWD_JTAG
endif

ifeq ("$(USE_SWD)","y")
CDEFINES += -DUSE_SWD
endif

# pull in the includes/sources corresponding to the target platform
# 包含本模块头文件
BOARD_TYPE_PATH=$(BOARD_MODULE_PATH)/$(PLATFORM)
include $(call rwildcard,$(BOARD_TYPE_PATH)/,include.mk)

