# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_L496ZG INTERFACE)

target_sources(NUCLEO_L496ZG
    INTERFACE
        PeripheralPins.c
)

target_include_directories(NUCLEO_L496ZG
    INTERFACE
        .
)

target_link_libraries(NUCLEO_L496ZG INTERFACE STM32L496xG)
