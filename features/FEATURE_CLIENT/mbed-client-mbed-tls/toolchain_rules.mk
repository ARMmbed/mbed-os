#
# Toolchain rules for building libraries
#
# Support multi-platform builds
# You may define PLATFORM=.. prefix for compiler
# if PLATFORM is defined it will owerride CC and AR
# You may also specify CC and AR separately.
# example: Compile with KEIL
# make CC=ArmCC AR=ArmAR
#
# example: Cross Compile with Gnu toolchain, for embedded Linux
# make PLATFORM=arm-linux-gnueabi-
#
# Special rules for IAR-ARM toolchains added, AR tool on that does not follow common arquements.
# make CC=iccarm  uses iarchive.exe for creating a archive.
#
# Special rules for IAR-RL78 toolchain added, uses own AR and compiler requires specific flags
# make CC=iccrl78
#
# When using ArmCC, Select CPU by defining one of following supported values
#  CPU=Cortex-M0
#  CPU=Cortex-M3
#  CPU=Cortex-M4
#
# example:
# make CC=armcc CPU=Cortex-M4

#
# If PLATFORM prefix is defined,
# generate CC and AR accordingly
#
ifneq ($(strip $(PLATFORM)),)
CC:=$(PLATFORM)gcc
AR:=$(PLATFORM)ar
endif

#
# Windows specific tweaks for echo and mkdir to work
#
ifeq ($(OS),Windows_NT)
SHELL := cmd.exe
mkdir = if not exist $(1) md $(subst /,\,$(1))
else
mkdir = mkdir -p $(1)
endif

#
# Append CC and CPU to library names
#
LIB:=$(LIB:%.a=%_$(CC).a)
ifneq (,$(CPU))
	LIB:=$(LIB:%.a=%_$(CPU).a)
endif

##########################################################
#        TOOLCHAIN SPECIFIC RULES AND FLAGS              #
##########################################################

#
# GCC toolchains
#
ifneq (,$(findstring gcc,$(CC)))
	# Flags for common toolchain, usually GCC or CC
	AROPTS=-rsc $@ $^
	override CFLAGS += -Wall -pedantic-errors
	# Dependency generation
	override CFLAGS += -MMD -MP
	ifneq (,$(CPU))
	# CPU must be lowercase on GCC
	CPU_L:=$(shell echo $(CPU) | tr A-Z a-z)
	override CFLAGS += -mcpu=$(CPU_L)
	# Use THUMB instructions of Cortex-M cores
	ifeq (cortex-m,$(findstring cortex-m,$(CPU_L)))
	override CFLAGS += -mthumb
	endif
	endif
	# Debug
	ifeq ($(DEBUG),1)
	override CFLAGS += -g -O0
	else
	override CFLAGS += -O2
	endif
	# Enable Coverage generation
	ifeq ($(COVERAGE),1)
	override CFLAGS += -ftest-coverage -fprofile-arcs
	override LDFLAGS += -ftest-coverage -fprofile-arcs
	endif
	COMPILE = $(CC) -std=gnu99 -c -o $@
	CXXCOMPILE = $(CC) -std=c++11 -c -o $@

#
# IAR-ARM toolchain
#
else ifneq (,$(findstring iccarm,$(CC)))
	AR:=iarchive
	AROPTS=$^ --create -o $@
	DLIB_FILE=$(subst bin\iccarm.exe,inc\c\DLib_Config_Full.h,$(shell where iccarm))
	override CFLAGS += --dlib_config '$(DLIB_FILE)' --cpu Cortex-M4 --vla --diag_suppress Pa50
	# Dependency generation
	override CFLAGS += --dependencies=m $(basename $@).d
	# Debug
	ifeq ($(DEBUG),1)
	override CFLAGS += --debug -On
	else
	override CFLAGS += -Om
	endif
	COMPILE = $(CC) -c -o $@

#
# ArmCC toolchain (Used by Keil)
#
else ifneq (,$(findstring armcc,$(CC)))
	AR:=armar
	AROPTS=-rsc $@ $^
	override CFLAGS += --c99 --no_wrap_diagnostics
	# Dependency generation
	override CFLAGS += --depend $(basename $@).d --phony_targets
	LIB:=$(LIB:%.a=%.lib)
	ifneq (,$(CPU))
	override CFLAGS += --cpu=$(CPU)
	endif
	# Debug
	ifeq ($(DEBUG),1)
	override CFLAGS += -g -O0
	else
	override CFLAGS += -O2
	endif
	COMPILE = $(CC) -c -o $@

#
# IAR Renesas78 toolchain
#
else ifneq (,$(findstring iccrl78,$(CC)))
	AR:=xar
	AROPTS=$@ $^
	# Hack to get \lib include path which contains dlib config headers for compiler
	DIR=$(subst iccrl78.exe,..\lib\,$(shell where iccrl78))
	override CFLAGS += --core rl78_1 --code_model far --data_model far --dlib_config full --vla --no_wrap_diagnostics -I'$(DIR)'
	# Dependency generation
	LIB:=$(LIB:%.a=%.lib)
	override CFLAGS += --dependencies=m $(basename $@).d
	# Debug
	ifeq ($(DEBUG),1)
	override CFLAGS += --debug -Oh
	else
	override CFLAGS += -Ohz
	endif
	COMPILE = $(CC) -o $@

#
# IAR MSP430 toolchain
#
else ifneq (,$(findstring icc430,$(CC)))
	AR:=xar
	AROPTS=$@ $^
	# Hack to get \lib include path which contains dlib config headers for compiler
	DIR=$(subst bin\icc430.exe,lib,$(shell where icc430))
	DLIB_FILE=$(subst bin\icc430.exe,lib\dlib\dl430xllff.h,$(shell where icc430))
	override CFLAGS += --dlib_config '$(DLIB_FILE)' --vla --core 430X --data_model large --no_wrap_diagnostics -I'$(DIR)'
	# Dependency generation
	LIB:=$(LIB:%.a=%.lib)
	override CFLAGS += --dependencies=m $(basename $@).d
	# Debug
	ifeq ($(DEBUG),1)
	override CFLAGS += --debug -On
	else
	override CFLAGS += -Ohz
	endif
	COMPILE = $(CC) -o $@

#
# CppCheck toolchain
# This is used only for static testing the code.
# cppcheck is used in place of compiler and linker phase is ignored
else ifneq (,$(findstring cppcheck,$(CC)))
	AR = cppcheck -q --enable=warning --enable=style --std=c99 --inline-suppr -DCPPCHECK $(CPPCHECK_OPTS) $(CFLAGS) $(SRCS)
	COMPILE = true
	CPPCHECK = 1
	LIB:= "ignored_with_cppcheck"


###################################
# End of toolchain specific rules #
###################################
endif

#
# Generate VERSION tag
#
ifeq (,$(CPPCHECK))
VERSION := $(shell git describe --tags --long --dirty --always)
CFLAGS += -DVERSION='"$(VERSION)"'
endif

############################################################
# AUTOMATICALLY GENERATE LIBRARY COMPILATION RULES         #
#                                                          #
# Use:                                                     #
#   $(eval $(call generate_rules,$(OUTPUT_LIB),$(SRCS)))   #
# at the end of your Makefile                              #
#                                                          #
############################################################

define generate_rules

DEST_LIB := $(1)
LIB_SRCS := $(2)
C_SRCS := $$(filter %.c,$$(LIB_SRCS))
CXX_SRCS := $$(filter %.cpp,$$(LIB_SRCS))
all: $$(DEST_LIB)

#
# Define build dir outside from src folders
#
BUILD_DIR := output/$$(CC)
ifneq (,$$(CPU))
BUILD_DIR := $$(BUILD_DIR)_$$(CPU)
endif
ifneq (generic,$$(CONFIG))
BUILD_DIR := $$(BUILD_DIR)_$$(CONFIG)
endif
OBJS := $$(C_SRCS:%.c=$$(BUILD_DIR)/%.o) $$(CXX_SRCS:%.cpp=$$(BUILD_DIR)/%.o)

# Include auto-dependencies
-include $$(OBJS:.o=.d)

# Create list of output directories.
OBJ_DIRS := $$(sort $$(dir $$(OBJS)))

# requires directories to be created before build.
# '|' means do not care about timestamps of requisities
$$(OBJS): | $$(OBJ_DIRS)

# Need to redefine build rule to find the sources
# from correct folder.
$$(BUILD_DIR)/%.o: %.c
ifeq ($(V),1)
	$$(COMPILE) $$(CFLAGS) $$<
else ifeq (1,$(CPPCHECK))
	@$$(COMPILE) $$(CFLAGS) $$<
else ifeq (1,$(UNITTEST))
	@echo Building unittest $$<
ifneq ($(OS),Windows_NT)
	ruby $(UNIT_TEST_BASE)/../auto/generate_test_runner.rb Test.c Test_Runner.c
endif
	@gcc $$(CFLAGS) $$(SRCS) -o $$(OUTPUT_LIB)
else
	@echo Building $$<
	@$$(COMPILE) $$(CFLAGS) $$<
endif

# Same again for C++
$$(BUILD_DIR)/%.o: %.cpp
ifeq ($(V),1)
	$$(CXXCOMPILE) $$(CFLAGS) $$<
else ifeq (1,$(CPPCHECK))
	@$$(CXXCOMPILE) $$(CFLAGS) $$<
else
	@echo Building $$<
	@$$(CXXCOMPILE) $$(CFLAGS) $$<
endif

# Rule to create a object directory
$$(OBJ_DIRS):
	@-$$(call mkdir,$$@)

$$(DEST_LIB): $$(OBJS)
ifeq ($(V),1)
	-$$(addprefix $$(RM) ,$$(wildcard $$@))
	$$(AR) $$(AROPTS)
else ifeq (1,$(CPPCHECK))
	@$$(AR) $$(AROPTS)
else ifeq (1,$(UNITTEST))
#ifneq ($(OS),Windows_NT)
#	lcov -q --capture -i --directory $(CURDIR) --output-file lib_base.info
#	./$$@
#	lcov -q --capture --directory $(CURDIR) --output-file lib_test.info
#	lcov -a lib_base.info -a lib_test.info -o coverage.info
#endif
else
	@echo Linking $$(notdir $$@)
	@-$$(addprefix $$(RM) ,$$(wildcard $$@))
	@$$(AR) $$(AROPTS)
endif

clean:
	@$(RM) -r $$(BUILD_DIR)
	@$(RM) $$(DEST_LIB)

clean-all:
	@$(RM) -r output *.a

endef
