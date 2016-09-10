# this imports the various paths provided by this module into another module
# note that MODULE is not set to this module but the importing module
# 包括本模块的各种路径的定义，通过该文件可以把这些文件导入到其他模块文件
# 本文件除了MODULE没有定义外，定义了一下变量

SUBSYS_MODULE_NAME = subsys
SUBSYS_MODULE_PATH ?= $(PROJECT_ROOT)/$(SUBSYS_MODULE_NAME)
SUBSYS_BUILD_PATH_EXT = $(BUILD_TARGET_PLATFORM)

# pull in the includes/sources corresponding to the target platform
# 包含本模块头文件
include $(call rwildcard,$(SUBSYS_MODULE_PATH)/$(PLATFORM_NAME)/inc/,include.mk)

SUBSYS_BUILD_PATH_EXT=$(BUILD_TARGET_PLATFORM)$(SUBSYS_TEST_FLAVOR)
SUBSYS_LIB_DIR = $(BUILD_PATH_BASE)/$(SUBSYS_MODULE_NAME)/$(SUBSYS_BUILD_PATH_EXT)
SUBSYS_LIB_DEP = $(SUBSYS_LIB_DIR)/lib$(SUBSYS_MODULE_NAME).a

