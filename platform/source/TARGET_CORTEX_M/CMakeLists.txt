# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    target_sources(mbed-core INTERFACE TOOLCHAIN_GCC/except.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    target_sources(mbed-core INTERFACE TOOLCHAIN_ARM/except.S)
endif()

target_sources(mbed-core
    INTERFACE
        mbed_fault_handler.c
)
