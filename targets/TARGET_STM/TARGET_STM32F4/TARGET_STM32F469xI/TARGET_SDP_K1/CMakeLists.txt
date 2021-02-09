# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(sdp-k1 INTERFACE)

target_sources(sdp-k1
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(sdp-k1
    INTERFACE
        .
)
 target_link_libraries(sdp-k1 INTERFACE mbed-stm32f469xi)
