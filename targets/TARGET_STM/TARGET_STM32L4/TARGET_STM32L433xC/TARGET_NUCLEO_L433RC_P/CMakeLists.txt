# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_L433RC_P INTERFACE)

target_sources(NUCLEO_L433RC_P
    INTERFACE
        PeripheralPins.c
)

target_include_directories(NUCLEO_L433RC_P
    INTERFACE
        .
)

target_link_libraries(NUCLEO_L433RC_P INTERFACE STM32L433xC)
