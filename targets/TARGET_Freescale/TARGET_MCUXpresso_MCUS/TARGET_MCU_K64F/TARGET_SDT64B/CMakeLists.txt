# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-sdt64b INTERFACE)

target_include_directories(mbed-sdt64b
    INTERFACE
        .
)

target_sources(mbed-sdt64b
    INTERFACE
        PeripheralPins.c
        crc.c
        fsl_clock_config.c
        fsl_phy.c
        mbed_overrides.c
)

target_link_libraries(mbed-sdt64b INTERFACE mbed-mcu-k64f)
