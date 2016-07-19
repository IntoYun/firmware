# makefile for ARM top level modules

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(patsubst %/,%,$(dir $(mkfile_path)))

include $(current_dir)/top-level-module.mk

ifeq ("$(ARCH)","arm")
include $(current_dir)/arm-tools.mk
include $(COMMON_BUILD)/arm-module.mk
else
ifeq ("$(ARCH)","xtensa")
include $(current_dir)/xtensa-tools.mk
include $(COMMON_BUILD)/xtensa-module.mk
else
ifeq ("$(ARCH)","gcc")
include $(current_dir)/gcc-tools.mk
include $(COMMON_BUILD)/gcc-module.mk
else
$(error "Unknown architecture '$(ARCH)'");
endif
endif
endif
