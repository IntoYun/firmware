# all build.mk files are loaded recursively
# This project has these build.mk files which act as "gatekeepers" only
# pulling in the required sources.
# (Include files are selected in import.mk)

SUBSYS_SRC_PATH = $(SUBSYS_MODULE_PATH)/$(PLATFORM_NAME)/src
include $(call rwildcard,$(SUBSYS_SRC_PATH)/,sources.mk)


