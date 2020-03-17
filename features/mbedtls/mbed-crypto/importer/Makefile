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
# Use this file to import an Mbed Crypto release into Mbed OS as follows:
#
#   1) Set the CRYPTO_RELEASE variable to the required Mbed Crypto release
#      tag
#   2) make update
#   3) make
#   4) commit and push changes via git
#

# Set the Mbed Crypto release to import (this can/should be edited before
# import)
CRYPTO_RELEASE ?= mbedcrypto-3.0.1
CRYPTO_REPO_URL ?= git@github.com:ARMmbed/mbed-crypto.git

# Translate between Mbed Crypto namespace and Mbed OS namespace
TARGET_PREFIX:=..
TARGET_INC:=$(TARGET_PREFIX)/inc
TARGET_SRC:=$(TARGET_PREFIX)/src

TLS_SRC := \
	$(TARGET_SRC)/error.c \
	$(TARGET_SRC)/version.c \
	$(TARGET_SRC)/version_features.c \
	# end

# A folder structure is introduced here for targets that have both a Secure
# Processing Environment (SPE) targets and Non-secure Processing Environment
# (NSPE). Documentation for each folder as follows:
# COMPONENT_PSA_SRV_IMPL - Include secure service implementation code. For
# example PSA Crypto or PSA Secure Time implementations
TARGET_SRV_IMPL:=$(TARGET_PREFIX)/platform/COMPONENT_PSA_SRV_IMPL
# COMPONENT_SPE - Include code that compiles ONLY to the SPE image and never
# compiles to the NSPE image
TARGET_SPE:=$(TARGET_PREFIX)/platform/COMPONENT_SPE
# COMPONENT_NSPE - Include code that compiles ONLY to the NSPE image and never
# compiles to the SPE image
TARGET_NSPE:=$(TARGET_SRV_IMPL)/COMPONENT_NSPE

# Mbed Crypto source directory - hidden from mbed via TARGET_IGNORE
CRYPTO_DIR:=TARGET_IGNORE/mbed-crypto
CRYPTO_API:=$(CRYPTO_DIR)/include/psa
OLD_CRYPTO_API:=$(CRYPTO_DIR)/include/mbedtls
CRYPTO_GIT_CFG=$(CRYPTO_DIR)/.git/config


.PHONY: all rsync clean update

all: rsync

rsync:
	#
	# Copying Mbed Crypto headers to includes...
	rm -rf $(TARGET_INC)
	mkdir -p $(TARGET_INC)
	rsync -a --delete --exclude='crypto_struct.h' $(CRYPTO_API) $(TARGET_INC)/
	# Copy legacy Mbed TLS crypto headers, excluding headers duplicated in
	# Mbed TLS.
	rsync -a --delete \
	    --exclude='check_config.h' \
	    --exclude='compat-1.3.h' \
	    --exclude='config.h' \
	    --exclude='error.h' \
	    --exclude='version.h' \
	    $(OLD_CRYPTO_API) $(TARGET_INC)/
	#
	# Copying licenses
	cp $(CRYPTO_DIR)/LICENSE $(TARGET_PREFIX)/
	#
	# Copying Mbed Crypto into Mbed OS...
	rm -rf $(TARGET_SRV_IMPL)
	rm -rf $(TARGET_SPE)
	rm -rf $(TARGET_SRC)

	mkdir -p $(TARGET_SRV_IMPL)
	mkdir -p $(TARGET_SPE)
	mkdir -p $(TARGET_NSPE)

	rsync -a --delete $(CRYPTO_API)/crypto_struct.h $(TARGET_NSPE)/
	rsync -a --delete $(CRYPTO_API)/crypto_struct.h $(TARGET_SPE)/crypto_struct_spe.h
	rsync -a --delete $(CRYPTO_DIR)/library/psa_*.c $(TARGET_SRV_IMPL)/
	rsync -a --delete $(CRYPTO_DIR)/library/psa_*.h $(TARGET_SRV_IMPL)/
	rsync -a --delete $(CRYPTO_DIR)/library/*.c $(TARGET_SRC)/
	#
	# Remove PSA-specific C files (they go into $(TARGET_SRV_IMPL))
	rm -rf $(TARGET_SRC)/psa_*.c
	#
	# Remove files that duplicate Mbed TLS
	rm -rf $(TLS_SRC)

update: $(CRYPTO_GIT_CFG)
	#
	# Updating to the specified Mbed Crypto library version
	# (If it is not an initial checkout we will start with the repository
	# being in a detached head state)
	git -C $(CRYPTO_DIR) fetch
	#
	# Checking out the required release
	git -C $(CRYPTO_DIR) checkout $(CRYPTO_RELEASE)
	#
	# Update and checkout git submodules
	git -C $(CRYPTO_DIR) submodule update --init --recursive
	#
	# Updating Mbed Crypto checked out version tag
	git -C $(CRYPTO_DIR) describe --tags --abbrev=12 --dirty --always > $(TARGET_PREFIX)/VERSION.txt

$(CRYPTO_GIT_CFG):
	rm -rf $(CRYPTO_DIR)
	git clone $(CRYPTO_REPO_URL) $(CRYPTO_DIR)

clean:
	rm -f $(TARGET_PREFIX)/LICENSE
	rm -f $(TARGET_PREFIX)/VERSION.txt
	rm -f $(TARGET_PREFIX)/AUTHORS.txt
	rm -rf $(TARGET_INC)
	rm -rf $(TARGET_SRC)
	rm -rf $(CRYPTO_DIR)
	rm -rf $(TARGET_SRV_IMPL)
	rm -rf $(TARGET_SPE)
