# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-sfe-artemis INTERFACE)

target_include_directories(mbed-sfe-artemis
    INTERFACE
        .
        bsp
)

target_sources(mbed-sfe-artemis
    INTERFACE
        bsp/am_bsp.c
        bsp/am_bsp_pins.c
)

target_link_libraries(mbed-sfe-artemis INTERFACE mbed-apollo3)
