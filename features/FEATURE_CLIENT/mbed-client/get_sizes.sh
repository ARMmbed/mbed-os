#!/bin/bash
# Copyright (c) 2016 ARM Limited. All rights reserved.
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

echo
echo "Build mbed Client API for executable size collection"
echo

BUILD_TARGET="frdm-k64f-armcc"

yt target $BUILD_TARGET
yt build

SIZE_CMD="size --totals"

OUTPUT_PATH="results/"
OUTPUT_FILE_POSTFIX="_${BUILD_TARGET}_size.txt"

MAIN_MODULE_NAME="mbed-client"

# yotta dependencies
YOTTA_DEPS=(
    'cmsis-core'
    'cmsis-core-k64f'
    'core-util'
    'dlmalloc'
    'greentea-client'
    'mbed-client-c'
    'mbed-client-mbed-os'
    'mbed-client-mbedtls'
    'mbed-drivers'
    'mbed-hal'
    'mbed-hal-frdm-k64f'
    'mbed-hal-k64f'
    'mbed-hal-ksdk-mcu'
    'mbedtls'
    'mbed-trace'
    'minar'
    'minar-platform-mbed'
    'nanostack-libservice'
    'sal'
    'sal-driver-lwip-k64f-eth'
    'sal-iface-eth'
    'sal-stack-lwip'
    'sockets'
    'ualloc'
    'uvisor-lib'
)


# yotta dummy dependencies, which have different naming
YOTTA_DUMMY_DEPS=(
    'cmsis-core-freescale'
    'compiler-polyfill'
    'mbed-hal-freescale'
    'minar-platform'
)


echo "Writing object file size informations to ${OUTPUT_PATH}"

# the "main" module is in build/<target>/source/<module>.ar
${SIZE_CMD} ./build/${BUILD_TARGET}/source/${MAIN_MODULE_NAME}.ar >${OUTPUT_PATH}${MAIN_MODULE_NAME}${OUTPUT_FILE_POSTFIX}

# these are the direct deps, found as build/<target>/ym/<module>/source/<module>.ar
for MODULE in "${YOTTA_DEPS[@]}"
do
    ${SIZE_CMD} ./build/${BUILD_TARGET}/ym/${MODULE}/source/${MODULE}.ar >${OUTPUT_PATH}${MODULE}${OUTPUT_FILE_POSTFIX}
done

# dummy libs, which are named with different logic
for MODULE in "${YOTTA_DUMMY_DEPS[@]}"
do
    # on paths the "-" char needs to be converted to "_"
    MODULE_PATH=${MODULE//-/_}
    ${SIZE_CMD} ./build/${BUILD_TARGET}/ym/${MODULE}/yotta_dummy_lib_${MODULE_PATH}/${MODULE}.ar >${OUTPUT_PATH}${MODULE}${OUTPUT_FILE_POSTFIX}
done
