# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-core
    INTERFACE
        ONBOARD_QUECTEL_BC95.cpp
        PeripheralPins.c
        system_clock.c
)

target_include_directories(mbed-core
    INTERFACE
        .
)
