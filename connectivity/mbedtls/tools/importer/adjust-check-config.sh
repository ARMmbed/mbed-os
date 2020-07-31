#!/bin/sh
#
# This file is part of mbed TLS (https://tls.mbed.org)
#
# Copyright (c) 2019, Arm Limited, All Rights Reserved
#
# SPDX-License-Identifier: Apache-2.0
# Licensed under the Apache License, Version 2.0 (the License); you may
# not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# * http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an AS IS BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Purpose
#
# Removes checks from check_config.h that aren't needed for Mbed OS
#
# Usage: adjust-check-config.sh [path to check_config file]
#
set -eu

if [ $# -ne 1 ]; then
    echo "Usage: $0 path/to/check_config.h" >&2
    exit 1
fi

FILE=$1

conf() {
    $SCRIPT -f $FILE --force $@
}

remove_code() {
    MATCH_PATTERN=$(IFS=""; printf "%s" "$*")

    perl -0pi -e "s/$MATCH_PATTERN//g" "$FILE"
}

# When using Mbed Crypto's PSA Entropy Injection feature on Mbed OS, it is
# not required to opt out of having entropy sources added to your entropy
# contexts by default (via MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES).
# As integrated in Mbed OS, MBEDTLS_PSA_INJECT_ENTROPY is compatible with
# actual entropy sources. PSA entropy injection is implemented using the
# standard Mbed TLS NV Seed feature, and is as compatible with other
# entropy sources as the standard Mbed TLS NV Seed feature which does
# support entropy mixing.
remove_code                                                                                 \
    "#if defined\(MBEDTLS_PSA_INJECT_ENTROPY\) &&              \\\\\n"                      \
    "    !defined\(MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES\)\n"                                  \
    "#error \"MBEDTLS_PSA_INJECT_ENTROPY is not compatible with actual entropy sources\"\n" \
    "#endif\n"                                                                              \
    "\n"
