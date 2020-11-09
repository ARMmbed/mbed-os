# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-core
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-core
    INTERFACE
        .
)
