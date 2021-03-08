# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-disco-l072cz-lrwan1 INTERFACE)

target_sources(mbed-disco-l072cz-lrwan1
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-disco-l072cz-lrwan1
    INTERFACE
        .
)

target_link_libraries(mbed-disco-l072cz-lrwan1 INTERFACE mbed-stm32l072xz)
