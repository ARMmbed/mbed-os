# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(DISCO_H747I INTERFACE)

target_sources(DISCO_H747I
    INTERFACE
        PeripheralPins.c
)

target_include_directories(DISCO_H747I
    INTERFACE
        .
)

target_link_libraries(DISCO_H747I INTERFACE STM32H747xI)
