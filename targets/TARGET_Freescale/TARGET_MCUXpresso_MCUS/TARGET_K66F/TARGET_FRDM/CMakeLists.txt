# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-frdm-k66f INTERFACE)

target_include_directories(mbed-frdm-k66f
    INTERFACE
        .
)

target_sources(mbed-frdm-k66f
    INTERFACE
        PeripheralPins.c
        crc.c
        fsl_clock_config.c
        fsl_phy.c
        mbed_overrides.c
)
