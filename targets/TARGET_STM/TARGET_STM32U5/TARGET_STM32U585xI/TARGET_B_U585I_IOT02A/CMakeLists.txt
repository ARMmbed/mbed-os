# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-b-u585i-iot02a INTERFACE)

target_sources(mbed-b-u585i-iot02a
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-b-u585i-iot02a
    INTERFACE
        .
)

target_link_libraries(mbed-b-u585i-iot02a INTERFACE mbed-stm32u585xi)
