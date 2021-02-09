# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(nucleo-f207zg INTERFACE)

target_sources(nucleo-f207zg
    INTERFACE
        PeripheralPins.c
)

target_include_directories(nucleo-f207zg
    INTERFACE
        .
)

target_link_libraries(nucleo-f207zg INTERFACE mbed-stm32f207xg)
