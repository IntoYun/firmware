
include $(NEWLIBNANO_MODULE_PATH)/src/sources.mk

CFLAGS += -DHAVE_MMAP=0
ifneq ("$(PLATFORM)","gcc")
CFLAGS += -Wno-unused-but-set-variable
CFLAGS += -Wno-unused-variable
endif

