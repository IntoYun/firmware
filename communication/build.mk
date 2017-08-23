
# all build.mk files are loaded recursively
# This project has these build.mk files which act as "gatekeepers"
# pulling in the required sources.
# (Include files are selected in import.mk)

COMMUNICATION_MODULE_PATH ?= $(PROJECT_ROOT)/$(COMMUNICATION_MODULE_NAME)
include $(call rwildcard,$(COMMUNICATION_MODULE_PATH)/$(PLATFORM_COMM)/,sources.mk)

