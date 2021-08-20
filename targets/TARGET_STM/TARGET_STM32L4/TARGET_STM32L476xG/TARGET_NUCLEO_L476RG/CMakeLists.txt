# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-l476rg INTERFACE)

target_sources(mbed-nucleo-l476rg
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-l476rg
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-l476rg INTERFACE mbed-stm32l476xg)
