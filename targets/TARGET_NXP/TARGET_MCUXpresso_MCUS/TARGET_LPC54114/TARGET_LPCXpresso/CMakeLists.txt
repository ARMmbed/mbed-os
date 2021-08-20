# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-lpc5114-xpresso INTERFACE)

target_include_directories(mbed-lpc5114-xpresso
    INTERFACE
        .
)

target_sources(mbed-lpc5114-xpresso
    INTERFACE
        PeripheralPins.c
        clock_config.c
        mbed_overrides.c
)
