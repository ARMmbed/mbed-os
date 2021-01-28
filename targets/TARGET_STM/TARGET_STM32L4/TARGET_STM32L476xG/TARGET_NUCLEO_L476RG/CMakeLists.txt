# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_L476RG INTERFACE)

target_sources(NUCLEO_L476RG
    INTERFACE
        PeripheralPins.c
)

target_include_directories(NUCLEO_L476RG
    INTERFACE
        .
)

target_link_libraries(NUCLEO_L476RG INTERFACE STM32L476xG)
