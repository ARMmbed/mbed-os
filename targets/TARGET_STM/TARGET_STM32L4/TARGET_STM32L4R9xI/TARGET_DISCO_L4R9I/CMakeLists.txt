# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(DISCO_L4R9I INTERFACE)

target_sources(DISCO_L4R9I
    INTERFACE
        PeripheralPins.c
)

target_include_directories(DISCO_L4R9I
    INTERFACE
        .
)

target_link_libraries(DISCO_L4R9I INTERFACE STM32L4R9xI)
