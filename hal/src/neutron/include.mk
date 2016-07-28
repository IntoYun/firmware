
# Define the prefix to this directory.
# Note: The name must be unique within this build and should be
#       based on the root of the project
HAL_SRC_NEUTRON_INCL_PATH = $(TARGET_HAL_PATH)/src/neutron
HAL_INCL_STM32_PATH = $(TARGET_HAL_PATH)/src/stm32

INCLUDE_DIRS += $(HAL_SRC_NEUTRON_INCL_PATH)
INCLUDE_DIRS += $(HAL_INCL_STM32_PATH)

HAL_LINK ?= $(findstring hal,$(MAKE_DEPENDENCIES))

# if hal is used as a make dependency (linked) then add linker commands
ifneq (,$(HAL_LINK))

#LDFLAGS += -Tlinker_$(STM32_DEVICE_LC)_dfu.ld
LDFLAGS += -Tlinker_$(STM32_DEVICE_LC).ld
LDFLAGS += -L$(COMMON_BUILD)/linker/arm
LINKER_DEPS += $(NEWLIB_TWEAK_SPECS)
#LDFLAGS += --specs=nano.specs --specs=$(NEWLIB_TWEAK_SPECS)
LDFLAGS += --specs=nano.specs
LDFLAGS += -Wl,--defsym,__STACKSIZE__=1400

# support for external linker file

# todo - factor out common code with photon include.mk
USE_PRINTF_FLOAT ?= n
ifeq ("$(USE_PRINTF_FLOAT)","y")
LDFLAGS += -u _printf_float
endif

LDFLAGS += -Wl,-Map,$(TARGET_BASE).map
#LDFLAGS += -u uxTopUsedPriority
#
# assembler startup script
ASRC += $(COMMON_BUILD)/startup/arm/startup_$(STM32_DEVICE_LC).S
ASFLAGS += -I$(COMMON_BUILD)/startup/arm
ASFLAGS +=  -Wa,--defsym -Wa,INTOROBOT_INIT_STARTUP=1
#
endif
