# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(DISCO_F769NI INTERFACE)

target_sources(DISCO_F769NI
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(DISCO_F769NI
    INTERFACE
        .
)

target_link_libraries(DISCO_F769NI INTERFACE STM32F769xI)
