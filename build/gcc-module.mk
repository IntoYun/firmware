# This is the common makefile used to build all top-level modules
# It contains common recipes for bulding C/CPP/asm files to objects, and
# to combine those objects into libraries or elf files.
include $(COMMON_BUILD)/macros.mk

SOURCE_PATH ?= $(MODULE_PATH)

# Recursive wildcard function - finds matching files in a directory tree
target_files = $(patsubst $(SOURCE_PATH)/%,%,$(call rwildcard,$(SOURCE_PATH)/$1,$2))
here_files = $(call wildcard,$(SOURCE_PATH)/$1$2)

# import this module's symbols
include $(MODULE_PATH)/import.mk

# pull in the include.mk files from each dependency, and make them relative to
# the dependency module directory
DEPS_INCLUDE_SCRIPTS =$(foreach module,$(DEPENDENCIES),$(PROJECT_ROOT)/$(module)/import.mk)
include $(DEPS_INCLUDE_SCRIPTS)

include $(COMMON_BUILD)/gcc-module-defaults.mk

include $(call rwildcard,$(MODULE_PATH)/,build.mk)

# add trailing slash
ifneq ("$(TARGET_PATH)","$(dir $(TARGET_PATH))")
TARGET_SEP = /
endif

TARGET_FILE_NAME ?= $(MODULE)

ifneq (,$(GLOBAL_DEFINES))
CFLAGS += $(addprefix -D,$(GLOBAL_DEFINES))
export GLOBAL_DEFINES
endif

# Collect all object and dep files
ALLOBJ += $(addprefix $(BUILD_PATH)/, $(CSRC:.c=.o))
ALLOBJ += $(addprefix $(BUILD_PATH)/, $(CPPSRC:.cpp=.o))
ALLOBJ += $(addprefix $(BUILD_PATH)/, $(INOSRC:.ino=.o))
ALLOBJ += $(addprefix $(BUILD_PATH)/, $(ASRC:.S=.o))
ALLOBJ += $(addprefix $(BUILD_PATH)/, $(patsubst $(COMMON_BUILD)/%,%,$(ASRC_STARTUP:.S=.o)))

ALLDEPS += $(addprefix $(BUILD_PATH)/, $(CSRC:.c=.o.d))
ALLDEPS += $(addprefix $(BUILD_PATH)/, $(CPPSRC:.cpp=.o.d))
ALLDEPS += $(addprefix $(BUILD_PATH)/, $(INOSRC:.ino=.o.d))
ALLDEPS += $(addprefix $(BUILD_PATH)/, $(ASRC:.S=.o.d))
ALLDEPS += $(addprefix $(BUILD_PATH)/, $(patsubst $(COMMON_BUILD)/%,%,$(ASRC_STARTUP:.S=.o.d)))

# All Target
all: $(MAKE_DEPENDENCIES) $(TARGET) postbuild

elf: $(TARGET_BASE).elf
bin: $(TARGET_BASE).bin
hex: $(TARGET_BASE).hex
lst: $(TARGET_BASE).lst
exe: $(TARGET_BASE)$(EXECUTABLE_EXTENSION)
	@echo Built x-compile executable at $(TARGET_BASE)$(EXECUTABLE_EXTENSION)
none:
	;

st-flash: $(TARGET_BASE).bin
	@echo Flashing $< using st-flash to address $(PLATFORM_DFU)
	st-flash write $< $(PLATFORM_DFU)

ifneq ("$(OPENOCD_HOME)","")

program-openocd: $(TARGET_BASE).bin
	@echo Flashing $< using openocd to address $(PLATFORM_DFU)
	$(OPENOCD_HOME)/openocd -f $(OPENOCD_HOME)/tcl/interface/ftdi/particle-ftdi.cfg -f $(OPENOCD_HOME)/tcl/target/stm32f2x.cfg  -c "init; reset halt" -c "flash protect 0 0 11 off" -c "program $< $(PLATFORM_DFU) reset exit"

endif

esptool: $(TARGET_BASE).bin
	@echo Flashing $< using esptool to address
	#$(ESP_TOOL) $(UPLOAD_VERB) -cd $(UPLOAD_RESET) -cb $(UPLOAD_SPEED) -cp $(UPLOAD_PORT) -ca 0x00000 -cf $(MAIN_EXE)

# Program the core using dfu-util. The core should have been placed
# in bootloader mode before invoking 'make program-dfu'
program-dfu: $(TARGET_BASE).dfu
ifdef START_DFU_FLASHER_SERIAL_SPEED
# PARTICLE_SERIAL_DEV should be set something like /dev/tty.usbxxxx and exported
#ifndef PARTICLE_SERIAL_DEV
ifeq ("$(wildcard $(PARTICLE_SERIAL_DEV))","")
	@echo Serial device PARTICLE_SERIAL_DEV : $(PARTICLE_SERIAL_DEV) not available
else
	@echo Entering dfu bootloader mode:
	stty -f $(PARTICLE_SERIAL_DEV) $(START_DFU_FLASHER_SERIAL_SPEED)
	sleep 1
endif
endif
	@echo Flashing using dfu:
	$(DFU) -d $(USBD_VID_INTOROBOT):$(USBD_PID_DFU) -a 0 -s $(PLATFORM_DFU)$(if $(PLATFORM_DFU_LEAVE),:leave) -D $<

# Display size
size: $(TARGET_BASE).elf
	$(call,echo,'Invoking: ARM GNU Print Size')
	$(VERBOSE)$(SIZE) --format=berkeley $<
	$(call,echo,)

# create a object listing from the elf file
%.lst: %.elf
	$(call,echo,'Invoking: ARM GNU Create Listing')
	$(VERBOSE)$(OBJDUMP) -h -S $< > $@
	$(call,echo,'Finished building: $@')
	$(call,echo,)

# Create a hex file from ELF file
%.hex : %.elf
	$(call,echo,'Invoking: ARM GNU Create Flash Image')
	$(VERBOSE)$(OBJCOPY) -O ihex $< $@
	$(call,echo,)

# Create a DFU file from bin file
%.dfu: %.bin
	@cp $< $@
	$(DFUSUFFIX) -v $(subst 0x,,$(USBD_VID_INTOROBOT)) -p $(subst 0x,,$(USBD_PID_DFU)) -a $@

# Create a bin file from ELF file
%.bin : %.elf
	$(call echo,'Invoking: ARM GNU Create Flash Image')
	$(VERBOSE)$(OBJCOPY) -O binary $< $@

$(TARGET_BASE).elf : $(ALLOBJ) $(LIB_DEPS) $(LINKER_DEPS)
	$(call echo,'Building target: $@')
	$(call echo,'Invoking: ARM GCC C++ Linker')
	$(VERBOSE)$(MKDIR) $(dir $@)
	$(VERBOSE)$(CPP) $(CFLAGS) $(ALLOBJ) --output $@ $(LDFLAGS)
	$(call echo,)

$(TARGET_BASE)$(EXECUTABLE_EXTENSION) : $(ALLOBJ) $(LIB_DEPS) $(LINKER_DEPS)
	$(call echo,'Building target: $@')
	$(call echo,'Invoking: GCC C++ Linker')
	$(VERBOSE)$(MKDIR) $(dir $@)
	$(VERBOSE)$(CPP) $(CFLAGS) $(ALLOBJ) --output $@ $(LDFLAGS)
	$(call echo,)

# Tool invocations
$(TARGET_BASE).a : $(ALLOBJ)
	$(call echo,'Building target: $@')
	$(call echo,'Invoking: ARM GCC Archiver')
	$(VERBOSE)$(MKDIR) $(dir $@)
	$(VERBOSE)$(AR) -cr $@ $^
	$(call echo,)

# C compiler to build .o from .c in $(BUILD_DIR)
$(BUILD_PATH)/%.o : $(SOURCE_PATH)/%.c
	$(call echo,'Building file: $<')
	$(call echo,'Invoking: ARM GCC C Compiler')
	$(VERBOSE)$(MKDIR) $(dir $@)
	$(VERBOSE)$(CC) $(CFLAGS) $(CONLYFLAGS) -c -o $@ $<
	$(call echo,)

# Assember to build .o from .S in $(BUILD_DIR)
$(BUILD_PATH)/%.o : $(COMMON_BUILD)/%.S
	$(call echo,'Building file: $<')
	$(call echo,'Invoking: ARM GCC Assembler')
	$(VERBOSE)$(MKDIR) $(dir $@)
	$(VERBOSE)$(CC) $(ASFLAGS) -c -o $@ $<
	$(call echo,)

# CPP compiler to build .o from .cpp in $(BUILD_DIR)
# Note: Calls standard $(CC) - gcc will invoke g++ as appropriate
$(BUILD_PATH)/%.o : $(SOURCE_PATH)/%.cpp
	$(call echo,'Building file: $<')
	$(call echo,'Invoking: ARM GCC CPP Compiler')
	$(VERBOSE)$(MKDIR) $(dir $@)
	$(VERBOSE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
	$(call echo,)

# Other Targets
clean: clean_deps
	$(VERBOSE)$(RM) $(ALLOBJ) $(ALLDEPS) $(TARGET)
	$(VERBOSE)$(RMDIR) $(BUILD_PATH)
	$(call,echo,)

.PHONY: all postbuild none elf bin hex size program-dfu program-cloud st-flash program-serial
.SECONDARY:

include $(COMMON_BUILD)/recurse.mk

# Include auto generated dependency files
ifneq ("MAKECMDGOALS","clean")
-include $(ALLDEPS)
endif

