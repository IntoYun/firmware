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
SOURCE_PATH = $(APPDIR)
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

# libraries only include sources in the "src" dir
LIBCPPSRC += $(call target_files_dirs,$(MODULE_LIBS)/,src/,*.cpp)
LIBCSRC += $(call target_files_dirs,$(MODULE_LIBS)/,src/,*.c)

LIBCPPSRC += $(call target_files_dirs,$(MODULE_PATH)/libraries,,*.cpp)
LIBCSRC += $(call target_files_dirs,$(MODULE_PATH)/libraries,,*.c)

#$(info LIBCPPSRC $(LIBCPPSRC))
#$(info LIBCSRC $(LIBCSRC))

CPPSRC += $(LIBCPPSRC)
CSRC += $(LIBCSRC)

# libraries contain their sources under a "src" folder
INCLUDE_DIRS += $(addsuffix /src,$(MODULE_LIBS))
INCLUDE_DIRS += $(addsuffix /src,$(shell find $(MODULE_PATH)/libraries -maxdepth 1 -type d))

CPPFLAGS += -std=gnu++11

#BUILTINS_EXCLUDE = malloc free realloc
#CFLAGS += $(addprefix -fno-builtin-,$(BUILTINS_EXCLUDE))

CFLAGS += $(EXTRA_CFLAGS)
