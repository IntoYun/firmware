
# Define the prefix to this directory.
# Note: The name must be unique within this build and should be
#       based on the root of the project
TARGET_HAL_STM32L1XX_SHARE_PATH = $(TARGET_HAL_PATH)/src/stm32l1xx-share

INCLUDE_DIRS += $(TARGET_HAL_STM32L1XX_SHARE_PATH)
include $(TARGET_HAL_PATH)/src/stm32/include.mk

# if hal is used as a make dependency (linked) then add linker commands
ifneq (,$(findstring hal,$(MAKE_DEPENDENCIES)))

LDFLAGS += -Tlinker_$(PLATFORM_DEVICE_LC)_app.ld
LDFLAGS += -L$(COMMON_BUILD)/linker/arm/$(PLATFORM_NAME)
LDFLAGS += -Wl,--defsym,__STACKSIZE__=400
LDFLAGS += --specs=nano.specs --specs=nosys.specs

USE_PRINTF_FLOAT = n
ifeq ("$(USE_PRINTF_FLOAT)","y")
LDFLAGS += -u _printf_float
endif
LDFLAGS += -Wl,-Map,$(TARGET_BASE).map

LIBS += gcc c

# assembler startup script
ASRC_STARTUP += $(COMMON_BUILD)/startup/arm/$(PLATFORM_NAME)/startup_$(PLATFORM_DEVICE_LC).S
ASFLAGS += -I$(COMMON_BUILD)/startup/arm
ASFLAGS += -Wa,--defsym -Wa,INTOROBOT_INIT_STARTUP=1
#
endif

