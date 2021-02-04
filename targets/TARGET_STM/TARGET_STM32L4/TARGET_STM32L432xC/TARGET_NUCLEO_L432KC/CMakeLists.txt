# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_L432KC INTERFACE)

target_sources(NUCLEO_L432KC
    INTERFACE
        PeripheralPins.c
)

target_include_directories(NUCLEO_L432KC
    INTERFACE
        .
)

target_link_libraries(NUCLEO_L432KC INTERFACE STM32L432xC)
