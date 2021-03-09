# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-mote-l152rc INTERFACE)

target_sources(mbed-mote-l152rc
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(mbed-mote-l152rc
    INTERFACE
        .
)

target_link_libraries(mbed-mote-l152rc INTERFACE mbed-stm32l152xc)
