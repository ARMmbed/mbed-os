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

# Set the mbed TLS release to import (this can/should be edited before import)
MBED_TLS_RELEASE:=yotta-2.3.0

# Translate between mbed TLS namespace and mbed namespace
TARGET_PREFIX:=../
TARGET_SRC:=$(TARGET_PREFIX)src
TARGET_INC:=$(TARGET_PREFIX)inc

# mbed TLS source directory - hidden from mbed via TARGET_IGNORE
MBED_TLS_URL:=https://github.com/ARMmbed/mbedtls.git
MBED_TLS_DIR:=TARGET_IGNORE/mbedtls
MBED_TLS_API:=$(MBED_TLS_DIR)/include/mbedtls
MBED_TLS_GIT_CFG=$(MBED_TLS_DIR)/.git/config

.PHONY: all deploy rsync mbedtls clean update

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
	cp $(MBED_TLS_DIR)/apache-2.0.txt $(TARGET_PREFIX)

deploy: rsync
	#
	# Adjusting the default mbed TLS config file to mbed purposes
	./adjust-config.sh $(MBED_TLS_DIR)/scripts/config.pl $(TARGET_INC)/mbedtls/config.h

update: $(MBED_TLS_GIT_CFG)
	#
	# Updating to latest mbed TLS library version
	git -C $(MBED_TLS_DIR) checkout development
	git -C $(MBED_TLS_DIR) pull --rebase origin development
	#
	# Checking out the required release
	git -C $(MBED_TLS_DIR) checkout $(MBED_TLS_RELEASE)
	#
	# Updating checked out version tag
	git -C $(MBED_TLS_DIR) describe --tags --abbrev=40 --dirty > $(TARGET_PREFIX)VERSION.txt
	#
	# Updated list of authors, sorted by contributions
	git -C $(MBED_TLS_DIR) shortlog -s -n > $(TARGET_PREFIX)AUTHORS.txt

$(MBED_TLS_GIT_CFG):
	rm -rf $(MBED_TLS_DIR)
	git clone $(MBED_TLS_URL) $(MBED_TLS_DIR)

clean:
	rm -f $(TARGET_PREFIX)LICENSE
	rm -f $(TARGET_PREFIX)apache-2.0.txt
	rm -f $(TARGET_PREFIX)VERSION.txt
	rm -f $(TARGET_PREFIX)AUTHORS.txt
	rm -rf $(TARGET_SRC)
	rm -rf $(TARGET_INC)
	rm -rf $(MBED_TLS_DIR)
