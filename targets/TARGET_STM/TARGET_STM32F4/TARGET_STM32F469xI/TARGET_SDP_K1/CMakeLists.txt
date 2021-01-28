# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(SDP_K1 INTERFACE)

target_sources(SDP_K1
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(SDP_K1
    INTERFACE
        .
)
 target_link_libraries(SDP_K1 INTERFACE STM32F469xI)
