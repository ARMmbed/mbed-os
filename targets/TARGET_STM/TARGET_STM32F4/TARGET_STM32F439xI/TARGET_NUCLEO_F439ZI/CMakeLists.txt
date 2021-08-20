# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-f439zi INTERFACE)

target_sources(mbed-nucleo-f439zi
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-f439zi
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-f439zi INTERFACE mbed-stm32f439xi)
