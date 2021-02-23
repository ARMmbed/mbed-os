# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cmsis-cortex-m INTERFACE)

target_include_directories(mbed-cmsis-cortex-m
    INTERFACE
        Include
)

target_sources(mbed-cmsis-cortex-m
    INTERFACE
        Source/mbed_tz_context.c
)
