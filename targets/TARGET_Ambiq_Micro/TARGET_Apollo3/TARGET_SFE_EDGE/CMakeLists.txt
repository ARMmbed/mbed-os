# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-sfe-edge INTERFACE)

target_include_directories(mbed-sfe-edge
    INTERFACE
        .
        bsp
)

target_sources(mbed-sfe-edge
    INTERFACE
        bsp/am_bsp.c
        bsp/am_bsp_pins.c
)

target_link_libraries(mbed-sfe-edge INTERFACE mbed-apollo3)
