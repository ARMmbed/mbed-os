# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_L452RE_P INTERFACE)

target_sources(NUCLEO_L452RE_P
    INTERFACE
        PeripheralPins.c
)

target_include_directories(NUCLEO_L452RE_P
    INTERFACE
        .
)

target_link_libraries(NUCLEO_L452RE_P INTERFACE STM32L452xE)
