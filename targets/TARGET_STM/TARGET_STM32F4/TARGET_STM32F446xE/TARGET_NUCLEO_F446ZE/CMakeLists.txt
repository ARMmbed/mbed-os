# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(nucleo-f446ze INTERFACE)

target_sources(nucleo-f446ze
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(nucleo-f446ze
    INTERFACE
        .
)

target_link_libraries(nucleo-f446ze INTERFACE mbed-stm32f446xe)
