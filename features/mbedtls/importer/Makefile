###########################################################################
#
#  Copyright (c) 2016, ARM Limited, All Rights Reserved
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

#
# Use this file to import an mbed TLS release into mbed-OS as follows:
#
#   1) Set the MBED_TLS_RELEASE variable to the required mbed TLS release tag
#   2) make update
#   3) make
#   4) commit and push changes via git
#

# Set the mbed TLS release to import (this can/should be edited before import)
MBED_TLS_RELEASE ?= mbedtls-2.20.0d1
MBED_TLS_REPO_URL ?= git@github.com:ARMmbed/mbedtls.git

# Translate between mbed TLS namespace and mbed namespace
TARGET_PREFIX:=../
TARGET_SRC:=$(TARGET_PREFIX)src
TARGET_INC:=$(TARGET_PREFIX)inc
TARGET_TESTS:=$(TARGET_PREFIX)TESTS

# mbed TLS source directory - hidden from mbed via TARGET_IGNORE
MBED_TLS_DIR:=TARGET_IGNORE/mbedtls
MBED_TLS_API:=$(MBED_TLS_DIR)/include/mbedtls
MBED_TLS_GIT_CFG=$(MBED_TLS_DIR)/.git/config

.PHONY: all deploy deploy-tests rsync mbedtls clean update

all: mbedtls

mbedtls: deploy

rsync:
	#
	# Copying mbed TLS into mbed library...
	rm -rf $(TARGET_SRC)
	rsync -a --exclude='*.txt' $(MBED_TLS_DIR)/library/ $(TARGET_SRC)
	#
	# Copying mbed TLS headers to mbed includes...
	rm -rf $(TARGET_INC)
	mkdir -p $(TARGET_INC)
	rsync -a --delete $(MBED_TLS_API) $(TARGET_INC)
	#
	# Copying licenses
	cp $(MBED_TLS_DIR)/LICENSE $(TARGET_PREFIX)
	#

deploy: rsync
	#
	# Adjusting the default mbed TLS config file to mbed purposes
	./adjust-config.sh $(MBED_TLS_DIR)/scripts/config.pl $(TARGET_INC)/mbedtls/config.h
	#
	# Adjusting the default mbed TLS check-config file to mbed purposes
	./adjust-check-config.sh $(TARGET_INC)/mbedtls/check_config.h
	#
	# Copy and adjust the trimmed config that does not require entropy source
	cp $(MBED_TLS_DIR)/configs/config-no-entropy.h $(TARGET_INC)/mbedtls/.
	./adjust-no-entropy-config.sh $(MBED_TLS_DIR)/scripts/config.pl $(TARGET_INC)/mbedtls/config-no-entropy.h

deploy-tests: deploy
	#
	# Copying mbed TLS tests...
	rm -rf $(TARGET_TESTS)
	mkdir -p $(TARGET_TESTS)
	rsync -a --delete $(MBED_TLS_DIR)/tests/TESTS/ $(TARGET_TESTS)
	mkdir -p $(TARGET_TESTS)/host_tests
	cp $(MBED_TLS_DIR)/tests/scripts/mbedtls_test.py $(TARGET_TESTS)/host_tests/

update: $(MBED_TLS_GIT_CFG)  $(MBED_TLS_HA_GIT_CFG)
	#
	# Updating to the specified mbed TLS library version
	# (If it is not an initial checkout we will start with the repository
	# being in a detached head state)
	git -C $(MBED_TLS_DIR) fetch
	#
	# Checking out the required release
	git -C $(MBED_TLS_DIR) checkout $(MBED_TLS_RELEASE)
	#
	# Update and checkout git submodules
	git -C $(MBED_TLS_DIR) submodule update --init --recursive
	#
	# Updating checked out version tag
	git -C $(MBED_TLS_DIR) describe --tags --abbrev=12 --dirty --always > $(TARGET_PREFIX)VERSION.txt

$(MBED_TLS_GIT_CFG):
	rm -rf $(MBED_TLS_DIR)
	git clone $(MBED_TLS_REPO_URL) $(MBED_TLS_DIR)

clean:
	rm -f $(TARGET_PREFIX)LICENSE
	rm -f $(TARGET_PREFIX)VERSION.txt
	rm -f $(TARGET_PREFIX)AUTHORS.txt
	rm -rf $(TARGET_SRC)
	rm -rf $(TARGET_INC)
	rm -rf $(MBED_TLS_DIR)
