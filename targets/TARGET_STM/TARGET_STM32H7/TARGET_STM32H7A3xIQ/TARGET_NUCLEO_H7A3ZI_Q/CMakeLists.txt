# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-h7a3zi-q INTERFACE)

target_sources(mbed-nucleo-h7a3zi-q
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-h7a3zi-q
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-h7a3zi-q INTERFACE mbed-stm32h7a3xiq)
