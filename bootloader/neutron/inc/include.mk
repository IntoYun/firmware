INCLUDE_DIRS += $(BOOTLOADER_TYPE_PATH)/inc

LDFLAGS += -Tlinker_$(STM32_DEVICE_LC)_boot.ld
LDFLAGS += -L$(COMMON_BUILD)/linker/arm


LDFLAGS += -Wl,-Map,$(TARGET_BASE).map

ASRC += $(COMMON_BUILD)/startup/arm/startup_$(STM32_DEVICE_LC).S

