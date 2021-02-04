# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_F767ZI INTERFACE)

target_sources(NUCLEO_F767ZI
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(NUCLEO_F767ZI
    INTERFACE
        .
)

target_link_libraries(NUCLEO_F767ZI INTERFACE STM32F767xI)
