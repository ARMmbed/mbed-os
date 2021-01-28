# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_F070RB INTERFACE)

target_sources(NUCLEO_F070RB
    INTERFACE
        PeripheralPins.c
)

target_include_directories(NUCLEO_F070RB
    INTERFACE
        .
)

target_link_libraries(NUCLEO_F070RB INTERFACE STM32F070xB)
