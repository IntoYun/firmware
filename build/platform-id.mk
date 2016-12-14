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

ifeq ("$(PLATFORM)","gprs")
PLATFORM_ID = 888007
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

################其他####################
ifeq ("$(PLATFORM)","gcc")
PLATFORM_ID = 888201
endif

ifeq ("$(PLATFORM)","neutron-net")
PLATFORM_ID = 888202
endif

ifeq ("$(PLATFORM)","anytest")
PLATFORM_ID = 888203
endif
#add  此处添加其他

################示例####################
ifeq ("$(PLATFORM)","newhal")
PLATFORM_ID = 600000
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

################创客核心板####################
ifeq ("$(PLATFORM_ID)","888002") #neutron
PLATFORM=neutron
PLATFORM_DEVICE=STM32F411xE
PLATFORM_NAME=neutron
PLATFORM_MCU=STM32F4xx
MCU_CORE=cortex-m4
PRODUCT_DESC=IntoRobot neutron, 512k flash, 128k sram
PLATFORM_DFU = 0x08020000
PLATFORM_THREADING=1
DEFAULT_PRODUCT_ID=888002
endif

ifeq ("$(PLATFORM_ID)","888003") #nut
ARCH=xtensa-lx106
PLATFORM=nut
PLATFORM_DEVICE=nut
PLATFORM_NAME=nut
PLATFORM_MCU=ESP8266-Arduino
PRODUCT_DESC=IntoRobot nut, ESP8266, 4MB Flash
DEFAULT_PRODUCT_ID=888003
endif

ifeq ("$(PLATFORM_ID)","888004") #atom  bootloader
PLATFORM=atom
PLATFORM_DEVICE=STM32F103xB
PLATFORM_NAME=atom
PLATFORM_MCU=STM32F1xx
MCU_CORE=cortex-m3
PRODUCT_DESC=IntoRobot Atom, 128k flash, 20k sram
PLATFORM_DFU = 0x08007000
DEFAULT_PRODUCT_ID=888004
endif

ifeq ("$(PLATFORM_ID)","888005") #fig
ARCH=xtensa-esp32
PLATFORM=fig
PLATFORM_DEVICE=fig
PLATFORM_NAME=fig
PLATFORM_MCU=ESP32-Arduino
PRODUCT_DESC=IntoRobot fig, ESP32, 4MB Flash
DEFAULT_PRODUCT_ID=888005
endif

ifeq ("$(PLATFORM_ID)","888006") #lora
PLATFORM=lora
PLATFORM_DEVICE=STM32L151xB
PLATFORM_NAME=lora
PLATFORM_MCU=STM32L1xx
MCU_CORE=cortex-m3
PRODUCT_DESC=IntoRobot lora, 128k flash, 16k sram
PLATFORM_DFU = 0x08000000
DEFAULT_PRODUCT_ID=888006
endif

ifeq ("$(PLATFORM_ID)","888007") #gprs
PLATFORM=gprs
PLATFORM_DEVICE=STM32L151xB
PLATFORM_NAME=gprs
PLATFORM_MCU=STM32L1xx
MCU_CORE=cortex-m3
PRODUCT_DESC=IntoRobot gprs, 128k flash, 16k sram
PLATFORM_DFU = 0x08000000
DEFAULT_PRODUCT_ID=888007
endif
# 此处新增创客核心板

################商业模块####################
ifeq ("$(PLATFORM_ID)","888101") #w67
ARCH=xtensa-lx106
PLATFORM=w67
PLATFORM_DEVICE=w67
PLATFORM_NAME=w67
PLATFORM_MCU=ESP8266-Arduino
PRODUCT_DESC=IntoRobot w67, ESP8266, 4MB Flash
DEFAULT_PRODUCT_ID=888101
endif

ifeq ("$(PLATFORM_ID)","888102") #w323
ARCH=xtensa-esp32
PLATFORM=w323
PLATFORM_DEVICE=w323
PLATFORM_NAME=w323
PLATFORM_MCU=ESP32-Arduino
PRODUCT_DESC=IntoRobot w323, ESP32, 4MB Flash
DEFAULT_PRODUCT_ID=888102
endif

ifeq ("$(PLATFORM_ID)","888103") #l6
PLATFORM=l6
PLATFORM_DEVICE=STM32L151xB
PLATFORM_NAME=l6
PLATFORM_MCU=STM32L1xx
MCU_CORE=cortex-m3
PRODUCT_DESC=IntoRobot lora, 128k flash, 16k sram
PLATFORM_DFU = 0x08000000
DEFAULT_PRODUCT_ID=888103
endif
# 此处新增模块

################其他####################
ifeq ("$(PLATFORM_ID)","888201") # gcc
ARCH=gcc
PLATFORM=gcc
PLATFORM_DEVICE=gcc
PLATFORM_NAME=gcc
PLATFORM_MCU=gcc
PRODUCT_DESC=GCC xcompile
# explicitly exclude platform headers
INTOROBOT_NO_PLATFORM=1
DEFAULT_PRODUCT_ID=2
PLATFORM_BOOTLOADER=888201
endif

ifeq ("$(PLATFORM_ID)","888202") #neutron-net
ARCH=xtensa-lx106
PLATFORM=neutron-net
PLATFORM_DEVICE=neutron-net
PLATFORM_NAME=neutron-net
PLATFORM_MCU=ESP8266
PRODUCT_DESC=ESP8266 4MB Flash
DEFAULT_PRODUCT_ID=888202
endif

ifeq ("$(PLATFORM_ID)","888203") #anytest
PLATFORM=anytest
PLATFORM_DEVICE=STM32F103xB
PLATFORM_NAME=anytest
PLATFORM_MCU=STM32F1xx
MCU_CORE=cortex-m3
PRODUCT_DESC=IntoRobot anytest, 128k flash, 20k sram
PLATFORM_DFU = 0x08007000
DEFAULT_PRODUCT_ID=888203
endif
# 此处新增其他


################实例####################
ifeq ("$(PLATFORM_ID)","600000") #newhal
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
DEFAULT_PRODUCT_ID=600000
PLATFORM_BOOTLOADER=0
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
