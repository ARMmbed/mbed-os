#!/bin/sh
#
# This file is part of mbed TLS (https://tls.mbed.org)
#
# Copyright (c) 2018, Arm Limited, All Rights Reserved
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
# Comments and uncomments #define lines in the given configuration header file
# to configure the file for use in mbed OS.
#
# Usage: adjust-no-entropy-config.sh [path to config script] [path to no-entropy config file]
#
set -eu

if [ $# -ne 2 ]; then
    echo "Usage: $0 path/to/config.pl path/to/config.h" >&2
    exit 1
fi

SCRIPT=$1
FILE=$2

conf() {
    $SCRIPT -o -f $FILE $@
}

add_code() {
    MATCH_PATTERN="$1"
    shift
    CODE=$(IFS=""; printf "%s" "$*")

    perl -i -pe                                    \
        "s/$MATCH_PATTERN/$MATCH_PATTERN$CODE/igs" \
        "$FILE"
}

conf set MBEDTLS_CMAC_C
conf unset MBEDTLS_CIPHER_MODE_XTS

# potentially save flash space by not enabling self-tests by default
conf unset MBEDTLS_SELF_TEST
