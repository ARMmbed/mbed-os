# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-core-sources INTERFACE TOOLCHAIN_GCC/except.S)

target_sources(mbed-core-sources
    INTERFACE
        mbed_fault_handler.c
)
