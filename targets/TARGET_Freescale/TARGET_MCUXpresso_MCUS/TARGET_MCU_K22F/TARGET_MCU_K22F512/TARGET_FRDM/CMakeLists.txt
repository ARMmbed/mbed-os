# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-frdm-mcu-k22f512 INTERFACE)

target_include_directories(mbed-frdm-mcu-k22f512
    INTERFACE
        .
)

target_sources(mbed-frdm-mcu-k22f512
    INTERFACE
        PeripheralPins.c
        fsl_clock_config.c
        mbed_overrides.c
)
