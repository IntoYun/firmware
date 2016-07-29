
TARGET_SERVICES_SRC_PATH = $(SERVICES_MODULE_PATH)/src

CPPSRC += $(call target_files,$(TARGET_SERVICES_SRC_PATH),*.cpp)
CSRC += $(call target_files,$(TARGET_SERVICES_SRC_PATH),*.c)

#BUILTINS_EXCLUDE = malloc free realloc
#CFLAGS += $(addprefix -fno-builtin-,$(BUILTINS_EXCLUDE))
