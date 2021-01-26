# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_WL55JC INTERFACE)

target_sources(NUCLEO_WL55JC
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(NUCLEO_WL55JC
    INTERFACE
        .
)

target_link_libraries(NUCLEO_WL55JC INTERFACE STM32WL55xC)
