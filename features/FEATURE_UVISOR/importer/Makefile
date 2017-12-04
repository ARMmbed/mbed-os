###########################################################################
#
#  Copyright (c) 2013-2016, ARM Limited, All Rights Reserved
#  SPDX-License-Identifier: Apache-2.0
#
#  Licensed under the Apache License, Version 2.0 (the "License"); you may
#  not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
#  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
###########################################################################

# Toolchain
PREFIX:=arm-none-eabi-
GDB:=$(PREFIX)gdb
OBJDUMP:=$(PREFIX)objdump

# Translate between uVisor namespace and mbed namespace
TARGET_TRANSLATION:=MCU_K64F.kinetis EFM32.efm32 STM32F4.stm32 ARM_BEETLE_SOC.beetle M480.m480
TARGET_PREFIX:=../
TARGET_SUPPORTED:=$(TARGET_PREFIX)targets/TARGET_UVISOR_SUPPORTED
TARGET_UNSUPPORTED:=$(TARGET_PREFIX)targets/TARGET_UVISOR_UNSUPPORTED
TARGET_INC:=$(TARGET_PREFIX)includes/uvisor/api
TARGET_LIB_SRC:=$(TARGET_PREFIX)source
TARGET_LIB_INC:=$(TARGET_PREFIX)includes/uvisor-lib

# uVisor source directory - hidden from mbed via TARGET_IGNORE
UVISOR_GIT_URL:=https://github.com/ARMmbed/uvisor
UVISOR_GIT_BRANCH:=master
UVISOR_DIR:=TARGET_IGNORE/uvisor
UVISOR_API:=$(UVISOR_DIR)/api
UVISOR_GIT_CFG=$(UVISOR_DIR)/.git/config

# Derive variables from user configuration
TARGET_LIST:=$(subst .,,$(suffix $(TARGET_TRANSLATION)))
TARGET_LIST_DIR_SRC:=$(addprefix $(UVISOR_API)/lib/,$(TARGET_LIST))
TARGET_LIST_DIR_DST:=$(addprefix $(TARGET_SUPPORTED)/,$(TARGET_LIST))
TARGET_LIST_RELEASE:=$(addsuffix /release,$(TARGET_LIST_DIR_DST))
TARGET_LIST_DEBUG:=$(addsuffix /debug,$(TARGET_LIST_DIR_DST))

# mbed OS paths
MBED_OS_ROOT:=../../..
MBED_OS_CMSIS:=$(MBED_OS_ROOT)/cmsis

.PHONY: all deploy rsync publish uvisor uvisor-compile clean cache update

all: uvisor

uvisor: uvisor-compile publish

rsync:
	#
	# Copying uVisor into mbed library...
	rm -rf $(TARGET_SUPPORTED)
	mkdir -p $(TARGET_SUPPORTED)
	rsync -a --exclude='*.txt' $(TARGET_LIST_DIR_SRC) $(TARGET_SUPPORTED)
	#
	# Copying uVisor headers to mbed includes...
	rm -rf $(TARGET_INC)
	mkdir -p $(TARGET_INC)
	rsync -a --delete $(UVISOR_API)/inc $(TARGET_INC)
	rsync -a --delete $(UVISOR_API)/rtx/inc/ $(TARGET_LIB_INC)/rtx
	#
	# Copying uVisor unsupported sources to unsupported target source...
	mkdir -p $(TARGET_UNSUPPORTED)
	cp $(UVISOR_API)/src/unsupported.c $(TARGET_UNSUPPORTED)/
	#
	# Copying uVisor shared sources to mbed source...
	rm -rf $(TARGET_LIB_SRC)
	mkdir -p $(TARGET_LIB_SRC)
	cp $(UVISOR_DIR)/core/system/src/page_allocator.c $(TARGET_LIB_SRC)/page_allocator.c_inc
	cp $(UVISOR_DIR)/core/system/inc/page_allocator_config.h $(TARGET_LIB_SRC)/page_allocator_config.h
	rsync -a --delete $(UVISOR_API)/rtx/src/ $(TARGET_LIB_SRC)/rtx
	#
	# Copying the secure API header file...
	# Note: This will not be needed when we upstream the file to CMSIS.
	cp $(UVISOR_DIR)/core/cmsis/inc/core_cmSecureAccess.h $(MBED_OS_CMSIS)/
	#
	# Copying the documentation...
	cp $(UVISOR_DIR)/docs/*/QUICKSTART.md $(TARGET_PREFIX)/README.md
	#
	# Copying licenses
	cp $(UVISOR_DIR)/LICENSE* $(TARGET_SUPPORTED)

TARGET_M%: TARGET_LIBS_FIND=$(wildcard $(TARGET_SUPPORTED)/*/*/*_cortex_m$(subst TARGET_M,,$@)*.a)
TARGET_M%: rsync
	@printf "#\n# Copying $@ files...\n"
	mkdir $(foreach file,$(TARGET_LIBS_FIND),$(dir $(file))$@)
	$(foreach file,$(TARGET_LIBS_FIND),mv $(file) $(dir $(file))$@/lib$(notdir $(file));)

publish: TARGET_M3 TARGET_M4
	#
	# Rename release directorires to TARGET_RELEASE filters...
	$(foreach dir, $(TARGET_LIST_RELEASE),mv $(dir) $(dir $(dir))TARGET_RELEASE;)
	#
	# Rename debug directorires to TARGET_DEBUG filters...
	$(foreach dir, $(TARGET_LIST_DEBUG),mv $(dir) $(dir $(dir))TARGET_DEBUG;)
	#
	# Rename target directorires to TARGET_* filters...
	$(foreach target, $(TARGET_TRANSLATION),mv $(TARGET_SUPPORTED)/$(subst .,,$(suffix $(target))) $(TARGET_SUPPORTED)/TARGET_$(basename $(target));)
	#
	# Updating checked out version tag
	git -C $(UVISOR_DIR) describe --tags --abbrev=40 --dirty > $(TARGET_PREFIX)VERSION.txt
	#
	# Updated list of authors, sorted by contributions
	git -C $(UVISOR_DIR) shortlog -s -n > $(TARGET_PREFIX)AUTHORS.txt
	# Updated version of uvisor-tests
	cp $(UVISOR_DIR)/tools/uvisor-tests.txt $(TARGET_PREFIX)uvisor-tests.txt

uvisor-compile: $(UVISOR_GIT_CFG)
	make -C $(UVISOR_DIR)

update: $(UVISOR_GIT_CFG)
	#
	# Updating to latest uVisor library version
	git -C $(UVISOR_DIR) pull --rebase

$(UVISOR_GIT_CFG):
	rm -rf $(UVISOR_DIR)
	git clone -b $(UVISOR_GIT_BRANCH) $(UVISOR_GIT_URL) $(UVISOR_DIR)

clean: $(UVISOR_GIT_CFG)
	make -C $(UVISOR_DIR) clean
