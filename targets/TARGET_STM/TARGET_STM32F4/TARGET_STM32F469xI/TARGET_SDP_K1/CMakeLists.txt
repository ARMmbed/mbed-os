# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-sdp-k1 INTERFACE)

target_sources(mbed-sdp-k1
    INTERFACE
        PeripheralPins.c
        TargetInit.c
)

target_include_directories(mbed-sdp-k1
    INTERFACE
        .
)
 target_link_libraries(mbed-sdp-k1 INTERFACE mbed-stm32f469xi)
