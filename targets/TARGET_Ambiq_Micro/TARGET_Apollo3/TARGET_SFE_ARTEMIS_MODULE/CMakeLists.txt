# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-sfe-artemis-module INTERFACE)

target_include_directories(mbed-sfe-artemis-module
    INTERFACE
        .
        bsp
)

target_sources(mbed-sfe-artemis-module
    INTERFACE
        bsp/am_bsp.c
        bsp/am_bsp_pins.c
)

target_link_libraries(mbed-sfe-artemis-module INTERFACE mbed-apollo3)
