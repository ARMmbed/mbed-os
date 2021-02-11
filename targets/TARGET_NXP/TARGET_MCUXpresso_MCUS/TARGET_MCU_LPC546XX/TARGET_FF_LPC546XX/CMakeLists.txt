# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-ff-lpc546xx INTERFACE)

target_include_directories(mbed-ff-lpc546xx
    INTERFACE
        .
)

target_sources(mbed-ff-lpc546xx
    INTERFACE
        PeripheralPins.c
        clock_config.c
        mbed_overrides.c
)

target_link_libraries(mbed-ff-lpc546xx INTERFACE mbed-mcu-lpc546xx)
