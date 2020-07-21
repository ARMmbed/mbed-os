# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

option(MBEDIDE "Use Arm compiler from Mbed Studio" OFF)
if(MBEDIDE)
    set(MBED_STUDIO_ARM_COMPILER "--ide=mbed")
endif()
include(${MBED_ROOT}/cmake/toolchains/${MBED_TOOLCHAIN}.cmake)
