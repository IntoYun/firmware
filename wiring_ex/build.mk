
# all build.mk files are loaded recursively
# This project has these build.mk files which act as "gatekeepers"
# pulling in the required sources.
# (Include files are selected in import.mk)

WIRING_EX_SRC_PATH = $(WIRING_EX_MODULE_PATH)/$(PLATFORM_NAME)/src
include $(call rwildcard,$(WIRING_EX_SRC_PATH)/,sources.mk)

#BUILTINS_EXCLUDE = malloc free realloc
#CFLAGS += $(addprefix -fno-builtin-,$(BUILTINS_EXCLUDE))

