# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-f303ze INTERFACE)

target_sources(mbed-nucleo-f303ze
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-f303ze
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-f303ze INTERFACE mbed-stm32f303xe)
