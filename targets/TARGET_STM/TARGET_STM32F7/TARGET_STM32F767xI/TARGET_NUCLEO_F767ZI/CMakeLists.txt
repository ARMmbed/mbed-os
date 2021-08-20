# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-f767zi INTERFACE)

target_sources(mbed-nucleo-f767zi
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(mbed-nucleo-f767zi
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-f767zi INTERFACE mbed-stm32f767xi)
