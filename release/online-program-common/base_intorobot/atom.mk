# 定义编译工具前缀
GCC_ARM_PATH ?= $(PROJECT_ROOT)/tools/gcc-arm-none-eabi/bin/
GCC_PREFIX ?= arm-none-eabi-

include common-tools.mk
-include common-arm.mk

## other flags

CFLAGS += -mcpu=cortex-m3
CFLAGS += -DSTM32F10X_MD
CFLAGS += -DUSE_STDPERIPH_DRIVER

LDFLAGS += --specs=nano.specs --specs=nosys.specs -lc -lnosys
LDFLAGS += -Wl,--start-group -lgcc -lc -Wl,--end-group

# FLAGS For Atom

CFLAGS += -I atom/inc/inc_board/
CFLAGS += -I atom/inc/inc_mcu/
CFLAGS += -I atom/inc/inc_board/ros_lib/
#CPPFLAGS =

LDFLAGS += -L atom/lib/ -lboard
LDFLAGS += -L atom/lib/ -lplatform
LDFLAGS += -T atom/linker/linker_stm32f10x_md.ld

STARTUP_OBJFILE += atom/startup/startup_stm32f10x_md.o

