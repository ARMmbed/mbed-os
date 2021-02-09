# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-hexiwear INTERFACE)

target_include_directories(mbed-hexiwear
    INTERFACE
        .
)

target_sources(mbed-hexiwear
    INTERFACE
        PeripheralPins.c
        fsl_clock_config.c
        mbed_overrides.c
)

target_link_libraries(mbed-hexiwear INTERFACE mbed-mcu-k64f)
