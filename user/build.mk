# SOURCE_PATH - the root of all sources. Defaults to the module dir
# USRSRC - relative path to SOURCE_PATH for the sources to build
MODULE_LIBS += $(call remove_slash,$(APPLIBS))

# determine where user sources are, relative to project root
ifdef APP
USER_MAKEFILE ?= $(APP).mk
# when TARGET_FILE is defined on the command line,
endif

ifdef APPDIR
# APPDIR is where the sources are found
# if TARGET_DIR is not defined defaults to $(APPDIR)/target
# if TARGET_FILE_NAME is not defined, defaults to the name of the $(APPDIR)
SOURCE_PATH = $(call remove_slash,$(APPDIR))

# the root of the application
APPROOT := $(SOURCE_PATH)

ifneq ($(wildcard $(APPROOT)/src),)
# add vendored libraries to module libraries
MODULE_LIBS += $(wildcard $(APPROOT)/lib/*)
SOURCE_PATH := $(APPROOT)
USRSRC = src
INCLUDE_DIRS += $(APPROOT)/lib
endif

endif

ifdef TEST
INCLUDE_PLATFORM?=1
include $(MODULE_PATH)/tests/tests.mk
-include $(MODULE_PATH)/$(USRSRC)/test.mk
endif

USRSRC_SLASH = $(and $(USRSRC),$(USRSRC)/)
USER_MAKEFILE ?= build.mk
usrmakefile = $(wildcard $(SOURCE_PATH)/$(USRSRC_SLASH)$(USER_MAKEFILE))
ifeq ("$(usrmakefile)","")
INCLUDE_DIRS += $(SOURCE_PATH)/$(USRSRC)  # add user sources to include path

# add C and CPP files - if USRSRC is not empty, then add a slash
INOSRC += $(call target_files,$(USRSRC_SLASH),*.ino)
CPPSRC += $(call target_files,$(USRSRC_SLASH),*.cpp)
CSRC += $(call target_files,$(USRSRC_SLASH),*.c)

APPSOURCES=$(call target_files,$(USRSRC_SLASH),*.ino)
APPSOURCES+=$(call target_files,$(USRSRC_SLASH),*.cpp)
APPSOURCES+=$(call target_files,$(USRSRC_SLASH),*.c)

ifeq ($(strip $(APPSOURCES)),)
$(error "No sources found in $(SOURCE_PATH)/$(USRSRC)")
endif
else
include $(usrmakefile)
endif

INCLUDE_DIRS += $(MODULE_PATH)/libraries

# Adds the sources from the specified library directories
LIBCPPSRC += $(call target_files_dirs,$(MODULE_LIBS)/,src/,*.cpp)
LIBCSRC += $(call target_files_dirs,$(MODULE_LIBS)/,src/,*.c)

CPPSRC += $(LIBCPPSRC)
CSRC += $(LIBCSRC)

# libraries contain their sources under a "src" folder
## add all module libraries as include directories
INCLUDE_DIRS += $(addsuffix /src,$(MODULE_LIBS))

#BUILTINS_EXCLUDE = malloc free realloc
#CFLAGS += $(addprefix -fno-builtin-,$(BUILTINS_EXCLUDE))

CFLAGS += $(EXTRA_CFLAGS)

