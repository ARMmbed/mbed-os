# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_L4R5ZI INTERFACE)

target_sources(NUCLEO_L4R5ZI
    INTERFACE
        PeripheralPins.c
)

target_include_directories(NUCLEO_L4R5ZI
    INTERFACE
        .
)

target_link_libraries(NUCLEO_L4R5ZI INTERFACE STM32L4R5xI)
