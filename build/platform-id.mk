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
ifeq ("$(PLATFORM)","atom")
PLATFORM_ID = 0
endif

ifeq ("$(PLATFORM)","neutron")
PLATFORM_ID = 1
endif

ifeq ("$(PLATFORM)","gcc")
PLATFORM_ID = 2
endif

ifeq ("$(PLATFORM)","neutron-net")
PLATFORM_ID = 3
endif

ifeq ("$(PLATFORM)","nut")
PLATFORM_ID = 4
endif

ifeq ("$(PLATFORM)","lora")
PLATFORM_ID = 5
endif

ifeq ("$(PLATFORM)","newhal")
PLATFORM_ID=60000
endif

ifeq (,$(PLATFORM_ID))
$(error "Unknown platform: $(PLATFORM))
endif
endif

ifndef PLATFORM_ID
PLATFORM_ID=0
endif

# Determine which is the target device

ARCH=arm

ifeq ("$(PLATFORM_ID)","0") #atom
PLATFORM=atom
PLATFORM_DEVICE=STM32F103xB
PLATFORM_NAME=atom
PLATFORM_MCU=STM32F1xx
MCU_CORE=cortex-m3
PRODUCT_DESC=IntoRobot Atom, 128k flash, 20k sram
PLATFORM_DFU = 0x08000000
DEFAULT_PRODUCT_ID=0
endif

ifeq ("$(PLATFORM_ID)","1") #neutron
PLATFORM=neutron
PLATFORM_DEVICE=STM32F411xE
PLATFORM_NAME=neutron
PLATFORM_MCU=STM32F4xx
MCU_CORE=cortex-m4
PRODUCT_DESC=IntoRobot neutron, 512k flash, 128k sram
PLATFORM_DFU = 0x08020000
PLATFORM_THREADING=1
DEFAULT_PRODUCT_ID=1
endif

ifeq ("$(PLATFORM_ID)","2") # gcc
ARCH=gcc
PLATFORM=gcc
PLATFORM_DEVICE=gcc
PLATFORM_NAME=gcc
PLATFORM_MCU=gcc
PRODUCT_DESC=GCC xcompile
# explicitly exclude platform headers
INTOROBOT_NO_PLATFORM=1
DEFAULT_PRODUCT_ID=2
endif

ifeq ("$(PLATFORM_ID)","3") #neutron-net
ARCH=xtensa
PLATFORM=neutron-net
PLATFORM_DEVICE=neutron-net
PLATFORM_NAME=neutron-net
PLATFORM_MCU=ESP8266
PRODUCT_DESC=ESP8266 4MB Flash
DEFAULT_PRODUCT_ID=3
endif

ifeq ("$(PLATFORM_ID)","4") #nut
ARCH=xtensa
PLATFORM=nut
PLATFORM_DEVICE=nut
PLATFORM_NAME=nut
PLATFORM_MCU=ESP8266
PRODUCT_DESC=IntoRobot nut, ESP8266, 4MB Flash
DEFAULT_PRODUCT_ID=4
endif

ifeq ("$(PLATFORM_ID)","5") #lora
PLATFORM=lora
PLATFORM_DEVICE=STM32L151xB
PLATFORM_NAME=lora
PLATFORM_MCU=STM32L1xx
MCU_CORE=cortex-m3
PRODUCT_DESC=IntoRobot neutron, 128k flash, 16k sram
PLATFORM_DFU = 0x08000000
DEFAULT_PRODUCT_ID=5
endif


ifeq ("$(PLATFORM_ID)","60000") #newhal
PLATFORM=newhal
# needed for conditional compilation of some stm32 specific files
PLATFORM_DEVICE=newhalcpu
# used to define the sources in hal/src/new-hal
PLATFORM_NAME=newhal
# define MCU-specific platform defines under platform/MCU/new-hal
PLATFORM_MCU=newhal-mcu
PLATFORM_NET=not-defined
PRODUCT_DESC=Test platform for producing a new HAL implementation
USBD_VID_INTOROBOT=0x1D50
USBD_PID_DFU=0x607F
USBD_PID_CDC=0x607D
DEFAULT_PRODUCT_ID=60000
endif

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

ifeq ("$(ARCH)","arm")
MCU_CORE ?= cortex-m3
PLATFORM_DFU ?= 0x08000000
# needed for conditional compilation of syshealth_hal.h
CDEFINES += -DPLATFORM_DEVICE
# needed for conditional compilation of some specific files
CDEFINES += -D$(PLATFORM_DEVICE)
CDEFINES += -DUSBD_VID_INTOROBOT=$(USBD_VID_INTOROBOT)
CDEFINES += -DUSBD_PID_DFU=$(USBD_PID_DFU)
CDEFINES += -DUSBD_PID_CDC=$(USBD_PID_CDC)
endif

PLATFORM_THREADING ?= 0
CDEFINES += -DPLATFORM_THREADING=$(PLATFORM_THREADING)
CDEFINES += -DPLATFORM_ID=$(PLATFORM_ID) -DPLATFORM_NAME=$(PLATFORM_NAME)

MODULE_FUNCTION_NONE            :=0
MODULE_FUNCTION_RESOURCE        :=1
MODULE_FUNCTION_BOOTLOADER      :=2
MODULE_FUNCTION_MONO_FIRMWARE   :=3
MODULE_FUNCTION_SYSTEM_PART     :=4
MODULE_FUNCTION_USER_PART       :=5


endif
