# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-sfe-artemis-thing-plus INTERFACE)

target_include_directories(mbed-sfe-artemis-thing-plus
    INTERFACE
        .
        bsp
)

target_sources(mbed-sfe-artemis-thing-plus
    INTERFACE
        bsp/am_bsp.c
        bsp/am_bsp_pins.c
)

target_link_libraries(mbed-sfe-artemis-thing-plus INTERFACE mbed-apollo3)
