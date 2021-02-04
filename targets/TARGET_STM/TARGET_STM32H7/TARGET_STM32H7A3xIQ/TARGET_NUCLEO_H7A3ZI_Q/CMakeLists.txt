# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_H7A3ZI_Q INTERFACE)

target_sources(NUCLEO_H7A3ZI_Q
    INTERFACE
        PeripheralPins.c
)

target_include_directories(NUCLEO_H7A3ZI_Q
    INTERFACE
        .
)

target_link_libraries(NUCLEO_H7A3ZI_Q INTERFACE STM32H7A3xIQ)
