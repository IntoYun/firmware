
# all build.mk files are loaded recursively
# This project has these build.mk files which act as "gatekeepers"
# pulling in the required sources.
# (Include files are selected in import.mk)

SYSTEM_EX_SRC_PATH = $(SYSTEM_EX_MODULE_PATH)/$(PLATFORM_NAME)/src
include $(call rwildcard,$(SYSTEM_EX_SRC_PATH)/,sources.mk)
