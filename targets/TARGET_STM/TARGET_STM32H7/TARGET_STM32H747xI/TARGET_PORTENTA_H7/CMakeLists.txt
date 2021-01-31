# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-core
    INTERFACE
        PeripheralPins.c
        system_clock_override.c
        portenta_power.cpp
)

target_include_directories(mbed-core
    INTERFACE
        .
)

