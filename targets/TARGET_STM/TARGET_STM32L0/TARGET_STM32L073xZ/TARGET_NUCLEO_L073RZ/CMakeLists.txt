# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_L073RZ INTERFACE)

target_sources(NUCLEO_L073RZ
    INTERFACE
        PeripheralPins.c
)

target_include_directories(NUCLEO_L073RZ
    INTERFACE
        .
)

target_link_libraries(NUCLEO_L073RZ INTERFACE STM32L073xZ)
