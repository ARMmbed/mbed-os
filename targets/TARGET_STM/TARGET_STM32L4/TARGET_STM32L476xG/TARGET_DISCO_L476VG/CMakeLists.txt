# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(DISCO_L476VG INTERFACE)

target_sources(DISCO_L476VG
    INTERFACE
        PeripheralPins.c
)

target_include_directories(DISCO_L476VG
    INTERFACE
        .
)

target_link_libraries(DISCO_L476VG INTERFACE STM32L476xG)
