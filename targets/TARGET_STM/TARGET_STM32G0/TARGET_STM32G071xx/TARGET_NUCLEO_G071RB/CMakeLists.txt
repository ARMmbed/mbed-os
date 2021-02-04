# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_G071RB INTERFACE)

target_sources(NUCLEO_G071RB
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(NUCLEO_G071RB
    INTERFACE
        .
)

target_link_libraries(NUCLEO_G071RB INTERFACE STM32G071xx)
