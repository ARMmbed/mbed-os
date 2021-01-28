# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(DISCO_L562QE INTERFACE)

target_sources(DISCO_L562QE
    INTERFACE
        PeripheralPins.c
)

target_include_directories(DISCO_L562QE
    INTERFACE
        .
)

target_link_libraries(DISCO_L562QE INTERFACE STM32L562xE)
