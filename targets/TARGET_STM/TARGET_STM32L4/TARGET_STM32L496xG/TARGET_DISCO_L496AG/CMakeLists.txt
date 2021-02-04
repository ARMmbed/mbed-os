# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(DISCO_L496AG INTERFACE)

target_sources(DISCO_L496AG
    INTERFACE
        PeripheralPins.c
)

target_include_directories(DISCO_L496AG
    INTERFACE
        .
)

target_link_libraries(DISCO_L496AG INTERFACE STM32L496xG)
