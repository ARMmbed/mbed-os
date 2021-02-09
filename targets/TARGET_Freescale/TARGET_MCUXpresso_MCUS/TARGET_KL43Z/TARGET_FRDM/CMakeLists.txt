# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-frdm-kl43z INTERFACE)

target_include_directories(mbed-frdm-kl43z
    INTERFACE
        .
)

target_sources(mbed-frdm-kl43z
    INTERFACE
        PeripheralPins.c
        fsl_clock_config.c
        mbed_overrides.c
)
