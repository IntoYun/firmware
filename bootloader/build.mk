# select sources from platform
#ASFLAGS +=  -Wa,--defsym -Wa,SPARK_INIT_STARTUP=1

BOOTLOADER_TYPE_PATH = $(BOOTLOADER_MODULE_PATH)/$(PLATFORM)
include $(call rwildcard,$(BOOTLOADER_TYPE_PATH)/,sources.mk)


