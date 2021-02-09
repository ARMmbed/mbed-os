# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-f103RB INTERFACE)

target_sources(mbed-nucleo-f103RB
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-f103RB
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-f103RB INTERFACE mbed-stm32f103xb)
