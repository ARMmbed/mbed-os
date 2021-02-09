# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(nucleo-f412zg INTERFACE)

target_sources(nucleo-f412zg
    INTERFACE
        PeripheralPins.c
)

target_include_directories(nucleo-f412zg
    INTERFACE
        .
)

target_link_libraries(nucleo-f412zg INTERFACE mbed-stm32f412xg)
