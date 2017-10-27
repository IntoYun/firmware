# Uncomment the following to enable serial bitrate specific dfu/ymodem flasher in code
START_DFU_FLASHER_SERIAL_SPEED=19200
# Uncommenting this increase the size of the firmware image because of ymodem addition
START_YMODEM_FLASHER_SERIAL_SPEED=57600


include $(COMMON_BUILD)/firmlib_version.mk

QUOTE='

ifeq ("$(DEBUG_BUILD)","y")
CDEFINES += -DDEBUG_BUILD
COMPILE_LTO ?= n
else
CDEFINES += -DRELEASE_BUILD
endif

ifdef START_DFU_FLASHER_SERIAL_SPEED
CDEFINES += -DSTART_DFU_FLASHER_SERIAL_SPEED=$(START_DFU_FLASHER_SERIAL_SPEED)
endif
ifdef START_YMODEM_FLASHER_SERIAL_SPEED
CDEFINES += -DSTART_YMODEM_FLASHER_SERIAL_SPEED=$(START_YMODEM_FLASHER_SERIAL_SPEED)
endif

CDEFINES += -DINTOROBOT=1 -DINTOYUN=1

# disable COMPILE_LTO when JTAG is enabled since it obfuscates the symbol mapping
# breaking step debugging
ifeq ($(USE_SWD),y)
COMPILE_LTO ?= n
endif

ifeq ($(USE_SWD_JTAG),y)
COMPILE_LTO ?= n
endif

WARNINGS_AS_ERRORS ?= y
ifeq ($(WARNINGS_AS_ERRORS),y)
CFLAGS += -Werror
endif

# add include directories
CINCLUDES += $(patsubst %,-I%,$(INCLUDE_DIRS)) -I.
# Generate dependency files automatically.
CFLAGS += -MD -MP -MF $@.d

LDFLAGS += $(LIBS_EXT)
LDFLAGS += $(patsubst %,-L%,$(LIB_DIRS))

LDFLAGS += -Wl,--start-group $(patsubst %,-l%,$(LIBS)) -Wl,--end-group

ifeq (y,$(MODULAR_FIRMWARE))
MODULAR_EXT = -m
endif

COMPILE_LTO ?= n
ifeq (y,$(COMPILE_LTO))
LTO_EXT = -lto
endif

ifeq ("$(TARGET_TYPE)","a")
TARGET_FILE_PREFIX = lib
endif

# TARGET_FILE_NAME is the file name (minus extension) of the target produced
# TARGET_NAME is the final filename, including any prefix
TARGET_NAME ?= $(TARGET_FILE_PREFIX)$(TARGET_FILE_NAME)
TARGET_PATH ?= $(BUILD_PATH)/$(call sanitize,$(TARGET_DIR_NAME))

TARGET_BASE_DIR ?= $(TARGET_PATH)$(TARGET_SEP)
TARGET_BASE ?= $(TARGET_BASE_DIR)$(TARGET_NAME)
TARGET ?= $(TARGET_BASE).$(TARGET_TYPE)

# add BUILD_PATH_EXT with a preceeding slash if not empty.
BUILD_PATH ?= $(BUILD_PATH_BASE)/$(MODULE)$(and $(BUILD_PATH_EXT),/$(BUILD_PATH_EXT))

BUILD_TARGET_PLATFORM = platform-$(PLATFORM_ID)$(MODULAR_EXT)$(LTO_EXT)
BUILD_PATH_EXT ?= $(BUILD_TARGET_PLATFORM)

EXECUTABLE_EXTENSION=