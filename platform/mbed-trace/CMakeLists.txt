# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(source)

target_include_directories(mbed-os
    PUBLIC
        include
        include/mbed-trace
)

target_sources(mbed-os
    PRIVATE
        source/mbed_trace.c
)
