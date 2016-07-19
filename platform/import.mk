# this imports the various paths provided by this module into another module
# note that MODULE is not set to this module but the importing module
# 包括本模块的各种路径的定义，通过该文件可以把这些文件导入到其他模块文件
# 本文件除了MODULE没有定义外，定义了一下变量

PLATFORM_MODULE_NAME = platform
PLATFORM_MODULE_PATH ?= $(PROJECT_ROOT)/$(PLATFORM_MODULE_NAME)
PLATFORM_BUILD_PATH_EXT = $(BUILD_TARGET_PLATFORM)

PLATFORM_LIB_DIR = $(BUILD_PATH_BASE)/$(PLATFORM_MODULE_NAME)/$(PLATFORM_BUILD_PATH_EXT)
PLATFORM_LIB_DEP = $(PLATFORM_LIB_DIR)/lib$(PLATFORM_MODULE_NAME).a

ifeq ("$(ARCH)","arm")
# Target specific defines
ifeq ($(PLATFORM_ID),0)
CDEFINES += -DUSE_STDPERIPH_DRIVER
else
CDEFINES += -DUSE_HAL_DRIVER
endif

#CDEFINES += -DDFU_BUILD_ENABLE

ifeq ("$(USE_SWD_JTAG)","y")
CDEFINES += -DUSE_SWD_JTAG
endif

ifeq ("$(USE_SWD)","y")
CDEFINES += -DUSE_SWD
endif

endif
# pull in the includes/sources corresponding to the target platform
# 包含本模块头文件
PLATFORM_MCU_PATH=$(PLATFORM_MODULE_PATH)/MCU/$(PLATFORM_MCU)
include $(call rwildcard,$(PLATFORM_MCU_PATH)/,include.mk)
