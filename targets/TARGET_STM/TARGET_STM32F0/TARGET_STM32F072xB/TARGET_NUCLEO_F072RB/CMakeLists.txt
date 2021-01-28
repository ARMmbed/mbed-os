# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_F072RB INTERFACE)

target_sources(NUCLEO_F072RB
    INTERFACE
        PeripheralPins.c
)

target_include_directories(NUCLEO_F072RB
    INTERFACE
        .
)

target_link_libraries(NUCLEO_F072RB INTERFACE STM32F072xB)
