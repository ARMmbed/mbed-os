# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-frdm-k82f INTERFACE)

target_include_directories(mbed-frdm-k82f
    INTERFACE
        .
)

target_sources(mbed-frdm-k82f
    INTERFACE
        PeripheralPins.c
        fsl_clock_config.c
        mbed_overrides.c
)
