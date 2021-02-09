# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(nucleo-f303ze INTERFACE)

target_sources(nucleo-f303ze
    INTERFACE
        PeripheralPins.c
)

target_include_directories(nucleo-f303ze
    INTERFACE
        .
)

target_link_libraries(nucleo-f303ze INTERFACE mbed-stm32f303xe)
