# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-f446ze INTERFACE)

target_sources(mbed-nucleo-f446ze
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-f446ze
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-f446ze INTERFACE mbed-stm32f446xe)
