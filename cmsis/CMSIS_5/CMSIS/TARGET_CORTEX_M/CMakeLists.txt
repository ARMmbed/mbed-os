# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cmsis-cortex-m OBJECT)

target_include_directories(mbed-cmsis-cortex-m
    PUBLIC
        Include
)

target_sources(mbed-cmsis-cortex-m
    PRIVATE
        Source/mbed_tz_context.c
)
