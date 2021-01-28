# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_F103RB INTERFACE)

target_sources(NUCLEO_F103RB
    INTERFACE
        PeripheralPins.c
)

target_include_directories(NUCLEO_F103RB
    INTERFACE
        .
)

target_link_libraries(NUCLEO_F103RB INTERFACE STM32F103xB)
