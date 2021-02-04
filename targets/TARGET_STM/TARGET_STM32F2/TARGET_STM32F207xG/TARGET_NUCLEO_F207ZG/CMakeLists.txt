# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_F207ZG INTERFACE)

target_sources(NUCLEO_F207ZG
    INTERFACE
        PeripheralPins.c
)

target_include_directories(NUCLEO_F207ZG
    INTERFACE
        .
)

target_link_libraries(NUCLEO_F207ZG INTERFACE STM32F207xG)
