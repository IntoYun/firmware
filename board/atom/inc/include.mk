INCLUDE_DIRS += $(BOARD_TYPE_PATH)/inc
INCLUDE_DIRS += $(BOARD_TYPE_PATH)/inc/ros_lib

ifneq (,$(findstring board,$(MAKE_DEPENDENCIES)))

LDFLAGS += -T$(COMMON_BUILD)/linker/arm/linker_$(STM32_DEVICE_LC).ld

USE_PRINTF_FLOAT ?= y
ifeq ("$(USE_PRINTF_FLOAT)","y")
LDFLAGS += -u _printf_float
endif
LDFLAGS += -Wl,-Map,$(TARGET_BASE).map

ASRC += $(COMMON_BUILD)/startup/arm/startup_$(STM32_DEVICE_LC).S

endif
