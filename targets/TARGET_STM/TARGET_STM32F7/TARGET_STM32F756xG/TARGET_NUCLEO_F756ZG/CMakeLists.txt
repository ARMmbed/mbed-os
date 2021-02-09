# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(nucleo-f756zg INTERFACE)

target_sources(nucleo-f756zg
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(nucleo-f756zg
    INTERFACE
        .
)

target_link_libraries(nucleo-f756zg INTERFACE mbed-stm32f756xg)
