# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_L486RG INTERFACE)

target_sources(NUCLEO_L486RG
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(NUCLEO_L486RG
    INTERFACE
        .
)

target_link_libraries(NUCLEO_L486RG INTERFACE STM32L486xG)
