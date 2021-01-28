# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_F091RC INTERFACE)

target_sources(NUCLEO_F091RC
    INTERFACE
        PeripheralPins.c
)

target_include_directories(NUCLEO_F091RC
    INTERFACE
        .
)

target_link_libraries(NUCLEO_F091RC INTERFACE STM32F091xC)
