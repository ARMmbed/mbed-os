# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-frdm-k64f INTERFACE)

target_sources(mbed-frdm-k64f
    INTERFACE
        crc.c
        fsl_clock_config.c
        fsl_phy.c
        mbed_overrides.c
        PeripheralPins.c
)

target_include_directories(mbed-frdm-k64f
    INTERFACE
        .
)
