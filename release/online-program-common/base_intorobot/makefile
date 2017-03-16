PROJECT_ROOT=$(cd "$(dirname "$0")"; pwd)..

##### Board Type #####
# "888001 for atom, 888002 for neutron, 888003 for nut, 888005 for fig"
# The .mk include the head files, lib of board, linker of board, startup files
# also include the CFLAGS, CPPFLAGS, LDFLAGS, ASFLAGS
# TODO:
# Change to atom neutron etc.
#BOARD?=888001
BOARD?=888002
ifeq ($(BOARD), 888001)
-include atom.mk
endif

ifeq ($(BOARD), 888002)
-include neutron.mk
endif

ifeq ($(BOARD), 888003)
-include nut.mk
endif

ifeq ($(BOARD), 888005)
-include fig.mk
endif


##### Define Useful  Functions #####
# Recursive wildcard function
rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
# enumerates files in the filesystem and returns their path relative to the project root
# $1 the directory relative to the project root
# $2 the pattern to match, e.g. *.cpp
target_files = $(patsubst $(SRC_PATH)%,%,$(call rwildcard,$(SRC_PATH)$1,$2))

# Search all the files in one single director

search_target_files_single = $(call rwildcard,$1,$2)

# Search all the files in directory recursively
search_target_file=$(foreach dir_for,$1,$(call search_target_files_single,$(dir_for),$2))

##### Defalut user's src, lib and the  public lib #####
# The user project and user lib and the public lib from the Cloud
#ID_PATH ?= user/xiaoming/APP
ID_PATH ?= ../users/xiaoming/APP
ID_ROOT_DIR := $(ID_PATH)/../
LIB_USR ?= DHT22_lib_usr_test1}V1_11  DHT33_lib_usr_test2}V2_22
LIB_PUBLIC ?= DHT00_lib_public_test1}V1_11 DHT11_lib_public_test2}V2_22
# User code
SRC_USR_PATH = $(ID_PATH)/src/
BIN_PLATFORM_OUT = $(ID_PATH)/out

# The public lib PATH
# TODO
LIB_PUBLIC_DIR=../libs_public/


##### Search User lib, header #####
# 用户调用的私有库The user self lib path
LIB_USR_PATH = $(shell echo ${LIB_USR} | awk '{ for(i=1; i<=NF; i++) print $$i }' | awk -F '}' '{print "${ID_ROOT_DIR}"$$0"/"$$1"/src/"}')
LIB_USR_INC = $(patsubst %,-I %../../, $(LIB_USR_PATH))
# Add self lib path  to the SRC_USER_PATH
SRC_USR_PATH += $(LIB_USR_PATH)

# 用户调用的公共库Public lib full path
LIB_PUBLIC_PATH = $(shell echo ${LIB_PUBLIC} | awk '{ for(i=1; i<=NF; i++) print $$i }' | awk -F '}' '{print "${LIB_PUBLIC_DIR}"$$0"/"$$1"/src/"}')
# Public lib header full path
LIB_PUBLIC_INC = $(patsubst %,-I %../../, $(LIB_PUBLIC_PATH))
# Add public lib to SRC_USER_PATH
SRC_USR_PATH += $(LIB_PUBLIC_PATH)


# 用户调用的头文件路径Head files path, the same path with SRC_USR_PATH
INC_USR_PATH :=$(SRC_USR_PATH)

# Search the user cpp file
CPPSRC_USR=$(call search_target_file,$(SRC_USR_PATH),*.cpp)
# Change from .cpp to .o
CPPOBJ_USR = $(patsubst %.cpp,%.o, $(CPPSRC_USR))

## 用户调用的所有库的头文件以及link的文件
# User lib head file
LIB_USR_INC_FILE=$(call search_target_file,$(LIB_USR_PATH),*.h)
# The head files of user lib need to be link
LIB_USR_INC_FILE_LINK=$(subst /src/,/, $(LIB_USR_INC_FILE))
# Public lib head file
LIB_PUBLIC_INC_FILE=$(call search_target_file,$(LIB_PUBLIC_PATH),*.h)
# The head files of public lib need to be link
LIB_PUBLIC_INC_FILE_LINK=$(subst /src/,/, $(LIB_PUBLIC_INC_FILE))

IN_INC_FILE=$(LIB_USR_INC_FILE) $(LIB_PUBLIC_INC_FILE)
IN_INC_FILE_LINK=$(LIB_USR_INC_FILE_LINK) $(LIB_PUBLIC_INC_FILE_LINK)


##### Add user and pulic files to CFLAGS  #####
CFLAGS += $(LIB_PUBLIC_INC)
CFLAGS += $(LIB_USR_INC)

##### Targets #####

# 编译平台固件的函数
release_platform_bin: clean_public_lib clean_platform_bin create_ln_inc release_usr_obj elf_platform bin_platform
	@echo " ================================= "
	@echo " === release_platform_bin done === "
	@echo " ================================= "

# 创建头文件的link，目的是头文件调用的时候，使用<lib/lib.h>格式
create_ln_inc:$(IN_INC_FILE)
	@echo " ================================= "
	@echo " ======= create_ln_inc done ====== "
	@echo " ================================= "

$(IN_INC_FILE):
	@if [ ! -e $(subst /src/,/, $@) ]; then ln $@ $(subst /src/,/, $@) ;  fi

clean_ln_inc:
	$(RM) $(IN_INC_FILE_LINK)


clean_platform_bin:clean_usr_lib
	$(RM) $(ID_PATH)/src/*.o
	$(RM) $(ID_PATH)/src/*.d*
	$(RM) $(BIN_PLATFORM_OUT)/*
	@echo " ================================= "
	@echo " ==== clean_platform_bin done ==== "
	@echo " ================================= "

elf_platform: $(BIN_PLATFORM_OUT)/platform.elf
	@echo " ================================= "
	@echo " ====== elf_plaform done ========= "
	@echo " ================================= "

bin_platform: $(BIN_PLATFORM_OUT)/platform.bin

$(BIN_PLATFORM_OUT)/platform.elf: $(LINKER_DEPS)
	@echo 'Building target: $@'
	@echo Invoking: ARM GCC C++ Linker
	$(MKDIR) $(dir $@)
	$(CPP) $(CFLAGS) $(CPPOBJ_USR) $(STARTUP_OBJFILE) $(LDFLAGS) --output $@
	@echo " "

# Clean the user .o files
LIB_USR_OBJ= $(patsubst %,%*.o, $(LIB_USR_PATH))
LIB_USR_DBJ= $(patsubst %,%*.d*, $(LIB_USR_PATH))

clean_usr_lib:clean_ln_inc
	rm -rf $(LIB_USR_OBJ)
	rm -rf $(LIB_USR_DBJ)
	@echo " ================================= "
	@echo " ====== clean_usr_lib done ======= "
	@echo " ================================= "

release_usr_obj:$(CPPOBJ_USR)
	@echo " ================================= "
	@echo " =====release_usr_obj done======== "
	@echo " ================================= "

#用户.o文件生成规则
$(ID_PATH)%.o:$(ID_PATH)%.c
	@echo Building file: $<
	@echo Invoking: ARM GCC CPP Compiler
	$(CC) $(CFLAGS) $(CONLYFLAGS) -c -o $@ $<
	@echo

$(ID_PATH)%.o:$(ID_PATH)%.cpp
	@echo Building file: $<
	@echo Invoking: ARM GCC CPP Compiler
	$(CPP) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
	@echo

$(LIB_PUBLIC_DIR)%.o:$(LIB_PUBLIC_DIR)%.c
	@echo Building file: $<
	@echo Invoking: ARM GCC CPP Compiler
	$(CC) $(CFLAGS) $(CONLYFLAGS) -c -o $@ $<
	@echo

$(LIB_PUBLIC_DIR)%.o:$(LIB_PUBLIC_DIR)%.cpp
	@echo Building file: $<
	@echo Invoking: ARM GCC CPP Compiler
	$(CPP) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
	@echo

# Clean the public lib .o files
LIB_PUBLIC_OBJ= $(patsubst %,%*.o, $(LIB_PUBLIC_PATH))
LIB_PUBLIC_DBJ= $(patsubst %,%*.d*, $(LIB_PUBLIC_PATH))

clean_public_lib:clean_ln_inc
	$(RM) $(LIB_PUBLIC_OBJ)
	$(RM) $(LIB_PUBLIC_DBJ)
	@echo " ================================= "
	@echo " ==== clean_public_lib done ====== "
	@echo " ================================= "

## Create a bin file from ELF file
#%.bin : %.elf
#	@echo Invoking: ARM GNU Create Flash Image
#	$(OBJCOPY) -O binary $< $@
#	@echo


.PHONY: clean_public_lib clean_usr_lib clean_platform_bin $(IN_INC_FILE) create_ln_inc release_platform_bin clean_ln_inc

