# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(cmsis
    PUBLIC
        Include
)

target_sources(cmsis
    PRIVATE
        Source/mbed_tz_context.c
)
