# This is the common makefile used to build all top-level modules
# It contains common recipes for bulding C/CPP/asm files to objects, and
# to combine those objects into libraries or elf files.
include $(COMMON_BUILD)/macros.mk

SOURCE_PATH ?= $(MODULE_PATH)

# import this module's symbols
include $(MODULE_PATH)/import.mk

# pull in the include.mk files from each dependency, and make them relative to
# the dependency module directory
DEPS_INCLUDE_SCRIPTS =$(foreach module,$(DEPENDENCIES),$(PROJECT_ROOT)/$(module)/import.mk)
include $(DEPS_INCLUDE_SCRIPTS)

include $(COMMON_BUILD)/xtensa-lx106-module-defaults.mk

include $(call rwildcard,$(MODULE_PATH)/,build.mk)

# add trailing slash
ifneq ("$(TARGET_PATH)","$(dir $(TARGET_PATH))")
TARGET_SEP = /
endif

TARGET_FILE_NAME ?= $(MODULE)

ifneq (,$(GLOBAL_DEFINES))
CDEFINES += $(addprefix -D,$(GLOBAL_DEFINES))
export GLOBAL_DEFINES
endif


# Collect all object and dep files
ALLOBJ += $(addprefix $(BUILD_PATH)/, $(CSRC:.c=.c.o))
ALLOBJ += $(addprefix $(BUILD_PATH)/, $(CPPSRC:.cpp=.cpp.o))
ALLOBJ += $(addprefix $(BUILD_PATH)/, $(INOSRC:.ino=.ino.o))
ALLOBJ += $(addprefix $(BUILD_PATH)/, $(ASRC:.S=.S.o))
ALLOBJ += $(addprefix $(BUILD_PATH)/, $(patsubst $(COMMON_BUILD)/%,%,$(ASRC_STARTUP:.S=.S.o)))

ALLDEPS += $(addprefix $(BUILD_PATH)/, $(CSRC:.c=.c.o.d))
ALLDEPS += $(addprefix $(BUILD_PATH)/, $(CPPSRC:.cpp=.cpp.o.d))
ALLDEPS += $(addprefix $(BUILD_PATH)/, $(INOSRC:.ino=.ino.o.d))
ALLDEPS += $(addprefix $(BUILD_PATH)/, $(ASRC:.S=.S.o.d))
ALLDEPS += $(addprefix $(BUILD_PATH)/, $(patsubst $(COMMON_BUILD)/%,%,$(ASRC_STARTUP:.S=.S.o.d)))

# All Target
all: $(MAKE_DEPENDENCIES) $(TARGET) postbuild

elf: $(TARGET_BASE).elf
bin: $(TARGET_BASE).bin
hex: $(TARGET_BASE).hex
lst: $(TARGET_BASE).lst

esptool: $(TARGET_BASE).bin
	@echo Flashing $< using esptool to address
ifeq ("$(MODULE)","bootloader")
	$(SUDO) $(ESP8266_TOOL) $(UPLOAD_VERB) -cd $(UPLOAD_RESET) -cb $(UPLOAD_SPEED) -cp $(UPLOAD_PORT) -ca $(PLATFORM_BOOT_ADDR) -cf $^ -cr
else
	$(SUDO) $(ESP8266_TOOL) $(UPLOAD_VERB) -cd $(UPLOAD_RESET) -cb $(UPLOAD_SPEED) -cp $(UPLOAD_PORT) -ca $(PLATFORM_APP_ADDR) -cf $^ -cr
endif
	$(call echo,)

# Display size
size: $(TARGET_BASE).elf
	$(call,echo,'Invoking: XTENSA GNU Print Size')
	$(VERBOSE)$(SIZE) --format=berkeley $<
	$(call,echo,)

# create a object listing from the elf file
%.lst: %.elf
	$(call,echo,'Invoking: XTENSA GNU Create Listing')
	$(VERBOSE)$(OBJDUMP) -h -S $< > $@
	$(call,echo,'Finished building: $@')
	$(call,echo,)

# Create a bin file from ELF file
%.bin : %.elf
	$(call echo,'Invoking: XTENSA GNU Create Flash Image')
ifeq ("$(MODULE)","bootloader")
	$(ESP8266_TOOL) -eo $^ -bo $@ -bm $(FLASH_MODE) -bf $(FLASH_SPEED) -bz $(FLASH_SIZE) -bs .text -bs .data -bs .rodata -bc -ec || true
else
	$(ESP8266_TOOL) -eo $< -bo $@ -bs .irom0.text -bs .text -bs .data -bs .rodata -bc -ec
endif
	$(call echo,)

$(TARGET_BASE).elf : $(ALLOBJ) $(LIB_DEPS) $(LINKER_DEPS)
	$(call echo,'Building target: $@')
	$(call echo,'Invoking: XTENSA GCC C++ Linker')
	$(VERBOSE)$(MKDIR) $(dir $@)
	$(VERBOSE)$(CPP) $(CFLAGS) $(ALLOBJ) --output $@ $(LDFLAGS)
	$(call echo,)

# Tool invocations
$(TARGET_BASE).a : $(ALLOBJ)
	$(call echo,'Building target: $@')
	$(call echo,'Invoking: XTENSA GCC Archiver')
	$(VERBOSE)$(MKDIR) $(dir $@)
	$(VERBOSE)$(AR) cru $@ $^
	$(call echo,)

define build_C_file
	$(call echo,'Building file: $<')
	$(call echo,'Invoking: XTENSA GCC C Compiler')
	$(VERBOSE)$(MKDIR) $(dir $@)
	$(VERBOSE)$(CC) $(CFLAGS) $(CDEFINES) $(CINCLUDES) $(CONLYFLAGS) -c -o $@ $<
	$(call echo,)
endef

define build_CPP_file
	$(call echo,'Building file: $<')
	$(call echo,'Invoking: XTENSA GCC CPP Compiler')
	$(VERBOSE)$(MKDIR) $(dir $@)
	$(VERBOSE)$(CPP) $(CDEFINES) $(CFLAGS) $(CPPFLAGS) $(CINCLUDES) -c -o $@ $<
	$(call echo,)
endef

# C compiler to build .c.o from .c in $(BUILD_DIR)
$(BUILD_PATH)/%.c.o : $(SOURCE_PATH)/%.c
	$(build_C_file)

# CPP compiler to build .cpp.o from .cpp in $(BUILD_DIR)
# Note: Calls standard $(CC) - gcc will invoke g++ as appropriate
$(BUILD_PATH)/%.cpp.o : $(SOURCE_PATH)/%.cpp
	$(build_CPP_file)

define build_LIB_files
$(BUILD_PATH)/$(notdir $1)/%.c.o : $1/%.c
	$$(build_C_file)

$(BUILD_PATH)/$(notdir $1)/%.cpp.o : $1/%.cpp
	$$(build_CPP_file)
endef

# define rules for each library
# only the sources added for each library are built (so for libraries only files under "src" are built.)
$(foreach lib,$(MODULE_LIBS),$(eval $(call build_LIB_files,$(lib))))


# Assember to build .S.o from .S in $(BUILD_DIR)
$(BUILD_PATH)/%.S.o : $(SOURCE_PATH)/%.S
	$(call echo,'Building file: $<')
	$(call echo,'Invoking: XTENSA GCC Assembler')
	$(VERBOSE)$(MKDIR) $(dir $@)
	$(VERBOSE)$(CC) $(ASFLAGS) -c -o $@ $<
	$(call echo,)

# Assember to build .S.o from .S in $(COMMON_BUILD)
$(BUILD_PATH)/%.S.o : $(COMMON_BUILD)/%.S
	$(call echo,'Building file: $<')
	$(call echo,'Invoking: XTENSA GCC Assembler')
	$(VERBOSE)$(MKDIR) $(dir $@)
	$(VERBOSE)$(CC) $(ASFLAGS) -c -o $@ $<
	$(call echo,)

# CPP compiler to build .ino.o from .ino in $(BUILD_DIR)
# Note: Calls standard $(CC) - gcc will invoke g++ as appropriate
$(BUILD_PATH)/%.ino.o : $(SOURCE_PATH)/%.ino
	$(call echo,'Building file: $<')
	$(call echo,'Invoking: ARM GCC CPP Compiler')
	$(VERBOSE)$(MKDIR) $(dir $@)
	$(VERBOSE)$(CPP) -x c++ -include $(INO_INCLUDE_HEADER) $(CDEFINES) $(CFLAGS) $(CPPFLAGS) $(CINCLUDES) -c -o $@ $<
	$(call echo,)

# Other Targets
clean: clean_deps
	$(VERBOSE)$(RM) $(ALLOBJ) $(ALLDEPS) $(TARGET)
	$(VERBOSE)$(RMDIR) $(BUILD_PATH)
	$(call,echo,)

.PHONY: all postbuild none elf bin hex size esptool
.SECONDARY:

include $(COMMON_BUILD)/recurse.mk

# Include auto generated dependency files
ifneq ("MAKECMDGOALS","clean")
-include $(ALLDEPS)
endif

