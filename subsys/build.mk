# all build.mk files are loaded recursively
# This project has these build.mk files which act as "gatekeepers" only
# pulling in the required sources.
# (Include files are selected in import.mk)

BOARD_TYPE_PATH = $(BOARD_MODULE_PATH)/$(PLATFORM)
include $(call rwildcard,$(BOARD_TYPE_PATH)/,sources.mk)


