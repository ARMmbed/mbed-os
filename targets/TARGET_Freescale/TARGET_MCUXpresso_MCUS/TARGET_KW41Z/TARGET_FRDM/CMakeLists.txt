# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-frdm-kw41z INTERFACE)

target_include_directories(mbed-frdm-kw41z
    INTERFACE
        .
)

target_sources(mbed-frdm-kw41z
    INTERFACE
        PeripheralPins.c
        fsl_clock_config.c
        mbed_overrides.c
)
