# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(nucleo-f746zg INTERFACE)

target_sources(nucleo-f746zg
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(nucleo-f746zg
    INTERFACE
        .
)

target_link_libraries(nucleo-f746zg INTERFACE mbed-stm32f746xg)
