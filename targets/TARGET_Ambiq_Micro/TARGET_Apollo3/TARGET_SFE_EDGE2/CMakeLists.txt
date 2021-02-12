# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-sfe-edge2 INTERFACE)

target_include_directories(mbed-sfe-edge2
    INTERFACE
        .
        bsp
)

target_sources(mbed-sfe-edge2
    INTERFACE
        bsp/am_bsp.c
        bsp/am_bsp_pins.c
)

target_link_libraries(mbed-sfe-edge2 INTERFACE mbed-apollo3)
