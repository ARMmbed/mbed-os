# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(nucleo-f429zi INTERFACE)

target_sources(nucleo-f429zi
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(nucleo-f429zi
    INTERFACE
        .
)

target_link_libraries(nucleo-f429zi INTERFACE mbed-stm32f429xi)
