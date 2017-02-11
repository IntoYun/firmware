# put deliverable products at the end
PROJECT_ROOT = .
COMMON_BUILD=build
BUILD_PATH_BASE=$(COMMON_BUILD)/target

include $(COMMON_BUILD)/platform-id.mk

ifeq ($(PLATFORM_BOOTLOADER),1)
MAKE_DEPENDENCIES += bootloader
endif
MAKE_DEPENDENCIES += main

msg = Building firmware for $(PRODUCT_DESC), platform ID: $(PLATFORM_ID)

$(info $(msg))

all: make_deps

include $(COMMON_BUILD)/common-tools.mk
include $(COMMON_BUILD)/recurse.mk
include $(COMMON_BUILD)/verbose.mk

clean: clean_deps
	$(VERBOSE)$(RMDIR) $(BUILD_PATH_BASE)


.PHONY: all
