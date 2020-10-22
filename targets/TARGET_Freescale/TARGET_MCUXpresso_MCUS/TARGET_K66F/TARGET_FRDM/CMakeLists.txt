# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-core
    PUBLIC
        .
)

target_sources(mbed-core
    PRIVATE
        PeripheralPins.c
        crc.c
        fsl_clock_config.c
        fsl_phy.c
        mbed_overrides.c
)
