# put deliverable products at the end
PROJECT_ROOT = .
COMMON_BUILD=build
BUILD_PATH_BASE=$(COMMON_BUILD)/target

ifdef INTOROBOT_PRODUCT_ID
PRODUCT_ID=$(INTOROBOT_PRODUCT_ID)
endif

include $(COMMON_BUILD)/platform-id.mk

ifeq ($(PLATFORM_BOOTLOADER),1)
MAKE_DEPENDENCIES += bootloader
endif
MAKE_DEPENDENCIES += main

ifdef PRODUCT_ID
msg_ext =, product ID: $(PRODUCT_ID)
endif

msg = Building firmware for $(PRODUCT_DESC), platform ID: $(PLATFORM_ID)$(msg_ext)

$(info $(msg))

all: make_deps

include $(COMMON_BUILD)/common-tools.mk
include $(COMMON_BUILD)/recurse.mk
include $(COMMON_BUILD)/verbose.mk

clean: clean_deps
	$(VERBOSE)$(RMDIR) $(BUILD_PATH_BASE)


.PHONY: all
