# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-core
    INTERFACE
        .
)

target_sources(mbed-core
    INTERFACE
        PeripheralPins.c
        crc.c
        fsl_clock_config.c
        fsl_phy.c
        mbed_overrides.c
)
