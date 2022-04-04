# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(source)

target_include_directories(mbed-core-flags
    INTERFACE
        include
        include/mbed-trace
)

target_sources(mbed-core-sources
    INTERFACE
        source/mbed_trace.c
)
