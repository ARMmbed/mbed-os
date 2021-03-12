# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_G431RB INTERFACE)

target_sources(NUCLEO_G431RB
    INTERFACE
        PeripheralPins.c
)

target_include_directories(NUCLEO_G431RB
    INTERFACE
        .
)

target_link_libraries(NUCLEO_G431RB INTERFACE STM32G431xB)
