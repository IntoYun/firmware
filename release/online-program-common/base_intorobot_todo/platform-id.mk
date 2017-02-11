ifeq ($(included_productid_mk),)
included_productid_mk := 1

# defines
# PLATFORM_NAME - a unique name for the platform, can be used to organise sources
#                 by platform
# PLATFORM_MCU  - an identifier for the MCU family
# PLATFORM_NET  - the network subsystem
# PLATFORM_DEVICE  - the specific device being targeted for platform builds
# ARCH		- architecture (ARM/GCC)
# PRODUCT_DESC  - text description of the product ID
# PLATFORM_DYNALIB_MODULES - if the device supports a modular build, the name
#		- of the subdirectory containing

# Default USB Device Vendor ID for Intorobot Products
USBD_VID_INTOROBOT=0x0483
# Default USB Device Product ID for DFU Class
USBD_PID_DFU=0xdf11
# Default USB Device Product ID for CDC Class
USBD_PID_CDC=0x5740

ifneq (,$(PLATFORM))

################创客核心板####################
ifeq ("$(PLATFORM)","atom-old")
PLATFORM_ID = 888001
endif

ifeq ("$(PLATFORM)","neutron")
PLATFORM_ID = 888002
endif

ifeq ("$(PLATFORM)","nut")
PLATFORM_ID = 888003
endif

ifeq ("$(PLATFORM)","atom")
PLATFORM_ID = 888004
endif

ifeq ("$(PLATFORM)","fig")
PLATFORM_ID = 888005
endif

ifeq ("$(PLATFORM)","lora")
PLATFORM_ID = 888006
endif

ifeq ("$(PLATFORM)","littlebee")
PLATFORM_ID = 888007
endif

ifeq ("$(PLATFORM)","gprs")
PLATFORM_ID = 888008
endif

#add 此处添加创客核心板

################商业模块####################
ifeq ("$(PLATFORM)","w67")
PLATFORM_ID = 888101
endif

ifeq ("$(PLATFORM)","w323")
PLATFORM_ID = 888102
endif

ifeq ("$(PLATFORM)","l6")
PLATFORM_ID = 888103
endif

#add  此处添加模块


ifeq (,$(PLATFORM_ID))
$(error "Unknown platform: $(PLATFORM))
endif
endif

ifndef PLATFORM_ID
PLATFORM_ID=0
endif

# Determine which is the target device

PLATFORM_ARCH=arm

################创客核心板####################
ifeq ("$(PLATFORM_ID)","888001") #atom-old without bootloader
PLATFORM=atom
PLATFORM_DEVICE=STM32F10X_MD
PLATFORM_NAME=atom
PLATFORM_MCU=STM32F1xx
MCU_CORE=cortex-m3
PRODUCT_DESC=IntoRobot Atom without bootloader, 128k flash, 20k sram
PLATFORM_BOOT_ADDR=0x08000000
PLATFORM_APP_ADDR=0x08000000
DEFAULT_PRODUCT_ID=888001
endif

ifeq ("$(PLATFORM_ID)","888002") #neutron
PLATFORM=neutron
PLATFORM_DEVICE=STM32F411xE
PLATFORM_NAME=neutron
PLATFORM_MCU=STM32F4xx
MCU_CORE=cortex-m4
PRODUCT_DESC=IntoRobot neutron, 512k flash, 128k sram
PLATFORM_BOOT_ADDR=0x08000000
PLATFORM_APP_ADDR=0x08020000
PLATFORM_THREADING=1
DEFAULT_PRODUCT_ID=888002
endif

ifeq ("$(PLATFORM_ID)","888003") #nut
PLATFORM_ARCH=xtensa-lx106
PLATFORM=nut
PLATFORM_DEVICE=nut
PLATFORM_NAME=nut
PLATFORM_MCU=ESP8266-Arduino
PRODUCT_DESC=IntoRobot nut, ESP8266, 4MB Flash
PLATFORM_BOOT_ADDR=0x00000
PLATFORM_APP_ADDR=0x14000
DEFAULT_PRODUCT_ID=888003
endif

ifeq ("$(PLATFORM_ID)","888004") #atom with bootloader
PLATFORM=atom
PLATFORM_DEVICE=STM32F103xB
PLATFORM_NAME=atom
PLATFORM_MCU=STM32F1xx
MCU_CORE=cortex-m3
PRODUCT_DESC=IntoRobot Atom with bootloader, 128k flash, 20k sram
PLATFORM_BOOT_ADDR=0x08000000
PLATFORM_APP_ADDR=0x08007000
DEFAULT_PRODUCT_ID=888004
endif

ifeq ("$(PLATFORM_ID)","888005") #fig
PLATFORM_ARCH=xtensa-esp32
PLATFORM=fig
PLATFORM_DEVICE=fig
PLATFORM_NAME=fig
PLATFORM_MCU=ESP32-Arduino
PRODUCT_DESC=IntoRobot fig, ESP32, 4MB Flash
PLATFORM_BOOT_ADDR=0x00000
PLATFORM_APP_ADDR=0x10000
DEFAULT_PRODUCT_ID=888005
endif

ifeq ("$(PLATFORM_ID)","888006") #lora
PLATFORM=lora
PLATFORM_DEVICE=STM32L151xB
PLATFORM_NAME=lora
PLATFORM_MCU=STM32L1xx
MCU_CORE=cortex-m3
PRODUCT_DESC=IntoRobot lora, 128k flash, 16k sram
PLATFORM_BOOT_ADDR = 0x08000000
PLATFORM_APP_ADDR = 0x08006000
DEFAULT_PRODUCT_ID=888006
endif

ifeq ("$(PLATFORM_ID)","888007") #littlebee
PLATFORM=littlebee
PLATFORM_DEVICE=STM32F411xE
PLATFORM_NAME=littlebee
PLATFORM_MCU=STM32F4xx
MCU_CORE=cortex-m4
PRODUCT_DESC=IntoRobot be, 512k flash, 128k sram
PLATFORM_BOOT_ADDR=0x08000000
PLATFORM_APP_ADDR=0x08020000
PLATFORM_THREADING=1
DEFAULT_PRODUCT_ID=888007
endif

ifeq ("$(PLATFORM_ID)","888008") #gprs
PLATFORM=gprs
PLATFORM_DEVICE=STM32L151xB
PLATFORM_NAME=gprs
PLATFORM_MCU=STM32L1xx
MCU_CORE=cortex-m3
PRODUCT_DESC=IntoRobot gprs, 128k flash, 16k sram
PLATFORM_BOOT_ADDR=0x08000000
PLATFORM_APP_ADDR=0x08008000
DEFAULT_PRODUCT_ID=888007
endif

# 此处新增创客核心板

################商业模块####################
ifeq ("$(PLATFORM_ID)","888101") #w67
PLATFORM_ARCH=xtensa-lx106
PLATFORM=w67
PLATFORM_DEVICE=w67
PLATFORM_NAME=w67
PLATFORM_MCU=ESP8266-Arduino
PRODUCT_DESC=IntoRobot w67, ESP8266, 4MB Flash
PLATFORM_BOOT_ADDR=0x00000
PLATFORM_APP_ADDR=0x14000
DEFAULT_PRODUCT_ID=888101
endif

ifeq ("$(PLATFORM_ID)","888102") #w323
PLATFORM_ARCH=xtensa-esp32
PLATFORM=w323
PLATFORM_DEVICE=w323
PLATFORM_NAME=w323
PLATFORM_MCU=ESP32-Arduino
PRODUCT_DESC=IntoRobot w323, ESP32, 4MB Flash
PLATFORM_BOOT_ADDR=0x00000
PLATFORM_APP_ADDR=0x10000
DEFAULT_PRODUCT_ID=888102
endif

ifeq ("$(PLATFORM_ID)","888103") #l6
PLATFORM=l6
PLATFORM_DEVICE=STM32L151xB
PLATFORM_NAME=l6
PLATFORM_MCU=STM32L1xx
MCU_CORE=cortex-m3
PRODUCT_DESC=IntoRobot lora, 128k flash, 16k sram
PLATFORM_BOOT_ADDR=0x08000000
PLATFORM_APP_ADDR=0x08006000
DEFAULT_PRODUCT_ID=888103
endif
# 此处新增模块

ifeq ("$(PLATFORM_MCU)","")
$(error PLATFORM_MCU not defined. Check platform id $(PLATFORM_ID))
endif

# lower case version of the PLATFORM_DEVICE string for use in filenames
PLATFORM_DEVICE_LC  = $(shell echo $(PLATFORM_DEVICE) | tr A-Z a-z)

ifeq ("$(PRODUCT_ID)","")
# ifeq (,$(submake))
# $(info PRODUCT_ID not defined - using default: $(DEFAULT_PRODUCT_ID))
# endif
PRODUCT_ID = $(DEFAULT_PRODUCT_ID)
endif

ifeq ("$(PLATFORM_ARCH)","arm")
MCU_CORE ?= cortex-m3
# needed for conditional compilation of syshealth_hal.h
CDEFINES += -DPLATFORM_DEVICE
# needed for conditional compilation of some specific files
CDEFINES += -D$(PLATFORM_DEVICE)
CDEFINES += -DUSBD_VID_INTOROBOT=$(USBD_VID_INTOROBOT)
CDEFINES += -DUSBD_PID_DFU=$(USBD_PID_DFU)
CDEFINES += -DUSBD_PID_CDC=$(USBD_PID_CDC)
endif

ifeq ("$(PLATFORM_ARCH)","arm")
CDEFINES += -DINTOROBOT_ARCH_ARM
else ifeq ($(strip $(PLATFORM_ARCH)),$(filter $(PLATFORM_ARCH),xtensa-lx106 xtensa-esp32))
CDEFINES += -DINTOROBOT_ARCH_XTENSA
endif

PLATFORM_THREADING ?= 0
PLATFORM_BOOTLOADER ?= 1
CDEFINES += -DPLATFORM_THREADING=$(PLATFORM_THREADING)
CDEFINES += -DPLATFORM_ID=$(PLATFORM_ID) -DPLATFORM_NAME=$(PLATFORM_NAME)

PRODUCT_FIRMWARE_VERSION ?= 65535
CDEFINES += -DPRODUCT_ID=$(PRODUCT_ID)
CDEFINES += -DPRODUCT_FIRMWARE_VERSION=$(PRODUCT_FIRMWARE_VERSION)

MODULE_FUNCTION_NONE            :=0
MODULE_FUNCTION_RESOURCE        :=1
MODULE_FUNCTION_BOOTLOADER      :=2
MODULE_FUNCTION_MONO_FIRMWARE   :=3
MODULE_FUNCTION_SYSTEM_PART     :=4
MODULE_FUNCTION_USER_PART       :=5

endif
