# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-f412zg INTERFACE)

target_sources(mbed-nucleo-f412zg
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-f412zg
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-f412zg INTERFACE mbed-stm32f412xg)
