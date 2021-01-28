# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(DISCO_F746NG INTERFACE)

target_sources(DISCO_F746NG
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(DISCO_F746NG
    INTERFACE
        .
)

target_link_libraries(DISCO_F746NG INTERFACE STM32F746xG)
