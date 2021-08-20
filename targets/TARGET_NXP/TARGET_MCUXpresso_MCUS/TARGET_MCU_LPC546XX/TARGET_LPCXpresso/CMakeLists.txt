# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-lpc546xx-xpresso INTERFACE)

target_include_directories(mbed-lpc546xx-xpresso
    INTERFACE
        .
)

target_sources(mbed-lpc546xx-xpresso
    INTERFACE
        PeripheralPins.c
        clock_config.c
        crc.c
        mbed_overrides.c
)
