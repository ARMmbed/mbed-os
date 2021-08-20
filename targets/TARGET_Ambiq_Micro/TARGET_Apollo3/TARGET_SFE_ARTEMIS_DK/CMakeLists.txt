# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-sfe-artemis-dk INTERFACE)

target_include_directories(mbed-sfe-artemis-dk
    INTERFACE
        .
        bsp
)

target_sources(mbed-sfe-artemis-dk
    INTERFACE
        bsp/am_bsp.c
        bsp/am_bsp_pins.c
)

target_link_libraries(mbed-sfe-artemis-dk INTERFACE mbed-apollo3)
