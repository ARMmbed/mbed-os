# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-greentea
    INTERFACE
        .
        unity
)

target_sources(mbed-greentea
    INTERFACE
        source/unity.c
)
