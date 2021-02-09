# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(nucleo-f767zi INTERFACE)

target_sources(nucleo-f767zi
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(nucleo-f767zi
    INTERFACE
        .
)

target_link_libraries(nucleo-f767zi INTERFACE mbed-stm32f767xi)
