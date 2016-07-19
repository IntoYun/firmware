INCLUDE_DIRS += $(BOARD_TYPE_PATH)/inc

ifneq (,$(findstring board,$(MAKE_DEPENDENCIES)))

LDFLAGS += -Tlinker_$(STM32_DEVICE_LC).ld
LDFLAGS += -L$(COMMON_BUILD)/linker/arm


USE_PRINTF_FLOAT ?= y
ifeq ("$(USE_PRINTF_FLOAT)","y")
LDFLAGS += -u _printf_float
endif

LDFLAGS += -Wl,-Map,$(TARGET_BASE).map

ASRC += $(COMMON_BUILD)/startup/arm/startup_$(STM32_DEVICE_LC).S

LIB_DIRS += $(BOARD_TYPE_PATH)/lib/
LIBS += PDMFilter_CM4_GCC


endif
